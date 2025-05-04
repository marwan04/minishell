/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 03:07:48 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function signatures
void perror_and_exit(const char *message);
static int exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data);
static void process_heredoc_node(t_ast *node, t_minishell *data);
static void collect_heredocs(t_ast *node, t_minishell *data);
static void apply_redirections(t_ast *node);
char **envp_to_array(t_env *env);
int is_builtin(const char *cmd);
void execute_builtin_cmds(t_ast *node, int *last_exit, t_env **env);
static void read_heredoc(const char *delim, int write_fd, int expand, t_minishell *data);
int exec_cmd_node(t_ast *node, int prev_fd, t_minishell *data);
int exec_and_or(t_ast *node, int prev_fd, t_minishell *data);
int handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data);
int exec_ast(t_ast *node, int prev_fd, t_minishell *data);

// Static function declarations
static int exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data);
static void process_heredoc_node(t_ast *node, t_minishell *data);
static void collect_heredocs(t_ast *node, t_minishell *data);
static void apply_redirections(t_ast *node);

void perror_and_exit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Fill an array of length (data->pipes_count + 1) with the
 * individual stage sub-ASTs from a left/right PIPE tree.
 */
// helper: recursively walk a PIPE tree and collect non-PIPE leaves
static int
count_pipe_leaves(t_ast *node)
{
    if (!node)
        return 0;
    if (node->type == NODE_PIPE)
        return count_pipe_leaves(node->left)
             + count_pipe_leaves(node->right);
    // anything that isn’t a PIPE is one “stage”
    return 1;
}

// -----------------------------------------------------------------------------
// 2) Helper to walk the tree and collect each non-PIPE leaf into stages[].
// -----------------------------------------------------------------------------
static void
flatten_pipeline_helper(t_ast *node, t_ast **stages, int *idx)
{
    if (!node)
        return;
    if (node->type == NODE_PIPE)
    {
        flatten_pipeline_helper(node->left,  stages, idx);
        flatten_pipeline_helper(node->right, stages, idx);
    }
    else
    {
        stages[(*idx)++] = node;
    }
}

// -----------------------------------------------------------------------------
// 3) Public flatten function: caller just needs an AST root and gets back
//    an array of exactly count_pipe_leaves(root) t_ast*’s.
// -----------------------------------------------------------------------------
static t_ast **
collect_pipeline_stages(t_ast *pipe_root, int *out_n)
{
    int n = count_pipe_leaves(pipe_root);
    t_ast **stages = malloc(sizeof *stages * n);
    if (!stages)
    {
        perror("malloc");
        *out_n = 0;
        return NULL;
    }
    int idx = 0;
    flatten_pipeline_helper(pipe_root, stages, &idx);
    // idx should equal n
    *out_n = n;
    return stages;
}
static void close_heredoc_in_node(t_ast *node)
{
    if (!node) return;
    if (node->type == NODE_HEREDOC
     && node->heredoc_pipe[0] > STDERR_FILENO)
    {
        close(node->heredoc_pipe[0]);
        node->heredoc_pipe[0] = -1;
    }
    close_heredoc_in_node(node->left);
    close_heredoc_in_node(node->right);
}

static void close_heredoc_pipes_in_stages(t_ast **stages, int n_stages)
{
    for (int i = 0; i < n_stages; i++)
        close_heredoc_in_node(stages[i]);
}
static int
exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data)
{
    int   (*pipes)[2] = NULL;
    pid_t *pids        = NULL;
    int    status, last_status = 0;
    int    pipe_count = n_stages - 1;

    // 1) Single stage?  Just fall back.
    if (n_stages <= 1)
        return exec_cmd_node(stages[0], prev_fd, data);

    // 2) Allocate
    pipes = malloc(pipe_count * sizeof *pipes);
    pids  = malloc(     n_stages  * sizeof *pids);
    if (!pipes || !pids)
    {
        perror("malloc");
        free(pipes);
        free(pids);
        return 1;
    }

    // 3) Create every pipe
    for (int i = 0; i < pipe_count; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            // close any we opened
            for (int j = 0; j < i; j++)
                close(pipes[j][0]), close(pipes[j][1]);
            free(pipes);
            free(pids);
            return 1;
        }
    }

    // 4) Fork each stage
    for (int i = 0; i < n_stages; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            // cleanup all pipes
            for (int j = 0; j < pipe_count; j++)
                close(pipes[j][0]), close(pipes[j][1]);
            free(pipes);
            free(pids);
            return 1;
        }

        if (pid == 0)
        {
            // Child: restore default signals
            signal(SIGINT,  SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            // a) Apply <, <<, >, >> on this stage first
            apply_redirections(stages[i]);

            // b) Detect whether stdin/stdout were redirected
            int used_in  = 0, used_out = 0;
            for (t_ast *r = stages[i];
                 r && (r->type == NODE_HEREDOC
                    || r->type == NODE_REDIR_IN
                    || r->type == NODE_REDIR_OUT
                    || r->type == NODE_APPEND);
                 r = r->right)
            {
                if (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN)
                    used_in = 1;
                if (r->type == NODE_REDIR_OUT || r->type == NODE_APPEND)
                    used_out = 1;
            }

            // c) Wire up pipes[*]
            for (int j = 0; j < pipe_count; j++)
            {
                int rd = pipes[j][0], wr = pipes[j][1];

                if (j == i - 1)
                {
                    // incoming
                    if (!used_in)  dup2(rd, STDIN_FILENO);
                    close(rd);
                    close(wr);
                }
                else if (j == i)
                {
                    // outgoing
                    close(rd);
                    if (!used_out) dup2(wr, STDOUT_FILENO);
                    close(wr);
                }
                else
                {
                    // unrelated
                    close(rd);
                    close(wr);
                }
            }

            // d) close prev_fd if it was > 2
            if (prev_fd > STDERR_FILENO)
                close(prev_fd);

            // e) Peel off redir nodes down to the real command
            t_ast *cmd = stages[i];
            while (cmd && (cmd->type == NODE_HEREDOC
                        || cmd->type == NODE_REDIR_IN
                        || cmd->type == NODE_REDIR_OUT
                        || cmd->type == NODE_APPEND))
                cmd = cmd->left;

            // f) Execute
            if (cmd && cmd->args && cmd->args[0])
            {
                if (is_builtin(cmd->args[0]))
                {
                    int ret = 0;
                    execute_builtin_cmds(cmd, &ret, &data->env);
                    exit(ret);
                }
                else
                {
                    char *path = ft_get_path(cmd->args[0], &data->env);
                    if (!path)
                    {
                        fprintf(stderr, "%s: command not found\n",
                                cmd->args[0]);
                        exit(127);
                    }
                    execve(path, cmd->args, envp_to_array(data->env));
                    perror("execve");
                    exit(126);
                }
            }

            // only redirs? exit cleanly
            exit(0);
        }

        // Parent: record PID
        pids[i] = pid;
    }

    // 5) Parent closes its pipe copies + prev_fd
    if (prev_fd > STDERR_FILENO)
        close(prev_fd);
    for (int j = 0; j < pipe_count; j++)
        close(pipes[j][0]), close(pipes[j][1]);

    // 6) Wait for all children
    for (int i = 0; i < n_stages; i++)
    {
        if (waitpid(pids[i], &status, 0) < 0)
        {
            perror("waitpid");
            last_status = 1;
        }
        else
        {
            last_status = WEXITSTATUS(status);
        }
    }

    // 7) NOW clean up *any* here-doc read-ends left open in the parent
    close_heredoc_pipes_in_stages(stages, n_stages);

    free(pipes);
    free(pids);
    return last_status;
}

char **envp_to_array(t_env *env)
{
    size_t count = 0;
    for (t_env *e = env; e; e = e->next)
        count++;
    char **arr = malloc((count + 1) * sizeof(char*));
    if (!arr) return NULL;
    size_t idx = 0;
    for (t_env *e = env; e; e = e->next)
    {
        size_t klen = strlen(e->key);
        size_t vlen = e->value ? strlen(e->value) : 0;
        char *entry = malloc(klen + 1 + vlen + 1);
        if (!entry) {
            while (idx > 0) {
                free(arr[--idx]);
            }
            free(arr);
            return NULL;
        }
        strcpy(entry, e->key);
        entry[klen] = '=';
        if (e->value) {
            strcpy(entry + klen + 1, e->value);
            entry[klen + 1 + vlen] = '\0';
        } else {
            entry[klen + 1] = '\0';
        }
        arr[idx++] = entry;
    }
    arr[idx] = NULL;
    return arr;
}

// Check if a command is a shell builtin
int is_builtin(const char *cmd)
{
    if (!cmd)
        return 0;
    return (
        strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "env") == 0 ||
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "export") == 0 ||
        strcmp(cmd, "unset") == 0 ||
        strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "exit") == 0
    );
}

// Execute builtins in-process
void execute_builtin_cmds(t_ast *node, int *last_exit, t_env **env)
{
    if (!node || !node->args || !node->args[0])
    {
        *last_exit = 1;
        return;
    }
    *last_exit = 0;
    if (strcmp(node->args[0], "pwd") == 0)
        handle_pwd();
    else if (strcmp(node->args[0], "env") == 0)
        handle_env(*env);
    else if (strcmp(node->args[0], "echo") == 0)
        handle_echo(node->args);
    else if (strcmp(node->args[0], "export") == 0)
        handle_export(node->args, env);
    else if (strcmp(node->args[0], "unset") == 0)
        handle_unset(node->args, env);
    else if (strcmp(node->args[0], "cd") == 0)
        handle_cd(node->args, env);
    else if (strcmp(node->args[0], "exit") == 0)
        *last_exit = handle_exit(node->args, last_exit);
}


static void
read_heredoc(const char *delim, int write_fd, int expand, t_minishell *data)
{
    char *line;

    signal(SIGINT, SIG_DFL); // Reset SIGINT to default for proper Ctrl+C handling

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            /* Ctrl-D: echo a newline so the next prompt isn't stuck */
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        if (strcmp(line, delim) == 0)
        {
            free(line);
            break;
        }
        if (expand)
        {
            char *expanded = expand_vars(line, data->env);
            free(line);
            line = expanded;
            if (!line) continue; // Protection against NULL after expansion
        }
        /* write the user's line (plus newline) into the heredoc pipe */
        write(write_fd, line, strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    close(write_fd);  // only close the WRITE end here
}

// Improved process_heredoc_node function
static void process_heredoc_node(t_ast *node, t_minishell *data)
{
    if (!node || !node->heredoc_delim)
        return;

    if (pipe(node->heredoc_pipe) < 0) {
        perror("pipe");
        data->last_exit_status = 1;
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(node->heredoc_pipe[0]);
        close(node->heredoc_pipe[1]);
        data->last_exit_status = 1;
        return;
    }

    if (pid == 0)
    {
        // Save current signals
        struct sigaction old_int, new_int;
        sigaction(SIGINT, NULL, &old_int);
        
        // Set SIGINT to default for the heredoc reader
        new_int.sa_handler = SIG_DFL;
        sigemptyset(&new_int.sa_mask);
        new_int.sa_flags = 0;
        sigaction(SIGINT, &new_int, NULL);
        
        /* child: only needs the write side */
        close(node->heredoc_pipe[0]);
        read_heredoc(node->heredoc_delim, 
                     node->heredoc_pipe[1],
                     node->heredoc_expand,
                     data);
        exit(EXIT_SUCCESS);
    }
    
    /* parent: only needs the read side */
    close(node->heredoc_pipe[1]);
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        close(node->heredoc_pipe[0]);
        data->last_exit_status = 1;
        return;
    }
    
    // Check if heredoc was interrupted by SIGINT (Ctrl+C)
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
        data->last_exit_status = 130;  // Standard exit code for SIGINT
    }
}

static void
collect_heredocs(t_ast *node, t_minishell *data)
{
    if (!node)
        return;
        
    if (node->type == NODE_HEREDOC)
        process_heredoc_node(node, data);
        
    if (node->left)
        collect_heredocs(node->left, data);
        
    if (node->right)
        collect_heredocs(node->right, data);
}

static void apply_redirections(t_ast *node)
{
    t_ast *cur;
    int    fd;

    if (!node)
        return;

    /* First, apply the redirection represented by this node */
    if (node->type == NODE_HEREDOC)
    {
        if (node->heredoc_pipe[0] > 0) {
            if (dup2(node->heredoc_pipe[0], STDIN_FILENO) < 0)
                perror_and_exit("dup2");
            close(node->heredoc_pipe[0]);
            node->heredoc_pipe[0] = -1; // Mark as used
        }
    }
    else if (node->type == NODE_REDIR_IN)
    {
        if (!node->file) {
            fprintf(stderr, "minishell: No file specified for input redirection\n");
            exit(1);
        }
        
        fd = open(node->file, O_RDONLY);
        if (fd < 0) {
            perror(node->file);
            exit(1);
        }
        if (dup2(fd, STDIN_FILENO) < 0)
            perror_and_exit("dup2");
        close(fd);
    }
    else if (node->type == NODE_REDIR_OUT)
    {
        if (!node->file) {
            fprintf(stderr, "minishell: No file specified for output redirection\n");
            exit(1);
        }
        
        fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror(node->file);
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
            perror_and_exit("dup2");
        close(fd);
    }
    else if (node->type == NODE_APPEND)
    {
        if (!node->file) {
            fprintf(stderr, "minishell: No file specified for append redirection\n");
            exit(1);
        }
        
        fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror(node->file);
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
            perror_and_exit("dup2");
        close(fd);
    }

    /* Then, apply any additional redirections chained on the right */
    cur = node->right;
    while (cur)
    {
        if (cur->type == NODE_HEREDOC)
        {
            if (cur->heredoc_pipe[0] > 0) {
                if (dup2(cur->heredoc_pipe[0], STDIN_FILENO) < 0)
                    perror_and_exit("dup2");
                close(cur->heredoc_pipe[0]);
                cur->heredoc_pipe[0] = -1; // Mark as used
            }
        }
        else if (cur->type == NODE_REDIR_IN)
        {
            if (!cur->file) {
                fprintf(stderr, "minishell: No file specified for input redirection\n");
                exit(1);
            }
            
            fd = open(cur->file, O_RDONLY);
            if (fd < 0) {
                perror(cur->file);
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) < 0)
                perror_and_exit("dup2");
            close(fd);
        }
        else if (cur->type == NODE_REDIR_OUT)
        {
            if (!cur->file) {
                fprintf(stderr, "minishell: No file specified for output redirection\n");
                exit(1);
            }
            
            fd = open(cur->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror(cur->file);
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
                perror_and_exit("dup2");
            close(fd);
        }
        else if (cur->type == NODE_APPEND)
        {
            if (!cur->file) {
                fprintf(stderr, "minishell: No file specified for append redirection\n");
                exit(1);
            }
            
            fd = open(cur->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror(cur->file);
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
                perror_and_exit("dup2");
            close(fd);
        }
        cur = cur->right;
    }
}

int exec_cmd_node(t_ast *node, int prev_fd, t_minishell *data)
{
    int   status;
    pid_t pid;

    if (!node || !node->args)
        return 0;

    // Check if a heredoc was interrupted
    if (data->last_exit_status == 130) 
        return (data->last_exit_status = 130);

    // Handle builtin in parent only if no redirections needed
    if (node->args[0] && is_builtin(node->args[0]) && 
        prev_fd == STDIN_FILENO && 
        node->type == NODE_CMD && !node->right) {
        execute_builtin_cmds(node, &status, &data->env);
        return (data->last_exit_status = status);
    }
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        if (prev_fd > STDERR_FILENO)
            close(prev_fd);
        return (data->last_exit_status = 1);
    }
    
    if (pid == 0) {
        // child
        if (prev_fd > STDIN_FILENO) {
            if (dup2(prev_fd, STDIN_FILENO) < 0)
                perror_and_exit("dup2");
            close(prev_fd);
        }
        
        // Apply any redirections
        apply_redirections(node);
        
        // If it's a command with arguments
        if (node->args && node->args[0]) {
            if (is_builtin(node->args[0])) {
                int exit_code;
                execute_builtin_cmds(node, &exit_code, &data->env);
                exit(exit_code);
            } else {
                char *path = ft_get_path(node->args[0], &data->env);
                if (!path) {
                    fprintf(stderr, "%s: command not found\n", node->args[0]);
                    exit(127);
                }
                
                char **envp = envp_to_array(data->env);
                if (!envp) {
                    perror("malloc");
                    exit(126);
                }
                
                execve(path, node->args, envp);
                perror("execve");
                free(path);
                
                // Free environment array if execve fails
                for (int i = 0; envp[i]; i++)
                    free(envp[i]);
                free(envp);
                
                exit(126);
            }
        }
        // Just redirections, no command - exit successfully
        exit(0);
    }
    
    // Parent process
    if (prev_fd > STDERR_FILENO)
        close(prev_fd);
    
    waitpid(pid, &status, 0);
    
    if (WIFSIGNALED(status)) {
        // Handle signal termination (like Ctrl+C)
        int sig = WTERMSIG(status);
        if (sig == SIGINT)
            data->last_exit_status = 130;
        else if (sig == SIGQUIT)
            data->last_exit_status = 131;
        else
            data->last_exit_status = 128 + sig;
    } else {
        data->last_exit_status = WEXITSTATUS(status);
    }
    
    return data->last_exit_status;
}

int
exec_and_or(t_ast *node, int prev_fd, t_minishell *data)
{
    int lhs;
    
    if (!node || !node->left)
        return 0;
        
    lhs = exec_ast(node->left, prev_fd, data);
    
    if ((node->type == NODE_AND && lhs != 0) || 
        (node->type == NODE_OR && lhs == 0) ||
        !node->right)
        return lhs;
        
    // For the right side, we pass STDIN_FILENO since 
    // we've already processed prev_fd on the left
    return exec_ast(node->right, STDIN_FILENO, data);
}

int
handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data)
{
    if (!node || !node->left || !node->right)
        return 1;
        
    // Count the number of pipes
    int pipe_count = 1;
    t_ast *cur = node->right;
    
    while (cur && cur->type == NODE_PIPE) {
        pipe_count++;
        cur = cur->right;
    }
    
    t_ast **stages = malloc(sizeof(t_ast*) * (pipe_count + 1));
    if (!stages) {
        perror("malloc");
        return 1;
    }
    
    // Fill stages array
    int     r = pipe_count + 1;
    flatten_pipeline_helper(node, stages, &r);
    
    // Execute pipeline
    int result = exec_pipeline(stages, pipe_count + 1, prev_fd, data);
    
    free(stages);
    return result;
}

int exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
    if (!node)
        return 0;
    
    int status = 0;
    collect_heredocs(node, data);
    // Check if heredoc collection was interrupted
    if (data->last_exit_status == 130) {
        data->pipes_count = 0;
        if (prev_fd > STDERR_FILENO || prev_fd != -1)
            close(prev_fd);
        return 130;
    }

    if (node->type == NODE_PIPE)
    {
        int n_stages;
        t_ast **stages = collect_pipeline_stages(node, &n_stages);
        if (!stages)
            return 1;  // malloc failure

        int status = exec_pipeline(stages, n_stages, prev_fd, data);
        free(stages);
        return status;
    }

    // Handle different node types
    switch (node->type)
    {
            
        case NODE_AND:
        case NODE_OR:
            return exec_and_or(node, prev_fd, data);
            
        case NODE_GROUP:
            {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork");
                    if (prev_fd > STDERR_FILENO)
                        close(prev_fd);
                    return (data->last_exit_status = 1);
                }
                
                if (pid == 0) {
                    if (prev_fd > STDERR_FILENO && prev_fd != -1) {
                        if (dup2(prev_fd, STDIN_FILENO) < 0)
                            perror_and_exit("dup2");
                        close(prev_fd);
                    }
                    exit(exec_ast(node->left, STDIN_FILENO, data));
                }
                
                if (prev_fd > STDERR_FILENO)
                    close(prev_fd);
                    
                waitpid(pid, &status, 0);
                
                if (WIFSIGNALED(status))
                    data->last_exit_status = 128 + WTERMSIG(status);
                else
                    data->last_exit_status = WEXITSTATUS(status);
                    
                return data->last_exit_status;
            }
            
        case NODE_CMD:
            return exec_cmd_node(node, prev_fd, data);
            
        case NODE_HEREDOC:
        case NODE_REDIR_IN:
        case NODE_REDIR_OUT:
        case NODE_APPEND:
            {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork");
                    if (prev_fd > STDERR_FILENO)
                        close(prev_fd);
                    return (data->last_exit_status = 1);
                }
                
                if (pid == 0) {
                    if (prev_fd > STDERR_FILENO) {
                        if (dup2(prev_fd, STDIN_FILENO) < 0)
                            perror_and_exit("dup2");
                        close(prev_fd);
                    }
                    
                    apply_redirections(node);
                    
                    // Execute the left subtree if it exists
                    if (node->left)
                        exit(exec_ast(node->left, STDIN_FILENO, data));
                    else
                        exit(0);
                }
                
                if (prev_fd > STDERR_FILENO)
                    close(prev_fd);
                    
                waitpid(pid, &status, 0);
                
                if (WIFSIGNALED(status))
                    data->last_exit_status = 128 + WTERMSIG(status);
                else
                    data->last_exit_status = WEXITSTATUS(status);
                    
                return data->last_exit_status;
            }
            
        default:
            // Unknown node type
            fprintf(stderr, "minishell: Unknown AST node type: %d\n", node->type);
            if (prev_fd > STDERR_FILENO)
                close(prev_fd);
            return (data->last_exit_status = 1);
    }
}

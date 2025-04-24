#include "minishell.h"

void print_escaped(FILE *stream, const char *str)
{
    if (!str) return;
    while (*str)
	{
        switch (*str)
		{
            case '\n': fprintf(stream, "\\n"); break;
            case '\"': fprintf(stream, "\\\""); break;
            case '\\': fprintf(stream, "\\\\"); break;
            default: fputc(*str, stream);
        }
        str++;
    }
}

void print_ast_dot(t_ast *node, FILE *stream)
{
    if (node == NULL) return;
    fprintf(stream, "\"%p\" [label=\"", (void*)node);
    switch (node->type)
	{
        case NODE_CMD: fprintf(stream, "CMD: "); break;
        case NODE_REDIR_IN:
        case NODE_REDIR_OUT:
        case NODE_APPEND:
        case NODE_HEREDOC: fprintf(stream, "REDIR: "); break;
        case NODE_PIPE: fprintf(stream, "|"); break;
        case NODE_AND: fprintf(stream, "&&"); break;
        case NODE_OR: fprintf(stream, "||"); break;
        case NODE_GROUP: fprintf(stream, "Group"); break;
        default: fprintf(stream, "UNKNOWN"); break;
    }
    if (node->args)
	{
        for (int i = 0; node->args[i] != NULL; i++)
		{
            if (i > 0) fprintf(stream, " ");
            print_escaped(stream, node->args[i]);
        }
    }
    fprintf(stream, "\"];\n");
    if (node->left != NULL)
	{
        fprintf(stream, "\"%p\" -> \"%p\" [label=\"L\"];\n", (void*)node, (void*)node->left);
        print_ast_dot(node->left, stream);
    }
    if (node->right != NULL)
	{
        fprintf(stream, "\"%p\" -> \"%p\" [label=\"R\"];\n", (void*)node, (void*)node->right);
        print_ast_dot(node->right, stream);
    }
}

void generate_ast_diagram(t_ast *root)
{
    FILE *stream = fopen("ast.dot", "w");
    if (stream == NULL)
	{
        perror("fopen");
        return;
    }
    fprintf(stream, "digraph AST {\n");
    print_ast_dot(root, stream);
    fprintf(stream, "}\n");
	fclose(stream);
}
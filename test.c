/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:48:42 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/20 09:35:36 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	if (tmp == NULL)
		printf("NO TOKENS FOUND");
	while (tmp)
	{
		printf("Token: [%s] (Type: %d)\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

char	*node_type_to_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("CMD");
	if (type == NODE_PIPE)
		return ("PIPE");
	if (type == NODE_REDIR_IN)
		return ("REDIR_IN");
	if (type == NODE_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == NODE_APPEND)
		return ("APPEND");
	if (type == NODE_HEREDOC)
		return ("HEREDOC");
	if (type == NODE_AND)
		return ("AND");
	if (type == NODE_OR)
		return ("OR");
	if (type == NODE_GROUP)
		return ("GROUP");
	return ("UNKNOWN");
}

void	print_indent(int depth, int is_left)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("│   ");
		i++;
	}
	if (depth > 0)
	{
		if (is_left)
			printf("%s── ", "├");
		else
			printf("%s── ", "└");
	}
}

void	print_ast_node_content(t_ast *node)
{
	int	i;

	i = 0;
	printf("[%s]", node_type_to_str(node->type));
	if (node->type == NODE_CMD && node->args)
	{
		printf(" args: ");
		while (node->args[i])
		{
			printf("\"%s\" ", node->args[i]);
			test_heredoc_node(node);
			i++;
			
		}
	}
	else if (node->file)
	{
		printf(" file: \"%s\"", node->file);
		test_heredoc_node(node);
	}
	printf("\n");
}

void	print_ast(t_ast *node, int depth, int is_left)
{
	if (!node)
	{
		print_indent(depth, is_left);
		printf("(null)\n");
		return ;
	}
	print_indent(depth, is_left);
	print_ast_node_content(node);
	if (node->left || node->right)
	{
		print_ast(node->left, depth + 1, 1);
		print_ast(node->right, depth + 1, 0);
	}
}

// void	print_ast(t_ast *node, int depth, int is_left)
// {
// 	int	i;
// 	i = 0;
// 	if (!node)
// 	{
// 		print_indent(depth, is_left);
// 		printf("(null)\n");
// 		return ;
// 	}
// 	print_indent(depth, is_left);
// 	printf("[%s]", node_type_to_str(node->type));
// 	if (node->type == NODE_CMD && node->args)
// 	{
// 		printf(" args: ");
// 		while (node->args[i])
// 		{
// 			printf("\"%s\" ", node->args[i]);
// 			i++;
// 		}
// 	}
// 	else if (node->file)
// 	{
// 		printf(" file: \"%s\"", node->file);
// 	}
// 	printf("\n");
// 	if (node->left || node->right)
// 	{
// 		print_ast(node->left, depth + 1, 1);
// 		print_ast(node->right, depth + 1, 0);
// 	}
// }

void	test_heredoc_node(t_ast *node)
{
	if (!node)
	{
		printf("Node is NULL\n");
		return;
	}
	if (node->type != NODE_HEREDOC)
	{
		printf("Node is not a HEREDOC node\n");
		return;
	}
	printf("Testing HEREDOC Node:\n");
	printf("Delimiter: \"%s\"\n", node->file ? node->file : "(null)");

	// Test the pipe: if readable
	if (node->heredoc_pipe[0] == -1 || node->heredoc_pipe[1] == -1)
	{
		printf("Heredoc pipe not set up properly\n");
		return;
	}

	// Try reading a bit to show that data exists
	char buffer[1024];
	ssize_t bytes = read(node->heredoc_pipe[0], buffer, sizeof(buffer) - 1);
	if (bytes > 0)
	{
		buffer[bytes] = '\0';
		printf("Read from heredoc pipe:\n%s", buffer);
	}
	else if (bytes == 0)
	{
		printf("Heredoc pipe is empty (EOF reached)\n");
	}
	else
	{
		perror("Failed to read from heredoc pipe");
	}
}

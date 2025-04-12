/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:48:42 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/12 14:42:47 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*tmp;
	// THIS FUNCTION IS FOR TESTING ONLY
	tmp = head;
	if(tmp == NULL)
		printf("NO TOKENS FOUND");
	while (tmp)
	{
		printf("Token: [%s] (Type: %d)\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

char *node_type_to_str(t_node_type type)
{
	if (type == NODE_CMD) return "CMD";
	if (type == NODE_PIPE) return "PIPE";
	if (type == NODE_REDIR_IN) return "REDIR_IN";
	if (type == NODE_REDIR_OUT) return "REDIR_OUT";
	if (type == NODE_APPEND) return "APPEND";
	if (type == NODE_HEREDOC) return "HEREDOC";
	return "UNKNOWN";
}

void print_indent(int depth, int is_left)
{
	for (int i = 0; i < depth; i++)
		printf("│   ");
	if (depth > 0)
		printf("%s── ", is_left ? "├" : "└");
}

void print_ast(t_ast *node, int depth, int is_left)
{
	if (!node)
	{
		print_indent(depth, is_left);
		printf("(null)\n");
		return;
	}

	print_indent(depth, is_left);
	printf("[%s]", node_type_to_str(node->type));

	if (node->type == NODE_CMD && node->args)
	{
		printf(" args: ");
		for (int i = 0; node->args[i]; i++)
			printf("\"%s\" ", node->args[i]);
	}
	else if (node->file)
	{
		printf(" file: \"%s\"", node->file);
	}
	printf("\n");

	// Recursively print children
	if (node->left || node->right)
	{
		print_ast(node->left, depth + 1, 1);  // LEFT
		print_ast(node->right, depth + 1, 0); // RIGHT
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:19:03 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/05 23:36:53 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token *new_token(char *value, t_token_type type)
{
t_token *token;

  token = malloc(sizeof(t_token));
  if (!token)
    return NULL;
  token->value = ft_strdup(value);
  token->type = type;
  token->next = NULL;
  return token;
}

t_token  *last_token(t_token *token)
{
  while (token && token->next)
    token = token->next;
  return (token);
}


void add_token(t_token **head, char *value, t_token_type type)
{
  t_token *tmp;
  t_token *new;
  
  new = new_token(value, type);
  if (!new)
    return;
  if(!*head)
  {
   *head = new;
  }
  else
  {
    tmp = last_token(*head);
    tmp->next = new;
  }
}

t_token *tokenizer(char *input)
{
    t_token *head;
    int i;
        
    head = NULL;
    i = 0;
    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        if (input[i] == '|')
        {
            add_token(&head, "|", PIPE);
            i++;
            continue;
        }
        if (input[i] == '>' || input[i] == '<')
        {
            if (input[i] == '>' && input[i + 1] == '>')
            {
                add_token(&head, ">>", APPEND);
                i += 2;
            }
            else if (input[i] == '<' && input[i + 1] == '<')
            {
                add_token(&head, "<<", HEREDOC);
                i += 2;
            }
            else
            {
                char symbol[2] = {input[i], '\0'};
                add_token(&head, symbol, (input[i] == '>') ? REDIR_OUT : REDIR_IN);
                i++;
            }
            continue;
        }
        if (input[i] == '"' || input[i] == '\'')
        {
            char quote = input[i];
            int start = ++i;
            while (input[i] && input[i] != quote)
                i++;
            if (input[i] == quote)
            {
                char *quoted_str = ft_substr(input, start, i - start);
                add_token(&head, quoted_str, WORD);
                free(quoted_str);
                i++;
            }
            continue;
        }
        if (input[i] != '\0' && input[i] != ' ' && input[i] != '|' && input[i] != '<' && input[i] != '>')
        {
            int start = i;
            while (input[i] && input[i] != ' ' && input[i] != '|' && input[i] != '<' && input[i] != '>')
                i++;
            char *word = ft_substr(input, start, i - start);
            add_token(&head, word, WORD);
            free(word);
            continue;
        }
    }
    return head;
}


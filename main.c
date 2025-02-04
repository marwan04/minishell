/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/04 22:38:59 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void print_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i]) {
        printf("Token[%d]: %s\n", i, tokens[i]);
        i++;
    }
}

//This file will only contain the main function
int main() {
    char *input;
    char **tokens;

    while (1) {
        input = readline("minishell> ");
        if (!input)
          break;
        if (*input)
        {
          add_history(input);
          tokens = tokenizer(input);      
        }
        printf("You entered: %s\n", input);
        print_tokens(tokens);
        free(input);
    }
    return 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:02:18 by malrifai          #+#    #+#             */
/*   Updated: 2024/09/14 17:12:20 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

int	ft_printf(const char *input, ...);
int	print_pointer(void *p);
int	print_unsigned(unsigned int nb);
int	print_string(char *s);
int	print_int(int n);
int	print_char(char c);
int	print_hex(unsigned long value, int uppercase);

#endif

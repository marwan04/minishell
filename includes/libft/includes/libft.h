/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 15:07:36 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/01 19:39:39 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdarg.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1028
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}					t_list;

t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *s1, const void *s2, size_t n);
void		*ft_calloc(size_t count, size_t size);
void		ft_free_double_list(char **lst);
void		ft_putstr_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
char		*ft_strchr(const char *s, int i);
char		*ft_strrchr(const char *s, int c);
char		*ft_strdup(const char *src);
char		*ft_strnstr(const char *big, const char *little, size_t len);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin_var(int count, ...);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int n);
char		*get_next_line(int fd);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
size_t		ft_strlen(const char *str);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlcat(char *dest, char *src, size_t size);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int			ft_strcmp(char *s1, char *s2);
int			ft_isalpha(int i);
int			ft_isdigit(int i);
int			ft_isnumeric(const char *str);
int			ft_isascii(int i);
int			ft_isprint(int i);
int			ft_atoi(const char *str);
int			ft_atoi_base(char *str, int base);
int			ft_toupper(int i);
int			ft_tolower(int i);
int			ft_isalnum(int i);
int			ft_strncmp(const char *s1, char *s2, size_t n);
int			ft_lstsize(t_list *lst);
int			ft_printf(const char *input, ...);
int			print_pointer(void *p);
int			print_unsigned(unsigned int nb);
int			print_string(char *s);
int			print_int(int n);
int			print_char(char c);
int			print_hex(unsigned long value, int uppercase);

#endif

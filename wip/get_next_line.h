/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:11:37 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/11 18:42:43 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include "minirt.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_list
{
	char			buf[BUFFER_SIZE];
	ssize_t			length;
	struct s_list	*next;
}	t_list;

t_list	*new_buf(void);
size_t	count_size(t_list *b);
size_t	end_of_line_size(t_list *b);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	ft_bzero(void *s, size_t n);
void	ft_clear(t_list *b);
char	*get_next_line(int fd);
#endif

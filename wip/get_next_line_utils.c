/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 13:42:35 by kdzhoha           #+#    #+#             */
/*   Updated: 2023/06/12 13:03:14 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	count_size(t_list *b)
{
	size_t	size;

	size = 0;
	while (b != NULL)
	{
		if (end_of_line_size(b) != 0)
		{
			size = size + end_of_line_size(b);
			return (size);
		}	
		size = size + b->length;
		b = b->next;
	}
	return (size);
}

t_list	*new_buf(void)
{
	t_list	*b_node;

	b_node = (t_list *)malloc(sizeof(t_list));
	if (b_node == NULL)
		return (NULL);
	b_node->length = 0;
	b_node->next = NULL;
	return (b_node);
}

size_t	end_of_line_size(t_list *b)
{
	ssize_t	i;

	i = 0;
	while (i < b->length)
	{
		if (b->buf[i] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*s;
	unsigned char		*d;

	if (dst == NULL && src == NULL)
		return (dst);
	s = (const unsigned char *)src;
	d = (unsigned char *)dst;
	while (n > 0)
	{
		*d = *s;
		s++;
		d++;
		n--;
	}
	return (dst);
}

void	ft_clear(t_list *b)
{
	t_list	*b_next;

	if (b == NULL)
		return ;
	while (b != NULL)
	{
		b_next = b->next;
		free(b);
		b = b_next;
	}
}

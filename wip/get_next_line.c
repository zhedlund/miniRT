/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 15:17:50 by kdzhoha           #+#    #+#             */
/*   Updated: 2023/06/12 14:14:11 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	read_line(int fd, t_list *b)
{
	t_list	*b_next;

	if (b->length == 0)
	{
		b->length = read(fd, b->buf, BUFFER_SIZE);
		if (b->length == -1)
			return (1);
		if (b->length < BUFFER_SIZE)
			return (0);
	}
	while (end_of_line_size(b) == 0)
	{
		b_next = new_buf();
		if (b_next == NULL)
			return (1);
		b->next = b_next;
		b_next->length = read(fd, b_next->buf, BUFFER_SIZE);
		if (b_next->length == -1)
			return (1);
		if (b_next->length < BUFFER_SIZE)
			return (0);
		b = b_next;
	}
	return (0);
}

static char	*create_str(t_list *b)
{
	char	*res;
	char	*str;
	size_t	size;

	if (b->length == 0)
		return (NULL);
	res = (char *)malloc(count_size(b) + 1);
	if (res == NULL)
		return (NULL);
	str = res;
	while (b != NULL)
	{
		size = end_of_line_size(b);
		if (size != 0)
		{
			ft_memcpy(str, b->buf, size);
			*(str + size) = '\0';
			return (res);
		}
		ft_memcpy(str, b->buf, b->length);
		str = str + b->length;
		b = b->next;
	}
	*str = '\0';
	return (res);
}

static void	write_remaining(t_list *buf_lst)
{
	size_t	size;
	t_list	*b;

	b = buf_lst;
	while (b->next != NULL)
		b = b->next;
	size = end_of_line_size(b);
	if (size == 0)
		buf_lst->length = 0;
	else
	{
		buf_lst->length = b->length - size;
		ft_memcpy(buf_lst->buf, b->buf + size, buf_lst->length);
	}
}

char	*get_next_line(int fd)
{
	char			*str;
	int				read_error;
	t_list			*buf_lst;
	static t_list	storage = {.length = 0, .next = NULL};

	buf_lst = &storage;
	read_error = 0;
	str = NULL;
	if (end_of_line_size(buf_lst) == 0)
		read_error = read_line(fd, buf_lst);
	if (!read_error)
		str = create_str(buf_lst);
	if (str == NULL)
		buf_lst->length = 0;
	else
		write_remaining(buf_lst);
	ft_clear(buf_lst->next);
	buf_lst->next = NULL;
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:01:28 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/30 13:47:32 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
//#include "parsing.h"

int	put_error(char *message)
{
	write(2, "Error\n", 6);
	write(2, message, ft_strlen(message));
	return (-1);
}

void	*malloc_error(void)
{
	write(2, "Malloc failed\n", 14);
	return (NULL);
}

//check for *.rt
int	is_valid_name(char *file)
{
	if (!file)
		return (0);
	while (*file != '.')
		file++;
	if (*(++file) != 'r')
		return (0);
	if (*(++file) != 't')
		return (0);
	if (*(++file) != '\0')
		return (0);
	return (1);
}

int	check_empty_line(char *str)
{
	if (*str == '\n' || !*str)
		return (0);
	else
		return (put_error("Invalid object identifier\n"));
}

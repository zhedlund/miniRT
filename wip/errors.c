/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:01:28 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/15 15:20:13 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_error(char *message)
{
	write(2, message, sizeof(message));
	exit(1);
}

void	*malloc_error(void)
{
	write(2, "Malloc failed", 13);
	return (NULL);
}

int	check_file(char *file)
{
	if (!file)
		return (0);
	//check for *.rt
	return (1);
}

void	check_input(char *str)
{
	if (*str == '\n' || *str == '\0')
		return ;
	else
		put_error("Wrong input");
}

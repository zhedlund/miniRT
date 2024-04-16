/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:07:57 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/16 19:01:52 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_numbers(char *str, char begin)
{
	int	i;

	i = 0;
	if (str[i] != begin)
		return (0);
	i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			if (str[i] != '.' && str[i] != ',' && str[i] != ' ')
				return (0);
		}
		i++;
	}
	return (i);
}

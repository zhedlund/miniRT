/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:04:05 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/02 18:20:47 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
//#include "parsing.h"

t_vec	*new_vec(float x, float y, float z)
{
	t_vec	*vec;

	vec = (t_vec *)malloc(sizeof(t_vec));
	if (!vec)
		return (malloc_error());
	vec->x = x;
	vec->y = y;
	vec->z = z;
	return (vec);
}

t_color	*new_color(float r, float g, float b)
{
	t_color	*new;

	new = (t_color *)malloc(sizeof(t_color));
	if (!new)
		return (malloc_error());
	new->r = r;
	new->g = g;
	new->b = b;
	return (new);
}

int	read_color(t_color *col, char *str)
{
	char	**color;

	if (count_words(str, ',') != 3)
		return (-1);
	color = ft_split(str, ',');
	col->r = is_color(color[0]);
	if (col->r == -1)
	{
		free_array(color);
		return (-1);
	}
	col->g = is_color(color[1]);
	if (col->g == -1)
	{
		free_array(color);
		return (-1);
	}
	col->b = is_color(color[2]);
	if (col->b == -1)
	{
		free_array(color);
		return (-1);
	}
	free_array(color);
	return (0);
}

int	read_vector(t_vec *vector, char *str)
{
	char	**vec;

	if (count_words(str, ',') != 3)
		return (-1);
	vec = ft_split(str, ',');
	if (check_vector(vec) == -1)
	{
		free_array(vec);
		return (-1);
	}
	vector->x = ft_atof(vec[0]);
	vector->y = ft_atof(vec[1]);
	vector->z = ft_atof(vec[2]);
	free_array(vec);
	return (0);
}

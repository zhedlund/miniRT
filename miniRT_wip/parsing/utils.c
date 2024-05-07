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

void	set_cylinder(t_cyl *cyl)
{
	cyl->top_p.x = cyl->center.x - cyl->normal.x * (cyl->h / 2);
	cyl->top_p.y = cyl->center.y - cyl->normal.y * (cyl->h / 2);
	cyl->top_p.z = cyl->center.z - cyl->normal.z * (cyl->h / 2);
	cyl->top.point = cyl->top_p;
	cyl->top.normal = vec_multiply(&cyl->normal, -1);
	cyl->top.color = cyl->color;
	cyl->bottom.point = vec3_add(cyl->center, vec_multiply(&cyl->normal, cyl->h / 2));
	cyl->bottom.normal = cyl->normal;
	cyl->bottom.color = cyl->color;
}

void	copy_vec(t_vec *src, t_vec *dst)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

void	copy_color(t_color *src, t_color *dst)
{
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:04:05 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/15 16:23:09 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

void	free_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
}

float	get_distance(t_vec *p1, t_vec *p2)
{
	float	res;

	if (!p1 || !p2)
		return (-1);

	res = sqrtf(powf((p2->x - p1->x), 2)
			+ powf((p2->y - p1->y), 2)
			+ powf((p2->z - p1->z), 2));
	return (res);
}

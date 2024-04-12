/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:53:29 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/12 18:59:04 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

void	ray_tracing(t_scene *scene)
{

	return ;
}

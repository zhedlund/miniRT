/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:51:44 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/21 16:08:15 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/* Calculates t1 and t2 for a quadratic equation.
   Returns a t_vec struct representing the two solutions.
*/
int	get_min(float t[3])
{
	int		i;
	int		res;
	float	min;

	i = 0;
	res = -1;
	min = FLT_MAX;
	while (i < 3)
	{
		if (t[i] > 0 && t[i] < min)
		{
			res = i;
			min = t[i];
		}
		i++;
	}
	return (res);
}

/* calculates intersection point of a ray and a sphere.
   Returns a t_vec struct representing the intersection point.
*/
t_vec	intersect_point(t_ray *r, float t)
{
	return ((t_vec){r->origin.x + (t * r->dir.x),
		r->origin.y + (t * r->dir.y),
		r->origin.z + (t * r->dir.z)});
}

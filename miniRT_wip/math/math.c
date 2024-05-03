/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:08:04 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/03 18:58:42 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/*	calculates the dot product between two 3D vectors u and v.
	repr cosine of the angle between them multiplied by the magnitudes of the vectors.
	u⋅v = ux⋅vx + uy⋅vy + uz⋅vz
*/
float	dot(t_vec *u, t_vec *v)
{
	return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
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

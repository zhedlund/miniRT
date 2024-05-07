/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:08:04 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/02 18:32:25 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/*	calculates the dot product between two 3D vectors u and v.
	repr cosine of the angle between them multiplied by the magnitudes of the vectors.
	u⋅v = ux⋅vx + uy⋅vy + uz⋅vz
*/
float	dot(const t_vec *u, const t_vec *v)
{
	return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
}

/* calculates intersection point of a ray and a sphere.
   Returns a t_vec struct representing the intersection point.
*/
t_vec	intersect_point(const t_ray *r, float t)
{
	return ((t_vec){r->origin.x + (t * r->dir.x),
		r->origin.y + (t * r->dir.y),
		r->origin.z + (t * r->dir.z)});
}

t_vec	vec_multiply(const t_vec *vec, float n)
{
	return ((const t_vec){vec->x * n, vec->y * n, vec->z * n});
}

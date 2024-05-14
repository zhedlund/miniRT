/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:08:04 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/14 17:35:07 by zhedlund         ###   ########.fr       */
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

t_vec vec3_divide(t_vec vec, float scalar)
{
	return (t_vec){ vec.x / scalar, vec.y / scalar, vec.z / scalar };
}

t_vec	vec_multiply(t_vec *vec, float n)
{
	return ((t_vec){vec->x * n, vec->y * n, vec->z * n});
}

t_vec	vec3_cross(t_vec a, t_vec b)
{
	return ((t_vec){(a.y * b.z) - (a.z * b.y),
					(a.z * b.x) - (a.x * b.z),
					(a.x * b.y) - (a.y * b.x)});
}
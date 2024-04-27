/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:08:04 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/27 19:55:15 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/*	calculates the dot product between two 3D vectors u and v.
	repr cosine of the angle between them multiplied by the magnitudes of the vectors. 
	u⋅v = ux⋅vx + uy⋅vy + uz⋅vz
*/
float dot(const t_vec *u, const t_vec *v) 
{
    return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
}

/* calculates intersection point of a ray and a sphere.
   Returns a t_vec struct representing the intersection point.
*/
t_vec intersect_point(const t_ray *r, float t)
{
    return ((t_vec){r->origin.x + (t * r->dir.x),
                   r->origin.y + (t * r->dir.y),
                   r->origin.z + (t * r->dir.z)});
}

/* multiplies each component of a vector by a scalar.
   Returns a new t_vec struct representing the result.
*/
t_vec vec3_multiply(const t_vec *v, float scalar)
{
    return ((t_vec){v->x * scalar, v->y * scalar, v->z * scalar});
}

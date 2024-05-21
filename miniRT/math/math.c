/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:08:04 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/21 19:02:57 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/*	calculates the dot product between two 3D vectors u and v. repr cosine
of the angle between them multiplied by the magnitudes of the vectors.
	u⋅v = ux⋅vx + uy⋅vy + uz⋅vz
*/
float	dot(t_vec *u, t_vec *v)
{
	return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
}

t_vec	vec3_divide(t_vec vec, float scalar)
{
	return ((t_vec){vec.x / scalar, vec.y / scalar, vec.z / scalar});
}

t_vec	vec_multiply(t_vec *vec, float n)
{
	return ((t_vec){vec->x * n, vec->y * n, vec->z * n});
}

t_vec	vec3_cross(t_vec a, t_vec b)
{
	return ((t_vec){(a.y * b.z) - (a.z * b.y),
		-((a.x * b.z) - (a.z * b.x)),
		(a.x * b.y) - (a.y * b.x)});
}

float	pow2(float n)
{
	return (powf(n, 2));
}

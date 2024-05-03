/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_vector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:28:43 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/03 18:59:15 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

/*	calculates the length of a vector represented by a t_vec struct,
	by summing the squares of the components of the vector and taking the square root.
*/
float	vec3_length(t_vec *v)
{
	return (sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));
}

/*	calculates the squared length of a vector represented by a t_vec struct,
	by summing the squares of the components of the vector.
*/
float	vec3_length_squared(t_vec *v)
{
	return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

/* 	Normalizes a vector represented by a t_vec struct.
	Returns a new t_vec struct representing the normalized vector.
*/
t_vec	vec3_unit_vector(t_vec *v)
{
	float	length;

	length = sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
	return ((t_vec){(v->x / length), (v->y / length), (v->z / length)});
}

/* Subtracts two vectors represented by t_vec structs.
	Returns a new t_vec struct representing the result of the subtraction.
*/
t_vec	vec3_subtract(t_vec a, t_vec b)
{
	return ((t_vec){(a.x - b.x), (a.y - b.y), (a.z - b.z)});
}

/* Adds two vectors represented by t_vec structs.
	Returns a new t_vec struct representing the result of the addition.
*/
t_vec	vec3_add(t_vec a, t_vec b)
{
	return ((t_vec){(a.x + b.x), (a.y + b.y), (a.z + b.z)});
}

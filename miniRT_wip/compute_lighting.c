/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_lighting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:32:34 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/08 16:11:36 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	c_side_normal(t_hit *hit, t_ray *ray)
{
	t_cyl	*cyl;
	t_vec	co;
	float	m;
	t_vec	point;
	t_vec	temp;
	t_vec	res;

	cyl = hit->objs->obj;
	co = vec3_subtract(ray->origin, cyl->top_p);
	point = vec_multiply(&ray->dir, hit->t);
	temp = vec3_add(vec_multiply(&ray->dir, hit->t), (const t_vec) co);
	m = dot(&temp, &cyl->normal);
	res = vec3_subtract(point, vec3_subtract(cyl->top_p, vec_multiply(&cyl->normal, m)));
	return (vec3_unit_vector(&res));
}

t_vec	cyl_normal(t_hit *hit, t_ray *ray)
{
	t_cyl	*cyl;

	cyl = hit->objs->obj;
	if (hit->c_part == 1)
		return (cyl->top.normal);
	else if (hit->c_part == 2)
		return (cyl->bottom.normal);
	else
		return (c_side_normal(hit, ray));
}

float	get_light_i(t_vec light, t_vec normal, float ratio)
{
	float	res;
	float	l_dot_n;

	l_dot_n = dot(&normal, &light);
	if (l_dot_n < 0)
		l_dot_n = 0;
	res = ratio * l_dot_n / (vec3_length(&normal) * vec3_length(&light));
	return (res);
}

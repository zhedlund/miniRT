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
	float	temp;
	t_vec	res;

	cyl = hit->objs->obj;
	co = vec3_subtract(ray->origin, cyl->top_p);
	point = vec3_add(ray->origin, vec_multiply(&ray->dir, hit->t));
	temp = dot(&ray->dir, &cyl->normal) * hit->t;
	m = temp + dot(&co, &cyl->normal);
	res = vec3_subtract(vec3_subtract(point, cyl->top_p), vec_multiply(&cyl->normal, m));
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

t_color	hitobject_color(t_hit *hit)
{
	if (hit->objs->id == SPHERE)
		return (((t_sph *)(hit->objs->obj))->color);
	else if (hit->objs->id == CYLINDER)
		return (((t_cyl *)(hit->objs->obj))->color);
	else
		return (((t_plane *)(hit->objs->obj))->color);
}

t_color	light_pixel(float l_dot_n, t_vec *light_r, t_hit *hit, t_scene *scene)
{
	t_color	color;
	float	light_i;

	color = hitobject_color(hit);
	if (l_dot_n < 0)
		l_dot_n = 0;
	light_i = l_dot_n / vec3_length(light_r);
	light_i = light_i * scene->l.ratio + scene->a.ratio;
	color.r = color.r * light_i;
	color.g = color.g * light_i;
	color.b = color.b * light_i;
	return (color);
}

t_color	shadow_pixel(float shadow_t, t_hit *hit, t_scene *scene)
{
	t_color	color;
	float	shadow_i;

	color = hitobject_color(hit);
	shadow_i = scene->a.ratio + shadow_t / 100;
	color.r = color.r * shadow_i;
	color.g = color.g * shadow_i;
	color.b = color.b * shadow_i;
	return (color);
}

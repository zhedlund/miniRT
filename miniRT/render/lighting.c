/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_lighting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:32:34 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/21 22:00:47 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_vec	c_side_normal(t_hit *hit, t_ray *ray)
{
	t_cyl	*cyl;
	t_vec	co;
	t_vec	point;
	float	temp;
	t_vec	res;

	cyl = hit->objs->obj;
	co = vec3_subtract(ray->origin, cyl->top_p);
	point = vec3_add(ray->origin, vec_multiply(&ray->dir, hit->t));
	temp = dot(&ray->dir, &cyl->normal) * hit->t;
	res = vec3_subtract(vec3_subtract(point, cyl->top_p),
			vec_multiply(&cyl->normal, temp + dot(&co, &cyl->normal)));
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
	t_color	a_color;
	t_color	l_color;
	float	light_i;

	color = hitobject_color(hit);
	a_color = scene->a.color;
	if (l_dot_n < 0)
		l_dot_n = 0;
	light_i = l_dot_n / vec3_length(light_r);
	a_color.r = a_color.r * scene->a.ratio * color.r;
	a_color.g = a_color.g * scene->a.ratio * color.g;
	a_color.b = a_color.b * scene->a.ratio * color.b;
	l_color = diffuse_color(&scene->l, &color, light_i);
	return (blend_color(&a_color, &l_color));
}

t_color	shadow_pixel(t_hit *hit, t_scene *scene)
{
	t_color	color;
	float	shadow_i;

	color = hitobject_color(hit);
	shadow_i = scene->a.ratio;
	color.r = color.r * shadow_i;
	color.g = color.g * shadow_i;
	color.b = color.b * shadow_i;
	return (color);
}

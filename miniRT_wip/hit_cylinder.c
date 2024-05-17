/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:46:37 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/17 23:11:19 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	hit_top(t_cyl *cyl, t_ray *ray)
{
	float	t;
	t_vec	p;
	t_vec	cp;

	t = hit_plane((t_plane *)&cyl->top, (t_ray *) ray);
	if (t < 0)
		return (-1);
	p = vec3_add((t_vec)ray->origin, vec_multiply(&ray->dir, t));
	cp = vec3_subtract((t_vec)p, (t_vec)cyl->top.point);
	if (vec3_length(&cp) > cyl->r)
		return (-1);
	return (t);
}

float	hit_bottom(t_cyl *cyl, t_ray *ray)
{
	float	t;
	t_vec	p;
	t_vec	cp;

	t = hit_plane((t_plane *)&cyl->bottom, (t_ray *) ray);
	if (t < 0)
		return (-1);
	p = vec3_add((t_vec)ray->origin, vec_multiply(&ray->dir, t));
	cp = vec3_subtract((t_vec)p, (t_vec)cyl->bottom.point);
	if (vec3_length(&cp) > cyl->r)
		return (-1);
	return (t);
}

// int	is_valid(float t, t_cyl *cyl, t_ray *ray, t_vec co)
// {
// 	float	m;
// 	t_vec	temp;

// 	if (t < 0)
// 		return (0);
// 	temp = vec3_add(vec_multiply(&ray->dir, t), (t_vec) co);
// 	m = dot(&temp, &cyl->normal);
// 	if (m < 0 || m > cyl->h)
// 		return (0);
// 	m = dot(&ray->dir, &cyl->normal) * t + dot(&co, &cyl->normal);
// 	if (m < 0 || m > cyl->h)
// 		return (0);
// 	return (1);
// }

float	is_valid(float t, t_cyl *cyl, t_ray *ray, t_vec co)
{
	float	m;
	t_vec	temp;

	if (t < 0)
		return (-1);
	temp = vec3_add(vec_multiply(&ray->dir, t), (t_vec) co);
	m = dot(&temp, &cyl->normal);
	if (m < 0 || m > cyl->h)
		return (-1);
	m = dot(&ray->dir, &cyl->normal) * t + dot(&co, &cyl->normal);
	if (m < 0 || m > cyl->h)
		return (-1);
	return (t);
}

float	hit_side(t_cyl *cyl, t_ray *ray, t_vec co)
{
	float	t[2];
	float	a;
	float	b;
	float	c;
	float	d;

	a = dot(&ray->dir, &ray->dir) - powf(dot(&ray->dir, &cyl->normal), 2);
	b = 2 * (dot(&ray->dir, &co) - dot(&ray->dir, &cyl->normal) * dot(&co, &cyl->normal));
	c = dot(&co, &co) - powf(dot(&co, &cyl->normal), 2) - powf(cyl->r, 2);
	d = b * b - (4 * a * c);
	if (d < 0)
		return (-1);
	t[0] = (-b + sqrtf(d)) / (2 * a);
	t[1] = (-b - sqrtf(d)) / (2 * a);
	t[0] = is_valid(t[0], cyl, ray, co);
	t[1] = is_valid(t[1], cyl, ray, co);
	if (t[0] < 0)
		return (t[1]);
	if (t[1] < 0)
		return (t[0]);
	if (t[0] < t[1])
		return (t[0]);
	else
		return (t[1]);
}

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
// t_hit hit_cylinder(t_obj *obj, t_ray *ray){ if hit return (hit{t, obj, c_part})}
float	hit_cylinder(t_cyl *cyl, t_ray *ray, t_hit *hit)
{
	float	t[3];
	int		res;
	t_vec	co;

	co = vec3_subtract(ray->origin, cyl->top_p);
	t[0] = hit_top(cyl, ray);
	t[1] = hit_bottom(cyl, ray);
	t[2] = hit_side(cyl, ray, co);
	res = get_min(t);
	if (res < 0)
		return (-1);
	if (hit && t[res] < hit->t)
		hit->c_part = res + 1;
	return (t[res]);
}

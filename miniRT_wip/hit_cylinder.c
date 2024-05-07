/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:46:37 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/02 21:16:51 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	hit_top(t_cyl *cyl, t_ray *ray)
{
	float	t;
	t_vec	p;
	t_vec	cp;

	t = hit_plane((const t_plane *)&cyl->top, (const t_ray *) ray);
	if (t < 0)
		return (-1);
	p = vec3_add((const t_vec)ray->origin, vec_multiply(&ray->dir, t));
	cp = vec3_subtract((const t_vec)p, (const t_vec)cyl->top.point);
	if (vec3_length(&cp) > cyl->r)
		return (-1);
	return (t);
	// if (hit->t < 0 || (hit->t > 0 && t < hit->t))
	// {
	// 	hit->t = t;
	// 	hit->c_part = 1;
	// }
}

float	hit_bottom(t_cyl *cyl, t_ray *ray)
{
	float	t;
	t_vec	p;
	t_vec	cp;

	t = hit_plane((const t_plane *)&cyl->bottom, (const t_ray *) ray);
	if (t < 0)
		return (-1);
	p = vec3_add((const t_vec)ray->origin, vec_multiply(&ray->dir, t));
	cp = vec3_subtract((const t_vec)p, (const t_vec)cyl->bottom.point);
	if (vec3_length(&cp) > cyl->r)
		return (-1);
	return (t);
	// if (hit->t < 0 || (hit->t > 0 && t < hit->t))
	// {
	// 	hit->t = t;
	// 	hit->c_part = 2;
	// }
}

int	is_valid(float t, t_cyl *cyl, t_ray *ray, t_vec co)
{
	float	m;
	t_vec	temp;

	if (t < 0)
		return (0);
	temp = vec3_add(vec_multiply(&ray->dir, t), (const t_vec) co);
	m = dot(&temp, &cyl->normal);
	if (m < 0 || m > cyl->h)
		return (0);
	m = dot(&ray->dir, &cyl->normal) * t + dot(&co, &cyl->normal);
	if (m < 0 || m > cyl->h)
		return (0);
	return (1);
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
	if (is_valid(t[0], cyl ,ray, co) && t[0] <= t[1])
		return (t[0]);
	else if (is_valid(t[1], cyl ,ray, co) && t[1] < t[0])
		return (t[1]);
	else
		return (-1);
}

// t_hit hit_cylinder(t_obj *obj, t_ray *ray){ if hit return (hit{t, obj, c_part})}
float	hit_cylinder(t_cyl *cyl, t_ray *ray, t_hit *hit)
{
	//t_hit	hit;
	float	t;
	float	res;
	t_vec	co;

	//hit = (t_hit){-1, 0, (t_obj *)cyl};
	res = -1;
	co = vec3_subtract(ray->origin, cyl->top_p);
	t = hit_top(cyl, ray);
	if (t > 0)
	{
		res = t;
		hit->c_part = 1;
	}
	t = hit_bottom(cyl, ray);
	if (t > 0 && (t < res || res < 0))
	{
		res = t;
		hit->c_part = 2;
	}
	t = hit_side(cyl, ray, co);
	if (t > 0 && (t < res || res < 0))
	{
		res = t;
		hit->c_part = 3;
	}
	return (res);
}

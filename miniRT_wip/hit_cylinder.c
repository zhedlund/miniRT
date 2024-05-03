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

void	set_top_point(t_cyl *cyl)
{
	cyl->top_p.x = cyl->center.x - cyl->normal.x * (cyl->h / 2);
	cyl->top_p.y = cyl->center.y - cyl->normal.y * (cyl->h / 2);
	cyl->top_p.z = cyl->center.z - cyl->normal.z * (cyl->h / 2);
}

// void	hit_top(t_cyl *cyl, t_ray *ray, t_hit *hit)
// {
// 	float	t;

// 	return ;
// }

// void	hit_bottom(t_cyl *cyl, t_ray *ray, t_hit *hit)
// {
// 	float	t;

// 	return ;
// }
t_vec	vec_multiply(const t_vec *vec, float n)
{
	return ((const t_vec){vec->x * n, vec->y * n, vec->z * n});
}

int	is_valid(float t, const t_cyl *cyl, const t_ray *ray)
{
	float	m;
	t_vec	oc;
	t_vec	temp;

	oc = vec3_subtract(ray->origin, cyl->top_p);
	temp = vec3_add(vec_multiply(&ray->dir, t), (const t_vec) oc);
	m = dot(&temp, &cyl->normal);
	if (m < 0 || m > cyl->h)
		return (0);
	m = dot(&ray->dir, &cyl->normal) * t + dot(&oc, &cyl->normal);
	if (m < 0 || m > cyl->h)
		return (0);
	return (1);
}

void	hit_side(const t_cyl *cyl, const t_ray *ray, t_hit *hit)
{
	float	t[2];
	t_vec	oc;
	float	a;
	float	b;
	float	c;
	float	d;

	oc = vec3_subtract(ray->origin, cyl->top_p);
	a = dot(&ray->dir, &ray->dir) - powf(dot(&ray->dir, &cyl->normal), 2);
	b = 2 * (dot(&ray->dir, &oc) - dot(&ray->dir, &cyl->normal) * dot(&oc, &cyl->normal));
	c = dot(&oc, &oc) - powf(dot(&oc, &cyl->normal), 2) - powf(cyl->r, 2);
	d = b * b - (4 * a * c);
	if (d < 0)
		return ;
	t[0] = (-b + sqrtf(d)) / (2 * a);
	t[1] = (-b - sqrtf(d)) / (2 * a);
	if (t[0] > 0 && is_valid(t[0], cyl ,ray) && t[0] <= t[1])
		hit->t = t[0];
	else if (t[1] > 0 && is_valid(t[1], cyl ,ray) && t[1] < t[0])
		hit->t = t[1];
	else
		return ;
	hit->c_part = 3;
}

float	hit_cylinder(const t_cyl *cyl, const t_ray *ray)
{
	t_hit	hit;

	hit = (t_hit){-1, 0, (t_obj *)cyl};
	set_top_point((t_cyl *)cyl);
	//hit_top(cyl, ray, &hit);
	//hit_bottom(cyl, ray, &hit);
	hit_side(cyl, ray, &hit);
	return (hit.t);
}

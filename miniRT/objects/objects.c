/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:13 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/21 22:00:12 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_vec	sphere_normal(t_sph *sp, t_vec *intersect)
{
	return ((t_vec){(intersect->x - sp->center.x) / sp->radius,
		(intersect->y - sp->center.y) / sp->radius,
		(intersect->z - sp->center.z) / sp->radius});
}

/* 	Calculates the point of intersection between a ray and a sphere.
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray does not intersect the sphere.
*/
float	hit_sphere(t_vec *center, float radius, t_ray *r)
{
	t_vec	oc;
	float	a;
	float	half_b;
	float	discriminant;
	float	t[2];

	oc = vec3_subtract(r->origin, *center);
	a = vec3_length_squared(&r->dir);
	half_b = dot(&oc, &r->dir);
	discriminant = half_b * half_b
		- a * (vec3_length_squared(&oc) - radius * radius);
	if (discriminant < 0)
		return (-1.0);
	t[0] = (-half_b - sqrt(discriminant)) / a;
	t[1] = (-half_b + sqrt(discriminant)) / a;
	if (t[0] < 0)
		return (t[1]);
	if (t[1] < 0)
		return (t[0]);
	if (t[0] < t[1])
		return (t[0]);
	else
		return (t[1]);
}

/*	Calculates the point of intersection between a ray and a plane.
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray is parallel to the plane,
	or the intersection point is behind the ray origin.
*/
float	hit_plane(t_plane *pl, t_ray *r)
{
	float	denominator;
	t_vec	origin_to_point;
	float	t;

	denominator = dot(&pl->normal, &r->dir);
	if (denominator == 0)
		return (-1.0);
	origin_to_point = (t_vec){(pl->point.x - r->origin.x),
		(pl->point.y - r->origin.y),
		(pl->point.z - r->origin.z)};
	t = dot(&origin_to_point, &pl->normal) / denominator;
	if (t < 0)
		return (-1.0);
	return (t);
}

float	hit_object(t_obj *obj, t_ray *r, t_hit *hit)
{
	float	t;

	t = -1.0;
	if (obj->id == SPHERE)
		t = hit_sphere(&((t_sph *)(obj->obj))->center,
				((t_sph *)(obj->obj))->radius, r);
	else if (obj->id == PLANE)
		t = hit_plane((t_plane *)(obj->obj), r);
	else if (obj->id == CYLINDER)
		t = hit_cylinder((t_cyl *)(obj->obj), (t_ray *)r, hit);
	return (t);
}

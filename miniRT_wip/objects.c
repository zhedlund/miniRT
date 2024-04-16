/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:13 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/16 14:43:25 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec sphere_normal(const t_sph *sp, const t_vec *intersect)
{
    return ((t_vec){(intersect->x - sp->center.x) / sp->radius,
                     (intersect->y - sp->center.y) / sp->radius,
                     (intersect->z - sp->center.z) / sp->radius});
}

/* 	Calculates the point of intersection between a ray and a sphere.
	Stores the intersection points in the sphere struct.
*/
float hit_sphere(const t_vec *center, float radius, const t_ray *r)
{
    t_vec oc = vec3_subtract(r->origin, *center);
	float a = vec3_length_squared(&r->dir); // 
    float half_b = dot(&oc, &r->dir); // half dot product of vector oc and the direction vector of the ray.
    float c = vec3_length_squared(&oc) - radius * radius;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return (-1.0);
    else
        return ((-half_b - sqrt(discriminant)) / a);
}

/* 	Calculates the point of intersection between a ray and a plane.
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray is parallel to the plane, or the intersection point is behind the ray origin.
*/
float hit_plane(const t_plane *pl, const t_ray *r)
{
    float denominator = dot(&pl->normal, &r->dir);
    if (denominator == 0)
        return (-1.0); // Ray is parallel to the plane
    t_vec origin_to_point = {
		(pl->point.x - r->origin.x),
		(pl->point.y - r->origin.y),
		(pl->point.z - r->origin.z)
		};
    float t = dot(&origin_to_point, &pl->normal) / denominator;
    if (t < 0)
        return (-1.0); // Intersection point is behind the ray origin
    return (t);
}
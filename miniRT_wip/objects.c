/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:13 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/15 17:34:13 by zhedlund         ###   ########.fr       */
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
double hit_sphere(const point3 *center, double radius, const ray *r)
{
    t_vec oc = vec3_subtract(r->origin, *center);
	double a = vec3_length_squared(&r->dir); // 
    double half_b = dot(&oc, &r->dir); // half dot product of vector oc and the direction vector of the ray.
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return (-1.0);
    else
        return ((-half_b - sqrt(discriminant)) / a);
}

/* 	Calculates the point of intersection between a ray and a plane.
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray is parallel to the plane, or the intersection point is behind the ray origin.
*/
double hit_plane(const plane *pl, const ray *r)
{
    double denominator = dot(&pl->normal, &r->dir);
    if (denominator == 0)
        return (-1.0); // Ray is parallel to the plane
    t_vec origin_to_point = {
		(pl->point.x - r->origin.x),
		(pl->point.y - r->origin.y),
		(pl->point.z - r->origin.z)
		};
    double t = dot(&origin_to_point, &pl->normal) / denominator;
    if (t < 0)
        return (-1.0); // Intersection point is behind the ray origin
    return (t);
}
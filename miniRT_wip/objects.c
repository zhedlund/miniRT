/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:13 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/27 19:24:46 by zhedlund         ###   ########.fr       */
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
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray does not intersect the sphere.
*/
float hit_sphere(const t_vec *center, float radius, const t_ray *r)
{
    t_vec oc;
	float a; 
    float half_b; // half dot product of vector oc and the direction vector of the ray.
    float c; 
    float discriminant;

	oc = vec3_subtract(r->origin, *center);
	a = vec3_length_squared(&r->dir);
	half_b = dot(&oc, &r->dir);
	c = vec3_length_squared(&oc) - radius * radius;
	discriminant = half_b * half_b - a * c;
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
    float denominator;
	t_vec origin_to_point;
	float t;
	
	denominator = dot(&pl->normal, &r->dir);
    if (denominator == 0)
        return (-1.0); // Ray is parallel to the plane
    origin_to_point = (t_vec){(pl->point.x - r->origin.x),
							(pl->point.y - r->origin.y),
							(pl->point.z - r->origin.z)};
    t = dot(&origin_to_point, &pl->normal) / denominator;
    if (t < 0)
        return (-1.0); // Intersection point is behind the ray origin
    return (t);
}

float	hit_object(const t_obj *obj, const t_ray *r)
{
	float t;
	
	t = -1.0;
	if (obj->id == SPHERE)
		t = hit_sphere(&((const t_sph*)(obj->obj))->center, ((const t_sph*)(obj->obj))->radius, r);
	else if (obj->id == PLANE)
		t = hit_plane((const t_plane*)(obj->obj), r);
	//else if (obj->id == CYLINDER)
		//t = hit_cylinder((const t_cyl*)(obj->obj), r);
	return (t);
}

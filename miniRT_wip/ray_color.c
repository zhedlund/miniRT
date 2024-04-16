/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/16 14:45:18 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* 	Calculates the color of a pixel based on the intersection of a ray with a sphere or plane.
	Returns a color value based on the normal vector at the intersection point.
*/
t_color ray_color(const t_ray *r, const t_sph *sp, const t_plane *pl, const t_scene *scene)
{
    float	ts = hit_sphere(&sp->center, sp->radius, r);
    float	tp = hit_plane(pl, r);
    t_color	px;

	if (ts > 0.0 && (tp < 0 || ts < tp)) // Intersection with the sphere
	{ 
        // Calculate intersection point and normal vector
		t_vec intersect = intersect_point(r, ts);
        t_vec normal = sphere_normal(sp, &intersect);
		t_color ambient_light = ambient_color(&scene->a, &sp->color);

        // Diffuse lighting for shadow effect
        t_vec light_dir = vec3_unit_vector(&scene->l.pos);
        float diffuse_factor = dot(&light_dir, &normal);
		t_color diffuse = diffuse_color(&scene->l, &sp->color, diffuse_factor);
        px = blend_color(&ambient_light, &diffuse); // add ambient and diffuse components
    } 
    else if (tp > 0) // Intersection with the plane
    {
        t_vec intersect_plane = intersect_point(r, tp);
        // Check if the sphere is blocking the light
        t_vec temp = vec3_subtract(scene->l.pos, intersect_plane);
		t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source

        t_ray shadow_ray = {intersect_plane, to_light};
        float t_shadow_sphere = hit_sphere(&sp->center, sp->radius, &shadow_ray);
        if (t_shadow_sphere > 0 && t_shadow_sphere < 1.0) // Sphere is blocking the light
		{
            px = (t_color){0.0, 0.3, 0.2}; // shadow color
		}
		else // No shadow
        {
            t_color ambient_light = ambient_color(&scene->a, &pl->color); // Ambient lighting for plane
            px = ambient_light; // Plane pixel color with ambient component
        }
    }
    else // No intersection with the plane
    {
        px = (t_color){0.5, 0.7, 1.0}; // Background color
    }
    return (px);
}
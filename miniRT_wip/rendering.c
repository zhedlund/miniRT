/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/16 22:11:51 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// will remove parameter sp & pl, when objects are added to scene struct
void	create_image(t_cam *cam, t_ray *ray, t_data *data, t_sph sp, t_plane pl, t_scene scene) 
{
	t_vec	px_center;
	t_vec 	ray_dir;
	t_color	px_color;
	
	int j = 0;
	while (j < HEIGHT)
	{
    	int i = 0;
    	while (i < WIDTH)
	    {
    	    // calculates position of the current pixel
        	px_center = (t_vec){cam->px_00.x + (i * cam->px_delta_u.x),
            					cam->px_00.y + (j * cam->px_delta_v.y),
        						cam->px_00.z};
		 	// calculates direction based on the pixel's position and cam center
        	ray_dir = (t_vec){px_center.x - cam->center.x,
            					px_center.y - cam->center.y,
            					px_center.z - cam->center.z};
			// Rotate the ray direction according to the camera orientation
        	//ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction
        	//ray_dir.x += cam.orientation.x;
			//ray_dir.y += cam.orientation.y;
			//ray_dir.z += cam.orientation.z;

			//ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction again
			ray = &(t_ray){cam->center, ray_dir};

        	// calculate pixel color and write to image buffer
        	px_color = ray_color(ray, &sp, &pl, &scene);
        	write_color(px_color, &data->img, i, j);
			i++;
    	}
    	j++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0); //display image
}


/* 	Calculates the color of a pixel based on the intersection of a ray with a sphere or plane.
	Returns a color value based on the normal vector at the intersection point.
*/
t_color ray_color(const t_ray *r, const t_sph *sp, const t_plane *pl, const t_scene *scene)
{
    float	ts = hit_sphere(&sp->center, sp->radius, r);
    float	tp = hit_plane(pl, r);
    t_color	px;

	if (ts > 0) //&& (tp < 0 || ts < tp)) // Intersection with the sphere
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
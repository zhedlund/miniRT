/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/17 22:57:04 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int render_image(t_data *data)
{
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

void	create_image(t_cam *cam, t_ray *ray, t_data *data, t_scene scene) 
{
	t_vec	px_center;
	t_vec 	ray_dir;
	t_color	px_color;
	int 	i;
	int 	j;

	j = 0;
	while (j < HEIGHT)
	{
    	i = 0;
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
        	px_color = ray_color(ray, &scene);
        	write_color(px_color, &data->img, i, j);
			i++;
    	}
    	j++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0); //display image
}

void print_object_list(const t_obj *head);
const char* shape_to_string(t_shape shape);

/* 	Calculates the color of a pixel based on the intersection of a ray with a sphere or plane.
	Returns a color value based on the normal vector at the intersection point.
*/
/*t_color ray_color(const t_ray *r, const t_sph *sp, const t_plane *pl, const t_scene *scene)
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
}*/


/*t_color ray_color(const t_ray *r, const t_scene *scene)
{
    float ts = hit_sphere(&((const t_sph*)(scene->objs->obj))->center, ((const t_sph*)(scene->objs->obj))->radius, r);
    float tp = hit_plane((const t_plane*)(scene->objs->next->obj), r);
    t_color px;

	px = (t_color){0.5, 0.7, 1.0}; // Background color
    if (ts > 0 && (tp < 0 || ts < tp)) // Intersection with the sphere
    { 
        // Calculate intersection point and normal vector
        t_vec intersect = intersect_point(r, ts);
        t_vec normal = sphere_normal((const t_sph*)(scene->objs->obj), &intersect);
        t_color ambient_light = ambient_color(&scene->a, &((const t_sph*)(scene->objs->obj))->color);

        // Diffuse lighting for shadow effect
        t_vec light_dir = vec3_unit_vector(&scene->l.pos);
        float diffuse_factor = dot(&light_dir, &normal);
        t_color diffuse = diffuse_color(&scene->l, &((const t_sph*)(scene->objs->obj))->color, diffuse_factor);
        px = blend_color(&ambient_light, &diffuse); // add ambient and diffuse components
    } 
    else if (tp > 0) // Intersection with the plane
    {
        t_vec intersect_plane = intersect_point(r, tp);
        // Check if the sphere is blocking the light
        t_vec temp = vec3_subtract(scene->l.pos, intersect_plane);
        t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source

        t_ray shadow_ray = {intersect_plane, to_light};
        float t_shadow_sphere = hit_sphere(&((const t_sph*)(scene->objs->obj))->center, ((const t_sph*)(scene->objs->obj))->radius, &shadow_ray);
        if (t_shadow_sphere > 0 && t_shadow_sphere < 1.0) // Sphere is blocking the light
        {
            px = (t_color){0.0, 0.3, 0.2}; // shadow color
        }
        else // No shadow
        {
            t_color ambient_light = ambient_color(&scene->a, &((const t_plane*)(scene->objs->next->obj))->color); // Ambient lighting for plane
            px = ambient_light; // Plane pixel color with ambient component
        }
    }
    return (px);
}*/


t_hit_point *calculate_intersection(const t_ray *r, const t_scene *scene)
{
    t_hit_point	*head;
	t_obj		*current;
	float		t;
	
	head = NULL;
	current = scene->objs;
    while (current != NULL)
    {
        t = -1.0; //t-value for current object
        if (current->id == SPHERE)
            t = hit_sphere(&((const t_sph *)(current->obj))->center, ((const t_sph *)(current->obj))->radius, r);
        else if (current->id == PLANE)
            t = hit_plane((const t_plane *)(current->obj), r);

        // If t is positive, create new t_hit_point and add to list
        if (t > 0)
        {
            t_hit_point *new_hit_point = malloc(sizeof(t_hit_point));
            if (!new_hit_point)
                return (NULL);
            new_hit_point->t = t;
            new_hit_point->obj = current;
            new_hit_point->next = head;
            head = new_hit_point;
        }
        current = current->next; // Move to the next object in the list
    }
    return (head); // Return the head of the linked list
}

// Function to check if a point is in shadow
int in_shadow(const t_vec *point, const t_light *light, const t_obj *objs)
{
    // Cast shadow ray from point to light source
    t_vec to_light = vec3_subtract(light->pos, *point);
    t_ray shadow_ray = {*point, vec3_unit_vector(&to_light)};
    const t_obj *current;
	float t;
	
	current = objs;
    while (current != NULL)
    {
        if (current->id == SPHERE)
        {
            t = hit_sphere(&((const t_sph*)(current->obj))->center, ((const t_sph*)(current->obj))->radius, &shadow_ray);
            if (t > 0 && t < 1.0) // Intersection with sphere
                return (1); // Point is in shadow
        }
        else if (current->id == PLANE)
        {
            t = hit_plane((const t_plane*)(current->obj), &shadow_ray);
            if (t > 0 && t < 1.0) // Intersection with plane
                return (1); // Point is in shadow
        }
        current = current->next;
    }
    return (0); // Point is not in shadow
}

t_color ray_color(const t_ray *r, const t_scene *scene)
{
	t_color px = (t_color){0.5, 0.7, 1.0}; // Background color
    t_hit_point *hit_points = calculate_intersection(r, scene);
    if (!hit_points)
        return (t_color){0.5, 0.7, 1.0}; // Background color
	
    // Find the smallest positive t-value
    float min_t = FLT_MAX;
    t_hit_point *min_hit_point = NULL;
    t_hit_point *current = hit_points;
    while (current != NULL)
    {
        if (current->t < min_t)
        {
            min_t = current->t;
            min_hit_point = current;
        }
        current = current->next;
    }
    if (min_hit_point->obj->id == SPHERE)
    {
        // Calculate intersection point and normal vector for sphere
        t_vec intersect = intersect_point(r, min_hit_point->t);
        t_vec normal = sphere_normal((const t_sph *)(min_hit_point->obj->obj), &intersect);
        t_color ambient_light = ambient_color(&scene->a, &((const t_sph *)(min_hit_point->obj->obj))->color);

        // Diffuse lighting for shadow effect
        t_vec light_dir = vec3_unit_vector(&scene->l.pos);
        float diffuse_factor = dot(&light_dir, &normal);
        t_color diffuse = diffuse_color(&scene->l, &((const t_sph *)(min_hit_point->obj->obj))->color, diffuse_factor);
        px = blend_color(&ambient_light, &diffuse); // add ambient and diffuse components
    }
    else if (min_hit_point->obj->id == PLANE)
    {
		t_vec intersect_plane = intersect_point(r, min_hit_point->t);
        // Check if the sphere is blocking the light
        t_vec temp = vec3_subtract(scene->l.pos, intersect_plane);
        t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source
        t_ray shadow_ray = {intersect_plane, to_light};
        float t_shadow_sphere = hit_sphere(&((const t_sph*)(scene->objs->obj))->center, ((const t_sph*)(scene->objs->obj))->radius, &shadow_ray);
        if (t_shadow_sphere > 0 && t_shadow_sphere < 1.0) // Sphere is blocking the light
        {
            px = (t_color){0.0, 0.3, 0.2}; // shadow color
        }

        /*t_vec intersect_plane = intersect_point(r, min_hit_point->t);
        if (in_shadow(&intersect_plane, &scene->l, scene->objs)) {
            px = (t_color){0.0, 0.3, 0.2}; // shadow color
        }*/ else {
            t_color ambient_light = ambient_color(&scene->a, &((const t_plane*)(scene->objs->next->obj))->color); // Ambient lighting for plane
            px = ambient_light; // Plane pixel color with ambient component
        }
    }
    // Free memory allocated for hit points
    while (hit_points != NULL)
    {
        t_hit_point *temp = hit_points;
        hit_points = hit_points->next;
        free(temp);
    }
    return (px);
}

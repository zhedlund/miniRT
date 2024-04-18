/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/18 18:16:03 by zhedlund         ###   ########.fr       */
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

// Function to check if a point is in shadow
/*int in_shadow(const t_vec *point, const t_light *light, const t_obj *objs)
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
}*/

t_hit *hit_object(const t_ray *r, const t_scene *scene)
{
    t_hit	*head;
	t_obj	*current;
	float	t;
	
	head = NULL;
	current = scene->objs;
    while (current != NULL)
    {
        t = -1.0; //t-value for current object
        if (current->id == SPHERE)
            t = hit_sphere(&((const t_sph *)(current->obj))->center, ((const t_sph *)(current->obj))->radius, r);
        else if (current->id == PLANE)
            t = hit_plane((const t_plane *)(current->obj), r);
        if (t > 0) // If t is positive, create new t_hit obj and add to list
        {
            t_hit *hit_obj = malloc(sizeof(t_hit));
            if (!hit_obj)
                return (NULL);
            hit_obj->t = t;
            hit_obj->objs = current;
            hit_obj->next = head;
            head = hit_obj;
        }
        current = current->next;
    }
    return (head); // Return the head of the linked list
}

void	find_closest_obj(t_hit *hit_point, t_hit **hitlist)
{
	t_hit	*current;
	float	min_t;

	*hitlist = NULL;
	min_t = FLT_MAX;
	current = hit_point;
	while (current != NULL) 
	{
		if (current->t < min_t) 
		{
			min_t = current->t;
			*hitlist = current;
		}
		current = current->next;
	}
}

t_color diffuse_lighting(t_color *px, const t_light *light, const t_vec *normal)
{
	t_vec light_dir = vec3_unit_vector(&light->pos);
	float diffuse_factor = dot(&light_dir, normal);
	t_color diffuse = diffuse_color(light, px, diffuse_factor);
	*px = blend_color(px, &diffuse); // add diffuse component
	return (*px);
}

float	shadow_ray(t_hit *hitlist, const t_light *light, const t_ray *r)
{
	t_vec intersect = intersect_point(r, hitlist->t);
	t_vec to_light = vec3_unit_vector(&light->pos); // Direction from plane to light source
	t_ray shadow_ray = {intersect, to_light};
	float t_shadow = hit_sphere(&((const t_sph *)(hitlist->objs->obj))->center, ((const t_sph *)(hitlist->objs->obj))->radius, &shadow_ray);
	return (t_shadow);
}

t_color ray_color(const t_ray *r, const t_scene *scene)
{
	t_color px;
	t_hit	*hit_point;
	t_hit	*hitlist;
	t_vec	intersect;

	px = (t_color){0.5, 0.7, 1.0}; // Background color
	hit_point = hit_object(r, scene);
    if (!hit_point)
		return (t_color){0.5, 0.7, 1.0}; // Background color
	find_closest_obj(hit_point, &hitlist);
	if (!hitlist)
		free_hit_list(hit_point);
	//float t_shadow = shadow_ray(hitlist, &scene->l, r);
	intersect = intersect_point(r, hitlist->t);
	t_vec temp = vec3_subtract(scene->l.pos, intersect);
    t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source
    t_ray shadow_ray = {intersect, to_light};
    
	if (hitlist->objs->id == SPHERE)
    {
        t_vec normal = sphere_normal((const t_sph *)(hitlist->objs->obj), &intersect);
		t_color amb_light = ambient_color(&scene->a, &((const t_sph *)(hitlist->objs->obj))->color);
		px = diffuse_lighting(&amb_light, &scene->l, &normal); //shadow effect
    }
    else if (hitlist->objs->id == PLANE)
    {
		//intersect = intersect_point(r, hitlist->t);
        
		// Check if the sphere is blocking the light
       //t_vec temp = vec3_subtract(scene->l.pos, intersect);
        //t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source
        //t_ray shadow_ray = {intersect, to_light};
        float t_shadow = hit_sphere(&((const t_sph *)(scene->objs->obj))->center, ((const t_sph*)(scene->objs->obj))->radius, &shadow_ray);
		
		if (t_shadow > 0 && t_shadow < 1.0) // Sphere is blocking the light
        {
            px = (t_color){0.0, 0.3, 0.2}; // shadow color
        }

        //if (in_shadow(&intersect_plane, &scene->l, scene->objs))
		//{ px = (t_color){0.0, 0.3, 0.2}; // shadow color}
		else
		{
            t_color amb_light = ambient_color(&scene->a, &((const t_plane*)(scene->objs->obj))->color); // Ambient lighting for plane
            px = amb_light; // Plane pixel color with ambient component
        }
    }
    free_hit_list(hit_point);
    return (px);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/27 20:14:11 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int render_image(t_data *data)
{
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

/* casts a ray for each pixel in the image */
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

t_hit *add_hit_obj(const t_ray *r, const t_scene *scene)
{
    t_hit	*head;
	t_obj	*current;
	float	t;
	t_hit	*hit;
	
	head = NULL;
	current = scene->objs;
    while (current != NULL)
    {
		t = hit_object(current, r);
        if (t > 0) // If t is positive, create list + add obj
        {
            hit = malloc(sizeof(t_hit));
            if (!hit)
                return (NULL);
            hit->t = t;
            hit->objs = current;
            hit->next = head;
            head = hit;		
        }
        current = current->next;
    }
    return (head);
}

void	find_closest_obj(t_hit *obj, t_hit **hit)
{
	t_hit	*current;
	float	min_t;

	*hit = NULL;
	min_t = FLT_MAX;
	current = obj;
	while (current != NULL) 
	{
		if (current->t < min_t) 
		{
			min_t = current->t;
			*hit = current;
		}
		current = current->next;
	}
}

t_color diffuse_lighting(t_color *px, const t_light *light, const t_vec *normal)
{
	t_vec	light_dir;
	float	diffuse_factor;
	t_color	diffuse;

	light_dir = vec3_unit_vector(&light->pos);
	diffuse_factor = dot(&light_dir, normal);
	if (diffuse_factor < 0)
		diffuse_factor = 0;
	diffuse = diffuse_color(light, px, diffuse_factor);
	*px = blend_color(px, &diffuse);
	return (*px);
}

float calculate_shadow(const t_vec *intersect, const t_scene *scene, const t_hit *hitlist)
{
    t_vec		shadow_dir;
    t_ray		shadow_ray;
    const t_obj *obj;
    float		shadow_t;

	shadow_dir = vec3_subtract(scene->l.pos, *intersect);
	shadow_ray = (t_ray){*intersect, vec3_unit_vector(&shadow_dir)};
	obj = scene->objs;
	shadow_t = 1.0;
    while (obj != NULL)
    {
        if (obj != hitlist->objs) // exclude current object being shaded
        {
            float t = hit_object(obj, &shadow_ray);
            if (t > 0 && t < shadow_t)
                shadow_t = t;
        }
        obj = obj->next;
    }
    return (shadow_t);
}

t_color darker_color(t_color *px)
{
	px->r *= 0.7;
	px->g *= 0.7;
	px->b *= 0.7;
	return (*px);
}

t_color ray_color(const t_ray *r, const t_scene *scene)
{
    t_color px;
    t_hit *hit_obj;
    t_hit *hitlist;
    t_vec intersect;
    t_vec normal;
    float shadow_t;

    px = (t_color){0.5, 0.7, 1.0}; // Background color
    hit_obj = add_hit_obj(r, scene);
    if (!hit_obj)
        return (px);
    find_closest_obj(hit_obj, &hitlist);
    if (hitlist != NULL)
    {
        intersect = intersect_point(r, hitlist->t);
        if (hitlist->objs->id == SPHERE)
        {
			normal = sphere_normal((const t_sph *)(hitlist->objs->obj), &intersect);
			shadow_t = calculate_shadow(&intersect, scene, hitlist);
			if (shadow_t < 1.0)
			{
				px = amb_color(&scene->a, &((const t_sph *)(hitlist->objs->obj))->color);
				//px = darker_color(&px);
			}
			else
			{
				px = amb_color(&scene->a, &((const t_sph *)(hitlist->objs->obj))->color);
            	px = diffuse_lighting(&px, &scene->l, &normal);
			}
		}
        else if (hitlist->objs->id == PLANE)
        {
			shadow_t = calculate_shadow(&intersect, scene, hitlist);
            if (shadow_t < 1.0)
			{
                px = (t_color){0.0, 0.3, 0.2};
				px = darker_color(&px);
			}
            else
                px = amb_color(&scene->a, &((const t_plane*)(hitlist->objs->obj))->color);
        }
        free_hitlist(hitlist);
    }
    return (px);
}

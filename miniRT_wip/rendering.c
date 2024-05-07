/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/02 19:34:33 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int render_image(t_data *data)
{
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

void	create_image(t_cam *cam, t_ray *ray, t_data *data, t_scene *scene)
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
			px_color = ray_color(ray, scene);
			write_color(px_color, &data->img, i, j);
			i++;
		}
		j++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0); //display image
}

t_color diffuse_lighting(t_color *px, const t_light *light, const t_vec *normal)
{
	t_vec	light_dir;
	float	diffuse_factor;
	t_color	diffuse;

	light_dir = vec3_unit_vector(&light->pos);
	diffuse_factor = dot(&light_dir, normal);
	diffuse = diffuse_color(light, px, diffuse_factor);
	*px = blend_color(px, &diffuse);
	return (*px);
}

float calculate_shadow(const t_vec *intersect, const t_scene *scene, t_hit *hitlist)
{
	t_vec		shadow_dir;
	t_ray		shadow_ray;
	const t_obj *obj;
	float		t;
	float		shadow_t;

	shadow_dir = vec3_subtract(scene->l.pos, *intersect);
	shadow_ray = (t_ray){*intersect, vec3_unit_vector(&shadow_dir)};
	obj = scene->objs;
	shadow_t = FLT_MAX;
	while (obj != NULL)
	{
		if (obj != hitlist->objs)
		{
			t = hit_object(obj, &shadow_ray, hitlist);
			if (t > 0 && t < shadow_t)
			shadow_t = t;
		}
		obj = obj->next;
	}
	return (shadow_t);
}

t_color darker_color(t_color *px)
{
	return ((t_color){px->r * 0.7, px->g * 0.7, px->b * 0.7});
}

t_hit	find_closest_obj(const t_ray *r, const t_scene *scene)
{
	t_obj	*current;
	t_hit	hitpoint;
	float	t;

	current = scene->objs;
	hitpoint = (t_hit){.t = FLT_MAX};
	while (current != NULL)
	{
		t = hit_object(current, r, &hitpoint);
		if (t > 0 && t < hitpoint.t)
		{
			hitpoint.t = t;
			hitpoint.objs = current;
		}
		current = current->next;
	}
	return (hitpoint);
}

t_color	ray_color(const t_ray *r, const t_scene *scene)
{
	t_color	px;
	t_hit	hitpoint;
	t_vec	intersect;
	t_vec	normal;
	float	shadow_t;

	px = (t_color){0.3, 0.7, 1.0};
	hitpoint = find_closest_obj(r, scene);
	if (hitpoint.objs != NULL)
	{
		intersect = intersect_point(r, hitpoint.t);
		shadow_t = calculate_shadow(&intersect, scene, &hitpoint);
		if (hitpoint.objs->id == SPHERE)
		{
			normal = sphere_normal((const t_sph *)(hitpoint.objs->obj), &intersect);
			px = amb_color(&scene->a, &((const t_sph *)(hitpoint.objs->obj))->color);
			if (shadow_t < 0.5)
				px = darker_color(&px);
			else
				px = diffuse_lighting(&px, &scene->l, &normal);
		}
		else if (hitpoint.objs->id == PLANE)
		{
			px = amb_color(&scene->a, &((const t_plane *)(hitpoint.objs->obj))->color);
			if (shadow_t < 1.0)
				px = darker_color(&px);
		}
		else if (hitpoint.objs->id == CYLINDER)
			px = ((t_cyl *)hitpoint.objs->obj)->color;
	}
	return (px);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:36:48 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/14 22:44:33 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int render_image(t_data *data)
{
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

void create_image(t_cam *cam, t_data *data, t_scene *scene)
{
	t_vec	px_center;
	t_vec	ray_dir;
	t_color	px_color;
	t_ray	ray;
	float	i;
	float	j;

	j = 0;
	while (j < HEIGHT)
	{
		i = 0;
		while (i < WIDTH)
		{
			px_center = vec3_add(cam->px_00, vec_multiply(&cam->px_delta_u, i));
			px_center = vec3_add(px_center, vec_multiply(&cam->px_delta_v, j));
			ray_dir = vec3_subtract(px_center, cam->center);
			ray = (t_ray){cam->center, ray_dir};
			px_color = ray_color(&ray, scene);
			write_color(px_color, &data->img, i, j);
			i++;
		}
		j++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
}

t_color diffuse_lighting(t_color *px, t_light *light, t_vec *normal)
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

float calculate_shadow(t_vec *intersect, t_scene *scene, t_hit *hitpoint)
{
	t_vec	l_ray;
	t_ray	shadow_ray;
	t_obj	*obj;
	float	t;
	float	shadow_t;
	float	max_t;

	l_ray = vec3_subtract(scene->l.pos, *intersect);
	shadow_ray = (t_ray){*intersect, vec3_unit_vector(&l_ray)};
	obj = scene->objs;
	max_t = vec3_length(&l_ray);
	shadow_t = max_t;
	while (obj != NULL)
	{
		if (obj != hitpoint->objs)
		{
			//t = hit_object(obj, &shadow_ray, hitpoint);
			t = hit_object(obj, &shadow_ray, NULL);
			if (t > 0 && t < shadow_t)
				shadow_t = t;
		}
		obj = obj->next;
	}
	if (shadow_t < max_t)
		return (shadow_t);
	return (-1);
}

t_hit	find_closest_obj(t_ray *r, t_scene *scene)
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

t_vec	get_point_normal(t_hit *hit, t_vec *hitpoint, t_ray *r)
{
	t_vec	normal;
	t_plane	*plane;

	if (hit->objs->id == SPHERE)
		normal = sphere_normal((t_sph *)(hit->objs->obj), hitpoint);
	else if (hit->objs->id == CYLINDER)
		normal = cyl_normal(hit, r);
	else
	{
		plane = hit->objs->obj;
		normal = plane->normal;
	}
	return (normal);
}

t_color	ray_color(t_ray *r, t_scene *scene)
{
	t_color	px;
	t_hit	hit;
	t_vec	hitpoint;
	t_vec	light_r;
	t_vec	normal;
	float	l_dot_n;
	float	sh_t;

	px = (t_color){scene->a.ratio, scene->a.ratio, scene->a.ratio};
	hit = find_closest_obj(r, scene);
	if (!hit.objs)
		return (px);
	hitpoint = intersect_point(r, hit.t);
	light_r = vec3_subtract(scene->l.pos, hitpoint);
	normal = get_point_normal(&hit, &hitpoint, r);
	l_dot_n = dot(&light_r, &normal);
	sh_t = -1;
	if (l_dot_n > 0)
		sh_t = calculate_shadow(&hitpoint, scene, &hit);
	if (sh_t > 0)
		px = shadow_pixel(sh_t, &hit, scene);
	else
		px = light_pixel(l_dot_n, &light_r, &hit, scene);
	return (px);
}

// t_color	ray_color(t_ray *r, t_scene *scene)
// {
// 	t_color	px;
// 	t_hit	hitpoint;
// 	t_vec	intersect;
// 	t_vec	normal;
// 	float	shadow_t;

// 	px = (t_color){1.0, 1.0, 1.0};
// 	hitpoint = find_closest_obj(r, scene);
// 	if (hitpoint.objs != NULL)
// 	{
// 		intersect = intersect_point(r, hitpoint.t);
// 		shadow_t = calculate_shadow(&intersect, scene, &hitpoint);
// 		if (hitpoint.objs->id == SPHERE)
// 		{
// 			normal = sphere_normal((t_sph *)(hitpoint.objs->obj), &intersect);
// 			px = amb_color(&scene->a, &((t_sph *)(hitpoint.objs->obj))->color);
// 			if (shadow_t < 0.5)
// 				px = darker_color(&px);
// 			else
// 				px = diffuse_lighting(&px, &scene->l, &normal);
// 		}
// 		else if (hitpoint.objs->id == PLANE)
// 		{
// 			px = amb_color(&scene->a, &((t_plane *)(hitpoint.objs->obj))->color);
// 			if (shadow_t < 1.0)
// 				px = darker_color(&px);
// 		}
// 		else if (hitpoint.objs->id == CYLINDER)
// 		{
// 			px = ((t_cyl *)hitpoint.objs->obj)->color;
// 		}
// 	}
// 	return (px);
// }

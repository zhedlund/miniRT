/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:41:30 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/21 22:00:50 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

void	create_image(t_data *data)
{
	t_color	px_color;
	t_ray	ray;
	float	x;
	float	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = create_ray(&data->scene->c, x, y);
			px_color = ray_color(&ray, data->scene);
			write_color(px_color, &data->img, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
		data->img.mlx_img, 0, 0);
}

float	calculate_shadow(t_vec *intersect, t_scene *scene, t_hit *hitpoint)
{
	t_vec	l_ray;
	t_ray	shadow_ray;
	t_obj	*obj;
	float	t;
	float	max_t;

	l_ray = vec3_subtract(scene->l.pos, *intersect);
	shadow_ray = (t_ray){(*intersect), vec3_unit_vector(&l_ray)};
	obj = scene->objs;
	max_t = vec3_length(&l_ray);
	while (obj != NULL)
	{
		if (obj != hitpoint->objs)
		{
			t = hit_object(obj, &shadow_ray, NULL);
			if (t > 0 && t < max_t)
				return (t);
		}
		obj = obj->next;
	}
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

t_color	pixel_color(t_scene *scene, t_hit *hit, t_vec hitpoint, t_vec *normal)
{
	t_color	px;
	t_vec	light_r;
	float	l_dot_n;
	float	sh_t;

	light_r = vec3_subtract(scene->l.pos, hitpoint);
	l_dot_n = dot(&light_r, normal);
	sh_t = -1;
	if (l_dot_n > 0)
		sh_t = calculate_shadow(&hitpoint, scene, hit);
	if (sh_t > 0)
		px = shadow_pixel(hit, scene);
	else
		px = light_pixel(l_dot_n, &light_r, hit, scene);
	return (px);
}

t_color	ray_color(t_ray *r, t_scene *scene)
{
	t_color	px;
	t_hit	hit;
	t_vec	hitpoint;
	t_vec	normal;

	px = (t_color){scene->a.ratio, scene->a.ratio, scene->a.ratio};
	hit = find_closest_obj(r, scene);
	if (!hit.objs)
		return (px);
	hitpoint = intersect_point(r, hit.t);
	normal = get_point_normal(&hit, &hitpoint, r);
	if (dot(&r->dir, &normal) > 0)
		return (shadow_pixel(&hit, scene));
	px = pixel_color(scene, &hit, hitpoint, &normal);
	return (px);
}

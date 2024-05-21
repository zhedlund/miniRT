/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:01:17 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/21 16:20:15 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_ray	create_ray(t_cam *cam, float x, float y)
{
	t_vec	px_center;
	t_vec	ray_dir;
	t_ray	ray;

	px_center = vec3_add(cam->px_00, vec_multiply(&cam->px_delta_u, x));
	px_center = vec3_add(px_center, vec_multiply(&cam->px_delta_v, y));
	ray_dir = vec3_subtract(px_center, cam->center);
	ray = (t_ray){cam->center, ray_dir};
	return (ray);
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

t_color	diffuse_lighting(t_color *px, t_light *light, t_vec *normal)
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

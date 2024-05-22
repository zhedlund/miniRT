/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:01:17 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/22 13:58:03 by zhedlund         ###   ########.fr       */
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

/* Calculates t values for the cylinder intersection.
   Returns an array of 3 floats.
*/
int	get_min(float t[3])
{
	int		i;
	int		res;
	float	min;

	i = 0;
	res = -1;
	min = FLT_MAX;
	while (i < 3)
	{
		if (t[i] > 0 && t[i] < min)
		{
			res = i;
			min = t[i];
		}
		i++;
	}
	return (res);
}

/* calculates intersection point of a ray and a sphere.
   Returns a t_vec struct representing the intersection point.
*/
t_vec	intersect_point(t_ray *r, float t)
{
	return ((t_vec){r->origin.x + (t * r->dir.x),
		r->origin.y + (t * r->dir.y),
		r->origin.z + (t * r->dir.z)});
}

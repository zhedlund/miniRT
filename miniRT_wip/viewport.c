/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_viewport.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:35:00 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/17 22:39:41 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_vec	set_vup(t_cam *c)
{
	t_vec	vup;

	vup = (t_vec){0, 1, 0};
	if (c->dir.x == 0 && c->dir.z == 0 && fabsf(c->dir.y) == 1)
		vup = (t_vec){0, 0, 1};
	return (vup);
}

static void	calc_view_dimensions(t_cam *c)
{
	float	fov_radians;

	fov_radians = c->fov * PI / 180.0;
	c->view_width = 2.0 * tan(fov_radians / 2.0);
	c->view_height = c->view_width * (HEIGHT / WIDTH);
}

void	calc_px_00(t_cam *c, t_vec w)
{
	c->view_up_l = vec3_subtract(c->center, w);
	c->view_up_l = vec3_subtract(c->view_up_l, vec3_divide(c->view_u, 2.0));
	c->view_up_l = vec3_subtract(c->view_up_l, vec3_divide(c->view_v, 2.0));
	c->px_00 = vec3_add(c->view_up_l,
			vec3_add(vec3_divide(c->px_delta_u, 2.0),
				vec3_divide(c->px_delta_v, 2.0)));
}

void	calculate_viewport(t_cam *c)
{
	t_vec	w;
	t_vec	u;
	t_vec	v;
	t_vec	vup;

	vup = set_vup(c);
	calc_view_dimensions(c);
	w = vec_multiply(&c->dir, -1);
	u = vec3_cross(vup, w);
	u = vec3_unit_vector(&u);
	v = vec3_cross(w, u);
	c->view_u = vec_multiply(&u, c->view_width);
	v = vec_multiply(&v, -1.0);
	c->view_v = vec_multiply(&v, c->view_height);
	c->px_delta_u = vec3_divide(c->view_u, WIDTH);
	c->px_delta_v = vec3_divide(c->view_v, HEIGHT);
	calc_px_00(c, w);
}

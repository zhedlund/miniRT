/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:49:35 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/15 21:04:00 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void init_cam(t_cam *cam)
{
	float fov_radians;
	float view_height;
	float view_width;
	t_vec view_up_left;
	t_vec viewport_u;
	t_vec viewport_v;
	t_vec w;
	t_vec u;
	t_vec v;
	t_vec vup;
	
	t_vec length = vec3_subtract(cam->center, cam->dir);
	cam->focal_length = vec3_length(&length);
	vup = (t_vec){0, 1, 0};
	fov_radians = cam->fov * PI / 180.0;
	view_height = 2.0 * tan(fov_radians / 2.0) * cam->focal_length;
	view_width = view_height * (WIDTH / HEIGHT);

	w = vec3_subtract(cam->center, cam->dir);
	w = vec3_unit_vector(&w);
	u = vec3_cross(vup, w);
	u = vec3_unit_vector(&u);
	v = vec3_cross(w, u);
	
	viewport_u = vec_multiply(&u, -view_width);
	v = vec_multiply(&v, -1.0);
	viewport_v = vec_multiply(&v, view_height);
	
	cam->px_delta_u = vec3_divide(viewport_u, WIDTH);
	cam->px_delta_v = vec3_divide(viewport_v, HEIGHT);
	 
	view_up_left = vec3_subtract(cam->center, vec_multiply(&w, cam->focal_length));
	view_up_left = vec3_subtract(view_up_left, vec3_divide(viewport_u, 2.0));
	view_up_left = vec3_subtract(view_up_left, vec3_divide(viewport_v, 2.0));

	cam->px_00 = vec3_add(view_up_left, vec3_add(vec3_divide(cam->px_delta_u, 2.0), vec3_divide(cam->px_delta_v, 2.0)));
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_scene	*scene;

	if (argc != 2)
		return (put_error("Invalid number of arguments\n"));
	if (!is_valid_name(argv[1]))
		return (put_error("Invalid file name\n"));
	scene = parse_input(argv[1]);
	if (!scene)
		return (1);
	data.scene = scene;
	print_object_list(scene->objs);
	init_cam(&scene->c);
	mlx_hooks_init(&data);
	create_image(&scene->c, &data, scene);
	mlx_loop(data.mlx_ptr);
	return (0);
}
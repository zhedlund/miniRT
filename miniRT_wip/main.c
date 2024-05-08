/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:49:35 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/08 22:49:43 by zhedlund         ###   ########.fr       */
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

	cam->dir = vec3_unit_vector(&cam->dir);
	fov_radians = cam->fov * PI / 180.0;
	view_height = 2.0 * tan(fov_radians / 2.0);
	view_width = view_height * (WIDTH / HEIGHT);
	viewport_u = (t_vec){view_width, 0, 0};
	viewport_v = (t_vec){0, -view_height, 0};
	cam->px_delta_u = vec3_divide(viewport_u, WIDTH);
	cam->px_delta_v = vec3_divide(viewport_v, HEIGHT);
	view_up_left = vec3_subtract(cam->center, vec3_divide(viewport_u, 2.0));
	view_up_left = vec3_subtract(view_up_left, vec3_divide(viewport_v, 2.0));
	view_up_left = vec3_subtract(view_up_left, cam->dir);
	cam->px_00 = vec3_add(view_up_left,
				vec3_add(vec3_divide(cam->px_delta_u, 2.0),
				vec3_divide(cam->px_delta_v, 2.0)));
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
	mlx_hooks_init(&data);
	print_object_list(scene->objs);
	init_cam(&scene->c);
	create_image(&scene->c, &data, scene);
	//mlx_loop_hook(data.mlx_ptr, &render_image, &data);
	mlx_loop(data.mlx_ptr);
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
	return (0);
}
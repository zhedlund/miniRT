/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:49:35 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/01 23:01:37 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_cam	cam;
	t_scene	*scene;
	t_ray	ray;
	float	fov_radians;

	if (argc != 2)
		return (put_error("Too many arguments\n"));
	if (!is_valid_name(argv[1]))
		return (put_error("Invalid file name\n"));
	scene = parse_input(argv[1]);
	if (!scene)
		return (1);
	mlx_hooks_init(&data);

	//init ray struct
	ray.origin = (t_vec){0, 0, 0}; // ray origin at the center of the viewport
	ray.dir = (t_vec){0, 0, 0}; // ray direction along the negative z-axis

	print_object_list(scene->objs);

	// initialize camera struct
	cam = scene->c;
	//cam.focal_length = 1.0;
	//scene->c.fov = 70.0;
	//cam.center = (t_vec){0, 0, 0.1}; // viewpoint coordinates. x = left-right, y = up-down, z = forward-backward
	//cam.orientation = (t_vec){0.0, 0.0, 0.0}; // normalized orientation vector. cam orientation along xyz axis

	fov_radians = cam.fov * PI / 180.0; // Convert FOV to radians
	cam.viewport_height = 2.0 * tan(fov_radians / 2.0); // viewport height based on FOV
	cam.viewport_width = cam.viewport_height * ((float)WIDTH / HEIGHT);

	// the vectors across the horizontal & down the vertical viewport edges
	cam.viewport_u = (t_vec){cam.viewport_width, 0, 0};
	cam.viewport_v = (t_vec){0, -cam.viewport_height, 0};

	// the horizontal and vertical delta vectors from pixel to pixel
	cam.px_delta_u = (t_vec){cam.viewport_u.x / WIDTH, 0, 0};
	cam.px_delta_v = (t_vec){0, cam.viewport_v.y / HEIGHT, 0};

	// location of the upper left pixel
	cam.viewport_up_left = (t_vec) {cam.center.x - 0.5 * cam.viewport_u.x - 0.5 * cam.viewport_v.x,
									cam.center.y - 0.5 * cam.viewport_u.y - 0.5 * cam.viewport_v.y,
									cam.center.z - 1.0}; //focal length = 1.0
	// coordinates of the upper-left corner of the viewport
	cam.px_00 = (t_vec) {cam.viewport_up_left.x + 0.5 * (cam.px_delta_u.x + cam.px_delta_v.x),
						cam.viewport_up_left.y + 0.5 * (cam.px_delta_u.y + cam.px_delta_v.y),
						cam.viewport_up_left.z};

	create_image(&cam, &ray, &data, scene);
	//mlx_loop_hook(data.mlx_ptr, &render_image, &data);
	mlx_loop(data.mlx_ptr);
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
	return (0);
}

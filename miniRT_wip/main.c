/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:49:35 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/16 21:36:47 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int main()
{
	t_data	data;
	t_cam	cam;
	t_sph	sp;
	t_plane	pl;
	t_scene	scene;
	t_amb   a;
	t_light l;
	t_ray	ray;
	float	fov_radians;
	
	mlx_hooks_init(&data);
	// Image
    //float aspect_ratio = 16.0 / 9.0;
	//int image_width = 400; // Image width
    //int image_height = (int)(image_width / aspect_ratio); // Calculate image height
	//if (image_height < 1)
    	//image_height = 1; // ensure height is at least 1

	//init ray struct
	//ray.origin = (t_vec){0, 0, 0}; // ray origin at the center of the viewport
	//ray.dir = (t_vec){0, 0, 0}; // ray direction along the negative z-axis
	
	// init sphere & plane structs 
    sp.center = (t_vec){0.0, 0.0, -1}; // center coordinates
    sp.radius = 0.5; 
	sp.color = (t_color){0.7, 0.1, 0.7}; // color of the sphere

    pl.point = (t_vec){0, -0.4, 0}; // point on the plane
    pl.normal = (t_vec){0, 1, 0}; // assigning normal vector
	pl.color = (t_color){0.1, 0.9, 0.6}; // color of the plane

	// init light structs
    a.ratio = 1; // ambient light ratio, higher = brighter
    a.color = (t_color){1.0, 1.0, 1.0}; 
    a.diffuse = 0.0; // higher value = more light is scattered, brighter surface shading

    l.pos = (t_vec){-40.0, 50.0, 0.0}; 
    l.ratio = 1; // direct light ratio, higher = brighter
    l.diffuse = -0.5;
    l.color = (t_color){1.0, 1.0, 0.0}; 

	// init scene struct
	scene.a = a;
	scene.l = l;
	//scene.objs = NULL;

    // initialize camera struct
    cam.focal_length = 1.0;
    cam.fov = 70.0;
	cam.center = (t_vec){0, 0, 0.1}; // viewpoint coordinates. x = left-right, y = up-down, z = forward-backward
	cam.orientation = (t_vec){0.0, 0.0, 0.0}; // normalized orientation vector. cam orientation along xyz axis

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
    								cam.center.z - cam.focal_length};
    // coordinates of the upper-left corner of the viewport
    cam.px_00 = (t_vec) {cam.viewport_up_left.x + 0.5 * (cam.px_delta_u.x + cam.px_delta_v.x),
    					cam.viewport_up_left.y + 0.5 * (cam.px_delta_u.y + cam.px_delta_v.y),
    					cam.viewport_up_left.z};

	create_image(&cam, &ray, &data, sp, pl, scene);
	//mlx_loop_hook(data.mlx_ptr, &render_image, &data);
	mlx_loop(data.mlx_ptr);
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
    return (0);
}

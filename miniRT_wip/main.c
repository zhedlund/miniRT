/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:49:35 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/15 17:51:44 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int main()
{
	t_data	data;
	t_cam	cam;
	t_sph	sp;
	t_plane	pl;
	t_amb	a;
	t_light	l;

	// Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio); // Calculate image height
	if (image_height < 1)
    	image_height = 1; // ensure height is at least 1

	// initialize mlx stuff
    data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (1);
	data.win_ptr = mlx_new_window(data.mlx_ptr, image_width, image_height, "MiniRT");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (1);
	}
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, image_width, image_height);
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);

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
    a.diffuse = 0.0; // higher value = more light is scattered, brighter shading on the surface

    l.dir = (vec3){-40.0, 50.0, 0.0}; 
    l.ratio = 1; // direct light ratio, higher = brighter
    l.diffuse = -0.5;
    l.color = (color){1.0, 1.0, 0.0}; 

    // initialize camera struct
    cam.focal_length = 1.0;
    cam.fov = 70.0;
	cam.center = (vec3){0, 0, 0.1}; // viewpoint coordinates. x = left-right, y = up-down, z = forward-backward
	cam.orientation = (vec3){0.0, 0.0, 0.0}; // normalized orientation vector. cam orientation along xyz axis

   	double fov_radians = cam.fov * M_PI / 180.0; // Convert FOV to radians
   	cam.viewport_height = 2.0 * tan(fov_radians / 2.0); // viewport height based on FOV
    cam.viewport_width = cam.viewport_height * ((double)image_width / image_height);

    // the vectors across the horizontal & down the vertical viewport edges
    cam.viewport_u = (vec3){cam.viewport_width, 0, 0};
    cam.viewport_v = (vec3){0, -cam.viewport_height, 0};

    // the horizontal and vertical delta vectors from pixel to pixel
    cam.px_delta_u = (vec3){cam.viewport_u.x / image_width, 0, 0};
    cam.px_delta_v = (vec3){0, cam.viewport_v.y / image_height, 0};

    // location of the upper left pixel
    cam.viewport_up_left = (vec3) {
    cam.center.x - 0.5 * cam.viewport_u.x - 0.5 * cam.viewport_v.x,
    cam.center.y - 0.5 * cam.viewport_u.y - 0.5 * cam.viewport_v.y,
    cam.center.z - cam.focal_length
	};
    // coordinates of the upper-left corner of the viewport
    cam.px_00 = (vec3) {
    cam.viewport_up_left.x + 0.5 * (cam.px_delta_u.x + cam.px_delta_v.x),
    cam.viewport_up_left.y + 0.5 * (cam.px_delta_u.y + cam.px_delta_v.y),
    cam.viewport_up_left.z
	};

	int j = 0;
	while (j < image_height)
	{
    	int i = 0;
    	while (i < image_width)
	    {
    	    // calculates position of the current pixel
        	vec3 px_center = {
            	cam.px_00.x + (i * cam.px_delta_u.x),
            	cam.px_00.y + (j * cam.px_delta_v.y),
        		cam.px_00.z
        	};
		 	// calculates direction based on the pixel's position and cam center
        	vec3 ray_dir = {
            	px_center.x - cam.center.x,
            	px_center.y - cam.center.y,
            	px_center.z - cam.center.z
        	};
			// Rotate the ray direction according to the camera orientation
        	//ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction
        	//ray_dir.x += cam.orientation.x;
			//ray_dir.y += cam.orientation.y;
			//ray_dir.z += cam.orientation.z;

			//ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction again
			ray r = {cam.center, ray_dir}; // create the ray

        	// calculate pixel color and write to image buffer
        	color px_color = ray_color(&r, &sp, &pl, &(lighting){a, l});
        	write_color(px_color, &data.img, i, j);
			i++;
    	}
    	j++;
	}
	mlx_put_image_to_window(data.mlx_ptr, data.win_ptr, data.img.mlx_img, 0, 0);
	//mlx_loop_hook(data.mlx_ptr, &render, &data);
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &key_handler, &data); 
	mlx_hook(data.win_ptr, 17, 0, &close_window, NULL); // close window by clicking x
	mlx_loop(data.mlx_ptr);
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
    return (0);
}

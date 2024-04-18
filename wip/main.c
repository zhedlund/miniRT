/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:45:57 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/18 19:14:51 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// compile: cc main.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm

// void ft_pixel_put(t_img *img, int x, int y, int color)
// {
//     char *pixel;

//     pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
//     *(unsigned int *)pixel = color;
// }

// int	close_window(t_data *data)
// {
// 	//mlx_destroy_window(data->mlx_ptr, data->win_ptr);
// 	//mlx_destroy_image(data->mlx_ptr, data->img.mlx_img);
// 	//mlx_destroy_display(data->mlx_ptr);
// 	//free(data->mlx_ptr);
// 	exit(0);
// }

// int key_handler(int keycode, t_data *data)
// {
// 	if (keycode == XK_Escape)
// 		close_window(data);
// 	return (0);
// }

// int render(t_data *data)
// {
// 	if (data->win_ptr == NULL)
// 		return (1);
// 	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
// 	return (0);
// }

int	main(int argc, char **argv)
{
	t_data	data;
	t_scene	*scene;
	t_sph	*sph;

	if (argc != 2)
		return (put_error("Too many arguments\n"));
	if (!is_valid_name(argv[1]))
		return (put_error("Invalid file name\n"));
	scene = parse_input(argv[1]);
	if (!scene)
		return (1);
	printf("amb color: %i,%i,%i\n", (int)scene->amb.color.r, (int)scene->amb.color.g, (int)scene->amb.color.b);
	printf("FOV: %i\n", scene->cam.fov);
	printf("spot light ratio: %f\n", scene->light.ratio);
	sph = scene->objs->obj;
	printf("sphere radius: %f", sph->r);
	free_scene(scene);
	return (0);
}

// int main(int argc, char **argv)
// {
// 	t_data	data;
// 	t_scene	*scene;

// 	if (argc != 2)
// 		return (put_error("Too many arguments\n"));
// 	if (!is_valid_name(argv[1]))
// 		return (put_error("Invalid file name\n"));
// 	scene = parse_input(argv[1]);
// 	if (!scene)
// 		return (1);
// 	/* initialize mlx stuff*/
// 	// data.mlx_ptr = mlx_init();
// 	// if (data.mlx_ptr == NULL)
// 	// 	return (1);
// 	// data.win_ptr = mlx_new_window(data.mlx_ptr, WIDTH, HEIGHT, "MiniRT");
// 	// if (data.win_ptr == NULL)
// 	// {
// 	// 	free(data.win_ptr);
// 	// 	return (1);
// 	// }
// 	/* set up hooks*/
// 	// data.img.mlx_img = mlx_new_image(data.mlx_ptr, WIDTH, HEIGHT);
// 	// data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);
// 	// ray_tracing(scene);
// 	// mlx_loop_hook(data.mlx_ptr, &render, &data); // connect render function
// 	// mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &key_handler, &data); // connect key_handler function
// 	//mlx_hook(data.win_ptr, ButtonPress, ButtonPressMask, &mouse_handler, &data); // connect mouse_handler function
// 	// mlx_hook(data.win_ptr, 17, 0, &close_window, NULL); // for closing window with clicking on x, connect close_window function
// 	// mlx_loop(data.mlx_ptr);
// 	/* if no window left, execute this code */
// 	// mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
// 	// mlx_destroy_display(data.mlx_ptr);
// 	// free_scene(scene);
// 	// free(data.mlx_ptr);
// 	//return (0);
// }

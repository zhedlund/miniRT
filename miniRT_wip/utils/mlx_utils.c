/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:30:12 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/15 20:57:31 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->img.mlx_img)
		mlx_destroy_image(data->mlx_ptr, data->img.mlx_img);
	if (data->win_ptr)
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	if (data->mlx_ptr)
		mlx_destroy_display(data->mlx_ptr);
	free(data->mlx_ptr);
	data = NULL;
}

int	close_window(t_data *data)
{
	free_data(data);
	exit(0);
}

int key_handler(int keycode, t_data *data)
{
	if (keycode == XK_Escape)
		close_window(data);
	if (keycode == XK_Left)
	{
		printf("Left arrow key pressed\n");
		data->scene->c.center.x -= 0.2;
	}
	else if (keycode == XK_Right)
	{
		printf("Right arrow key pressed\n");
		data->scene->c.center.x += 0.2;
	}
	else if (keycode == XK_Up)
	{
		printf("Up arrow key pressed\n");
		data->scene->c.center.y += 0.2;
	}
	else if (keycode == XK_Down)
	{
		printf("Down arrow key pressed\n");
		data->scene->c.center.y -= 0.2;
	}
	else if (keycode == XK_equal)
	{
		printf("Plus/equal key pressed\n");
		data->scene->c.center.z += 0.2;
	}
	else if (keycode == XK_minus)
	{
		printf("Minus key pressed\n");
		data->scene->c.center.z -= 0.2;
	}
	init_cam(&data->scene->c);
	create_image(&data->scene->c, data, data->scene);
	return (0);
}

void ft_pixel_put(t_img *img, int x, int y, int color)
{
	char *pixel;

	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)pixel = color;
}

void	mlx_hooks_init(t_data *data)
{
	data->mlx_ptr = mlx_init();
	if (data->mlx_ptr == NULL)
	{
		write(2, "Error: mlx_init failed\n", 23);
		exit(1);
	}
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIDTH, HEIGHT, "MiniRT");
	if (data->win_ptr == NULL)
	{
		free(data->win_ptr);
		write(2, "Error: mlx_new_window failed\n", 29);
		exit(1);
	}
	data->offset_x = 0;
	data->offset_y = 0;
	data->img.mlx_img = mlx_new_image(data->mlx_ptr, WIDTH, HEIGHT);
	data->img.addr = mlx_get_data_addr(data->img.mlx_img, &data->img.bpp, &data->img.line_len, &data->img.endian);
	mlx_hook(data->win_ptr, KeyPress, KeyPressMask, &key_handler, data); 
	mlx_hook(data->win_ptr, 17, 0, &close_window, NULL);
}
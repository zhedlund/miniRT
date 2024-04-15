/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:45:57 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/04 19:56:26 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// compile: cc main.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm 

void ft_pixel_put(t_img *img, int x, int y, int color)
{
    char *pixel;

    pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

int	close_window(t_data *data)
{
	//mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	//mlx_destroy_image(data->mlx_ptr, data->img.mlx_img);
	//mlx_destroy_display(data->mlx_ptr);
	//free(data->mlx_ptr);
	exit(0);
}

int key_handler(int keycode, t_data *data)
{
	if (keycode == XK_Escape)
		close_window(data);
	return (0);
}

void render_circle(t_img *mlx_img, int xc, int yc, int radius, int color)
{
	if (radius == 0)
		return ;
    for (int x = 0; x <= radius; x++)
    {
        int y = (int)sqrt(radius * radius - x * x);
        ft_pixel_put(mlx_img, xc + x, yc + y, 0xFFFFFF);
        ft_pixel_put(mlx_img, xc + x, yc - y, 0xFF00FF);
        ft_pixel_put(mlx_img, xc - x, yc + y, 0x0000FF);
        ft_pixel_put(mlx_img, xc - x, yc - y, color);
    }
	render_circle(mlx_img, xc, yc, radius - 1, color);
}

int render(t_data *data)
{
	if (data->win_ptr == NULL)
		return (1);
	render_circle(&data->img, WIDTH / 2, HEIGHT / 2, 200, 0xFF0000);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

int main(void)
{
    t_data data;

	/* initialize mlx stuff*/
    data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (1);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WIDTH, HEIGHT, "MiniRT");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (1);
	}
	/* set up hooks*/
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, WIDTH, HEIGHT);
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);
	mlx_loop_hook(data.mlx_ptr, &render, &data);
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &key_handler, &data); 
	//mlx_hook(data.win_ptr, ButtonPress, ButtonPressMask, &mouse_handler, &data); // connect mouse_handler function
	mlx_hook(data.win_ptr, 17, 0, &close_window, NULL); // for closing window with clicking on x, connect close_window function
	mlx_loop(data.mlx_ptr);
	/* if no window left, execute this code */
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
	return (0);
}

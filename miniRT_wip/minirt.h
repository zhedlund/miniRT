/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:43:05 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/15 15:04:14 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "../minilibx-linux/mlx.h"

# define WIDTH 1024
# define HEIGHT	576
# define DEPTH 500

typedef struct s_img
{
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	img;
}	t_data;

typedef struct s_vec
{
	float	x;
	float	y;
	float	z;
}	t_vec;

typedef struct s_cam
{
	t_vec	vec;
	t_vec	dir;
	int		fov;
}	t_cam;

typedef struct s_sph
{
	t_vec	vec;
	float	r;
	int		color;
}	t_sph;

typedef struct s_obj
{
	char			id;
	void			*obj;
	struct s_obj	*next;
}	t_obj;

typedef struct s_scene
{
	t_cam	*cam;
	t_obj	*objs;
}	t_scene;

#endif

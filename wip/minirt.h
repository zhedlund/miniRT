/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:43:05 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/15 15:20:40 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "minilibx-linux/mlx.h"
# include "get_next_line.h"

# define WIDTH 1024
# define HEIGHT	576

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

typedef struct s_color
{
	float	r;
	float	g;
	float	b;
}	t_color;

typedef struct s_amb
{
	float	ratio;
	t_color	color;
	float	diffuse;
}	t_amb;

typedef struct s_light
{
	t_vec	pos;
	float	ratio;
	t_color	color;
}	t_light;

typedef struct s_cam
{
	t_vec	vec;
	t_vec	dir;
	int		fov;
}	t_cam;

typedef struct s_sph
{
	t_vec	pos;
	float	r;
	t_color	color;
}	t_sph;

typedef struct s_plane
{
	t_vec	point;
	t_vec	normal;
	t_color	color;
}	t_plane;

typedef struct s_cyl
{
	t_vec	pos;
	t_vec	normal;
	float	r;
	float	h;
	t_color	color;
}	t_cyl;

typedef struct s_obj
{
	char			id;
	void			*obj;
	struct s_obj	*next;
}	t_obj;

typedef struct s_scene
{
	t_amb	amb;
	t_cam	cam;
	t_light	light;
	t_obj	*objs;
}	t_scene;

typedef struct s_hit_point
{
	float				t;
	t_obj				*obj;
	struct s_hit_point	*next;
}	t_hit_point;

char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);
void	put_error(char *message);
void	*malloc_error(void);
t_scene	*parse_input(char *file);
void	add_light(char *str, t_scene *scene);
void	add_sphere(char *str, t_scene *scene);
void	add_plane(char *str, t_scene *scene);
void	add_cylinder(char *str, t_scene *scene);
t_vec	*new_vec(float x, float y, float z);

#endif

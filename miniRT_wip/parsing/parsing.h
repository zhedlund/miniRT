/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:43:05 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/30 14:25:04 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include <X11/X.h>
# include <X11/keysym.h>
//# include "minilibx-linux/mlx.h"
# include "../minirt.h"
# include "get_next_line.h"

// typedef struct s_img
// {
// 	void	*mlx_img;
// 	char	*addr;
// 	int		bpp;
// 	int		line_len;
// 	int		endian;
// }	t_img;

// typedef struct s_data
// {
// 	void	*mlx_ptr;
// 	void	*win_ptr;
// 	t_img	img;
// }	t_data;

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
	float	diffuse;
}	t_light;

// typedef struct s_cam
// {
// 	t_vec	vec;
// 	t_vec	dir;
// 	int		fov;
// }	t_cam;

typedef struct s_cam
{
	t_vec	vec;
	t_vec	dir;
	int		fov;
	t_vec	center;
	float	focal_length;
	t_vec	orientation;
	float	viewport_height;
	float	viewport_width;
	t_vec	viewport_u;
	t_vec	viewport_v;
	t_vec	px_delta_u;
	t_vec	px_delta_v;
	t_vec	viewport_up_left;
	t_vec	px_00;
}	t_cam;

// typedef struct s_sph
// {
// 	t_vec	pos;
// 	float	r;
// 	t_color	color;
// }	t_sph;

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

typedef enum e_shape
{
	SPHERE,
	PLANE,
	CYLINDER,
}	t_shape;

typedef struct s_obj
{
	t_shape			id; // enum to represent the shape type
	//char			id;
	void			*obj;
	struct s_obj	*next;
}	t_obj;

// typedef struct s_obj
// {
// 	char			id;
// 	void			*obj;
// 	struct s_obj	*next;
// }	t_obj;

typedef struct s_scene
{
	t_amb	a;
	t_cam	c;
	t_light	l;
	t_obj	*objs;
}	t_scene;

//c_part = 0, 1, 2, 3:
//0 - not a cylinder, 1 - top of cylinder, 2 - bottom of cylinder, 3 - side of cylinder
// typedef struct s_hit_point
// {
// 	float				t;
// 	t_obj				*obj;
// 	int					c_part;
// 	struct s_hit_point	*next;
// }	t_hit_point;

char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);
int		count_words(char const *str, char c);
float	ft_atof(const char *str);
int		put_error(char *message);
int		is_valid_name(char *file);
int		is_numbers(char *str, char begin);
void	*malloc_error(void);
t_scene	*parse_input(char *file);
int		read_color(t_color *col, char *str);
int		add_light(char *str, t_light *light);
int		add_sphere(char *str, t_scene *scene);
int		add_plane(char *str, t_scene *scene);
int		add_cylinder(char *str, t_scene *scene);
int		check_empty_line(char *str);
t_vec	*new_vec(float x, float y, float z);
int		is_uns_int(char *str);
int		is_float(char *str);
float	is_color(char *str);
float	valid_ratio(char *str);
int		read_color(t_color *col, char *str);
int		read_vector(t_vec *vector, char *str);
int		check_vector(char **vec);
int		is_unit_vec(t_vec *vec);
void	free_array(char **str);
void	*free_scene(t_scene *scene);

#endif
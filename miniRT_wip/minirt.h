/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:43:05 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/16 16:06:08 by zhedlund         ###   ########.fr       */
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
//# include "get_next_line.h"

# define WIDTH 640
# define HEIGHT	360
# define PI M_PI

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

typedef struct s_ray
{
    t_vec origin; // Origin point of the ray
    t_vec dir;  // Direction of the ray
} t_ray;

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

typedef struct s_sph
{
	t_vec	center;
	float	radius;
	t_vec	normal;
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
	t_amb	a;
	t_cam	c;
	t_light	l;
	//t_obj	*objs;
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
t_scene	*parse_input(char *file);
void	add_light(char *str, t_scene *scene);
void	add_sphere(char *str, t_scene *scene);
void	add_plane(char *str, t_scene *scene);
void	add_cylinder(char *str, t_scene *scene);

/* objects */
t_vec	sphere_normal(const t_sph *sp, const t_vec *intersect);
float	hit_sphere(const t_vec *center, float radius, const t_ray *r);
float	hit_plane(const t_plane *pl, const t_ray *r);

/* color */
t_color	ambient_color(const t_amb *a, const t_color *c);
t_color	diffuse_color(const t_light *l, const t_color *c, float diffuse_factor);
void	write_color(t_color px, t_img *img, int x, int y);
t_color	blend_color(const t_color *c1, const t_color *c2);
t_color ray_color(const t_ray *r, const t_sph *sp, const t_plane *pl, const t_scene *lights);

/* math */
float 	dot(const t_vec *u, const t_vec *v);
t_vec	intersect_point(const t_ray *r, float t);
t_vec	vec3_subtract(const t_vec a, const t_vec b);
float	vec3_length_squared(const t_vec *v);
t_vec	vec3_unit_vector(const t_vec *v);
t_vec	vec3_add(const t_vec a, const t_vec b);
float	vec3_length(const t_vec *v);

/* mlx */
int		close_window(t_data *data);
int 	key_handler(int keycode, t_data *data);
void	ft_pixel_put(t_img *img, int x, int y, int color);

/* utils */
int		ft_atoi(const char *str);
float	ft_atof(const char *str);

#endif

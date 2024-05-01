/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:43:05 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/30 14:24:34 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <float.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "minilibx-linux/mlx.h"
# include "parsing/parsing.h"
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

// typedef struct s_vec
// {
// 	float	x;
// 	float	y;
// 	float	z;
// }	t_vec;

typedef struct s_ray
{
	t_vec origin; // Origin point of the ray
	t_vec dir;  // Direction of the ray
}	t_ray;

// typedef struct s_color
// {
// 	float	r;
// 	float	g;
// 	float	b;
// }	t_color;

// typedef struct s_amb
// {
// 	float	ratio;
// 	t_color	color;
// 	float	diffuse;
// }	t_amb;

// typedef struct s_light
// {
// 	t_vec	pos;
// 	float	ratio;
// 	t_color	color;
// 	float	diffuse;
// }	t_light;

// typedef struct s_cam
// {
// 	t_vec	vec;
// 	t_vec	dir;
// 	int		fov;
// 	t_vec	center;
// 	float	focal_length;
// 	t_vec	orientation;
// 	float	viewport_height;
// 	float	viewport_width;
// 	t_vec	viewport_u;
// 	t_vec	viewport_v;
// 	t_vec	px_delta_u;
// 	t_vec	px_delta_v;
// 	t_vec	viewport_up_left;
// 	t_vec	px_00;
// }	t_cam;

typedef struct s_sph
{
	t_vec	center;
	float	radius;
	t_vec	normal;
	t_color	color;
}	t_sph;

// typedef struct s_plane
// {
// 	t_vec	point;
// 	t_vec	normal;
// 	t_color	color;
// }	t_plane;

// typedef struct s_cyl
// {
// 	t_vec	pos;
// 	t_vec	normal;
// 	float	r;
// 	float	h;
// 	t_color	color;
// }	t_cyl;

// typedef enum e_shape
// {
// 	SPHERE,
// 	PLANE,
// 	CYLINDER,
// } t_shape;

// typedef struct s_obj
// {
// 	t_shape			id; // enum to represent the shape type
// 	//char			id;
// 	void			*obj;
// 	struct s_obj	*next;
// }	t_obj;

// typedef struct s_scene
// {
// 	t_amb	a;
// 	t_cam	c;
// 	t_light	l;
// 	t_obj	*objs;
// }	t_scene;

// c_part = 0, 1, 2, 3:
// 0 - not a cylinder, 1 - top of cylinder, 2 - bottom of cylinder, 3 - side of cylinder
typedef struct s_hit
{
	float			t;
	float			shadow_t;
	int				c_part;
	t_obj			*objs;
	struct s_hit	*next;
}	t_hit;


// char	*get_next_line(int fd);
// char	**ft_split(char const *s, char c);
// void	put_error(char *message);
// t_scene	*parse_input(char *file);
// void	add_light(char *str, t_scene *scene);
// void	add_sphere(char *str, t_scene *scene);
// void	add_plane(char *str, t_scene *scene);
// void	add_cylinder(char *str, t_scene *scene);

/* objects */
t_vec	sphere_normal(const t_sph *sp, const t_vec *intersect);
float	hit_sphere(const t_vec *center, float radius, const t_ray *r);
float	hit_plane(const t_plane *pl, const t_ray *r);
float	hit_object(const t_obj *obj, const t_ray *r);

/* color */
t_color	amb_color(const t_amb *a, const t_color *c);
t_color	diffuse_color(const t_light *l, const t_color *c, float diffuse_factor);
void	write_color(t_color px, t_img *img, int x, int y);
t_color	blend_color(const t_color *c1, const t_color *c2);

/* rendering */
void	create_image(t_cam *cam, t_ray *ray, t_data *data, t_scene *scene);
t_color	ray_color(const t_ray *r, const t_scene *scene);
int		render_image(t_data *data);

/* math */
float 	dot(const t_vec *u, const t_vec *v);
t_vec	intersect_point(const t_ray *r, float t);
t_vec	vec3_subtract(const t_vec a, const t_vec b);
float	vec3_length_squared(const t_vec *v);
t_vec	vec3_unit_vector(const t_vec *v);
t_vec	vec3_add(const t_vec a, const t_vec b);
float	vec3_length(const t_vec *v);

/* mlx */
void	mlx_hooks_init(t_data *data);
int		close_window(t_data *data);
int 	key_handler(int keycode, t_data *data);
void	ft_pixel_put(t_img *img, int x, int y, int color);

/* lists */
//void	add_object(t_scene* scene, t_obj* obj);
void	free_obj_list(t_scene* scene);
void	free_hitlist(t_hit* hit);

/* utils */
// int		ft_atoi(const char *str);
// float	ft_atof(const char *str);
// int		ft_isdigit(int c);
// size_t	ft_strlen(const char *s);

/* debug */
void		print_object_list(const t_obj *head);
const char 	*shape_to_string(t_shape shape);

#endif

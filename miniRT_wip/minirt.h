/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:43:05 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/17 21:10:06 by zhedlund         ###   ########.fr       */
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

# define WIDTH 960.0
# define HEIGHT	540.0
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
	float	offset_x;
	float	offset_y;
	t_img	img;
	t_scene	*scene;
}	t_data;

typedef struct s_ray
{
	t_vec origin;
	t_vec dir;
}	t_ray;

typedef struct s_sph
{
	t_vec	center;
	float	radius;
	t_vec	normal;
	t_color	color;
}	t_sph;

// c_part = 0, 1, 2, 3:
// 0 - not a cylinder, 1 - top of cylinder, 2 - bottom of cylinder, 3 - side of cylinder
typedef struct s_hit
{
	float			t;
	int				c_part;
	t_obj			*objs;
}	t_hit;

/* objects */
t_vec	sphere_normal(t_sph *sp, t_vec *intersect);
float	hit_sphere(t_vec *center, float radius, t_ray *r);
float	hit_plane(t_plane *pl, t_ray *r);
float	hit_cylinder(t_cyl *cyl, t_ray *ray, t_hit *hit);
float	hit_object(t_obj *obj, t_ray *r, t_hit *hit);

/* rendering */
void	init_cam(t_cam *cam);
void	create_image(t_data *data);
t_color	ray_color(t_ray *r, t_scene *scene);
t_vec	cyl_normal(t_hit *hit, t_ray *ray);
t_color	light_pixel(float l_dot_n, t_vec *light_r, t_hit *hit, t_scene *scene);
t_color	shadow_pixel(float shadow_t, t_hit *hit, t_scene *scene);

/* color */
t_color	amb_light(t_amb *a, t_color *c);
t_color	diffuse_color(t_light *l, t_color *c, float diffuse_factor);
void	write_color(t_color px, t_img *img, int x, int y);
t_color	blend_color(t_color *c1, t_color *c2);
t_color alpha_color(t_color c1, t_color c2, float alpha);
t_color darker_color(t_color *px);

/* math */
float 	dot(t_vec *u, t_vec *v);
t_vec	intersect_point(t_ray *r, float t);
t_vec	vec3_subtract(t_vec a, t_vec b);
float	vec3_length_squared(t_vec *v);
t_vec	vec3_unit_vector(t_vec *v);
t_vec	vec3_add(t_vec a, t_vec b);
float	vec3_length(t_vec *v);
t_vec	vec3_divide(t_vec vec, float scalar);
t_vec	vec_multiply(t_vec *vec, float n);
t_vec	vec3_cross(t_vec a, t_vec b);
float	pow2(float n);

/* mlx */
void	mlx_hooks_init(t_data *data);
int		close_window(t_data *data);
int 	key_handler(int keycode, t_data *data);
void	ft_pixel_put(t_img *img, int x, int y, int color);
void	free_mlx(t_data *data);

/* debug */
void	print_object_list(t_obj *head);
char 	*shape_to_string(t_shape shape);

#endif

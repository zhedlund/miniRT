/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 18:16:35 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/16 15:33:16 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
//#include "parsing.h"

int	add_amb_light(char *str, t_amb *amb)
{
	char	**args;

	if (!is_numbers(str + 1, ' '))
		return (put_error("Unexpected arguments after A: ambient light\n"));
	if (count_words(str, ' ') != 3)
		return (put_error("Invalid number of arguments for ambient light\n"));
	args = ft_split(str, ' ');
	amb->ratio = valid_ratio(args[1]);
	if (amb->ratio == -1)
	{
		free_array(args);
		return (put_error("Invalid ambient light ratio\n"));
	}
	if (read_color(&amb->color, args[2]) == -1)
	{
		free_array(args);
		return (put_error("Invalid color value of ambient light\n"));
	}
	amb->diffuse = 0.5;
	free_array(args);
	return (0);
}

int	add_camera(char *str, t_cam *cam)
{
	char	**args;

	if (!is_numbers(str + 1, ' '))
		return (put_error("Unexpected arguments after C: camera\n"));
	if (count_words(str, ' ') != 4)
		return (put_error("Invalid number of arguments for camera\n"));
	args = ft_split(str, ' ');
	if (read_vector(&cam->center, args[1]) == -1)
	{
		free_array(args);
		return (put_error("Invalid coordinates for camera center\n"));
	}
	if (read_vector(&cam->dir, args[2]) == -1 || !is_unit_vec(&cam->dir))
	{
		free_array(args);
		return (put_error("Invalid coordinates for camera direction\n"));
	}
	if (!is_uns_int(args[3]) || ft_atof(args[3]) > 180)
	{
		free_array(args);
		return (put_error("Invalid FOV value\n"));
	}
	cam->fov = ft_atof(args[3]);
	free_array(args);
	return (0);
}

void	default_scene(t_scene *scene)
{
	scene->a.color = (t_color){0, 0, 0};
	scene->a.ratio = 0;
	scene->a.diffuse = 0;
	scene->c.center = (t_vec){0, 0, 0};
	scene->c.dir = (t_vec){0, 0, 1};
	scene->c.fov = 55;
	scene->l.pos = (t_vec){0, 0, 0};
	scene->l.color = (t_color){0, 0, 0};
	scene->l.ratio = 0;
	scene->l.diffuse = 0;
	scene->objs = NULL;
}
//create functions for each type of ojects to add to scene
int	fill_scene(char *str, t_scene *scene)
{
	while (*str == ' ')
		str++;
	if (*str == 'A')
		return (add_amb_light(str, &scene->a));
	else if (*str == 'C')
		return (add_camera(str, &scene->c));
	else if (*str == 'L')
		return (add_light(str, &scene->l));
	else if (*str == 's')
		return (add_sphere(str, scene));
	else if (*str == 'p')
		return (add_plane(str, scene));
	else if (*str == 'c')
		return (add_cylinder(str, scene));
	else
		return (check_empty_line(str));
}
// all objects in scene should be assigned to zero values
t_scene	*write_scene(int fd)
{
	char	*str;
	t_scene	*scene;

	str = get_next_line(fd);
	if (!str)
		return (NULL);
	scene = (t_scene *)malloc(sizeof(t_scene));
	if (!scene)
		malloc_error();
	default_scene(scene);
	while (str)
	{
		if (fill_scene(str, scene) == -1)
		{
			free(str);
			return (free_scene(scene));
		}
		free(str);
		str = get_next_line(fd);
	}
	return (scene);
}

t_scene	*parse_input(char *file)
{
	int		fd;
	t_scene	*scene;

	if (!is_valid_name(file))
		return (NULL);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (NULL);
	scene = write_scene(fd);
	close(fd);
	return (scene);
}

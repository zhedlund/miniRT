/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 18:16:35 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/15 16:20:34 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// check for errors needed: is_digits(char *str), is_valid_color(float color), is_unit_vec(float num), is_range(int grad)
void	add_amb_light(char *str, t_scene *scene)
{
	char	**args;
	char	**color;

	args = ft_split(str, ' ');
	scene->amb.ratio = atof(args[1]);
	color = ft_split(args[2], ',');
	scene->amb.color.r = atof(color[0]);
	scene->amb.color.g = atof(color[1]);
	scene->amb.color.b = atof(color[2]);
	scene->amb.diffuse = 0;
	free_array(color);
	free_array(args);
	// scene->amb.ratio = 0.2;
	// scene->amb.color.r = 255;
	// scene->amb.color.g = 255;
	// scene->amb.color.b = 255;
}

void	add_camera(char *str, t_scene *scene)
{
	(void) str;
	scene->cam.vec.x = 0;
	scene->cam.vec.y = 0;
	scene->cam.vec.z = 0;
	scene->cam.dir.x = 0;
	scene->cam.dir.y = 0;
	scene->cam.dir.z = 1;
	scene->cam.fov = 70;
}

//create functions for each type of ojects to add to scene
void	fill_scene(char *str, t_scene *scene)
{
	while (*str == ' ')
		str++;
	if (*str == 'A')
		add_amb_light(str, scene);
	else if (*str == 'C')
		add_camera(str, scene);
	else if (*str == 'L')
		add_light(str, scene);
	else if (*str == 's')
		add_sphere(str, scene);
	else if (*str == 'p')
		add_plane(str, scene);
	else if (*str == 'c')
		add_cylinder(str, scene);
	else
		check_input(str);
	free(str);
}

t_scene	*write_scene(int fd)
{
	char	*str;
	t_scene	*scene;

	str = get_next_line(fd);
	if (!str)
		return (NULL);
	scene = (t_scene *)malloc(sizeof(t_scene));
	if (*str != '/n' && *str)
	{
		scene->objs = NULL;
		fill_scene(str, scene);
	}
	while (str)
	{
		str = get_next_line(fd);
		if (*str != '/n' && *str)
			fill_scene(str, scene);
	}
	return (scene);
}

//this is temporary function for hard coded scene before parsing is finished
t_scene	*test_scene(void)
{
	t_scene	*scene;

	scene = (t_scene *)malloc(sizeof(t_scene));
	add_amb_light(NULL, scene);
	add_camera(NULL, scene);
	add_light(NULL, scene);
	add_sphere(NULL, scene);
	add_plane(NULL, scene);
	return (scene);
}

t_scene	*parse_input(char *file)
{
	int		fd;
	t_scene	*scene;

	if (!check_file(file))
		return ;
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return ;
	scene = test_scene();
	close(fd);
	return (scene);
}

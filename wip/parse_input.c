/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 18:16:35 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/12 15:45:44 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	add_amb_light(char *str, t_scene *scene)
{
	(void) str;
	scene->amb.ratio = 0.2;
	scene->amb.color.r = 255;
	scene->amb.color.g = 255;
	scene->amb.color.b = 255;
	scene->amb.diffuse = 0;
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

t_scene	*create_scene(int fd)
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

t_scene	*parse_input(char *file)
{
	int		fd;
	t_scene	*scene;

	if (!check_file(file))
		return ;
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return ;
	scene = create_scene(fd);
	close(fd);
	return (scene);
}

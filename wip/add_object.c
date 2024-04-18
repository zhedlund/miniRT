/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:35:45 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/18 19:06:39 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	add_object(t_obj *object, t_scene *scene)
{
	t_obj	*cur;

	if (!scene->objs)
	{
		scene->objs = object;
		return ;
	}
	cur = scene->objs;
	while (cur->next)
		cur = cur->next;
	cur->next = object;
}

int	add_light(char *str, t_light *light)
{
	char	**args;

	if (!is_numbers(str + 1, ' '))
		return (put_error("Unexpected arguments after L: spot light\n"));
	if (count_words(str, ' ') != 4)
		return (put_error("Invalid number of arguments for spot light\n"));
	args = ft_split(str, ' ');
	if (read_vector(&light->pos, args[1]) == -1)
	{
		free_array(args);
		return (put_error("Invalid coordinates for spot light\n"));
	}
	light->ratio = valid_ratio(args[2]);
	if (light->ratio == -1)
	{
		free_array(args);
		return (put_error("Invalid spot light ratio\n"));
	}
	if (read_color(&light->color, args[3]) == -1)
	{
		free_array(args);
		return (put_error("Invalid color value of spot light\n"));
	}
	free_array(args);
	return (0);
}
//parse str to get real input; protect malloc;

int	add_sphere(char *str, t_scene *scene)
{
	t_obj	*object;
	t_sph	*sphere;
	char	**args;

	if (*(str + 1) != 'p' || !is_numbers(str + 2, ' '))
		return (put_error("Unexpected arguments after sp: sphere\n"));
	if (count_words(str, ' ') != 4)
		return (put_error("Invalid number of arguments for sphere\n"));
	object = (t_obj *)malloc(sizeof(t_obj));
	if (!object)
		return (malloc_error(), -1);
	sphere = (t_sph *)malloc(sizeof(t_sph));
	if (!sphere)
		return (malloc_error(), -1);
	args = ft_split(str, ' ');
	if (read_vector(&sphere->pos, args[1]) == -1)
	{
		free_array(args);
		return (put_error("Invalid coordinates for sphere center\n"));
	}
	if (!is_float(args[2]) || ft_atof(args[2]) <= 0)
	{
		free_array(args);
		return (put_error("Invalid sphere diameter value\n"));
	}
	sphere->r = ft_atof(args[2]) / 2;
	if (read_color(&sphere->color, args[3]) == -1)
	{
		free_array(args);
		return (put_error("Invalid color value of sphere\n"));
	}
	free_array(args);
	object->id = 's';
	object->obj = sphere;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

int	add_plane(char *str, t_scene *scene)
{
	t_obj	*object;
	t_plane	*pl;
	char	**args;

	if (*(str + 1) != 'l' || !is_numbers(str + 2, ' '))
		return (put_error("Unexpected arguments after pl: plane\n"));
	if (count_words(str, ' ') != 4)
		return (put_error("Invalid number of arguments for plane\n"));
	object = (t_obj *)malloc(sizeof(t_obj));
	if (!object)
		return (malloc_error(), -1);
	pl = (t_plane *)malloc(sizeof(t_plane));
	if (!pl)
		return (malloc_error(), -1);
	args = ft_split(str, ' ');
	if (read_vector(&pl->point, args[1]) == -1)
	{
		free_array(args);
		return (put_error("Invalid coordinates for plane point\n"));
	}
	if (read_vector(&pl->normal, args[2]) == -1 || !is_unit_vec(&pl->normal))
	{
		free_array(args);
		return (put_error("Invalid value of plane normal vector\n"));
	}
	if (read_color(&pl->color, args[3]) == -1)
	{
		free_array(args);
		return (put_error("Invalid color value of plane\n"));
	}
	free_array(args);
	object->id = 'p';
	object->obj = pl;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

int	add_cylinder(char *str, t_scene *scene)
{
	t_obj	*object;

	(void) str;
	(void) scene;

	object = NULL;
	return (0);
}

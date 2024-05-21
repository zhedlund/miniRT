/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:35:45 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/21 19:03:36 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

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

int	read_plane_input(char *str, t_plane *pl)
{
	char	**args;

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
	return (0);
}

int	add_plane(char *str, t_scene *scene)
{
	t_obj	*object;
	t_plane	*pl;

	if (*(str + 1) != 'l' || !is_numbers(str + 2, ' '))
		return (put_error("Unexpected arguments after pl: plane\n"));
	if (count_words(str, ' ') != 4)
		return (put_error("Invalid number of arguments for plane\n"));
	pl = (t_plane *)malloc(sizeof(t_plane));
	if (!pl)
		return (malloc_error(), -1);
	if (read_plane_input(str, pl) == -1)
	{
		free(pl);
		return (-1);
	}
	object = (t_obj *)malloc(sizeof(t_obj));
	if (!object)
		return (malloc_error(), -1);
	object->id = PLANE;
	object->obj = pl;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

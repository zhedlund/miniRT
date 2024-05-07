/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:35:45 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/30 14:26:42 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
//#include "parsing.h"

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
	if (read_vector(&sphere->center, args[1]) == -1)
	{
		free_array(args);
		return (put_error("Invalid coordinates for sphere center\n"));
	}
	if (!is_float(args[2]) || ft_atof(args[2]) <= 0)
	{
		free_array(args);
		return (put_error("Invalid sphere diameter value\n"));
	}
	sphere->radius = ft_atof(args[2]) / 2;
	if (read_color(&sphere->color, args[3]) == -1)
	{
		free_array(args);
		return (put_error("Invalid color value of sphere\n"));
	}
	free_array(args);
	object->id = SPHERE;
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
	object->id = PLANE;
	object->obj = pl;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

int	add_cylinder(char *str, t_scene *scene)
{
	t_obj	*object;
	t_cyl	*cyl;
	char	**args;

	if (*(str + 1) != 'y' || !is_numbers(str + 2, ' '))
		return (put_error("Unexpected arguments after cy: cylinder\n"));
	if (count_words(str, ' ') != 6)
		return (put_error("Invalid number of arguments for cylinder\n"));
	object = (t_obj *)malloc(sizeof(t_obj));
	if (!object)
		return (malloc_error(), -1);
	cyl = (t_cyl *)malloc(sizeof(t_cyl));
	if (!cyl)
		return (malloc_error(), -1);
	args = ft_split(str, ' ');
	if (read_vector(&cyl->center, args[1]) == -1)
	{
		free_array(args);
		return (put_error("Invalid coordinates for cylinder center\n"));
	}
	if (read_vector(&cyl->normal, args[2]) == -1 || !is_unit_vec(&cyl->normal))
	{
		free_array(args);
		return (put_error("Invalid value of cylinder normal vector\n"));
	}
	if (!is_float(args[3]) || ft_atof(args[3]) <= 0)
	{
		free_array(args);
		return (put_error("Invalid cylinder diameter value\n"));
	}
	cyl->r = ft_atof(args[3]) / 2;
	if (!is_float(args[4]) || ft_atof(args[4]) <= 0)
	{
		free_array(args);
		return (put_error("Invalid cylinder height value\n"));
	}
	cyl->h = ft_atof(args[4]);
	if (read_color(&cyl->color, args[5]) == -1)
	{
		free_array(args);
		return (put_error("Invalid color value of cylinder\n"));
	}
	free_array(args);
	set_cylinder(cyl);
	printf("cylinder top color: %f,%f,%f\n", cyl->top.color.r * 255, cyl->top.color.g * 255, cyl->top.color.b * 255);
	printf("cylinder bottom normal: %f,%f,%f\n", cyl->bottom.normal.x, cyl->bottom.normal.y, cyl->bottom.normal.z);
	object->id = CYLINDER;
	object->obj = cyl;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:54:59 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/17 16:04:11 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

int	read_sphere_input(char *str, t_sph *sphere)
{
	char	**args;

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
	return (0);
}

int	add_sphere(char *str, t_scene *scene)
{
	t_obj	*object;
	t_sph	*sphere;

	if (*(str + 1) != 'p' || !is_numbers(str + 2, ' '))
		return (put_error("Unexpected arguments after sp: sphere\n"));
	if (count_words(str, ' ') != 4)
		return (put_error("Invalid number of arguments for sphere\n"));
	sphere = (t_sph *)malloc(sizeof(t_sph));
	if (!sphere)
		return (malloc_error(), -1);
	if (read_sphere_input(str, sphere) == -1)
	{
		free(sphere);
		return (-1);
	}
	object = (t_obj *)malloc(sizeof(t_obj));
	if (!object)
		return (malloc_error(), -1);
	object->id = SPHERE;
	object->obj = sphere;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:54:34 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/17 19:28:14 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

static int	frerror(char **args, char *msg)
{
	free_array(args);
	return (put_error(msg));
}

int	read_cylinder_input(char *str, t_cyl *cyl)
{
	char	**args;

	args = ft_split(str, ' ');
	if (read_vector(&cyl->center, args[1]) == -1)
		return (frerror(args, "Invalid coordinates for cylinder center\n"));
	if (read_vector(&cyl->normal, args[2]) == -1 || !is_unit_vec(&cyl->normal))
		return (frerror(args, "Invalid value of cylinder normal vector\n"));
	if (!is_float(args[3]) || ft_atof(args[3]) <= 0)
		return (frerror(args, "Invalid cylinder diameter value\n"));
	cyl->r = ft_atof(args[3]) / 2;
	if (!is_float(args[4]) || ft_atof(args[4]) <= 0)
		return (frerror(args, "Invalid cylinder height value\n"));
	cyl->h = ft_atof(args[4]);
	if (read_color(&cyl->color, args[5]) == -1)
		return (frerror(args, "Invalid color value of cylinder\n"));
	free_array(args);
	return (0);
}

int	add_cylinder(char *str, t_scene *scene)
{
	t_obj	*object;
	t_cyl	*cyl;

	if (*(str + 1) != 'y' || !is_numbers(str + 2, ' '))
		return (put_error("Unexpected arguments after cy: cylinder\n"));
	if (count_words(str, ' ') != 6)
		return (put_error("Invalid number of arguments for cylinder\n"));
	cyl = (t_cyl *)malloc(sizeof(t_cyl));
	if (!cyl)
		return (malloc_error(), -1);
	if (read_cylinder_input(str, cyl) == -1)
	{
		free(cyl);
		return (-1);
	}
	set_cylinder(cyl);
	object = (t_obj *)malloc(sizeof(t_obj));
	if (!object)
		return (malloc_error(), -1);
	object->id = CYLINDER;
	object->obj = cyl;
	object->next = NULL;
	add_object(object, scene);
	return (0);
}

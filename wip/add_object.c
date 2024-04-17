/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:35:45 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/17 20:12:53 by kdzhoha          ###   ########.fr       */
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
	(void) str;

	// scene->light.pos.x = -40;
	// scene->light.pos.y = 50;
	// scene->light.pos.z = 0;
	// scene->light.ratio = 0.6;
	// scene->light.color.r = 255;
	// scene->light.color.g = 255;
	// scene->light.color.b = 0;
	return (0);
}
//parse str to get real input; protect malloc;

int	add_sphere(char *str, t_scene *scene)
{
	t_obj	*object;
	t_sph	*sphere;

	(void) str;

	object = (t_obj *)malloc(sizeof(t_obj));
	sphere = (t_sph *)malloc(sizeof(t_sph));
	sphere->pos.x = 0;
	sphere->pos.y = 0;
	sphere->pos.z = 20.6;
	sphere->r = 6.3;
	sphere->color.r = 10;
	sphere->color.g = 0;
	sphere->color.b = 255;
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

	(void) str;

	object = (t_obj *)malloc(sizeof(t_obj));
	pl = (t_plane *)malloc(sizeof(t_plane));
	pl->point.x = 0;
	pl->point.y = -7;
	pl->point.z = -10;
	pl->normal.x = 0;
	pl->normal.y = 1;
	pl->normal.z = 0;
	pl->color.r = 0;
	pl->color.g = 0;
	pl->color.b = 225;
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

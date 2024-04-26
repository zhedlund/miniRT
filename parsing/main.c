/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:45:57 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/26 15:29:14 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_scene	*scene;
	t_sph	*sph;

	if (argc != 2)
		return (put_error("Too many arguments\n"));
	if (!is_valid_name(argv[1]))
		return (put_error("Invalid file name\n"));
	scene = parse_input(argv[1]);
	if (!scene)
		return (1);
	printf("amb color: %i,%i,%i\n", (int)scene->amb.color.r, (int)scene->amb.color.g, (int)scene->amb.color.b);
	printf("FOV: %i\n", scene->cam.fov);
	printf("spot light ratio: %f\n", scene->light.ratio);
	sph = scene->objs->obj;
	printf("sphere radius: %f", sph->r);
	free_scene(scene);
	return (0);
}

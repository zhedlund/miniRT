/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:53:29 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/15 15:40:49 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	*get_px_delta(t_cam *cam)
{
	t_vec	*vec;

	vec = NULL;
	return (vec);
}

void	ray_tracing(t_scene *scene)
{
	t_vec	*pixel_delta;
	t_vec	*pixel00;

	pixel00 = get_px_0();
	pixel_delta = get_px_delta(&scene->cam);
	return ;
}

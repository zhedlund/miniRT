/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traicing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:53:29 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/04 18:45:55 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

typedef struct s_cam
{
	float	x;
	float	y;
	float	z;
	float	dir[3];
	int		fov;
}	t_cam;

typedef struct s_sph
{
	float	x;
	float	y;
	float	z;
	float	r;
	int		color;
}	t_sph;

typedef struct s_obj
{
	char			id;
	void			*obj;
	struct s_obj	*next;
}	t_obj;

typedef struct s_scene
{
	t_cam	*cam;
	t_obj	*objs;
}	t_scene;


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:46:37 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/01 19:50:21 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_top_point(t_cyl *cyl)
{
	cyl->top_p.x = cyl->top_p.x - cyl->normal.x * cyl->h / 2;
	cyl->top_p.y = cyl->top_p.y - cyl->normal.y * cyl->h / 2;
	cyl->top_p.z = cyl->top_p.z - cyl->normal.z * cyl->h / 2;
}

void	hit_cylinder(t_cyl *cyl, t_ray *ray, t_hit *hit)
{
	set_top_point(cyl);
	hit_top(cyl, ray, hit);
	hit_bottom(cyl, ray, hit);
	hit_side(cyl, ray, hit);
	return (0);
}

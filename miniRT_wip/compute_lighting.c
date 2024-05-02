/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_lighting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:32:34 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/01 19:42:26 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	get_light_i(t_vec light, t_vec normal, float ratio)
{
	float	res;

	res = ratio * dot(&normal, &light) / (vec3_length(&normal) * vec3_length(&light));
	return (res);
}

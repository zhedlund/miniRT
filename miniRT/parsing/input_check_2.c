/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:27:24 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/05/17 16:28:10 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

int	check_vector(char **vec)
{
	int	i;

	i = 0;
	while (vec[i])
	{
		if (!is_float(vec[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_unit_vec(t_vec *vec)
{
	float	sum;

	if (vec->x < -1.0 || vec->x > 1.0)
		return (0);
	if (vec->y < -1.0 || vec->y > 1.0)
		return (0);
	if (vec->z < -1.0 || vec->z > 1.0)
		return (0);
	sum = pow2(vec->x) + pow2(vec->y) + pow2(vec->z);
	if (sum < 0.99 || sum > 1.01)
		return (0);
	return (1);
}

float	valid_ratio(char *str)
{
	float	res;

	if (!is_float(str))
		return (-1);
	res = ft_atof(str);
	if (res < 0.0 || res > 1.0)
		return (-1);
	return (res);
}

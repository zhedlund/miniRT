/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:07:57 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/17 19:30:44 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


int	is_digit(char c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}

int	is_numbers(char *str, char begin)
{
	if (*str != begin)
		return (0);
	str++;
	while (*str)
	{
		if (!is_digit(*str))
		{
			if (*str != '.' && *str != ',' && *str != ' ' && *str != '-')
				return (0);
		}
		str++;
	}
	return (1);
}

int	is_uns_int(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (!is_digit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	is_float(char *str)
{
	if (!str)
		return (0);
	if (*str == '-')
		str++;
	while (*str && *str != '.')
	{
		if (!is_digit(*str))
			return (0);
		str++;
	}
	if (*str == '.')
		str++;
	while (*str)
	{
		if (!is_digit(*str))
			return (0);
		str++;
	}
	return (1);
}

float	is_color(char *str)
{
	float	result;

	if (!str || !is_uns_int(str))
		return (-1);
	result = ft_atof(str);
	if (result < 0 || result > 255)
		return (-1);
	return (result);
}
// check array of strings representing float number before ft_atof
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
	if (vec->x < -1.0 || vec->x > 1.0)
		return (0);
	if (vec->y < -1.0 || vec->y > 1.0)
		return (0);
	if (vec->z < -1.0 || vec->z > 1.0)
		return (0);
	return (1);
}

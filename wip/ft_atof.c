/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atof.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 13:02:59 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/16 15:05:21 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_atoi(const char *nptr)
{
	int	nb;
	int	sign;

	nb = 0;
	sign = 1;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == 45 || *nptr == 43)
	{
		if (*nptr == 45)
			sign = -1;
		nptr++;
	}
	while (*nptr >= 48 && *nptr <= 57)
	{
		nb = nb * 10 + (*nptr - 48);
		nptr++;
	}
	return (nb * sign);
}

float	ft_atof(const char *str)
{
	float	result;
	float	power;
	char	*dec_point;
	int		sign;
	int		i;

	sign = 1;
	power = 1.0;
	if (str[0] == '-')
	{
		sign = -1;
		str++;
	}
	result = (float)ft_atoi(str++);
	dec_point = ft_strchr(str, '.');
	if (dec_point)
	{
		i = dec_point - str + 1;
		while (ft_isdigit(str[i]))
		{
			result = result * 10.0 + (str[i++] - '0');
			power *= 10.0;
		}
	}
	return (result * sign / power);
}


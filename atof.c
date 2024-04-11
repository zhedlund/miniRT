/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atof.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 13:02:59 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/11 18:55:35 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


double	ft_atof(const char *str)
{
	double	result;
	double	power;
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
	result = (double)ft_atoi(str++);
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


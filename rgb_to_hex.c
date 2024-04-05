/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_to_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:15:09 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/04 18:17:39 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

unsigned long rgb_to_hex(int r, int g, int b)
{   
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	// to add alpha: + (a & 0xff);
}

int main()
{
	int r = 255;
	int g = 255;
	int b = 255;
	unsigned long color = rgb_to_hex(r, g, b);
	printf("0x%06lx\n", color);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:26:07 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/27 19:54:53 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


/* 	Calculates the ambient color of a pixel. The ambient color is the color 
	of the pixel when no light source is illuminating it.
*/
t_color amb_color(const t_amb *a, const t_color *c)
{
    return ((t_color){a->ratio * c->r, a->ratio * c->g, a->ratio * c->b});
}

/* 	Calculates the diffuse color of a pixel. The diffuse color is the color 
	of the pixel when illuminated by a light source. 
	fmax returns the maximum of the two values.
*/
/*t_color diffuse_color(const t_light *l, const t_color *c, float diffuse_factor)
{
    return ((t_color){fmaxf(0, diffuse_factor) * c->r * l->color.r * l->ratio,
                     fmaxf(0, diffuse_factor) * c->g * l->color.g * l->ratio,
                     fmaxf(0, diffuse_factor) * c->b * l->color.b * l->ratio});
}*/

t_color diffuse_color(const t_light *l, const t_color *c, float diffuse_factor)
{
    return ((t_color){diffuse_factor * c->r * l->color.r * l->ratio,
                     diffuse_factor * c->g * l->color.g * l->ratio,
                     diffuse_factor * c->b * l->color.b * l->ratio});
}

/* 	Writes the color value of a pixel to the image buffer.
	Color values are represented as 24-bit RGB values.
*/
void write_color(t_color px, t_img *img, int x, int y) 
{
    int color_value = ((int)(255.999 * px.r) << 16) +
                      ((int)(255.999 * px.g) << 8) +
                      (int)(255.999 * px.b);
    ft_pixel_put(img, x, y, color_value);
}

/* 	Adds two color values together.
	Returns a new color value that is the sum of the two input color values.
*/
t_color blend_color(const t_color *c1, const t_color *c2)
{
	float r;
	float g;
	float b;

	r = c1->r + c2->r;
	g = c1->g + c2->g;
	b = c1->b + c2->b;
	
	if (r > 1.0)
		r = 1.0;
	if (g > 1.0)
		g = 1.0;
	if (b > 1.0)
		b = 1.0;
    return ((t_color){r, g, b});
}
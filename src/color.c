/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 11:52:23 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 15:37:53 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

unsigned int	c_rgb(unsigned int r, unsigned int g, unsigned int b)
{
	return ((r << 16) | (g << 8) | b);
}

unsigned int	c_darken(unsigned int color, float darkness)
{
	color = c_rgb(
	(unsigned char)clampf(((color & 0x00ff0000) >> 16) - darkness, 0, 255),
	(unsigned char)clampf(((color & 0x0000ff00) >> 8) - darkness, 0, 255),
	(unsigned char)clampf((color & 0x000000ff) - darkness, 0, 255));
	return (color);
}

unsigned int	c_greenify(unsigned int color, float green)
{
	color = c_rgb(
	(unsigned char)clampf(((color & 0x00ff0000) >> 16), 0, 255),
	(unsigned char)clampf(((color & 0x0000ff00) >> 8) + green, 0, 255),
	(unsigned char)clampf((color & 0x000000ff), 0, 255));
	return (color);
}

void			draw_green(t_main *m)
{
	int x;
	int y;
	int color;

	y = -1;
	while (++y < m->sdl.img.h)
	{
		x = -1;
		while (++x < m->sdl.img.w)
		{
			color = m->sdl.img.pixels[x + y * m->sdl.img.w];
			m->sdl.img.pixels[x + y * m->sdl.img.w] = c_greenify(color, 100);
		}
	}
}

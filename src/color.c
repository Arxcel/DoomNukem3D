/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 11:52:23 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 12:01:25 by vkozlov          ###   ########.fr       */
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

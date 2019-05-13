/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_create_image.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arxcel <arxcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 18:43:37 by vkozlov           #+#    #+#             */
/*   Updated: 2019/05/13 23:41:38 by arxcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sdl.h"

t_img				sdl_create_image(size_t w, size_t h)
{
	t_img img;

	img.w = w;
	img.h = h;
	if (!(img.pixels = ft_memalloc(sizeof(unsigned int) * img.w * img.h)))
		MSG("Error allocating memory for pixels");
	sdl_clear_image(&img);
	return (img);
}

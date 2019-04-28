/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_put_image.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 18:45:31 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 18:37:30 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sdl.h"

void				sdl_put_image(t_sdl *sdl)
{
	SDL_UpdateTexture(sdl->texture, NULL,
		sdl->img.pixels, sdl->img.w * sizeof(unsigned int));
	sdl_clear_image(&sdl->img);
	SDL_RenderCopy(sdl->ren, sdl->texture, NULL, NULL);
}

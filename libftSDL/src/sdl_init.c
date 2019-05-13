/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arxcel <arxcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 18:42:04 by vkozlov           #+#    #+#             */
/*   Updated: 2019/05/13 23:42:25 by arxcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sdl.h"

void				sdl_init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		MSG(SDL_GetError());
	if (!(sdl->win = SDL_CreateWindow("My SDL", SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, sdl->win_w,
								sdl->win_h, SDL_WINDOW_SHOWN |
											SDL_WINDOW_RESIZABLE |
											SDL_WINDOW_ALLOW_HIGHDPI)))
		MSG(SDL_GetError());
	if (!(sdl->ren = SDL_CreateRenderer(sdl->win, -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		MSG(SDL_GetError());
	if (!(sdl->texture = SDL_CreateTexture(sdl->ren,
								SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STATIC,
								sdl->win_w, sdl->win_h)))
		MSG(SDL_GetError());
	// SDL_ShowCursor(SDL_DISABLE);
	// SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_SetRenderDrawColor(sdl->ren, 255, 255, 255, 255);
	SDL_RenderClear(sdl->ren);
	SDL_RenderPresent(sdl->ren);
	sdl->img = sdl_create_image(sdl->isRetina ? sdl->win_w * 2 : sdl->win_w, sdl->isRetina ? sdl->win_h * 2 : sdl->win_h);
	sdl->running = 1;
}

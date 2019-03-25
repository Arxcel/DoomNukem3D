/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_load_surface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 17:01:44 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/25 00:23:59 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sdl.h"
#include "ft_libftu.h"

SDL_Surface	*sdl_load_surface(const char *path, int is_format)
{
	SDL_Surface		*out;
	SDL_Surface		*temp;
	SDL_PixelFormat	fmt;

	out = NULL;
	temp = NULL;
	if ((temp = IMG_Load(path)) == NULL)
	{
		ft_putendl(SDL_GetError());
		return (NULL);
	}
	if (is_format)
	{
		fmt = (SDL_PixelFormat){.BytesPerPixel = 4,
		.BitsPerPixel = 32, .format = SDL_PIXELFORMAT_ARGB8888};
		if ((out = SDL_ConvertSurface(temp, &fmt, 0)) == NULL)
		{
			ft_putendl(SDL_GetError());
			return (NULL);
		}
	}
	return (is_format ? out : temp);
}

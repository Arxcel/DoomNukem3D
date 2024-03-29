/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_load_surface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:37:24 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 18:37:25 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sdl.h"
#include "ft_libftu.h"

SDL_Surface	*sdl_load_surface(const char *path, int is_format, size_t format)
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
		fmt = (SDL_PixelFormat){.BytesPerPixel = 4, .BitsPerPixel = 32,
								.format = format};
		if ((out = SDL_ConvertSurface(temp, &fmt, 0)) == NULL)
		{
			ft_putendl(SDL_GetError());
			return (NULL);
		}
	}
	return (is_format ? out : temp);
}

SDL_Surface	*sdl_load_surface_from_res(SDL_RWops *src,
											int is_format, size_t format)
{
	SDL_Surface		*out;
	SDL_Surface		*temp;
	SDL_PixelFormat	fmt;

	out = NULL;
	temp = NULL;
	if ((temp = IMG_Load_RW(src, SDL_TRUE)) == NULL)
	{
		ft_putendl(SDL_GetError());
		return (NULL);
	}
	if (is_format)
	{
		fmt = (SDL_PixelFormat){.BytesPerPixel = 4, .BitsPerPixel = 32,
								.format = format};
		if ((out = SDL_ConvertSurface(temp, &fmt, 0)) == NULL)
		{
			ft_putendl(SDL_GetError());
			return (NULL);
		}
	}
	return (is_format ? out : temp);
}

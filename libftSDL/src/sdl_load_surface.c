/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_load_surface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 17:01:44 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/21 17:23:55 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sdl.h"
#include "ft_libftu.h"

SDL_Surface	*sdl_load_surface(const char *path)
{
	SDL_Surface	*out;

	out = NULL;
	if ((out = IMG_Load(path)) == NULL)
	{
		ft_putendl(SDL_GetError());
		return (NULL);
	}
	return (out);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_text_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 19:16:34 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 19:16:41 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int					create_text(t_main *m, t_text *menu, int i, SDL_Color fg)
{
	SDL_Surface		*text_surface;
	int				shift;
	int				line;
	SDL_Rect		dstrect;

	shift = i > menu->rows - 1 ? 250 : 0;
	line = i > menu->rows - 1 ? i % menu->rows : i;
	dstrect = (SDL_Rect){m->sdl.win_w * 0.7 + shift, 15 + line * LETTER_HEIGHT,
		strlen(menu[i].text) * LETTER_WIDTH, LETTER_HEIGHT};
	text_surface = TTF_RenderText_Shaded(m->hud.font, menu[i].text,
											fg, (SDL_Color){0, 0, 0, 255});
	menu[i].text_texture =
						SDL_CreateTextureFromSurface(m->sdl.ren, text_surface);
	SDL_FreeSurface(text_surface);
	menu[i].dstrect = dstrect;
	return (0);
}

void				update_text(t_main *m, t_text *menu, int i, int str)
{
	SDL_Color bg;

	bg = (SDL_Color){255, 255, 255, 255};
	SDL_DestroyTexture(menu[i].text_texture);
	free(menu[i].text);
	menu[i].text = ft_itoa(str);
	create_text(m, menu, i, bg);
}

void				update_text_color(t_main *m, t_text *menu, int i)
{
	SDL_Color selected;
	SDL_Color bg;

	selected = (SDL_Color){0, 0, 255, 255};
	bg = (SDL_Color){255, 255, 255, 255};
	SDL_DestroyTexture(menu[i].text_texture);
	if (menu[i].selected)
		create_text(m, menu, i, selected);
	else
		create_text(m, menu, i, bg);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_text.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:49:06 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/28 15:29:43 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		create_text(t_main *m, t_text *menu, int i, SDL_Color fg)
{
	SDL_Surface		*textSurface;
	int				shift;
	int				line;

	shift = i > TEXT_MENU_ROW - 1 ? 250 : 0;
	line = i > TEXT_MENU_ROW - 1 ? i % TEXT_MENU_ROW + 1 : i;
	SDL_Rect		dstrect = {m->sdl.win_w * 0.7 + shift, 15 + line * LETTER_HEIGHT,
		strlen(menu[i].text) * LETTER_WIDTH, LETTER_HEIGHT};
	SDL_Color bg = {0, 0, 0};

	textSurface = TTF_RenderText_Shaded(m->hud.font, menu[i].text, fg, bg);
	menu[i].text_texture = SDL_CreateTextureFromSurface(m->sdl.ren, textSurface);
	SDL_FreeSurface(textSurface);
	menu[i].dstrect = dstrect;
	return (0);
}

void		update_text(t_main *m, t_text *menu, int i, int str)
{
	SDL_Color bg = {255, 255, 255};

	SDL_DestroyTexture(menu[i].text_texture);
	free(menu[i].text);
	menu[i].text = ft_itoa(str);
	create_text(m, menu, i, bg);
}

void		update_text_color(t_main *m, t_text *menu, int i, SDL_Color color)
{
	SDL_DestroyTexture(menu[i].text_texture);
	create_text(m, menu, i, color);
}

void		create_text_menu(t_main *m, t_text *menu)
{
	int i;
	SDL_Color bg = {255, 255, 255};

	menu[0].text = ft_strdup("Sectors");
	menu[1].text = ft_strdup("Sector:");
	menu[2].text = ft_strdup("Walls:");
	menu[3].text = ft_strdup("Texture:");
	menu[4].text = ft_strdup("Floor Height:");
	menu[5].text = ft_strdup("Ceiling Height:");
	menu[6].text = ft_strdup("Is lifted");
	menu[7].text = ft_strdup("From:");
	menu[8].text = ft_strdup("To:");
	menu[9].text = ft_strdup("0");
	menu[10].text = ft_strdup("0");
	menu[11].text =  ft_strdup("0");
	menu[12].text = ft_strdup("0");
	menu[13].text = ft_strdup("100");
	menu[14].text = ft_strdup("0");
	menu[15].text = ft_strdup("0");
	menu[16].text = ft_strdup("10");
	i = -1;
	while( ++i < TEXT_MENU)
	{
		menu[i].selected = false;
		create_text(m, menu, i, bg);
	}
}

void				update_all_menu(t_main *m, t_map_editor *e)
{
	int i;

	i = -1;
	while( ++i < TEXT_MENU)
		e->menu[i].selected = false;
	e->menu[e->selected_row].selected = true;
	update_text(m, e->menu, 9, e->n);
	if (e->sectors[e->n].num_walls > -1)
		update_text(m, e->menu, 10, e->sectors[e->n].num_walls);
	if (e->sectors[e->n].num_walls > 0)
		update_text(m, e->menu, 11, e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture);
	update_text(m, e->menu, 12, e->sectors[e->n].floor_height);
	update_text(m, e->menu, 13, e->sectors[e->n].ceiling_height);
}

int					remove_text_menu(t_text *menu)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
	{
		SDL_DestroyTexture(menu[i].text_texture);
		free(menu[i].text);
	}
	return (0);
}

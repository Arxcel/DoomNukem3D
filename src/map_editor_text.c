#include "map_editor.h"
#include "doom_nukem.h"

int		create_text(t_main *m, t_text *menu, int i, const char *str)
{
	SDL_Surface		*textSurface;
	int				shift;
	int				line;

	shift = i > TEXT_MENU_ROW - 1 ? 250 : 0;
	line = i > TEXT_MENU_ROW - 1 ? i % TEXT_MENU_ROW + 1 : i;
	SDL_Rect		dstrect = {m->sdl.win_w * 0.7 + shift, 15 + line * LETTER_HEIGHT,
		strlen(str) * LETTER_WIDTH, LETTER_HEIGHT};
	SDL_Color fg = {255, 255, 255};
	SDL_Color bg = {0, 0, 0};

	textSurface = TTF_RenderText_Shaded(m->font, str, fg, bg);
	menu[i].text_texture = SDL_CreateTextureFromSurface(m->sdl.ren, textSurface);
	SDL_FreeSurface(textSurface);
	menu[i].dstrect = dstrect;
	return (0);
}

void		update_text(t_main *m, t_text *menu, int i, int str)
{
	char *buf;

	SDL_DestroyTexture(menu[i].text_texture);
	buf = ft_itoa(str);
	create_text(m, menu, i, buf);
	free(buf);
}

void		create_text_menu(t_main *m, t_text *menu)
{
	create_text(m, menu, 0, "Sectors");
	create_text(m, menu, 1, "Sector:");
	create_text(m, menu, 2, "Walls:");
	create_text(m, menu, 3, "Texture:");
	create_text(m, menu, 4, "Floor Height:");
	create_text(m, menu, 5, "Ceiling Height:");
	create_text(m, menu, 6, "0");
	create_text(m, menu, 7, "0");
	create_text(m, menu, 8, "0");
	create_text(m, menu, 9, "0");
	create_text(m, menu, 10, "100");
}

void				update_all_menu(t_main *m, t_map_editor *e)
{
	update_text(m, e->menu, 6, e->n);
	if (e->sectors[e->n].num_walls > -1)
		update_text(m, e->menu, 7, e->sectors[e->n].num_walls);
	if (e->sectors[e->n].num_walls > 0)
		update_text(m, e->menu, 8, e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture);
	update_text(m, e->menu, 9, e->sectors[e->n].floor_height);
	update_text(m, e->menu, 10, e->sectors[e->n].ceiling_height);
}

int					remove_text_menu(t_text *menu)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
		SDL_DestroyTexture(menu[i].text_texture);
	return (0);
}

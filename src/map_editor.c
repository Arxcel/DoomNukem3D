#include "map_editor.h"
#include "doom_nukem.h"

t_dot	module(t_dot a, t_dot b)
{
	t_dot d;

	d.x = abs(b.x - a.x);
	d.y = abs(b.y - a.y);
	return (d);
}

t_dot	direction(t_dot a, t_dot b)
{
	t_dot s;

	s.x = a.x < b.x ? 1 : -1;
	s.y = a.y < b.y ? 1 : -1;
	return (s);
}


void	print_vector(t_editor_wall wall, t_main *main)
{
	if (wall.begin.x > -1 && wall.begin.y > -1 &&
		wall.begin.x < main->sdl.win_w && wall.begin.y <  main->sdl.win_h)
		sdl_pixel_put(&main->sdl.img, wall.begin.x, wall.begin.y, wall.color);
}

bool	compare(t_dot a, t_dot b)
{
	return ((a.x == b.x - 1 && a.y == b.y - 1) || (a.x == b.x + 1 && a.y == b.y + 1)
		|| (a.x == b.x && a.y == b.y) || (a.x == b.x - 1 && a.y == b.y + 1)
		|| (a.x == b.x + 1 && a.y == b.y - 1));
}

int		intersect(t_editor_wall wall, t_dot cur)
{
	t_dot error;
	t_dot d;
	t_dot s;

	d = module(wall.begin, wall.end);
	s = direction(wall.begin, wall.end);
	error.x = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		if (compare(wall.begin, cur))
			return (1);
		if (wall.begin.x == wall.end.x && wall.begin.y == wall.end.y)
			break ;
		error.y = error.x;
		if (error.y > -d.x)
		{
			error.x -= d.y;
			wall.begin.x += s.x;
		}
		if (error.y < d.y)
		{
			error.x += d.x;
			wall.begin.y += s.y;
		}
	}
	return (0);
}

int		line(t_editor_wall wall, t_main *main)
{
	t_dot error;
	t_dot d;
	t_dot s;

	d = module(wall.begin, wall.end);
	s = direction(wall.begin, wall.end);
	error.x = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		print_vector(wall, main);
		if (wall.begin.x == wall.end.x && wall.begin.y == wall.end.y)
			break ;
		error.y = error.x;
		if (error.y > -d.x)
		{
			error.x -= d.y;
			wall.begin.x += s.x;
		}
		if (error.y < d.y)
		{
			error.x += d.x;
			wall.begin.y += s.y;
		}
	}
	return (0);
}

int		init_map_editor(t_main *main)
{
	TTF_Init();
	if (!main || !(main->font = TTF_OpenFont("./assets/fonts/ARIAL.TTF",24)))
		return (1);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_SetRenderDrawColor(main->sdl.ren, 0, 0, 0, 0);
	SDL_RenderClear(main->sdl.ren);
	SDL_RenderPresent(main->sdl.ren);
	return (0);
}

int				editor_clear_sdl(t_sdl *sdl)
{
	puts("free");
	SDL_DestroyTexture(sdl->texture);
	SDL_DestroyRenderer(sdl->ren);
	SDL_DestroyWindow(sdl->win);
	free(sdl->img.pixels);
	SDL_Quit();
	return (0);
}


/*Returns number of walls in sector*/
int		close_sector(t_main *m, int i, t_editor_sector *sector)
{
	if (sector[i].num_walls > 1 && sector[i].num_walls < WALLS_CNT)
	{
		sector[i].wall_vertice[sector[i].num_walls].color = YELLOW;
		sector[i].wall_vertice[sector[i].num_walls].begin.x =
			sector[i].wall_vertice[sector[i].num_walls - 1].end.x;
		sector[i].wall_vertice[sector[i].num_walls].begin.y =
			sector[i].wall_vertice[sector[i].num_walls - 1].end.y;
		sector[i].wall_vertice[sector[i].num_walls].end.x = sector[i].wall_vertice[0].begin.x;
		sector[i].wall_vertice[sector[i].num_walls].end.y = sector[i].wall_vertice[0].begin.y;

		sector[i].num_walls++;
		return (sector[i].num_walls);
	}
	return (0);
}

void	print_sectors(t_editor_sector *sectors, int n)
{
	int				i;
	int				j;
	t_editor_wall	w;
	int				gl;

	i = -1;
	gl = -1;
	while(++i <= n)
	{
		printf("Sector %i\n", i);
		j = -1;
		while (++j < sectors[i].num_walls)
		{
			sectors[i].wall_vertice[j].global_index = ++gl;
			w = sectors[i].wall_vertice[j];
			printf("Wall %i: begin x = %i, y = %i end x = %i, y = %i neighbor = %i global index = %i\n",
				j, w.begin.x, w.begin.y, w.end.x, w.end.y, sectors[i].neighbors[j], w.global_index);
		}
	}
	

}

void	shift_left(t_editor_sector *sectors, int n)
{
	t_dot	min;
	int		i;
	int		j;
	int		gl;

	gl = 0;
	min = sectors[0].wall_vertice[0].begin;
	i = -1;
	while (++i < n && (j = -1))
		while (++j < sectors[i].num_walls)
		{
			if (sectors[i].wall_vertice[j].begin.x < min.x)
				min.x = sectors[i].wall_vertice[j].begin.x;
			if (sectors[i].wall_vertice[j].begin.y < min.y)
				min.y = sectors[i].wall_vertice[j].begin.y;
		}
	i = -1;
	while (++i < n && (j = -1))
		while (++j < sectors[i].num_walls)
		{
			sectors[i].wall_vertice[j].begin.x -= min.x;
			sectors[i].wall_vertice[j].begin.y -= min.y;
			sectors[i].wall_vertice[j].end.x -= min.x;
			sectors[i].wall_vertice[j].end.y -= min.y;
			sectors[i].wall_vertice[j].global_index = gl++;
		}
}

int		check_intersection(t_editor_sector *sectors, int n, int intersected, t_dot mouse)
{
	int j;

	j = -1;
	while (++j < sectors[n].num_walls)
	{
		if (intersect(sectors[n].wall_vertice[j], mouse))
		{
			if (intersected == j)
			{
				intersected = -1;
				sectors[n].wall_vertice[j].intersected = false;
				sectors[n].wall_vertice[j].color = YELLOW;
			}
			else if (intersected == -1)
			{
				intersected = j;
				sectors[n].wall_vertice[j].intersected = true;
				sectors[n].wall_vertice[j].color = BLUE;
			}
			break;
		}
	}
	return (intersected);
}

int		create_wall(t_editor_sector *sectors, int n, int intersected, int mode)
{
	t_dot mouse;

	SDL_GetMouseState(&mouse.x, &mouse.y);
	if (mode == TEXTURE && sectors[n].num_walls < WALLS_CNT - 1)
	{
		if (sectors[n].num_walls == -1)
			sectors[n].wall_vertice[0].begin = mouse;
		else
		{
			if (sectors[n].num_walls > 0)
				sectors[n].wall_vertice[sectors[n].num_walls].begin =
					sectors[n].wall_vertice[sectors[n].num_walls - 1].end;
			sectors[n].wall_vertice[sectors[n].num_walls].end = mouse;
		}
		sectors[n].num_walls++;
	}
	else if (mode == PORTAL && n < SECTORS_CNT - 1)
		intersected = check_intersection(sectors, n, intersected, mouse);
	return (intersected);
}


int		create_text(t_main *m, t_text *text_menu, int i, const char *str)
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
	text_menu[i].text_texture = SDL_CreateTextureFromSurface(m->sdl.ren, textSurface);
	SDL_FreeSurface(textSurface);
	text_menu[i].dstrect = dstrect;
	return (0);
}

void		update_text(t_main *m, t_text *text_menu, int i, int str)
{
	char *buf;

	SDL_DestroyTexture(text_menu[i].text_texture);
	buf = ft_itoa(str);
	create_text(m, text_menu, i, buf);
	free(buf);
}

void		create_text_menu(t_main *m, t_text *text_menu)
{
	create_text(m, text_menu, 0, "Sectors");
	create_text(m, text_menu, 1, "Sector:");
	create_text(m, text_menu, 2, "Walls:");
	create_text(m, text_menu, 3, "Texture:");
	create_text(m, text_menu, 4, "Floor Height:");
	create_text(m, text_menu, 5, "Ceiling Height:");
	create_text(m, text_menu, 6, "0");
	create_text(m, text_menu, 7, "0");
	create_text(m, text_menu, 8, "0");
	create_text(m, text_menu, 9, "0");
	create_text(m, text_menu, 10, "100");
}

int		draw(t_main *m, t_editor_sector *sectors, int n,
			int intersected, bool mode, t_text *text_menu)
{
	int i;
	int j;

	i = -1;
	while ((j = -1) && ++i < n + 1)
		while (++j < sectors[i].num_walls)
			line(sectors[i].wall_vertice[j], m);
	SDL_UpdateTexture(m->sdl.texture, NULL,
	m->sdl.img.pixels, m->sdl.img.w * sizeof(unsigned int));
	sdl_clear_image(&m->sdl.img);
	SDL_RenderCopy(m->sdl.ren, m->sdl.texture, NULL, NULL);
	i = -1;
	while (++i < TEXT_MENU)
		SDL_RenderCopy(m->sdl.ren, text_menu[i].text_texture, NULL, &text_menu[i].dstrect);
	SDL_RenderPresent(m->sdl.ren);
	return (intersected);
}

int					init_sectors(t_editor_sector *sectors,
	int *mode, int *intersected, int *n)
{
	int i;
	int j;

	i = -1;
	while (++i < SECTORS_CNT && (j = -1))
		while (++j < WALLS_CNT)
		{
			sectors[i].neighbors[j] = -1;
			sectors[i].num_walls = 1;
			sectors[i].wall_vertice[j].texture = 0;
			sectors[i].wall_vertice[j].color = YELLOW;
			sectors[i].ceiling_height = 100;
			sectors[i].floor_height = 0;
		}
	sectors[0].num_walls = -1;
	*n = 0;
	*mode = TEXTURE;
	*intersected = -1;
	return (0);
}

int					remove_text_menu(t_text *text_menu)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
		SDL_DestroyTexture(text_menu[i].text_texture);
	return (0);
}

void				update_all_menu(t_main *m, t_text *text_menu,t_editor_sector *sectors, int n)
{
	update_text(m, text_menu, 6, n);
	if (sectors[n].num_walls > -1)
		update_text(m, text_menu, 7, sectors[n].num_walls);
	if (sectors[n].num_walls > 0)
		update_text(m, text_menu, 8, sectors[n].wall_vertice[sectors[n].num_walls - 1].texture);
	update_text(m, text_menu, 9, sectors[n].floor_height);
	update_text(m, text_menu, 10, sectors[n].ceiling_height);
}

int				left_arrow_key(
	SDL_Keycode sym, t_editor_sector *sectors, int n, int *mode)
{
	if (sym != SDLK_LEFT  || (n && sectors[n].num_walls < 2))
		return (1);
	if (!n && sectors[n].num_walls < 0)
		return (1);
	sectors[n].wall_vertice[sectors[n].num_walls - 1].texture = 0;
	sectors[n].num_walls--;
	if (*mode == CLOSE)
		(*mode)--;
	return (0);
}
int				arrow_keys(SDL_Keycode sym, t_editor_sector *sectors, int n, int *mode)
{
	if (sym != SDLK_UP && sym != SDLK_DOWN && sym != SDLK_LEFT)
		return (1);
	if ((*mode == TEXTURE || *mode == CLOSE) && !left_arrow_key(sym, sectors, n, mode))
		return (1);
	if ((*mode == TEXTURE || *mode == CLOSE) && sectors[n].num_walls > 0)
	{
		if (sym == SDLK_UP && sectors[n].wall_vertice[sectors[n].num_walls - 1].texture < TEXTURE_MAX)
			sectors[n].wall_vertice[sectors[n].num_walls - 1].texture++;
		else if (sym == SDLK_DOWN && sectors[n].wall_vertice[sectors[n].num_walls - 1].texture > 0)
			sectors[n].wall_vertice[sectors[n].num_walls - 1].texture--;
		else
			return (1);
	}
	else if (*mode == FLOOR_HEIGHT)
	{
		if (sym == SDLK_UP && sectors[n].floor_height + 10 < sectors[n].ceiling_height)
			sectors[n].floor_height += 10;
		else if (sym == SDLK_DOWN && sectors[n].floor_height - 10 >= MIN_FLOOR_HEIGHT)
			sectors[n].floor_height -= 10;
		else
			return (1);		
	}
	else if (*mode == CEILING_HEIGHT)
	{
		if (sym == SDLK_UP && sectors[n].ceiling_height + 10 <= MAX_CEILING_HEIGHT)
			sectors[n].ceiling_height += 10;
		else if (sym == SDLK_DOWN && sectors[n].ceiling_height - 10 > sectors[n].floor_height)
			sectors[n].ceiling_height -= 10;
		else
			return (1);
	}
	else
		return (1);
	return (0);
}

int					pnpoly(int num_walls, t_editor_wall *walls, t_dot dot)
{
	int c;

	c = 0;
	for (int i = 0, j = num_walls - 1; i < num_walls; j = i++) 
	{
		if ((
		(walls[i].begin.y < walls[j].begin.y) && (walls[i].begin.y <= dot.y)
		&& (dot.y <= walls[j].begin.y) &&
		((walls[j].begin.y - walls[i].begin.y) * (dot.x - walls[i].begin.x) >
		(walls[j].begin.x -  walls[i].begin.x) * (dot.y - walls[i].begin.y))
		) || (
		(walls[i].begin.y > walls[j].begin.y) &&
		(walls[j].begin.y <= dot.y) && (dot.y <= walls[i].begin.y) &&
		((walls[j].begin.y - walls[i].begin.y) * (dot.x - walls[i].begin.x) <
		(walls[j].begin.x - walls[i].begin.x) * (dot.y - walls[i].begin.y))
	))
		c = !c;
	}
	return c;
}

int					player_save_keys(t_main *m, int n,
	t_editor_sector *sectors, int mode)
{
	int i;

	if (m->sdl.e.key.keysym.sym != SDLK_p && m->sdl.e.key.keysym.sym != SDLK_s)
		return (1);
	if (m->sdl.e.key.keysym.sym == SDLK_s && mode == CLOSE)
	{
		shift_left(sectors, n + 1);
		serialize_map(m, sectors, n);
		return (0);
	}
	t_dot d;
	SDL_GetMouseState(&d.x, &d.y);
	m->map.player.position.x = d.x;
	m->map.player.position.y = d.y;
	i = -1;
	while (++i <= n)
		if (pnpoly(sectors[i].num_walls, sectors[i].wall_vertice, d))
			m->map.player.sector_number = i;
	return (0);
}

void				sdl_keydown(t_main *m, t_editor_sector *sectors,
	int *n, int *mode, int *intersected)
{
	if (m->sdl.e.type == SDL_KEYDOWN)
	{
		if (arrow_keys(m->sdl.e.key.keysym.sym, sectors, *n, mode)
			&& player_save_keys(m, *n, sectors, *mode) &&
			(*n < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN))
		{
			if ((sectors[*n].num_walls > 0 && *mode == TEXTURE
				&& close_sector(m, *n, sectors))
				|| (*mode > TEXTURE && *mode < PORTAL))
				(*mode)++;
			if (*mode == PORTAL && *intersected != -1)
			{
				sectors[*n].neighbors[*intersected] = *n + 1;
				(*n)++;
				sectors[*n].wall_vertice[0] = sectors[*n - 1].wall_vertice[*intersected];
				sectors[*n].neighbors[0] = *n - 1;
				*intersected = -1;
				*mode = TEXTURE;
			}
		}
	}
}

int					map_editor_loop(t_main *m)
{
	int 			n;
	t_editor_sector	sectors[SECTORS_CNT];
	int				mode;
	int				intersected;
	t_text 			text_menu[TEXT_MENU];

	init_sectors(sectors, &mode, &intersected, &n);
	create_text_menu(m, text_menu);
	while(m->sdl.running)
	{
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
				m->sdl.running = 0;
			if (n < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type)
				intersected = create_wall(sectors, n, intersected, mode);
			sdl_keydown(m, sectors, &n, &mode, &intersected);
			if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
				update_all_menu(m, text_menu, sectors, n);
			intersected = draw(m, sectors, n, intersected, mode, text_menu);
		}
	}
	return (remove_text_menu(text_menu));
}

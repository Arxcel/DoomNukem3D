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
int		close_sector(t_main *m, t_map_editor *e)
{
	if (e->sectors[e->n].num_walls > 1 && e->sectors[e->n].num_walls < WALLS_CNT)
	{
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].color = YELLOW;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].begin.x =
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].end.x;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].begin.y =
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].end.y;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].end.x =
			e->sectors[e->n].wall_vertice[0].begin.x;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].end.y =
			e->sectors[e->n].wall_vertice[0].begin.y;
		e->sectors[e->n].num_walls++;
		return (e->sectors[e->n].num_walls);
	}
	return (0);
}



void	shift_left(t_map_editor *e)
{
	t_dot	min;
	int		i;
	int		j;
	int		gl;

	gl = 0;
	min = e->sectors[0].wall_vertice[0].begin;
	i = -1;
	while (++i <= e->n && (j = -1))
		while (++j < e->sectors[i].num_walls)
		{
			if (e->sectors[i].wall_vertice[j].begin.x < min.x)
				min.x = e->sectors[i].wall_vertice[j].begin.x;
			if (e->sectors[i].wall_vertice[j].begin.y < min.y)
				min.y = e->sectors[i].wall_vertice[j].begin.y;
		}
	i = -1;
	while (++i <= e->n && (j = -1))
		while (++j < e->sectors[i].num_walls)
		{
			e->sectors[i].wall_vertice[j].begin.x -= min.x;
			e->sectors[i].wall_vertice[j].begin.y -= min.y;
			e->sectors[i].wall_vertice[j].end.x -= min.x;
			e->sectors[i].wall_vertice[j].end.y -= min.y;
			e->sectors[i].wall_vertice[j].global_index = gl++;
		}
}

int		check_intersection(t_map_editor *e, t_dot mouse)
{
	int j;

	j = -1;
	while (++j < e->sectors[e->n].num_walls)
	{
		if (intersect(e->sectors[e->n].wall_vertice[j], mouse))
		{
			if (e->chosen == j)
			{
				e->chosen = -1;
				e->sectors[e->n].wall_vertice[j].color = YELLOW;
			}
			else if (e->chosen == -1)
			{
				e->chosen = j;
				e->sectors[e->n].wall_vertice[j].color = BLUE;
			}
			break;
		}
	}
	return (e->chosen);
}

int		create_wall(t_map_editor *e)
{
	t_dot mouse;

	SDL_GetMouseState(&mouse.x, &mouse.y);
	if (e->mode == TEXTURE && e->sectors[e->n].num_walls < WALLS_CNT - 1)
	{
		if (e->sectors[e->n].num_walls == -1)
			e->sectors[e->n].wall_vertice[0].begin = mouse;
		else
		{
			if (e->sectors[e->n].num_walls > 0)
				e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].begin =
					e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].end;
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].end = mouse;
		}
		e->sectors[e->n].num_walls++;
	}
	else if (e->mode == PORTAL && e->n < SECTORS_CNT - 1)
		e->chosen = check_intersection(e, mouse);
	return (e->chosen);
}


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

int		draw_circle(int color, t_main *m)
{
	sdl_pixel_put(&m->sdl.img, m->map.player.position.x,
		m->map.player.position.y, color);
	sdl_pixel_put(&m->sdl.img, m->map.player.position.x + 1,
		m->map.player.position.y, color);
	sdl_pixel_put(&m->sdl.img, m->map.player.position.x - 1,
		m->map.player.position.y, color);
	sdl_pixel_put(&m->sdl.img, m->map.player.position.x,
		m->map.player.position.y, color);
	sdl_pixel_put(&m->sdl.img, m->map.player.position.x,
		m->map.player.position.y + 1, color);
	sdl_pixel_put(&m->sdl.img, m->map.player.position.x,
		m->map.player.position.y - 1, color);
	return (0);
}

int		draw(t_main *m, t_map_editor *e)
{
	int i;
	int j;

	i = -1;
	while ((j = -1) && ++i < e->n + 1)
		while (++j < e->sectors[i].num_walls)
			line(e->sectors[i].wall_vertice[j], m);
	SDL_UpdateTexture(m->sdl.texture, NULL,
	m->sdl.img.pixels, m->sdl.img.w * sizeof(unsigned int));
	sdl_clear_image(&m->sdl.img);
	SDL_RenderCopy(m->sdl.ren, m->sdl.texture, NULL, NULL);
	i = -1;
	while (++i < TEXT_MENU)
		SDL_RenderCopy(m->sdl.ren, e->menu[i].text_texture, NULL, &e->menu[i].dstrect);
	SDL_RenderPresent(m->sdl.ren);
	return (e->chosen);
}

int					init_sectors(t_map_editor *e)
{
	int i;
	int j;

	i = -1;
	while (++i < SECTORS_CNT && (j = -1))
		while (++j < WALLS_CNT)
		{
			e->sectors[i].neighbors[j] = -1;
			e->sectors[i].num_walls = 1;
			e->sectors[i].wall_vertice[j].texture = 0;
			e->sectors[i].wall_vertice[j].color = YELLOW;
			e->sectors[i].ceiling_height = 100;
			e->sectors[i].floor_height = 0;
		}
	e->sectors[0].num_walls = -1;
	e->n = 0;
	e->mode = TEXTURE;
	e->chosen = -1;
	return (0);
}

int					remove_text_menu(t_text *menu)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
		SDL_DestroyTexture(menu[i].text_texture);
	return (0);
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

int				left_arrow_key(
	SDL_Keycode sym, t_map_editor *e)
{
	if (sym != SDLK_LEFT  || (e->n && e->sectors[e->n].num_walls < 2))
		return (1);
	if (!e->n && e->sectors[e->n].num_walls < 0)
		return (1);
	e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture = 0;
	e->sectors[e->n].num_walls--;
	if (e->mode == CLOSE)
		(e->mode)--;
	return (0);
}
int				arrow_keys(SDL_Keycode sym, t_map_editor *e)
{
	if (sym != SDLK_UP && sym != SDLK_DOWN && sym != SDLK_LEFT)
		return (1);
	if ((e->mode == TEXTURE || e->mode == CLOSE) && !left_arrow_key(sym, e))
		return (1);
	if ((e->mode == TEXTURE || e->mode == CLOSE) && e->sectors[e->n].num_walls > 0)
	{
		if (sym == SDLK_UP && e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture < TEXTURE_MAX)
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture++;
		else if (sym == SDLK_DOWN && e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture > 0)
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture--;
		else
			return (1);
	}
	else if (e->mode == FLOOR_HEIGHT)
	{
		if (sym == SDLK_UP && e->sectors[e->n].floor_height + 10 < e->sectors[e->n].ceiling_height)
			e->sectors[e->n].floor_height += 10;
		else if (sym == SDLK_DOWN && e->sectors[e->n].floor_height - 10 >= MIN_FLOOR_HEIGHT)
			e->sectors[e->n].floor_height -= 10;
		else
			return (1);		
	}
	else if (e->mode == CEILING_HEIGHT)
	{
		if (sym == SDLK_UP && e->sectors[e->n].ceiling_height + 10 <= MAX_CEILING_HEIGHT)
			e->sectors[e->n].ceiling_height += 10;
		else if (sym == SDLK_DOWN && e->sectors[e->n].ceiling_height - 10 > e->sectors[e->n].floor_height)
			e->sectors[e->n].ceiling_height -= 10;
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

int					player_save_keys(t_main *m, t_map_editor *e)
{
	int i;

	if (m->sdl.e.key.keysym.sym == SDLK_s && e->mode >= CEILING_HEIGHT)
	{
		if (e->mode < PLAYER)
		{
			shift_left(e);
			e->mode = PLAYER;
		}
		else
			serialize_map(m, e->sectors, e->n);
		return (0);
	}
	if (e->mode < PLAYER)
		return (1);
	t_dot d;
	SDL_GetMouseState(&d.x, &d.y);
	i = -1;
	while (++i <= e->n)
		if (pnpoly(e->sectors[i].num_walls, e->sectors[i].wall_vertice, d))
		{
			m->map.player.sector_number = i;
			m->map.player.position.x = d.x;
			m->map.player.position.y = d.y;
			return (0);
		}
	return (1);
}

void				sdl_keydown(t_main *m, t_map_editor	*e)
{
	if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (arrow_keys(m->sdl.e.key.keysym.sym, e)
			&& player_save_keys(m, e) &&
			(e->n < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN))
		{
			if ((e->sectors[e->n].num_walls > 0 && e->mode == TEXTURE
				&& close_sector(m, e))
				|| (e->mode > TEXTURE && e->mode < PORTAL))
				(e->mode)++;
			if (e->mode == PORTAL && e->chosen != -1)
			{
				e->sectors[e->n].neighbors[e->chosen] = e->n + 1;
				(e->n)++;
				e->sectors[e->n].wall_vertice[0] = e->sectors[e->n - 1].wall_vertice[e->chosen];
				e->sectors[e->n].neighbors[0] = e->n - 1;
				e->chosen = -1;
				e->mode = TEXTURE;
			}
		}
	}
}

int					map_editor_loop(t_main *m)
{
	t_map_editor	e;
	init_sectors(&e);
	create_text_menu(m, e.menu);
	while(m->sdl.running)
	{
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
				m->sdl.running = 0;
			if (e.n < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type)
				e.chosen = create_wall(&e);
			sdl_keydown(m, &e);
			if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
				update_all_menu(m, &e);
			e.chosen = draw(m, &e);
			if (e.mode == PLAYER)
				draw_circle(RED, m);
		}
	}
	return (remove_text_menu(e.menu));
}

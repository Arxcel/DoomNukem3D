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
	return ((a.x == b.x - 1 && a.y == b.y - 1) || (a.x == b.x + 1 && a.y == b.y + 1) || (a.x == b.x && a.y == b.y)
		|| (a.x == b.x - 1 && a.y == b.y + 1) || (a.x == b.x + 1 && a.y == b.y - 1));
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
int		close_sector(t_main *m, int i, t_text_sector *sector)
{
	if (sector[i].num_walls > 1 && sector[i].num_walls < WALLS_CNT)
	{
		sector[i].wall_vertice[sector[i].num_walls].color = 0xFFFF00;
		sector[i].wall_vertice[sector[i].num_walls].begin.x =
			sector[i].wall_vertice[sector[i].num_walls - 1].end.x;
		sector[i].wall_vertice[sector[i].num_walls].begin.y =
			sector[i].wall_vertice[sector[i].num_walls - 1].end.y;
		sector[i].wall_vertice[sector[i].num_walls].end.x = sector[i].wall_vertice[0].begin.x;
		sector[i].wall_vertice[sector[i].num_walls].end.y = sector[i].wall_vertice[0].begin.y;

		sector[i].num_walls++;
		printf("Walls %i\n", sector[i].num_walls);
		return (sector[i].num_walls);
	}
	return (0);
}
void	print_sectors(t_text_sector *sectors, int n)
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

void	shift_left(t_text_sector *sectors, int n)
{
	t_dot	min;
	int		i;
	int		j;

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
		}
}

int		check_intersection(t_text_sector *sectors, int n, int intersected, t_dot mouse)
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

int		create_wall(t_text_sector *sectors, int n, int intersected, bool select_portal_mode)
{
	t_dot mouse;

	SDL_GetMouseState(&mouse.x, &mouse.y);
	if (!select_portal_mode && sectors[n].num_walls < WALLS_CNT - 1)
	{
		sectors[n].wall_vertice[sectors[n].num_walls].color = YELLOW;
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
	else if (n < SECTORS_CNT - 1)
		intersected = check_intersection(sectors, n, intersected, mouse);	
	return (intersected);
}

int		draw(t_main *m, t_text_sector *sectors, int n,
			int intersected, bool select_portal_mode)
{
	int i;
	int j;

	if (n < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type )
		intersected = create_wall(sectors, n, intersected, select_portal_mode);
	i = -1;
	while((j = -1) && ++i < n + 1)
		while (++j < sectors[i].num_walls)
			line(sectors[i].wall_vertice[j], m);
	SDL_UpdateTexture(m->sdl.texture, NULL,
	m->sdl.img.pixels, m->sdl.img.w * sizeof(unsigned int));
	SDL_RenderCopy(m->sdl.ren, m->sdl.texture, NULL, NULL);
	// SDL_RenderCopy(m->sdl.ren, mTexture, &srcrect, &dstrect);
	SDL_RenderPresent(m->sdl.ren);
	return (intersected);
}

void		pool_event(t_main *m, t_text_sector *sectors, int intersected, bool select_portal_mode, int n)
{
	int j;
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
				m->sdl.running = 0;
			else if (n < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN)
			{
				if (!select_portal_mode && close_sector(m, n, sectors))
					select_portal_mode = true;
				if (intersected != -1)
				{
					sectors[n].neighbors[intersected] = n + 1;
					n++;
					sectors[n].wall_vertice[0] = sectors[n - 1].wall_vertice[intersected];
					sectors[n].num_walls = 1;
					sectors[n].neighbors[0] = n - 1;
					j = 0;
					while (++j < WALLS_CNT)
						sectors[n].neighbors[j] = -1;
					intersected = -1;
					select_portal_mode = false;
					
				}
			}
			if (m->sdl.e.key.keysym.sym == SDLK_s && select_portal_mode)
			{
				puts("Saving");
				shift_left(sectors, n + 1);
				print_sectors(sectors, n);
				// serialize_map(sectors, n);
			}
			intersected = draw(m, sectors, n, intersected, select_portal_mode);
		}

}
int     map_editor_loop(t_main *m)
{
	// SDL_Color foregroundColor = { 255, 255, 255 };
	// SDL_Color backgroundColor = { 0, 0, 0 };

	// SDL_Surface* textSurface = TTF_RenderText_Shaded(m->font, "Sectors", foregroundColor, backgroundColor);


	// SDL_Rect srcrect = {0, 0, 312, 32};
	// SDL_Texture *mTexture = SDL_CreateTextureFromSurface( m->sdl.ren, textSurface );

	// SDL_Rect dstrect = {m->sdl.win_w * 0.75,  15 , strlen("Sectors") * 15, 32};

	//t_dot wall_vertice[WALLS_CNT];
	int j;
	int n;
	t_text_sector sectors[SECTORS_CNT];
	bool select_portal_mode = false;
	int intersected = -1;

	j = -1;
	n = 0;
	sectors[0].num_walls = -1;
	while (++j < WALLS_CNT)
		sectors[0].neighbors[j] = -1;
	while(m->sdl.running)
	{
		// pool_event(m, sectors, intersected, select_portal_mode, n);
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
				m->sdl.running = 0;
			else if (n < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN)
			{
				if (!select_portal_mode && close_sector(m, n, sectors))
					select_portal_mode = true;
				if (intersected != -1)
				{
					sectors[n].neighbors[intersected] = n + 1;
					n++;
					sectors[n].wall_vertice[0] = sectors[n - 1].wall_vertice[intersected];
					sectors[n].num_walls = 1;
					sectors[n].neighbors[0] = n - 1;
					j = 0;
					while (++j < WALLS_CNT)
						sectors[n].neighbors[j] = -1;
					intersected = -1;
					select_portal_mode = false;
					
				}
			}
			if (m->sdl.e.key.keysym.sym == SDLK_s && select_portal_mode)
			{
				puts("Saving");
				shift_left(sectors, n + 1);
				print_sectors(sectors, n);
				// serialize_map(sectors, n);
			}
			intersected = draw(m, sectors, n, intersected, select_portal_mode);
		}
	}
	return (0);
}

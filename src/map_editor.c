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
	main->font = TTF_OpenFont("./assets/fonts/ARIAL.TTF",24);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_SetRenderDrawColor(main->sdl.ren, 0, 0, 0, 0);
	SDL_RenderClear(main->sdl.ren);
	SDL_RenderPresent(main->sdl.ren);
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

int     map_editor_loop(t_main *m)
{
	int x = 0;
	int y = 0;
	SDL_Color foregroundColor = { 255, 255, 255 };
	SDL_Color backgroundColor = { 0, 0, 0 };

	SDL_Surface* textSurface = TTF_RenderText_Shaded(m->font, "Sectors", foregroundColor, backgroundColor);


	SDL_Rect srcrect = {0, 0, 312, 32};
	SDL_Texture *mTexture = SDL_CreateTextureFromSurface( m->sdl.ren, textSurface );

	SDL_Rect dstrect = {m->sdl.win_w * 0.75,  15 , strlen("Sectors") * 15, 32};

	//t_dot wall_vertice[WALLS_CNT];
	int i = 0;
	int j;
	int num_sectors = 0;
	t_text_sector sectors[SECTORS_CNT];
	sectors[0].num_walls = -1;
	bool select_portal_mode = false;
	int cnt_sec = -1;
	int intersected = -1;
	while(m->sdl.running)
	{
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
			{
				m->sdl.running = 0;
			}
			else if (num_sectors < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN)
			{
				if (close_sector(m, num_sectors, sectors) && !select_portal_mode)
				{
					select_portal_mode = true;
					i = 0;
				}
				if (intersected != -1)
				{
					num_sectors++;
					sectors[num_sectors].wall_vertice[0] = sectors[num_sectors - 1].wall_vertice[intersected];
					sectors[num_sectors].num_walls = 1;
					i = 2;
					intersected = -1;
					select_portal_mode = false;
					
				}
			}
			if (num_sectors < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type )
			{
				SDL_GetMouseState(&x, &y);
				if (!select_portal_mode && sectors[num_sectors].num_walls < WALLS_CNT - 1)
				{
					i = sectors[num_sectors].num_walls;
					sectors[num_sectors].wall_vertice[i].color = YELLOW;
					printf("x = %i y = %i\n", x, y);
					if (i == -1)
					{
						sectors[num_sectors].wall_vertice[0].begin.x = x;
						sectors[num_sectors].wall_vertice[0].begin.y = y;
					}
					else
					{
						if (i > 0)
						{
							sectors[num_sectors].wall_vertice[i].begin.x =
								sectors[num_sectors].wall_vertice[i - 1].end.x;
							sectors[num_sectors].wall_vertice[i].begin.y =
								sectors[num_sectors].wall_vertice[i - 1].end.y;
						}
						sectors[num_sectors].wall_vertice[i].end.x = x;
						sectors[num_sectors].wall_vertice[i].end.y = y;
					}
					sectors[num_sectors].num_walls++;
				}
				else
				{

					j = -1;
					t_dot d = {x,y};
					while (++j < sectors[num_sectors].num_walls)
					{
						if (intersect(sectors[num_sectors].wall_vertice[j], d))
						{
							if (intersected == j)
							{
								intersected = -1;
								sectors[num_sectors].wall_vertice[j].intersected = false;
								sectors[num_sectors].wall_vertice[j].color = YELLOW;
							}
							else if (intersected == -1)
							{
								intersected = j;
								sectors[num_sectors].wall_vertice[j].intersected = true;
								sectors[num_sectors].wall_vertice[j].color = BLUE;
							}
							break;
						}
					}
					
				}
				

			}
			cnt_sec = -1;
			while(++cnt_sec < num_sectors + 1)
			{
				j = -1;
				while (++j < sectors[cnt_sec].num_walls)
				{
					line(sectors[cnt_sec].wall_vertice[j], m);
				}
			}
			

		
			// sdl_put_image(&m->sdl);
			SDL_UpdateTexture(m->sdl.texture, NULL,
			m->sdl.img.pixels, m->sdl.img.w * sizeof(unsigned int));
			// sdl_clear_image(&m->sdl.img);
			SDL_RenderCopy(m->sdl.ren, m->sdl.texture, NULL, NULL);
			
			SDL_RenderCopy(m->sdl.ren, mTexture, &srcrect, &dstrect);
			SDL_RenderPresent(m->sdl.ren);
		}
	}
	return (0);
}

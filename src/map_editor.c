#include "map_editor.h"

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


void	print_vector(t_dot a, t_main *main)
{
	if (a.x > -1 && a.y > -1 && a.x < main->sdl.win_w && a.y <  main->sdl.win_h)
		sdl_pixel_put(&main->sdl.img, a.x, a.y, 0xFFFF00);
}

bool	compare(t_dot a, t_dot b)
{
	return (a.x == b.x && a.y == b.y);
}

int		intersect(t_dot a, t_dot b, t_dot cur)
{
	t_dot error;
	t_dot d;
	t_dot s;

	d = module(a, b);
	s = direction(a, b);
	error.x = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		if (compare(a, cur))
			return (1);
		if (a.x == b.x && a.y == b.y)
			break ;
		error.y = error.x;
		if (error.y > -d.x)
		{
			error.x -= d.y;
			a.x += s.x;
		}
		if (error.y < d.y)
		{
			error.x += d.x;
			a.y += s.y;
		}
	}
	return (0);
}

int		line(t_dot a, t_dot b, t_main *main)
{
	t_dot error;
	t_dot d;
	t_dot s;

	d = module(a, b);
	s = direction(a, b);
	error.x = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		print_vector(a, main);
		if (a.x == b.x && a.y == b.y)
			break ;
		error.y = error.x;
		if (error.y > -d.x)
		{
			error.x -= d.y;
			a.x += s.x;
		}
		if (error.y < d.y)
		{
			error.x += d.x;
			a.y += s.y;
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
int		close_sector(t_main *m, int *i, t_dot *wall_vertice)
{
	if (*i > 2 && *i < WALLS_CNT)
	{
		wall_vertice[*i] = wall_vertice[0];
		printf("Walls %i\n", *i);
		(*i)++;
		return (*i);
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
	//int i = 0;
	int j;
	int num_sectors = 0;
	t_text_sector sectors[SECTORS_CNT];
	sectors[0].num_walls = 0;
	sectors[0].i = 0;
	bool select_portal_mode = false;
	int cnt_sec = -1;
	while(m->sdl.running)
	{
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
			{
				m->sdl.running = 0;
			}
			else if (num_sectors < SECTORS_CNT && !sectors[num_sectors].num_walls && m->sdl.e.key.keysym.sym == SDLK_RETURN)
			{
				sectors[num_sectors].num_walls = close_sector(m, &sectors[num_sectors].i, sectors[num_sectors].wall_vertice);
				if (sectors[num_sectors].num_walls)
				{
					num_sectors++;
					sectors[num_sectors].num_walls = 0;
					sectors[num_sectors].i = 0;
					select_portal_mode = true;
				}
			}
			if (num_sectors < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type && sectors[num_sectors].i < WALLS_CNT - 1 && !sectors[num_sectors].num_walls)
			{
				SDL_GetMouseState(&x, &y);
				if (!select_portal_mode)
				{
					printf("x = %i y = %i\n", x, y);
					sectors[num_sectors].wall_vertice[sectors[num_sectors].i].x = x;
					sectors[num_sectors].wall_vertice[sectors[num_sectors].i].y = y;
					sectors[num_sectors].i++;
					printf("i = %i\n", sectors[num_sectors].i);
				}
				else
				{
					cnt_sec = -1;
					while(++cnt_sec < num_sectors + 1)
					{
						j = 0;
						t_dot d = {x,y};
						while (sectors[cnt_sec].i > 1 && j < sectors[cnt_sec].i - 1)
						{
							if (intersect(sectors[cnt_sec].wall_vertice[j], sectors[cnt_sec].wall_vertice[j+1], d))
							{
								puts("intersect");
								break;
							}
							j++;
						}
					}
				}
				

			}
			cnt_sec = -1;
			while(++cnt_sec < num_sectors + 1)
			{
				j = 0;
				while (sectors[cnt_sec].i > 1 && j < sectors[cnt_sec].i - 1)
				{
					line(sectors[cnt_sec].wall_vertice[j], sectors[cnt_sec].wall_vertice[j+1], m);
					j++;
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

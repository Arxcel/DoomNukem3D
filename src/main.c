/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/27 18:22:19 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void calcDeltaTime(t_main *m)
{
	m->prev_time = m->curr_time;
	m->curr_time = SDL_GetPerformanceCounter();
	m->delta_time += (m->curr_time - m->prev_time) / (float)(SDL_GetPerformanceFrequency());
}

static void LoadData(t_map *map)
{
	FILE* fp = fopen("maps/map.txt", "rt");
	if(!fp)
	{
		perror("maps/map.txt"); exit(1);
	}

	char Buf[256], word[256], *ptr;
	t_vertex *vert;
	t_vertex v;

	int n, m, NumVertices = 0;
	vert = malloc(1);
	map->sectors = malloc(1);
	ft_bzero(&v, sizeof(t_vertex));
	while(fgets(Buf, sizeof Buf, fp))
	{
		switch(sscanf(ptr = Buf, "%32s%n", word, &n) == 1 ? word[0] : '\0')
		{
			case 'v': // vertex
				for(sscanf(ptr += n, "%f%n", &v.y, &n); sscanf(ptr += n, "%f%n", &v.x, &n) == 1; )
				{
					vert = realloc(vert, ++map->number_vertices * sizeof(*vert)); vert[map->number_vertices - 1] = v;
				}
				break;
			case 's': // sector
				map->sectors = realloc(map->sectors, ++map->number_sectors * sizeof(*map->sectors));
				t_sector *sect = &map->sectors[map->number_sectors - 1];
				int* num = NULL;
				num = malloc(1);
				sscanf(ptr += n, "%f%f%n", &sect->floor_height,&sect->ceil_height, &n);
				for(m=0; sscanf(ptr += n, "%32s%n", word, &n) == 1 && word[0] != '#'; )
				{
					num = realloc(num, ++m * sizeof(*num)); num[m-1] = word[0]=='x' ? -1 : atoi(word);
				}
				sect->number_vertices   = m /= 2;
				sect->neighbors = malloc( (m  ) * sizeof(*sect->neighbors) );
				sect->vertices  = malloc( (m + 1) * sizeof(*sect->vertices));
				for(n = 0; n < m; ++n)
					sect->neighbors[n] = num[m + n];
				sect->min = vert[num[0]];
				sect->max = vert[num[0]];
				for(n=0; n<m; ++n)
				{
					t_vertex t = vert[num[n]];
					sect->vertices[n + 1]  = t; // TODO: Range checking

					if (vert[num[n]].x >= sect->max.x)
						sect->max.x = vert[num[n]].x;
					if (vert[num[n]].y >= sect->max.y)
						sect->max.y = vert[num[n]].y;
					if (vert[num[n]].x <= sect->min.x)
						sect->min.x = vert[num[n]].x;
					if (vert[num[n]].y <= sect->min.y)
						sect->min.y = vert[num[n]].y;
					t_vertex min = sect->min;
					t_vertex max = sect->max;
				}
				sect->vertices[0] = sect->vertices[m]; // Ensure the vertexes form a loop
				free(num);
				break;
			case 'p':; // player
				float angle;
				sscanf(ptr += n, "%f %f %f %d", &v.x, &v.y, &angle,&n);

				map->player.velocity.x = 0;
				map->player.velocity.y = 0;
				map->player.velocity.z = 0;

				map->player.angle = angle;
				map->player.anglesin = 0;
				map->player.anglecos = 0;
				map->player.pitch = 0;
				map->player.sector_number = n;

				map->player.position.x = v.x;
				map->player.position.y = v.y;
				map->player.position.z = map->sectors[map->player.sector_number].floor_height + STANDHEIGHT;

		}
	}
	fclose(fp);
	free(vert);
}

static void UnloadData(t_map *m)
{
	for(size_t a = 0;  a < m->number_sectors; ++a)
		free(m->sectors[a].vertices);
	for(size_t a = 0; a < m->number_sectors; ++a)
		free(m->sectors[a].neighbors);
	free(m->sectors);
	m->sectors  = NULL;
}

static void testWalls(t_main *m)
{
	int w = m->sdl.img.w / 2;
	int h = m->sdl.img.h / 2;
	int *pix = m->tex.t.textures[2]->pixels;

	int y = -1;
	while (++y < h)
	{
		int x = m->sdl.img.w / 2 - 1;
		while (++x < w + m->sdl.img.w / 2)
		{
			sdl_pixel_put(&m->sdl.img, x, y, pix[x % 1024 + (y % 1024) * m->tex.t.textures[2]->w]);
		}
	}
}

void				sdl_loop(t_main *m)
{
	while (m->sdl.running)
	{
		calcDeltaTime(m);
		sdl_hook(m);
		if (m->delta_time > 0.016f)
		{
			draw_screen(m);
			dn_choose_weapon_render(m->wsys, &m->tex.wpns,&m->sdl.img);
			drawMinimap(&m->sdl.img, &m->map);
			sdl_put_image(&m->sdl);
			move_player(m);
			// printf("Delta time: %f\n", m->delta_time);
			m->delta_time = 0.f;
		}
	}
}

static void	load_block_textures(const char *path, SDL_Surface **target)
{
	if ((*target = IMG_Load(path)) == NULL)
	{
		ft_putendl(SDL_GetError());
		exit(-3);
	}
}

static void			load_textures(t_main *m)
{
	m->tex.t.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10);
	load_block_textures("assets/walls/1.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/2.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/3.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/4.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/5.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/6.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/7.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/8.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/9.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/10.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
}

static void			clear_textures(t_main *m)
{
	while (m->tex.t.numTextures)
		SDL_FreeSurface(m->tex.t.textures[--m->tex.t.numTextures]);
	free(m->tex.t.textures);
}

int					main(int ac, char **av)
{
	t_main			m;
	(void)av;
	ft_bzero(&m, sizeof(t_main));
	_ISZ(t_wsys, m.wsys, 1);
	load_textures(&m);
	LoadData(&m.map);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	_NOTIS_F(dn_init_ck_map((t_hinit){&m.tex.wpns, WPNS_MAP, WPNS_TEX_BG,
							WPNS_MAP_BG, WPNS_MAX_TEXTURES}, WEAPON_SCALE));
	_NOTIS_F(dn_init_weapons(m.wsys, &m.tex.wpns));
	sdl_loop(&m);
	clear_textures(&m);
	UnloadData(&m.map);
	SDL_Quit();
	return (0);
}

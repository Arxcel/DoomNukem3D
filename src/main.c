/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/22 17:13:02 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void LoadData(t_map *map)
{
	FILE* fp = fopen("maps/map.txt", "rt");
	if(!fp)
	{
		perror("maps/map.txt"); exit(1);
	}

	char Buf[256], word[256], *ptr;
	// struct xy* vert = NULL, v;
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

void				sdl_loop(t_main *m)
{
	while (m->sdl.running)
	{
		sdl_hook(m);
		draw_screen(&m->sdl.img, &m->map);
		drawMinimap(&m->sdl.img, &m->map);
		sdl_put_image(&m->sdl);
		move_player(m);
		SDL_Delay(10);
	}
}

int					main(int ac, char **av)
{
	t_main			m;

	(void)av;
	ft_bzero(&m, sizeof(t_main));
	LoadData(&m.map);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	_NOTIS_F(dn_init_textures_map(&m.tex));
	sdl_loop(&m);
	UnloadData(&m.map);
	SDL_Quit();
	return (0);
}

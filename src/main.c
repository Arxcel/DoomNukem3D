/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/23 14:13:08 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomNukem.h"
#include "mathUtils.h"
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
    while(fgets(Buf, sizeof Buf, fp))
    {
        switch(sscanf(ptr = Buf, "%32s%n", word, &n) == 1 ? word[0] : '\0')
        {
            case 'v': // vertex
                for(sscanf(ptr += n, "%f%n", &v.y, &n); sscanf(ptr += n, "%f%n", &v.x, &n) == 1; )
                {
                    vert = realloc(vert, ++map->numberVertices * sizeof(*vert)); vert[map->numberVertices-1] = v;
                }
                break;
            case 's': // sector
                map->sectors = realloc(map->sectors, ++map->numberSectors * sizeof(*map->sectors));
                t_sector *sect = &map->sectors[map->numberSectors - 1];
                int* num = NULL;
                sscanf(ptr += n, "%f%f%n", &sect->floorHeight,&sect->ceilHeight, &n);
                for(m=0; sscanf(ptr += n, "%32s%n", word, &n) == 1 && word[0] != '#'; )
                {
                    num = realloc(num, ++m * sizeof(*num)); num[m-1] = word[0]=='x' ? -1 : atoi(word);
                }
                sect->numberSectorVertices   = m /= 2;
                sect->neighbors = malloc( (m  ) * sizeof(*sect->neighbors) );
                sect->vertices  = malloc( (m+1) * sizeof(*sect->vertices)    );
                for(n = 0; n < m; ++n)
                    sect->neighbors[n] = num[m + n];
                for(n=0; n<m; ++n)
                    sect->vertices[n + 1]  = vert[num[n]]; // TODO: Range checking
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
                map->player.sectorNumber = n;

                map->player.position.x = v.x;
                map->player.position.y = v.y;
                map->player.position.z = map->sectors[map->player.sectorNumber].floorHeight + StandHeight;

        }
    }
    fclose(fp);
    free(vert);
}
static void UnloadData(t_map *m)
{
    for(size_t a = 0;  a < m->numberSectors; ++a)
        free(m->sectors[a].vertices);
    for(size_t a = 0; a < m->numberSectors; ++a)
        free(m->sectors[a].neighbors);
    free(m->sectors);
    m->sectors  = NULL;
}

void				sdl_loop(t_main *m)
{
    m->map.player.isFalling = 1;
	while (m->sdl.running)
	{
		sdl_hook(m);
        drawScreen(&m->sdl.img, &m->map);
        sdl_put_image(&m->sdl);
        movePlayer(m);
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
	
	sdl_loop(&m);

	UnloadData(&m.map);
    SDL_Quit();
	return (0);
}

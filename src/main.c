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

                map->player.velocity[0] = 0;
                map->player.velocity[1] = 0;
                map->player.velocity[2] = 0;

                map->player.angle = angle;
                map->player.anglesin = 0;
                map->player.anglecos = 0;
                map->player.yaw = 0;
                map->player.sectorNumber = n;

                map->player.position[0] = v.x;
                map->player.position[1] = v.y;
                map->player.position[2] = map->sectors[map->player.sectorNumber].floorHeight + StandHeight;

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

static void vline(t_img *img, int x, int y1,int y2, int top,int middle,int bottom)
{
    // int *pix = (int*) surface->pixels;
    y1 = clamp(y1, 0, H-1);
    y2 = clamp(y2, 0, H-1);
    if(y2 == y1)
		sdl_pixel_put(img, x, y1, middle);
    else if(y2 > y1)
    {
		sdl_pixel_put(img, x, y1, top);
        for(int y=y1+1; y<y2; ++y)
				sdl_pixel_put(img, x, y, middle);
		sdl_pixel_put(img, x, y2, bottom);
    }
}

static void DrawScreen(t_main *m)
{
    enum { MaxQueue = 32 };  // maximum number of pending portal renders
    struct item { int sectorno,sx1,sx2; } queue[MaxQueue], *head=queue, *tail=queue;

    int ytop[W]={0}, ybottom[W], renderedsectors[m->map.numberSectors];
    for(unsigned x=0; x<W; ++x)
        ybottom[x] = H-1;
    for(unsigned n=0; n<m->map.numberSectors; ++n)
        renderedsectors[n] = 0;

    /* Begin whole-screen rendering from where the player is. */
    *head = (struct item) { m->map.player.sectorNumber, 0, W-1 };
    if(++head == queue+MaxQueue) head = queue;

    do {
    /* Pick a sector & slice from the queue to draw */
    const struct item now = *tail;
    if(++tail == queue+MaxQueue)
        tail = queue;

    if(renderedsectors[now.sectorno] & 0x21)
        continue; // Odd = still rendering, 0x20 = give up
    ++renderedsectors[now.sectorno];
    t_sector const* sect = &m->map.sectors[now.sectorno];
    /* Render each wall of this sector that is facing towards player. */
    for(unsigned s = 0; s < sect->numberSectorVertices; ++s)
    {
        /* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
        float vx1 = sect->vertices[s+0].x - m->map.player.position[0], vy1 = sect->vertices[s+0].y - m->map.player.position[1];
        float vx2 = sect->vertices[s+1].x - m->map.player.position[0], vy2 = sect->vertices[s+1].y - m->map.player.position[1];
        /* Rotate them around the player's view */
        float pcos = m->map.player.anglecos, psin = m->map.player.anglesin;
        float tx1 = vx1 * psin - vy1 * pcos,  tz1 = vx1 * pcos + vy1 * psin;
        float tx2 = vx2 * psin - vy2 * pcos,  tz2 = vx2 * pcos + vy2 * psin;
        /* Is the wall at least partially in front of the player? */
        if(tz1 <= 0 && tz2 <= 0)
            continue;
        /* If it's partially behind the player, clip it against player's view frustrum */
        if(tz1 <= 0 || tz2 <= 0)
        {
            float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
            // Find an intersection between the wall and the approximate edges of player's view
            t_vertex i1 = Intersect(tx1,tz1,tx2,tz2, -nearside,nearz, -farside,farz);
            t_vertex i2 = Intersect(tx1,tz1,tx2,tz2,  nearside,nearz,  farside,farz);
            if(tz1 < nearz)
            {
                if(i1.y > 0)
                {
                    tx1 = i1.x; tz1 = i1.y;
                }
                else
                {
                    tx1 = i2.x; tz1 = i2.y;
                }
            }
            if(tz2 < nearz)
            {
                if(i1.y > 0)
                {
                    tx2 = i1.x; tz2 = i1.y;
                }
                else
                { 
                    tx2 = i2.x; tz2 = i2.y;
                }
            }
        }
        /* Do perspective transformation */
        float xscale1 = hfov / tz1, yscale1 = vfov / tz1;    int x1 = W/2 - (int)(tx1 * xscale1);
        float xscale2 = hfov / tz2, yscale2 = vfov / tz2;    int x2 = W/2 - (int)(tx2 * xscale2);
        if(x1 >= x2 || x2 < now.sx1 || x1 > now.sx2)
            continue; // Only render if it's visible
        /* Acquire the floor and ceiling heights, relative to where the player's view is */
        float yceil  = sect->ceilHeight  - m->map.player.position[2];
        float yfloor = sect->floorHeight - m->map.player.position[2];
        /* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
        int neighbor = sect->neighbors[s];
        float nyceil=0, nyfloor=0;
        if(neighbor >= 0) // Is another sector showing through this portal?
        {
            nyceil  = m->map.sectors[neighbor].ceilHeight  - m->map.player.position[2];
            nyfloor = m->map.sectors[neighbor].floorHeight - m->map.player.position[2];
        }
        /* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
        #define Yaw(y,z) (y + z*m->map.player.yaw)
        int y1a  = H/2 - (int)(Yaw(yceil, tz1) * yscale1),  y1b = H/2 - (int)(Yaw(yfloor, tz1) * yscale1);
        int y2a  = H/2 - (int)(Yaw(yceil, tz2) * yscale2),  y2b = H/2 - (int)(Yaw(yfloor, tz2) * yscale2);
        /* The same for the neighboring sector */
        int ny1a = H/2 - (int)(Yaw(nyceil, tz1) * yscale1), ny1b = H/2 - (int)(Yaw(nyfloor, tz1) * yscale1);
        int ny2a = H/2 - (int)(Yaw(nyceil, tz2) * yscale2), ny2b = H/2 - (int)(Yaw(nyfloor, tz2) * yscale2);

        /* Render the wall. */
        int beginx = max(x1, now.sx1), endx = min(x2, now.sx2);
        for(int x = beginx; x <= endx; ++x)
        {

            /* Calculate the Z coordinate for this point. (Only used for lighting.) */
            int z = ((x - x1) * (tz2-tz1) / (x2-x1) + tz1) * 8;
            /* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
            int ya = (x - x1) * (y2a-y1a) / (x2-x1) + y1a, cya = clamp(ya, ytop[x],ybottom[x]); // top
            int yb = (x - x1) * (y2b-y1b) / (x2-x1) + y1b, cyb = clamp(yb, ytop[x],ybottom[x]); // bottom

            /* Render ceiling: everything above this sector's ceiling height. */
            vline(&m->sdl.img, x, ytop[x], cya-1, 0x111111 ,0x222222,0x111111);
            /* Render floor: everything below this sector's floor height. */
            vline(&m->sdl.img, x, cyb+1, ybottom[x], 255,255,255);

            /* Is there another sector behind this edge? */
            if(neighbor >= 0)
            {
                /* Same for _their_ floor and ceiling */
                int nya = (x - x1) * (ny2a-ny1a) / (x2-x1) + ny1a, cnya = clamp(nya, ytop[x],ybottom[x]);
                int nyb = (x - x1) * (ny2b-ny1b) / (x2-x1) + ny1b, cnyb = clamp(nyb, ytop[x],ybottom[x]);
                /* If our ceiling is higher than their ceiling, render upper wall */
                unsigned r1 = 0x010101 * (255-z), r2 = 0x040007 * (31-z/8);
                vline(&m->sdl.img, x, cya, cnya-1, 0, x==x1||x==x2 ? 0 : r1, 0); // Between our and their ceiling
                ytop[x] = clamp(max(cya, cnya), ytop[x], H-1);   // Shrink the remaining window below these ceilings
                /* If our floor is lower than their floor, render bottom wall */
                vline(&m->sdl.img, x, cnyb+1, cyb, 0, x==x1||x==x2 ? 0 : r2, 0); // Between their and our floor
                ybottom[x] = clamp(min(cyb, cnyb), 0, ybottom[x]); // Shrink the remaining window above these floors
            }
            else
            {
                /* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
                unsigned r = 0x010101 * (255-z);
                vline(&m->sdl.img, x, cya, cyb, 0, x==x1||x==x2 ? 0 : r, 0);
            }
        }
        /* Schedule the neighboring sector for rendering within the window formed by this wall. */
        if(neighbor >= 0 && endx >= beginx && (head+MaxQueue+1-tail)%MaxQueue)
        {
            *head = (struct item) { neighbor, beginx, endx };
            if(++head == queue+MaxQueue) head = queue;
        }
    } // for s in sector's edges
    ++renderedsectors[now.sectorno];
    } while(head != tail); // render any other queued sectors
}


static void playerVerticalMovement(t_main *m)
{
    float eyeheight;
    float nextz;

    eyeheight = m->map.player.isCrouching ? CrouchingHeight : StandHeight;
    m->map.player.velocity[2] -= 0.05f;
    nextz = m->map.player.position[2] + m->map.player.velocity[2];
    if (m->map.player.velocity[2] < 0 && nextz  < m->map.sectors[m->map.player.sectorNumber].floorHeight + eyeheight)
    {
        m->map.player.position[2]    = m->map.sectors[m->map.player.sectorNumber].floorHeight + eyeheight;
        m->map.player.velocity[2] = 0;
        m->map.player.isFalling = 0;
        m->map.player.isStanding  = 1;
    }
    else if (m->map.player.velocity[2] > 0 && nextz > m->map.sectors[m->map.player.sectorNumber].ceilHeight)
    {
        m->map.player.velocity[2] = 0;
        m->map.player.isFalling = 1;
    }
    if (m->map.player.isFalling)
    {
        m->map.player.position[2] += m->map.player.velocity[2];
        m->map.player.isMoving = 1;
    }
}




void				sdl_loop(t_main *m)
{
	// int wsad[4]={0,0,0,0}, ground=0, falling=1, moving=0, crouching=0;
    m->map.player.isFalling = 1;
	while (m->sdl.running)
	{
		sdl_hook(m);
        DrawScreen(m);
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

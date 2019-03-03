#include <stdio.h>
#include "doomNukem.h"
#define FLOOR_COLOR 0x005599
#define CEILING_COLOR 0x222222


static void drawSector()
{
    
}

static void drawWall()
{
}


static void drawLine(t_img *img, int x, int y1, int y2, int colorTop, int colorMiddle, int colorBottom)
{
    y1 = clampf(y1, 0, H - 1);
    y2 = clampf(y2, 0, H - 1);
    if(y2 == y1)
		sdl_pixel_put(img, x, y1, colorMiddle);
    else if(y2 > y1)
    {
		sdl_pixel_put(img, x, y1, colorTop);
        int y = y1;
        while (++y < y2)
            sdl_pixel_put(img, x, y, colorMiddle);
		sdl_pixel_put(img, x, y2, colorBottom);
    }
}


void drawScreen(t_img *img, t_map *map)
{
    
    t_renderItem queue[MaxQueue];
    t_renderItem *head;
    t_renderItem *tail;
    int renderedSectors[map->numberSectors];
    int ytop[W];
    int ybottom[W];
    int x = -1;

    head = queue;
    tail = queue;
    ft_bzero(renderedSectors, map->numberSectors * sizeof(int));
    ft_bzero(ytop, W * sizeof(int));
    while(++x < W)
        ybottom[x] = H - 1;


    /* Begin whole-screen rendering from where the player is. */
    *head = (t_renderItem) { map->player.sectorNumber, 0, W - 1 };
    ++head;
    if (head == queue + MaxQueue)
        head = queue;

    while (head != tail) // render any other queued sectors
    {
        /* Pick a sector & slice from the queue to draw */
        t_renderItem const currentSector = *tail;
        ++tail;
        if(tail == queue + MaxQueue)
            tail = queue;

        if (renderedSectors[currentSector.sectorno] == MaxQueue)
            continue;

        ++renderedSectors[currentSector.sectorno];
        t_sector const* sect = &map->sectors[currentSector.sectorno];

        /* Render each wall of this sector that is facing towards player. */
        int s = -1;
        while(++s < sect->numberSectorVertices)
        {
            // {
                /* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
                float vx1 = sect->vertices[s].x - map->player.position.x;
                float vy1 = sect->vertices[s].y - map->player.position.y;
                float vx2 = sect->vertices[s + 1].x - map->player.position.x;
                float vy2 = sect->vertices[s + 1].y - map->player.position.y;

                /* Rotate them around the player's view */
                float tx1 = vx1 * map->player.anglesin - vy1 * map->player.anglecos;
                float tz1 = vx1 * map->player.anglecos + vy1 * map->player.anglesin;
                float tx2 = vx2 * map->player.anglesin - vy2 * map->player.anglecos;
                float tz2 = vx2 * map->player.anglecos + vy2 * map->player.anglesin;
            // }

            /* Is the wall at least partially in front of the player? */
            if (tz1 <= 0 && tz2 <= 0)
                continue;

            // /* If it's partially behind the player, clip it against player's view frustrum */
            if (tz1 <= 0 || tz2 <= 0)
            {
                float nearz = 1e-4f;
                float farz = 5;
                float nearside = 1e-5f;
                float farside = 20.f;
                // Find an intersection between the wall and the approximate edges of player's view
                t_vertex intersectionPoint1 = intersectLine((t_vertex){tx1, tz1}, (t_vertex){tx2, tz2}, (t_vertex){-nearside, nearz}, (t_vertex){-farside, farz});
                t_vertex intersectionPoint2 = intersectLine((t_vertex){tx1, tz1}, (t_vertex){tx2, tz2}, (t_vertex){nearside, nearz},  (t_vertex){farside, farz});
                if (tz1 < nearz)
                {
                    if(intersectionPoint1.y > 0)
                    {
                        tx1 = intersectionPoint1.x;
                        tz1 = intersectionPoint1.y;
                    }
                    else
                    {
                        tx1 = intersectionPoint2.x;
                        tz1 = intersectionPoint2.y;
                    }
                }
                if(tz2 < nearz)
                {
                    if(intersectionPoint1.y > 0)
                    {
                        tx2 = intersectionPoint1.x;
                        tz2 = intersectionPoint1.y;
                    }
                    else
                    { 
                        tx2 = intersectionPoint2.x;
                        tz2 = intersectionPoint2.y;
                    }
                }
            }
            /* Do perspective transformation */
            float xscale1 = hfov / tz1;
            float yscale1 = vfov / tz1;

            int x1 = W / 2 - (int)(tx1 * xscale1);

            float xscale2 = hfov / tz2;
            float yscale2 = vfov / tz2;

            int x2 = W / 2 - (int)(tx2 * xscale2);

            if (x1 >= x2 || x2 < currentSector.sx1 || x1 > currentSector.sx2)
                continue; // Only render if it's visible
            /* Acquire the floor and ceiling heights, relative to where the player's view is */
            float yceil  = sect->ceilHeight  - map->player.position.z;
            float yfloor = sect->floorHeight - map->player.position.z;
            /* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
            int neighbor = sect->neighbors[s];
            float nyceil = 0;
            float nyfloor = 0;
            if (neighbor >= 0) // Is another sector showing through this portal?
            {
                nyceil  = map->sectors[neighbor].ceilHeight  - map->player.position.z;
                nyfloor = map->sectors[neighbor].floorHeight - map->player.position.z;
            }
            // /* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
            float const cPitch = map->player.pitch;
            int y1a = H / 2 - (int)(calcPitch(yceil, tz1, cPitch) * yscale1);
            int y1b = H / 2 - (int)(calcPitch(yfloor, tz1, cPitch) * yscale1);
            int y2a = H / 2 - (int)(calcPitch(yceil, tz2, cPitch) * yscale2);
            int y2b = H / 2 - (int)(calcPitch(yfloor, tz2, cPitch) * yscale2);
            // /* The same for the neighboring sector */
            int ny1a = H / 2 - (int)(calcPitch(nyceil, tz1, cPitch) * yscale1);
            int ny1b = H / 2 - (int)(calcPitch(nyfloor, tz1, cPitch) * yscale1);
            int ny2a = H / 2 - (int)(calcPitch(nyceil, tz2, cPitch) * yscale2);
            int ny2b = H / 2 - (int)(calcPitch(nyfloor, tz2, cPitch) * yscale2);


            
    /* Render the wall. */
    int beginx = maxf(x1, currentSector.sx1);
    int endx = minf(x2, currentSector.sx2);
    int x = beginx - 1;
    while (++x <= endx)
    {
        /* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
        int ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a;
        int cya = clampf(ya, ytop[x], ybottom[x]); // top
        int yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b;
        int cyb = clampf(yb, ytop[x], ybottom[x]); // bottom

        /* Render ceiling: everything above this sector's ceiling height. */
        drawLine(img, x, ytop[x], cya - 1, 0x000000 ,CEILING_COLOR, 0x000000);
        /* Render floor: everything below this sector's floor height. */
        drawLine(img, x, cyb + 1, ybottom[x], 0x000000, FLOOR_COLOR, 0x000000);

        /* Is there another sector behind this edge? */
        if (neighbor >= 0)
        {
            unsigned r = x == x1 || x == x2 ? 0 : 0x884400;
            drawLine(img, x, cya, cyb, r, r, r);
        //     /* Same for _their_ floor and ceiling */
            int nya = (x - x1) * (ny2a - ny1a) / (x2 - x1) + ny1a;
            int cnya = clampf(nya, ytop[x], ybottom[x]);
            int nyb = (x - x1) * (ny2b - ny1b) / (x2 - x1) + ny1b;
            int cnyb = clampf(nyb, ytop[x], ybottom[x]);
            /* If our ceiling is higher than their ceiling, render upper wall */
            unsigned r1 = 0x040007;
            unsigned r2 = 0x040007;
            drawLine(img, x, cya, cnya - 1, 0, x == x1 || x == x2 ? 0 : r1, 0); // Between our and their ceiling
            ytop[x] = clampf(maxf(cya, cnya), ytop[x], H-1);   // Shrink the remaining window below these ceilings
            /* If our floor is lower than their floor, render bottom wall */
            drawLine(img, x, cnyb + 1, cyb, 0, x == x1 || x == x2 ? 0 : r2, 0); // Between their and our floor
            ybottom[x] = clampf(minf(cyb, cnyb), 0, ybottom[x]); // Shrink the remaining window above these floors
        }
        else
        {
            /* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
            unsigned r = x == x1 || x == x2 ? 0 : 0x444444;
            drawLine(img, x, cya, cyb, 0x000000, r, 0x000000);
        }
    }
            /* Schedule the neighboring sector for rendering within the window formed by this wall. */
            // if (neighbor >= 0 && endx >= beginx)
            // {
            //     *head = (t_renderItem) { neighbor, beginx, endx };
            //     if (++head == queue + MaxQueue)
            //         head = queue;
            // }
        } // for s in sector's edges
        renderedSectors[currentSector.sectorno]++;
    } 
}

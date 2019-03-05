#include <stdio.h>
#include "doom_nukem.h"
#include "graphics_renderer.h"
#include <stdlib.h>

static void drawLine(t_img *img, t_vline *vline)//int x, int y1, int y2, int colorTop, int colorMiddle, int colorBottom)
{
    int y;

    vline->yTop = clampf(vline->yTop, 0, img->h - 1);
    vline->yBottom = clampf(vline->yBottom, 0, img->h - 1);
    if (vline->yBottom == vline->yTop)
        sdl_pixel_put(img, vline->x, vline->yTop, vline->colorMain);
    else if (vline->yBottom > vline->yTop)
    {
        sdl_pixel_put(img, vline->x, vline->yTop, vline->colorTop);
        y = vline->yTop;
        while (++y < vline->yBottom)
            sdl_pixel_put(img, vline->x, y, vline->colorMain);
        sdl_pixel_put(img, vline->x, vline->yBottom, vline->colorBottom);
    }
}

static void initRenderer(t_renderer *renderer, t_img *imgToRender, int numberSectorsToRender)
{
    int x;

    ft_bzero(renderer, sizeof(t_renderer));
    renderer->renderedSectors = (int *)malloc(numberSectorsToRender * sizeof(int));
    renderer->topLimit = (int *)malloc(imgToRender->w * sizeof(int));
    renderer->bottomLimit = (int *)malloc(imgToRender->w * sizeof(int));
    renderer->head = renderer->queue;
    renderer->tail = renderer->queue;
    ft_bzero(renderer->renderedSectors, numberSectorsToRender * sizeof(int));
    ft_bzero(renderer->topLimit, imgToRender->w * sizeof(int));
    x = -1;
    while (++x < imgToRender->w)
        renderer->bottomLimit[x] = imgToRender->h - 1;
}

static void freeRenderer(t_renderer *renderer)
{
    free(renderer->renderedSectors);
    free(renderer->topLimit);
    free(renderer->bottomLimit);
}


static void renderWall(t_img *img, t_renderer *renderer, t_renderItem const *currentSector, t_wall *wall)
{
    int     beginx;
    int     endx;
    int     x;
    t_vline vline;

    beginx = maxf(wall->x1, currentSector->limitXLeft);
    endx = minf(wall->x2, currentSector->limitXRight);
    x = beginx - 1;
    while (++x <= endx)
    {
        /* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
        int ya = (x - wall->x1) * (wall->y2[0] - wall->y1[0]) / (wall->x2 - wall->x1) + wall->y1[0];
        int cya = clampf(ya, renderer->topLimit[x], renderer->bottomLimit[x]); // top
        int yb = (x - wall->x1) * (wall->y2[1] - wall->y1[1]) / (wall->x2 - wall->x1) + wall->y1[1];
        int cyb = clampf(yb, renderer->topLimit[x], renderer->bottomLimit[x]); // bottom

        /* Render ceiling: everything above this sector's ceiling height. */
        vline = (t_vline){ x, renderer->topLimit[x], cya , 0x000000, 0x003399, 0x000000 };
        drawLine(img, &vline);
        /* Render floor: everything below this sector's floor height. */
        vline =(t_vline){ x, cyb, renderer->bottomLimit[x], 0x000000, 0x009933, 0x000000 };
        drawLine(img, &vline);

        /* Is there another sector behind this edge? */
        if (wall->neighbor >= 0)
        {
            /* Same for _their_ floor and ceiling */
            int nya = (x - wall->x1) * (wall->neighbor_y2[0] - wall->neighbor_y1[0]) / (wall->x2 - wall->x1) + wall->neighbor_y1[0];
            int cnya = clampf(nya, renderer->topLimit[x], renderer->bottomLimit[x]);
            int nyb = (x - wall->x1) * (wall->neighbor_y2[1] - wall->neighbor_y1[1]) / (wall->x2 - wall->x1) + wall->neighbor_y1[1];
            int cnyb = clampf(nyb, renderer->topLimit[x], renderer->bottomLimit[x]);

            vline = (t_vline){ x, cya, cnya - 1, 0, 0x440077, 0 };
            drawLine(img, &vline); // Between our and their ceiling
            renderer->topLimit[x] = clampf(maxf(cya, cnya), renderer->topLimit[x], img->h - 1);

            vline = (t_vline){ x, cnyb + 1, cyb, 0, x == wall->x1 || x == wall->x2 ? 0 : 0x770044, 0 };
            drawLine(img, &vline); // Between their and our floor
            renderer->bottomLimit[x] = clampf(minf(cyb, cnyb), 0, renderer->bottomLimit[x]);
        }
        else
        {
            /* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
            vline = (t_vline){ x, cya, cyb, 0, x == wall->x1 || x == wall->x2 ? 0 : 0x444444, 0 };
            drawLine(img, &vline);
        }
    }
    /* Schedule the neighboring sector for rendering within the window formed by this wall. */
    if (wall->neighbor >= 0 && endx >= beginx)
    {
        *renderer->head = (t_renderItem){ wall->neighbor, beginx, endx };
        if (++renderer->head == renderer->queue + MaxQueue)
            renderer->head = renderer->queue;
    }
}

/* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
/* and rotate them around the player's view */
static t_vector calculateEdges(t_player *aPlayer, t_vertex *vertex)
{
    t_vector res;

    res.x = (vertex->x - aPlayer->position.x) * aPlayer->anglesin - (vertex->y - aPlayer->position.y) * aPlayer->anglecos;
    res.z = (vertex->x - aPlayer->position.x) * aPlayer->anglecos + (vertex->y - aPlayer->position.y) * aPlayer->anglesin;

    return res;
}

static void clampPoint(t_vector *point, t_vertex *i1, t_vertex *i2)
{
        if (i1->y > 0)
        {
            point->x = i1->x;
            point->z = i1->y;
        }
        else
        {
            point->x = i2->x;
            point->z = i2->y;
        }
}

// /* If it's partially behind the player, clip it against player's view frustrum */
// Find an intersection between the wall and the approximate edges of player's view
static void clampEdgesWithPlayerView(t_renderer *renderer)
{
    t_vertex intersectionPoint1;
    t_vertex intersectionPoint2;

    intersectionPoint1 = intersectLine((t_vertex){renderer->t1.x, renderer->t1.z},
                                        (t_vertex){renderer->t2.x, renderer->t2.z},
                                        (t_vertex){-NEAR_SIDE, NEAR_Z},
                                        (t_vertex){-FAR_SIDE, FAR_Z});
    intersectionPoint2 = intersectLine((t_vertex){renderer->t1.x, renderer->t1.z},
                                        (t_vertex){renderer->t2.x, renderer->t2.z},
                                        (t_vertex){NEAR_SIDE, NEAR_Z}, 
                                        (t_vertex){FAR_SIDE, FAR_Z});
    if (renderer->t1.z < NEAR_Z)
        clampPoint(&renderer->t1, &intersectionPoint1, &intersectionPoint2);
    
    if (renderer->t2.z < NEAR_Z)
        clampPoint(&renderer->t2, &intersectionPoint1, &intersectionPoint2);

}

/* Do perspective transformation */
static t_wall doPerspective(t_renderer *renderer, int width)
{
    t_wall ret;

    ret.scale1.x = hfov / renderer->t1.z;
    ret.scale1.y = vfov / renderer->t1.z;
    ret.x1 = width / 2 - (int)(renderer->t1.x * ret.scale1.x);
    ret.scale2.x = hfov / renderer->t2.z;
    ret.scale2.y = vfov / renderer->t2.z;
    ret.x2 = width / 2 - (int)(renderer->t2.x * ret.scale2.x);
    return ret;
}

static float calcPitch(float y, float z, float currentYaw)
{
    return (y + z * currentYaw);
}

/* Render each wall of this sector that is facing towards player. */
static void renderSector(t_img *img, t_map *map, t_renderer *renderer, t_renderItem const *currentSector)
{
    int       s;
    t_sector *sect;
    t_wall     wall;

    s = -1;
    sect = &map->sectors[currentSector->sectorno];
    while (++s < sect->numberSectorVertices)
    {
        renderer->t1 = calculateEdges(&map->player, &sect->vertices[s]);
        renderer->t2 = calculateEdges(&map->player, &sect->vertices[s + 1]);
        if (renderer->t1.z <= 0 && renderer->t2.z <= 0)
            continue;
        if (renderer->t1.z <= 0 || renderer->t2.z <= 0)
            clampEdgesWithPlayerView(renderer);
        wall = doPerspective(renderer, img->w);
        if (wall.x1 >= wall.x2 || wall.x2 < currentSector->limitXLeft || wall.x1 > currentSector->limitXRight)
            continue;
        wall.ceil = sect->ceilHeight - map->player.position.z;
        wall.floor = sect->floorHeight - map->player.position.z;
        wall.neighbor = sect->neighbors[s];
        if (wall.neighbor >= 0) // Is another sector showing through this portal?
        {
            wall.neighbor_ceil = map->sectors[wall.neighbor].ceilHeight - map->player.position.z;
             wall.neighbor_floor = map->sectors[wall.neighbor].floorHeight - map->player.position.z;
        }

        // /* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
        wall.y1[0] = img->h / 2 - (int)(calcPitch(wall.ceil, renderer->t1.z, map->player.pitch) * wall.scale1.y);
        wall.y1[1] = img->h / 2 - (int)(calcPitch(wall.floor, renderer->t1.z, map->player.pitch) * wall.scale1.y);
        wall.y2[0] = img->h / 2 - (int)(calcPitch(wall.ceil, renderer->t2.z, map->player.pitch) * wall.scale2.y);
        wall.y2[1] = img->h / 2 - (int)(calcPitch(wall.floor, renderer->t2.z, map->player.pitch) * wall.scale2.y);
        // /* The same for the neighboring sector */
        wall.neighbor_y1[0] = img->h / 2 - (int)(calcPitch(wall.neighbor_ceil, renderer->t1.z, map->player.pitch) * wall.scale1.y);
        wall.neighbor_y1[1]= img->h / 2 - (int)(calcPitch(wall.neighbor_floor, renderer->t1.z, map->player.pitch) * wall.scale1.y);
        wall.neighbor_y2[0] = img->h / 2 - (int)(calcPitch(wall.neighbor_ceil, renderer->t2.z, map->player.pitch) * wall.scale2.y);
        wall.neighbor_y2[1] = img->h / 2 - (int)(calcPitch(wall.neighbor_floor, renderer->t2.z, map->player.pitch) * wall.scale2.y);

        renderWall(img, renderer, currentSector, &wall);
    }
}

void drawScreen(t_img *img, t_map *map)
{
    t_renderer renderer;
    t_renderItem currentSector;
    
    initRenderer(&renderer, img, map->numberSectors);
    (*renderer.head) = (t_renderItem){map->player.sectorNumber, 0, img->w - 1};
    ++renderer.head;
    if (renderer.head == renderer.queue + MaxQueue)
        renderer.head = renderer.queue;
    while (renderer.head != renderer.tail)
    {
        currentSector = *renderer.tail;
        ++renderer.tail;
        if (renderer.tail == renderer.queue + MaxQueue)
            renderer.tail = renderer.queue;
        if (renderer.renderedSectors[currentSector.sectorno] == MaxQueue)
            continue;
        renderer.renderedSectors[currentSector.sectorno]++;
        renderSector(img, map, &renderer, &currentSector);
        renderer.renderedSectors[currentSector.sectorno]++;
    }
    freeRenderer(&renderer);
}

#include "doomNukem.h"

static void transformPlayer(t_map *map, float dx, float dy)
{
    float px = map->player.position.x;
    float py = map->player.position.y;

    t_sector const* sect = &map->sectors[map->player.sectorNumber];
    t_vertex const* vert = sect->vertices;
    int i = -1;
    while (++i < sect->numberSectorVertices)
        if(sect->neighbors[s] >= 0
        && intersectBox((t_vertex){px, py}, (t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y})
        && pointSide((t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y}) < 0)
        {
            map->player.sectorNumber = sect->neighbors[s];
            break;
        }
    map->player.position.x += dx;
    map->player.position.y += dy;
    map->player.anglesin = sinf(map->player.angle);
    map->player.anglecos = cosf(map->player.angle);
}

static void playerVerticalMovement(t_main *m)
{
    float eyeheight;
    float nextz;

    eyeheight = m->map.player.isCrouching ? CrouchingHeight : StandHeight;
    m->map.player.velocity.z -= 0.01f;
    nextz = m->map.player.position.z + m->map.player.velocity.z;
    if (m->map.player.velocity.z < 0 && nextz  < m->map.sectors[m->map.player.sectorNumber].floorHeight + eyeheight)
    {
        m->map.player.position.z    = m->map.sectors[m->map.player.sectorNumber].floorHeight + eyeheight;
        m->map.player.velocity.z = 0;
        m->map.player.isFalling = 0;
        m->map.player.isStanding  = 1;
    }
    else if (m->map.player.velocity.z > 0 && nextz > m->map.sectors[m->map.player.sectorNumber].ceilHeight)
    {
        m->map.player.velocity.z = 0;
        m->map.player.isFalling = 1;
    }
    if (m->map.player.isFalling)
    {
        m->map.player.position.z += m->map.player.velocity.z;
        m->map.player.isMoving = 1;
    }
}

static void playerHorizontalMovement(t_main *m)
{
    float px = m->map.player.position.x;
    float py = m->map.player.position.y;
    float dx = m->map.player.velocity.x;
    float dy = m->map.player.velocity.y;
    float eyeheight = m->map.player.isCrouching ? CrouchingHeight : StandHeight;
    t_sector *sect = &m->map.sectors[m->map.player.sectorNumber];
    t_vertex *vert = sect->vertices;

    /* Check if the player is about to cross one of the sector's edges */
    int s = -1;
    while (++s < sect->numberSectorVertices)
        if(intersectBox((t_vertex){px, py}, (t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y})
        && pointSide((t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y}) < 0)
        {
            /* Check where the hole is. */
            float hole_low  = sect->neighbors[s] < 0 ? -1 : max(sect->floorHeight, m->map.sectors[sect->neighbors[s]].floorHeight);
            float hole_high = sect->neighbors[s] < 0 ? -1 : min(sect->ceilHeight,  m->map.sectors[sect->neighbors[s]].ceilHeight );
            /* Check whether we're bumping into a wall. */
            if(hole_high < m->map.player.position.z + HeadMargin
            || hole_low  > m->map.player.position.z - eyeheight + KneeHeight)
            {
                dx = 0;
                dy = 0;
                m->map.player.isMoving = 0;
            }
        }
    transformPlayer(&m->map, dx, dy);
    m->map.player.isFalling = 1;
}

static t_vertex getPlayerDirection(t_main *m)
{
    int x;
    int y;
    float yaw;
    t_vertex moveDir;

    yaw = 0;
    ft_bzero(&moveDir, sizeof(t_vertex));
    SDL_GetRelativeMouseState(&x, &y);
    m->map.player.angle += x * 0.03f;
    yaw = clampf(yaw + y * 0.05f, -5, 5);
    m->map.player.yaw = yaw - m->map.player.velocity.z * 0.5f;
    transformPlayer(&m->map, 0, 0);
    if(m->map.player.dir == Forward)
    {
        moveDir.x += m->map.player.anglecos * 0.1f;
        moveDir.y += m->map.player.anglesin * 0.1f;
    }
    if(m->map.player.dir == Backward)
    {
        moveDir.x -= m->map.player.anglecos * 0.1f;
        moveDir.y -= m->map.player.anglesin * 0.1f;
    }
    if(m->map.player.dir == Left)
    {
        moveDir.x += m->map.player.anglesin * 0.1f;
        moveDir.y -= m->map.player.anglecos * 0.1f;
    }
    if(m->map.player.dir == Right)
    {
        moveDir.x -= m->map.player.anglesin * 0.1f;
        moveDir.y += m->map.player.anglecos * 0.1f;
    }
    return moveDir;
}

void movePlayer(t_main *m)
{
    t_vertex moveDir;
    int pushing;
    float acceleration;

    m->map.player.isStanding = !m->map.player.isFalling;
    if (m->map.player.isFalling)
        playerVerticalMovement(m);
    if (m->map.player.isMoving)
        playerHorizontalMovement(m);
    moveDir = getPlayerDirection(m);
    pushing = m->map.player.dir;
    acceleration = pushing ? 0.4 : 0.2;
    m->map.player.velocity.x = m->map.player.velocity.x *
                            (1 - acceleration) + moveDir.x * acceleration;
    m->map.player.velocity.y = m->map.player.velocity.y *
                            (1 - acceleration) + moveDir.y * acceleration;
    if(pushing)
        m->map.player.isMoving = 1;
}


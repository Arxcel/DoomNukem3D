#include "doomNukem.h"

// /* MovePlayer(dx,dy): Moves the player by (dx,dy) in the map, and
//  * also updates their anglesin/anglecos/sector properties properly.
//  */
static void transformPlayer(t_map *map, float dx, float dy)
{
    float px = map->player.position[0], py = map->player.position[1];
    /* Check if this movement crosses one of this sector's edges
     * that have a neighboring sector on the other side.
     * Because the edge vertices of each sector are defined in
     * clockwise order, PointSide will always return -1 for a point
     * that is outside the sector and 0 or 1 for a point that is inside.
     */
    t_sector const* sect = &map->sectors[map->player.sectorNumber];
    t_vertex const* vert = sect->vertices;
    for(unsigned s = 0; s < sect->numberSectorVertices; ++s)
        if(sect->neighbors[s] >= 0
        && IntersectBox(px,py, px+dx,py+dy, vert[s+0].x, vert[s+0].y, vert[s+1].x, vert[s+1].y)
        && PointSide(px+dx, py+dy, vert[s+0].x, vert[s+0].y, vert[s+1].x, vert[s+1].y) < 0)
        {
            map->player.sectorNumber = sect->neighbors[s];
            break;
        }

    map->player.position[0] += dx;
    map->player.position[1] += dy;
    map->player.anglesin = sinf(map->player.angle);
    map->player.anglecos = cosf(map->player.angle);
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

static void playerHorizontalMovement(t_main *m)
{
            float px = m->map.player.position[0],    py = m->map.player.position[1];
            float dx = m->map.player.velocity[0], dy = m->map.player.velocity[1];
            float eyeheight = m->map.player.isCrouching ? CrouchingHeight : StandHeight;

            t_sector const *sect = &m->map.sectors[m->map.player.sectorNumber];
            t_vertex *vert = sect->vertices;
            /* Check if the player is about to cross one of the sector's edges */
            for (unsigned s = 0; s < sect->numberSectorVertices; ++s)
                if(IntersectBox(px,py, px+dx,py+dy, vert[s+0].x, vert[s+0].y, vert[s+1].x, vert[s+1].y)
                && PointSide(px+dx, py+dy, vert[s+0].x, vert[s+0].y, vert[s+1].x, vert[s+1].y) < 0)
                {
                    /* Check where the hole is. */
                    float hole_low  = sect->neighbors[s] < 0 ?  9e9 : max(sect->floorHeight, m->map.sectors[sect->neighbors[s]].floorHeight);
                    float hole_high = sect->neighbors[s] < 0 ? -9e9 : min(sect->ceilHeight,  m->map.sectors[sect->neighbors[s]].ceilHeight );
                    /* Check whether we're bumping into a wall. */
                    if(hole_high < m->map.player.position[2] + HeadMargin
                    || hole_low  > m->map.player.position[2] - eyeheight + KneeHeight)
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
        int x,y;
        float yaw = 0;
        SDL_GetRelativeMouseState(&x, &y);
        m->map.player.angle += x * 0.03f;
        yaw = clamp(yaw + y * 0.05f, -5, 5);
        m->map.player.yaw   = yaw - m->map.player.velocity[2] * 0.5f;
        transformPlayer(&m->map, 0, 0);

        t_vertex moveDir;
        ft_bzero(&moveDir, sizeof(t_vertex));
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

    m->map.player.isStanding = !m->map.player.isFalling;
    if (m->map.player.isFalling)
    {
        playerVerticalMovement(m);
    }
    if (m->map.player.isMoving)
    {
        playerHorizontalMovement(m);
    }

    moveDir = getPlayerDirection(m);

    int pushing = m->map.player.dir;
    float acceleration = pushing ? 0.4 : 0.2;

    m->map.player.velocity[0] = m->map.player.velocity[0] * (1 - acceleration) + moveDir.x * acceleration;
    m->map.player.velocity[1] = m->map.player.velocity[1] * (1 - acceleration) + moveDir.y * acceleration;

    if(pushing)
        m->map.player.isMoving = 1;
}


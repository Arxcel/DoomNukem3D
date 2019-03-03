#ifndef STRUCTURE_H
# define STRUCTURE_H
# include "enum.h"
# include <ctype.h>
typedef struct		s_vector
{
    float           x;
    float           y;
    float           z;
}					t_vector;

typedef struct		s_vertex
{
    float           x;
    float           y;
}					t_vertex;

typedef struct		s_sector
{
    float           floorHeight;
    float           ceilHeight;
    short           *neighbors;
    t_vertex        *vertices;
    size_t          numberSectorVertices;
}                   t_sector;

typedef struct		s_sprite
{
    t_vector        position;
    float           angle;
    float           yaw;
    float           anglesin;
    float           anglecos;
    size_t          sectorNumber;
}                   t_sprite;

typedef struct		s_player
{
    t_vector        position;
    t_vector        velocity;
    float           angle;
    float           pitch;
    float           anglesin;
    float           anglecos;
    size_t          sectorNumber;

    short           isMoving;
    short           isCrouching;
    short           isFalling;
    short           isStanding;

    MoveDir         dir;
}                   t_player;

typedef struct		s_map
{
	t_vertex        *vertices;
    t_sector        *sectors;
    t_sprite        *sprites;
    t_player        player;
    size_t          numberVertices;
    size_t          numberSectors;
    size_t          numberEnemies;
    size_t          numberSptites;
}					t_map;

typedef struct		s_renderItem
{
    int sectorno;
    int sx1;
    int sx2;
}                   t_renderItem;

typedef struct		s_slice
{
    int yTop;
    int yBottom;
}                   t_slice;



#endif

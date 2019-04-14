#include "structure.h"
#define WALLS_CNT 10
#define SECTORS_CNT 100
#define YELLOW 0xFFFF00
#define BLUE 0x0000FF
typedef struct  s_dot
{
   int x;
   int y;
}               t_dot;

typedef struct	s_editor_wall
{
	t_dot		begin;
	t_dot		end;
	int color;
	bool intersected;

}				t_editor_wall;

typedef struct	s_text_sector
{
	SDL_Surface		*text_surface;
	SDL_Texture 	*text_texture;
	SDL_Rect		dstrect;
	t_editor_wall	wall_vertice[WALLS_CNT];
	int				neighbors[WALLS_CNT];
	int				num_walls;
	int				color;

}				t_text_sector;
int		init_map_editor(t_main *main);
int     map_editor_loop(t_main *m);

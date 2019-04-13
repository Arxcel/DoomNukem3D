#include "structure.h"
#define WALLS_CNT 10
#define SECTORS_CNT 10
typedef struct  s_dot
{
   int x;
   int y;
   int color;
   int intersect;
}               t_dot;

typedef struct	s_text_sector
{
	SDL_Surface	*text_surface;
	SDL_Texture *text_texture;
	SDL_Rect	dstrect;
	t_dot		wall_vertice[WALLS_CNT];
	int			num_walls;
	int			i;
	int			color;

}				t_text_sector;
int		init_map_editor(t_main *main);
int     map_editor_loop(t_main *m);

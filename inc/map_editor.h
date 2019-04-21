# include "structure.h"
# define WALLS_CNT 10
# define SECTORS_CNT 100
# define YELLOW 0xFFFF00
# define BLUE 0x0000FF
# define TEXT_MENU 11
# define LETTER_WIDTH 15
# define LETTER_HEIGHT 30
# define TEXT_MENU_ROW 6
# define TEXTURE_MAX 10
# define MAX_CEILING_HEIGHT 1000
# define MIN_FLOOR_HEIGHT -20

typedef	enum
{
	TEXTURE, CLOSE, FLOOR_HEIGHT, CEILING_HEIGHT, PORTAL
}					e_select_mode;
typedef struct		s_dot
{
   int x;
   int y;
}					t_dot;

typedef struct		s_editor_wall
{
	t_dot			begin;
	t_dot			end;
	int				color;
	int				texture;
	bool			intersected;
	int				global_index;

}					t_editor_wall;

typedef struct		s_editor_sector
{
	t_editor_wall	wall_vertice[WALLS_CNT];
	int				neighbors[WALLS_CNT];
	int				num_walls;
	int				color;
	int				floor_height;
	int				ceiling_height;
}					t_editor_sector;

typedef struct		s_text
{
	SDL_Texture 	*text_texture;
	SDL_Rect		dstrect;
}					t_text;

int					init_map_editor(t_main *main);
int					map_editor_loop(t_main *m);
int     			serialize_map(t_editor_sector *sectors, int num_sectors);
int					editor_clear_sdl(t_sdl *sdl);

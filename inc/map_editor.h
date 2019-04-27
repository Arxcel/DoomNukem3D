# include "structure.h"
# define WALLS_CNT 10
# define SECTORS_CNT 100
# define YELLOW 0xFFFF00
# define BLUE 0x0000FF
# define RED 0xFF0000
# define TEXT_MENU 11
# define LETTER_WIDTH 15
# define LETTER_HEIGHT 30
# define TEXT_MENU_ROW 6
# define TEXTURE_MAX 10
# define MAX_CEILING_HEIGHT 1000
# define MIN_FLOOR_HEIGHT -20

typedef	enum
{
	TEXTURE, CLOSE, FLOOR_HEIGHT, CEILING_HEIGHT, PORTAL, PLAYER, SPRITE
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
	int				global_index;

}					t_editor_wall;

typedef struct		s_editor_sector
{
	t_editor_wall	wall_vertice[WALLS_CNT];
	int				neighbors[WALLS_CNT];
	int				num_walls;
	int				floor_height;
	int				ceiling_height;
}					t_editor_sector;

typedef struct		s_text
{
	SDL_Texture 	*text_texture;
	SDL_Rect		dstrect;
}					t_text;

typedef struct		s_map_editor
{
	int 			n;
	t_editor_sector	sectors[SECTORS_CNT];
	int				mode;
	int				chosen;
	t_text 			menu[TEXT_MENU];
	int				global_index;
}					t_map_editor;

int					init_map_editor(t_main *main);
int					map_editor_loop(t_main *m);
int     			serialize_map(t_main *m, t_editor_sector *sectors, int num_sectors);
int					editor_clear_sdl(t_sdl *sdl);
int					intersect(t_editor_wall wall, t_dot cur);
int					line(t_editor_wall wall, t_main *main);
int					init_sectors(t_map_editor *e);

void				create_text_menu(t_main *m, t_text *menu);
void				update_text(t_main *m, t_text *menu, int i, int str);
int					create_text(t_main *m, t_text *menu, int i, const char *str);
void				update_all_menu(t_main *m, t_map_editor *e);

void				print_vector(t_editor_wall wall, t_main *main);
int					draw(t_main *m, t_map_editor *e);
int					draw_circle(int color, t_main *m);

int					pnpoly(int num_walls, t_editor_wall *walls, t_dot dot);
int					check_intersection(t_map_editor *e, t_dot mouse);


int					player_save_keys(t_main *m, t_map_editor *e);
int					arrow_keys(SDL_Keycode sym, t_map_editor *e);
int					left_arrow_key(SDL_Keycode sym, t_map_editor *e);

void				shift_left(t_map_editor *e);

int					remove_text_menu(t_text *menu);






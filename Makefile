NAME := doom_nukem

CC := clang
KEYS := #-Wall -Wextra -Werror
FLAGS := -g

EXT := doom_nukem.h \
		structure.h \
		utils.h \
		player.h \
		graphics_renderer.h \
		weapons.h \
		textures.h \
		macroses.h
IDIR := $(CURDIR)/inc
EXTENSIONS := $(addprefix $(IDIR)/,$(EXT))

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CFLAGS := -I$(IDIR) \
		-I./libft/inc/ \
		-I./libftSDL/inc/ \
		-I./libJson/inc/ \
		-I /usr/local/include/SDL2/ \
		-I /usr/include/SDL2/
else
CFLAGS := -I$(IDIR) \
		-I./libft/inc/ \
		-I./libSDL/SDL2.framework/Headers/ \
		-I./libSDL/SDL2_image.framework/Headers/ \
		-I./libSDL/SDL2_ttf.framework/Headers/ \
		-I./libftSDL/inc/ \
		-I./libJson/inc/
endif

LIBFT = libft
LIBJSON = libJson
LIBFTSDL = libftSDL

ifeq ($(UNAME), Linux)
SDL2_F := -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm
else
SDL2_F := -framework SDL2 -framework SDL2_image -framework SDL2_ttf -F ./libSDL/
endif
SDL2_P := -rpath @loader_path/libSDL/

HEADER := inc
_DEPS := doom_nukem.h \
		structure.h \
		utils.h \
		player.h \
		graphics_renderer.h \
		weapons.h \
		textures.h \
		macroses.h
DEPS := $(patsubst %,$(HEADER)/%,$(_DEPS))

DIR_S := src
DIR_O := obj
DIR_OW := weapons
SOURCES =   main.c \
			sdl_handle.c \
			player_movement.c \
			player_direction.c \
			utils.c \
			render_utils.c \
			render_sector.c \
			render_main.c \
			geom_utils.c \
			dn_init_textures_map.c \
			line.c \
			minimap.c \
			weapons/dn_init_weapons.c \
			weapons/dn_choose_weapon_render.c \
			weapons/dn_render_pistol.c \
			weapons/dn_render_pistol_states.c \
			weapons/dn_sdl_handle_key_wchoose_wstate.c \
			weapons/dn_sdl_handle_mouse_wstate.c
SRCS = $(addprefix $(DIR_S)/,$(SOURCES))
OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: obj $(NAME)

$(NAME): $(OBJS) $(EXTENSIONS)
		make libs
		$(CC) -o $(NAME) $(OBJS) $(FLAGS) $(CFLAGS) \
		-L $(LIBFTSDL) -lftSDL -L $(LIBJSON) -lJSON \
		$(SDL2_P) $(SDL2_F) -L $(LIBFT) -lft

libs: 
	make -C $(LIBFT)
	make -C $(LIBJSON)
	make -C $(LIBFTSDL)

obj:
	mkdir -p $(DIR_O)
	mkdir -p $(DIR_O)/$(DIR_OW)

$(DIR_O)/%.o: $(DIR_S)/%.c $(DEPS) $(EXTENSIONS)
		$(CC) -c -o $@ $< $(FLAGS) $(CFLAGS)

norme:
		make norme -C $(LIBFT)
		make norme -C $(LIBFTSDL)
		echo "--------------------Checking header files $(NAME)"
		norminette ./$(HEADER)
		echo "--------------------Checking source files $(NAME)"
		norminette ./$(DIR_S)

clean:
		rm -f $(OBJS)
		make clean -C $(LIBFT)
		make clean -C $(LIBFTSDL)
		make clean -C $(LIBJSON)
		rm -rf $(DIR_O)/$(DIR_OW)
		rm -rf $(DIR_O)

fclean: clean
		rm -f $(NAME)
		make fclean -C $(LIBFT)
		make fclean -C $(LIBFTSDL)
		make fclean -C $(LIBJSON)

pre_clean:
		rm -f $(DIR_O)/$(DIR_OW)/*.o
		rm -f $(DIR_O)/*.o
		rm -f $(NAME)
pre: pre_clean all

re: fclean all

.PHONY: all, obj, norme, clean, fclean, re, pre
.NOTPARALLEL:  all, obj, norme, clean, fclean, re, pre
# .SILENT:

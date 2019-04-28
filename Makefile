NAME := doom-nukem

CC := clang
KEYS := -Wall -Wextra -Werror
FLAGS :=

PDIR := $(shell pwd)

PDIR := $(shell pwd)

EXT := doom_nukem.h \
		structure.h \
		utils.h

IDIR := $(CURDIR)/inc
EXTENSIONS := $(addprefix $(IDIR)/, $(EXT))



LZIP_P := $(PDIR)/libzip/bin
LZIP_SRC := ./libzip

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CFLAGS := -I$(IDIR) \
		-I./libft/inc/ \
		-I./libftSDL/inc/ \
		-I./libJson/inc/ \
		-I /usr/local/include/SDL2/ \
		-I /usr/include/SDL2/ \
		-I./libzip/bin/include 
else
CFLAGS := -I$(IDIR) \
		-I./libft/inc/ \
		-I./libSDL/SDL2.framework/Headers/ \
		-I./libSDL/SDL2_mixer.framework/Headers/ \
		-I./libSDL/SDL2_image.framework/Headers/ \
		-I./libSDL/SDL2_ttf.framework/Headers/ \
		-I./libftSDL/inc/ \
		-I./libJson/inc/ \
		-I./libzip/bin/include 
endif

LIBFT := libft
LIBJSON := libJson
LIBFTSDL := libftSDL

ifeq ($(UNAME), Linux)
SDL2_F := -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm
else
SDL2_F := -framework SDL2 -framework SDL2_mixer -framework SDL2_image -framework SDL2_ttf -F ./libSDL/
endif
SDL2_P := -rpath @loader_path/libSDL/ \
			-rpath @loader_path/libzip/bin/lib 

HEADER := inc
_DEPS := doom_nukem.h \
		structure.h \
		utils.h

DEPS := $(patsubst %,$(HEADER)/%,$(_DEPS))

DIR_S := src
DIR_O := obj
SOURCES :=  main.c \
			sdl_handle.c \
			player_movement.c \
			player_direction.c \
			utils.c \
			render_utils.c \
			render_sector.c \
			render_main.c \
			draw_sprites.c \
			geom_utils.c \
			line.c \
			minimap.c \
			cast_ray.c \
			init_parser.c \
			parser_utils.c \
			remove_parser.c \
			intersections.c \
			textures.c \
			perspective.c \
			basic_edges.c \
			map_editor.c \
			map_editor_draw.c \
			map_editor_helpers.c \
			map_editor_init.c \
			map_editor_keys.c \
			map_editor_lines.c \
			map_editor_text.c \
			serialize_map.c \
			sprites_utils.c \
			color.c \
			sounds.c \
			sprite_collisions.c \
			hud.c \
			hud_loader.c \
			timers.c \
			draw_modes.c \
			menu.c \
			menu_option.c \
			victory.c \
			lift.c

SRCS := $(addprefix $(DIR_S)/,$(SOURCES))
OBJS := $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: obj $(NAME)

$(NAME): libs $(OBJS) $(EXTENSIONS)
		$(CC) -o $(NAME) $(OBJS) $(FLAGS) $(CFLAGS) \
		-L $(LIBFTSDL) -lftSDL -L $(LIBJSON) -lJSON \
		$(SDL2_P) $(SDL2_F) -L $(LIBFT) -lft -L libzip/bin/lib -lzip

libs:
	make -C $(LIBFT)
	make -C $(LIBJSON)
	make -C $(LIBFTSDL)
	make lz

obj:
	mkdir -p $(DIR_O)

$(DIR_O)/%.o: $(DIR_S)/%.c $(DEPS) $(EXTENSIONS)
		$(CC) -c -o $@ $< $(FLAGS) $(CFLAGS)

lz:
	if ! [ -d "$(LZIP_SRC)/build" ]; then \
		cd "$(LZIP_SRC)"; \
		mkdir -p build ; \
		cd build ; \
		cmake .. -DCMAKE_INSTALL_PREFIX='$(LZIP_P)'; \
		make ; \
		make install ; \
		cd $(PDIR) ; \
	fi

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
		rm -rf "$(LZIP_SRC)/build"
		rm -rf "$(LZIP_SRC)/bin"

pre_clean:
		rm -f $(DIR_O)/$(DIR_OW)/*.o
		rm -f $(DIR_O)/*.o
		rm -f $(NAME)
pre: pre_clean all

re: fclean all

.PHONY: all, obj, norme, clean, fclean, re, pre
.NOTPARALLEL:  all, obj, norme, clean, fclean, re, pre
.SILENT:

CC = clang

NAME = doom_nukem

KEYS = #-Wall -Wextra -Werror

FLAGS = -g

IDIR = ./inc

EXTENSIONS = $(addprefix $(IDIR)/,$(EXT))

EXT =	doom_nukem.h \
		enum.h \
		structure.h \
		utils.h \
		player.h \
		graphics_renderer.h

CFLAGS = -I$(IDIR) \
		-I./libft/inc \
		-I./libCL/inc \
		-I./libSDL/SDL2.framework/Headers/ \
		-I./libSDL/SDL2_image.framework/Headers/ \
		-I./libSDL/SDL2_ttf.framework/Headers/ \
		-I./libftSDL/inc \
		-I./libJson/inc 

LIBFT = libft

LIBJSON = libJson

LIBFTSDL = libftSDL

SDL2_F		= -framework SDL2 -framework SDL2_image -framework SDL2_ttf -F ./libSDL/

SDL2_P		= -rpath @loader_path/libSDL/

DIR_S = src

DIR_O = obj

HEADER = inc

_DEPS = doom_nukem.h \
		enum.h \
		structure.h \
		utils.h \
		player.h \
		graphics_renderer.h \
		macroses.h

DEPS = $(patsubst %,$(HEADER)/%,$(_DEPS))

SOURCES =   main.c \
			sdl_handle.c \
			player_movement.c \
			player_direction.c \
			utils.c \
			render_utils.c \
			render_sector.c \
			render_main.c \
			geom_utils.c \
			dn_init_textures_map.c

SRCS = $(addprefix $(DIR_S)/,$(SOURCES))

OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: obj $(NAME)

$(NAME): $(OBJS) $(EXTENSIONS)
		make libs
		$(CC) -o $(NAME) $(OBJS) $(FLAGS) $(CFLAGS) -L $(LIBFT) -lft -L $(LIBJSON) -lJSON $(SDL2_P) $(SDL2_F) -L $(LIBFTSDL) -lftsdl

libs: 
	make -C $(LIBFT)
	make -C $(LIBJSON)
	make -C $(LIBFTSDL)

obj:
	mkdir -p obj

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
		rm -rf $(DIR_O)

fclean: clean
		rm -f $(NAME)
		make fclean -C $(LIBFT)
		make fclean -C $(LIBFTSDL)
		make fclean -C $(LIBJSON)

re: fclean all

.PHONY: all, obj, norme, clean, fclean, re
.NOTPARALLEL:  all, obj, norme, clean, fclean, re
.SILENT:

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz -lbsd

SRCS_DIR = srcs
INCLUDES_DIR = includes
MLX_DIR = /home/csicsi/minilibx-linux

SRCS = $(SRCS_DIR)/cub3d.c

OBJS = $(SRCS:.c=.o)
INCLUDES = -I $(INCLUDES_DIR) -I $(MLX_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX_FLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re

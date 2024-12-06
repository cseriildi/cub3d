# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: icseri <icseri@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 10:41:15 by icseri            #+#    #+#              #
#    Updated: 2024/12/06 13:30:36 by icseri           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

MLX_FLAGS = -lmlx -lXext -lX11 -lm -lz -lbsd

SRCS = main.c \
		init.c \
		parsing.c \
		parsing_utils.c \
		parsing_textures.c \
		parsing_map.c \
		mlx_utils.c \
		cleanup.c \
		cast_rays.c \
		minimap.c \
		draw_utils.c \
		movement.c \

OBJS = ${SRCS:.c=.o}

COMP = cc

CFLAGS = -Wall -Wextra -Werror -g

all: ${NAME}

.c.o:
	${COMP} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	@${MAKE} -C ./libft --no-print-directory
	@${COMP} ${CFLAGS} ${OBJS} ./libft/libft.a -o ${NAME} $(MLX_FLAGS)

clean:
	@${MAKE} -C ./libft --no-print-directory fclean
	@rm -rf ${OBJS}

fclean: clean
	@rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean reNAME = cub3d

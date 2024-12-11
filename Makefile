# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 10:41:15 by icseri            #+#    #+#              #
#    Updated: 2024/12/11 09:56:06 by dcsicsak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3d

BONUS_NAME = cub3d_bonus

MLX_FLAGS = -lmlx -lXext -lX11 -lm -lz -lbsd

COMMON_SRCS = init.c \
		parsing.c \
		parsing_utils.c \
		parsing_textures.c \
		parsing_map.c \
		mlx_utils.c \
		cleanup.c \
		cast_rays.c \
		cast_rays_horizontal.c \
		cast_rays_vertical.c \
		minimap.c \
		draw_utils.c \
		movement.c \

SRCS = main.c ${COMMON_SRCS}

BONUS_SRCS = main_bonus.c ${COMMON_SRCS}

OBJS = ${SRCS:.c=.o}

BONUS_OBJS = ${BONUS_SRCS:.c=.o}

COMP = cc

CFLAGS = -Wall -Wextra -Werror -g

all: ${NAME}

.c.o:
	${COMP} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	@${MAKE} -C ./libft --no-print-directory
	@${COMP} ${CFLAGS} ${OBJS} ./libft/libft.a -o ${NAME} $(MLX_FLAGS)

bonus: ${BONUS_OBJS}
	@${MAKE} -C ./libft --no-print-directory
	@${COMP} ${CFLAGS} ${BONUS_OBJS} ./libft/libft.a -o ${BONUS_NAME} $(MLX_FLAGS)

clean:
	@${MAKE} -C ./libft --no-print-directory fclean
	@rm -rf ${OBJS} ${BONUS_OBJS}

fclean: clean
	@rm -rf ${NAME} ${BONUS_NAME}

re: fclean all

.PHONY: all clean fclean bonus reNAME = cub3d

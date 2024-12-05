# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: icseri <icseri@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 10:41:15 by icseri            #+#    #+#              #
#    Updated: 2024/12/05 12:49:19 by icseri           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz -lbsd

SRCS_DIR = srcs
INCLUDES_DIR = includes
MLX_DIR = /home/csicsi/minilibx-linux

INCLUDES = -I $(INCLUDES_DIR) -I $(MLX_DIR)

SRCS = main.c \
		parsing.c \
		cleanup.c \
		parsing_utils.c \
		parsing_textures.c \
		$(SRCS_DIR)/cub3d.c \
		$(SRCS_DIR)/draw_utils.c \
		$(SRCS_DIR)/minimap.c \
		$(SRCS_DIR)/movement.c \
		$(SRCS_DIR)/raycaster.c

OBJS = ${SRCS:.c=.o}

COMP = cc

CFLAGS = -Wall -Wextra -Werror -g

all: ${NAME}

.c.o: 
	${COMP} ${CFLAGS} $(INCLUDES) -c $< -o $@

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

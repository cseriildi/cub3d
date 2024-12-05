# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 10:41:15 by icseri            #+#    #+#              #
#    Updated: 2024/12/05 15:29:51 by dcsicsak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

MLX_FLAGS = -lmlx -lXext -lX11 -lm -lz -lbsd

SRCS = main.c \
		parsing.c \
		cleanup.c \
		parsing_utils.c \
		parsing_textures.c

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

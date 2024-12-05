/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:35 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 13:24:43 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>

typedef struct s_map
{
	char	*north; //mlx_xpm_file_to_image
	char	*south;
	char	*west;
	char	*east;
	char	*floor;
	char	*ceiling;
	char	**map;
	int		row;
	int		column;
	int		player[2];
	int		enemy[2];
	int		fd;
}	t_map;

//parsing
void	parsing(int argc, char **argv, t_map *map);
void	get_texture(char *line, char **texture, t_map *map);
void	get_color(char *line, char **color, t_map *map);
void	get_map(char *line, t_map *map);
void	check_map(t_map *map);
void	list_to_arr(t_list **map_list, t_map *map);

//cleanup
void	print_error(int count, ...);
void	safe_exit(t_map *map, int exit_code);
void	free_array(char ***arr);
void	free_list(t_list **list);

#endif

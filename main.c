/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 15:42:11 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_map(t_map *map);

int	parse(int argc, char **argv)
{
	t_map	map;

	init_map(&map);
	parsing(argc, argv, &map);
	printf("[%s], [%s], [%s], [%s], [%d], [%d], [%d], [%d], [%d, %d], [%d, %d], [%d, %d], [%d, %d], [%d, %d], [%d, %d]\n", map.north, 
	map.south,
	map.west ,
	map.east ,
	map.floor,
	map.ceiling,
	map.row,
	map.column,
	map.n_size[0], map.n_size[1],
	map.s_size[0], map.s_size[1],
	map.e_size[0], map.e_size[1],
	map.w_size[0], map.w_size[1],
	map.player[0], map.player[1],
	map.enemy[0], map.enemy[1]);
	safe_exit(&map, EXIT_SUCCESS);
	return 0;
}

void	init_map(t_map *map)
{
	map->north = NULL;
	map->south = NULL;
	map->west = NULL;
	map->east = NULL;
	map->map = NULL;
	map->row = 0;
	map->column = 0;
	map->floor = 0;
	map->ceiling = 0;
	map->fd = -1;
	map->player[0] = -1;
	map->player[1] = -1;
	map->enemy[0] = -1;
	map->enemy[1] = -1;
	map->n_size[0] = 0;
	map->n_size[1] = 0;
	map->s_size[0] = 0;
	map->s_size[1] = 0;
	map->e_size[0] = 0;
	map->e_size[1] = 0;
	map->w_size[0] = 0;
	map->w_size[1] = 0;
	
}

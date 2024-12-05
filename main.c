/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 14:23:52 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_data(t_map *map);

int	parse(int argc, char **argv, t_data *data)
{
	t_map	map;

	data->map_data = &map;
	init_data(data->map_data);
	parsing(argc, argv, data->map_data);
	return (0);
}

void	init_data(t_map *map)
{
	map->north = NULL;
	map->south = NULL;
	map->west = NULL;
	map->east = NULL;
	map->map = NULL;
	map->row = 0;
	map->column = 0;
	map->floor = NULL;
	map->ceiling = NULL;
	map->fd = -1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:32:37 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 13:44:23 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_in(int row, int col, t_map *map)
{
	if (row == 0 || col == 0 || row == map->row - 1 || col == map->column - 1
		|| map->map[row][col - 1] == ' ' || map->map[row][col + 1] == ' '
		|| map->map[row - 1][col] == ' ' || map->map[row + 1][col] == ' ')
		return (false);
	return (true);
}

bool	door_is_good(int row, int col, t_map *map)
{
	bool	east;
	bool	west;
	bool	south;
	bool	north;

	east = map->map[row][col + 1] == '1';
	west = map->map[row][col - 1] == '1';
	south = map->map[row + 1][col] == '1';
	north = map->map[row - 1][col] == '1';
	if (map->is_bonus == false || map->door.textures == NULL
		|| is_in(row, col, map) == false
		|| (east && (west == false || south || north))
		|| (north && (south == false || east || west))
		|| (east == false && north == false)
		|| map->door.count != 1)
		return (false);
	return (true);
}

bool	is_valid(int row, int col, int *player_count, t_map *map)
{
	if (!ft_strchr("WENS01D ", map->map[row][col]))
		return (false);
	else if (!ft_strchr("D1 ", map->map[row][col]))
	{
		if (map->map[row][col] != '0')
		{
			(*player_count)++;
			if (*player_count > 1)
				return (false);
			map->player[0] = row;
			map->player[1] = col;
		}
		return (is_in(row, col, map));
	}
	else if (map->map[row][col] == 'D')
		return (door_is_good(row, col, map));
	return (true);
}

void	check_map(t_map *map)
{
	int		player_count;
	int		row;
	int		column;

	player_count = 0;
	row = -1;
	while (map->map[++row])
	{
		column = -1;
		while (map->map[row][++column])
		{
			if (!is_valid(row, column, &player_count, map))
				safe_exit(map, MAP);
		}
	}
	if (player_count == 0)
		safe_exit(map, MAP);
}

void	list_to_arr(t_list **map_list, char ***arr, t_map *map)
{
	int		row;
	int		column;
	t_list	*current;

	if (!map_list)
		return ;
	column = 0;
	*arr = ft_calloc(sizeof(char *), ft_lstsize(*map_list) + 1);
	if (!*arr)
		return (ft_lstclear(map_list, &free), safe_exit(map, MALLOC));
	current = *map_list;
	row = -1;
	while (current)
	{
		(*arr)[++row] = ft_strdup(current->content);
		if (!(*arr)[row])
			return (ft_lstclear(map_list, &free), safe_exit(map, MALLOC));
		current = current->next;
	}
	ft_lstclear(map_list, &free);
}

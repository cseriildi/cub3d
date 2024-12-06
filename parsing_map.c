/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:32:37 by icseri            #+#    #+#             */
/*   Updated: 2024/12/06 12:57:16 by icseri           ###   ########.fr       */
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
	if (is_in(row, col, map) == false
		|| (east && (west == false || south || north))
		|| (north && (south == false || east || west))
		|| (east == false && north == false))
		return (false);
	return (true);
}

void	check_map(t_map *map)
{
	int		player_count;
	int		row;
	int		column;
	bool	valid;

	player_count = 0;
	row = -1;
	valid = true;
	while (valid && map->map[++row])
	{
		column = -1;
		while (valid && map->map[row][++column])
		{
			if (!ft_strchr("WENS01XD ", map->map[row][column]))
				valid = false;
			else if (!ft_strchr("D1 ", map->map[row][column]))
			{
				if (map->map[row][column] == 'X')
				{
					map->enemy[0] = row;
					map->enemy[1] = column;
				}
				else if (map->map[row][column] != '0')
				{
					player_count++;
					valid = (player_count <= 1);
					map->player[0] = row;
					map->player[1] = column;
				}
				valid = is_in(row, column, map);
			}
			else if (map->map[row][column] == 'D')
				valid = door_is_good(row, column, map);
		}
	}
	if (valid == false || player_count == 0)
		safe_exit(map, COLOR);
}

void	list_to_arr(t_list **map_list, t_map *map)
{
	int		row;
	t_list	*current;

	map->map = ft_calloc(sizeof(char *), map->row + 1);
	if (!map->map)
		return (free_list(map_list), safe_exit(map, MALLOC));
	current = *map_list;
	while (current)
	{
		if ((int)(ft_strlen(current->content)) > map->column)
			map->column = ft_strlen(current->content);
		current = current->next;
	}
	current = *map_list;
	row = -1;
	while (current)
	{
		map->map[++row] = malloc(map->column + 1);
		if (!map->map[row])
			return (free_list(map_list), safe_exit(map, MALLOC));
		ft_memset(map->map[row], ' ', map->column);
		map->map[row][map->column] = '\0';
		ft_memcpy(map->map[row], current->content, ft_strlen(current->content));
		current = current->next;
	}
	free_list(map_list);
}

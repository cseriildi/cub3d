/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:03 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 13:27:06 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_file(char	*filename, bool is_cub);
void	parse_file(t_map *map);

void	parsing(int argc, char **argv, t_map *map)
{
	if (argc != 2)
	{
		print_error(3, "Error\nUsage: ", argv[0], " <map-filename>.cub");
		exit(EXIT_FAILURE);
	}
	check_file(argv[1], true);
	map->fd = open(argv[1], O_RDONLY, 0644);
	if (map->fd == -1)
	{
		print_error(4, "Error\n", argv[1], " ", strerror(errno));
		exit(EXIT_FAILURE);
	}
	parse_file(map);
}

void	check_file(char	*filename, bool is_cub)
{
	size_t	len;

	len = ft_strlen(filename);
	if (is_cub && (len <= 4 || ft_strncmp(filename + len - 4, ".cub", 5) != 0))
	{
		print_error(1, "Error\nFilename should end with .cub");
		exit(EXIT_FAILURE);
	}
	if (access(filename, F_OK) == -1)
	{
		print_error(4, "Error\n", filename, " ", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (access(filename, R_OK) == -1)
	{
		print_error(4, "Error\n", filename, " ", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void	parse_file(t_map *map)
{
	char	*line;

	line = get_next_line(map->fd);
	while (line)
	{
		if (ft_strncmp(line, "NO ", 3) == 0)
			get_texture(line, &map->north, map);
		else if (ft_strncmp(line, "EA ", 3) == 0)
			get_texture(line, &map->east, map);
		else if (ft_strncmp(line, "WE ", 3) == 0)
			get_texture(line, &map->west, map);
		else if (ft_strncmp(line, "SO ", 3) == 0)
			get_texture(line, &map->south, map);
		else if (ft_strncmp(line, "C ", 2) == 0)
			get_color(line, &map->ceiling, map);
		else if (ft_strncmp(line, "F ", 2) == 0)
			get_color(line, &map->floor, map);
		else if (ft_strncmp(line, "\n", 2) != 0)
		{
			get_map(line, map);
			break ;
		}
		else
			ft_free(&line);
		line = get_next_line(map->fd);
	}
}

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
	bool east;
	bool west;
	bool south;
	bool north;

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
	while (valid && ++row < map->row)
	{
		column = -1;
		while (valid && map->map && map->map[row][++column])
		{
			if (!ft_strchr("WENS01XD ", map->map[row][column]))
				valid = false;
			else if (!ft_strchr("D1 ", map->map[row][column]))
			{
				if (ft_strchr("WENS", map->map[row][column]) )
				{
					valid = (++player_count <= 1);
					map->player[0] = row;
					map->player[1] = column;
				}
				else if (map->map[row][column] == 'X')
				{
					map->enemy[0] = row;
					map->enemy[1] = column;
				}
				valid = is_in(row, column, map);
			}
			else if (map->map[row][column] == 'D')
				valid = door_is_good(row, column, map);
		}
	}
	if (valid == false || player_count == 0)
	{
		print_error(1, "Error\nInvalid map");
		safe_exit(map, 1);
	}
}

void	list_to_arr(t_list **map_list, t_map *map)
{
	int		row;
	t_list	*current;

	row = 0;
	map->map = ft_calloc(sizeof(char *), map->row + 1);
	if (!map->map)
	{
		print_error(1, "Error\nMalloc fail");
		free_list(map_list);
		safe_exit(map, EXIT_FAILURE);
	}
	current = *map_list;
	while (current)
	{
		if ((int)(ft_strlen(current->content)) > map->column)
			map->column = ft_strlen(current->content);
		current = current->next;
	}
	current = *map_list;
	while (current)
	{
		map->map[row] = malloc(map->column + 1);
		if (!map->map[row])
		{
			print_error(1, "Error\nMalloc fail");
			free_list(map_list);
			safe_exit(map, EXIT_FAILURE);
		}
		ft_memset(map->map[row], ' ', map->column);
		map->map[row][map->column] = '\0';
		ft_memcpy(map->map[row], current->content, ft_strlen(current->content));
		row++;
		current = current->next;
	}
	free_list(map_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:03 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 16:16:24 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_file(char	*filename, bool is_cub, t_map *map);
void	parse_file(t_map *map);

void	parsing(int argc, char **argv, t_map *map)
{
	if (argc != 2)
	{
		print_error(3, "Error\nUsage: ", argv[0], " <map-filename>.cub");
		safe_exit(map, EXIT_FAILURE);
	}
	check_file(argv[1], true, map);
	parse_file(map);
	close(map->fd);
}

void	check_file(char	*filename, bool is_cub, t_map *map)
{
	size_t	len;

	len = ft_strlen(filename);
	if (is_cub && (len <= 4 || ft_strncmp(filename + len - 4, ".cub", 5) != 0))
	{
		print_error(1, "Error\nFilename should end with .cub");
		safe_exit(map, EXIT_FAILURE);
	}
	if (ft_strncmp(filename, "./", 2) == 0)
		map->fd = open(filename + 2, O_RDONLY, 0644);
	else
		map->fd = open(filename, O_RDONLY, 0644);
	if (map->fd == -1)
	{
		print_error(4, "Error\n", filename, " ", strerror(errno));
		safe_exit(map, EXIT_FAILURE);
	}
}

void	set_size(int *size, char *texture, t_map *map)
{
	char	*line;
	char	**numbers;

	check_file(texture, false, map);
	line = get_next_line(map->fd);
	ft_free(&line);
	line = get_next_line(map->fd);
	ft_free(&line);
	line = get_next_line(map->fd);
	close(map->fd);
	if (!line || ft_strlen(line) < 10)
	{
		ft_free(&line);
		print_error(1, "Error\nInvalid texture");
		safe_exit(map, EXIT_FAILURE);
	}
	numbers = ft_split(line + 1, ' ');
	ft_free(&line);
	if (!numbers)
	{
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
	size[0] = ft_atoi(numbers[0]);
	size[1] = ft_atoi(numbers[1]);
	free_array(&numbers);
}


void	check_textures(t_map *map)
{
	if (map->ceiling < 0 || map->floor < 0
		|| !map->east || !map->west || !map->north || !map->south)
	{
		print_error(1, "Error\nInvalid map");
		safe_exit(map, EXIT_FAILURE);
	}
	set_size(map->n_size, map->north, map);
	set_size(map->s_size, map->south, map);
	set_size(map->w_size, map->west, map);
	set_size(map->e_size, map->east, map);
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
			check_textures(map);
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

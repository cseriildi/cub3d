/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:03 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 12:28:45 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_file(char	*filename, bool is_cub);
void	parse_file(t_data *data);

void	parsing(int argc, char **argv, t_data *data)
{
	if (argc != 2)
	{
		print_error(3, "Error\nUsage: ", argv[0], " <map-filename>.cub");
		exit(EXIT_FAILURE);
	}
	check_file(argv[1], true);
	data->fd = open(argv[1], O_RDONLY, 0644);
	if (data->fd == -1)
	{
		print_error(4, "Error\n", argv[1], " ", strerror(errno));
		exit(EXIT_FAILURE);
	}
	parse_file(data);
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

void	parse_file(t_data *data)
{
	char	*line;

	line = get_next_line(data->fd);
	while (line)
	{
		if (ft_strncmp(line, "NO ", 3) == 0)
			get_texture(line, &data->north, data);
		else if (ft_strncmp(line, "EA ", 3) == 0)
			get_texture(line, &data->east, data);
		else if (ft_strncmp(line, "WE ", 3) == 0)
			get_texture(line, &data->west, data);
		else if (ft_strncmp(line, "SO ", 3) == 0)
			get_texture(line, &data->south, data);
		else if (ft_strncmp(line, "C ", 2) == 0)
			get_color(line, &data->ceiling, data);
		else if (ft_strncmp(line, "F ", 2) == 0)
			get_color(line, &data->floor, data);
		else if (ft_strncmp(line, "\n", 2) != 0)
		{
			get_map(line, data);
			break ;
		}
		else
			ft_free(&line);
		line = get_next_line(data->fd);
	}
}

bool	is_in(int row, int col, t_data *data)
{
	if (row == 0 || col == 0 || row == data->row - 1 || col == data->column - 1
		|| data->map[row][col - 1] == ' ' || data->map[row][col + 1] == ' '
		|| data->map[row - 1][col] == ' ' || data->map[row + 1][col] == ' ')
		return (false);
	return (true);
}

bool	door_is_good(int row, int col, t_data *data)
{
	bool east;
	bool west;
	bool south;
	bool north;

	east = data->map[row][col + 1] == '1';
	west = data->map[row][col - 1] == '1';
	south = data->map[row + 1][col] == '1';
	north = data->map[row - 1][col] == '1';

	if (is_in(row, col, data) == false
		|| (east && (west == false || south || north))
		|| (north && (south == false || east || west))
		|| (east == false && north == false))
		return (false);
	return (true);
}

void	check_map(t_data *data)
{
	int		player_count;
	int		row;
	int		column;
	bool	valid;

	player_count = 0;
	row = -1;
	valid = true;
	while (valid && ++row < data->row)
	{
		column = -1;
		while (valid && data->map && data->map[row][++column])
		{
			if (!ft_strchr("WENS01XD ", data->map[row][column]))
				valid = false;
			else if (!ft_strchr("D1 ", data->map[row][column]))
			{
				if (ft_strchr("WENS", data->map[row][column]) )
				{
					valid = (++player_count <= 1);
					data->player[0] = row;
					data->player[1] = column;
				}
				else if (data->map[row][column] == 'X')
				{
					data->enemy[0] = row;
					data->enemy[1] = column;
				}
				valid = is_in(row, column, data);
			}
			else if (data->map[row][column] == 'D')
				valid = door_is_good(row, column, data);
		}
	}
	if (valid == false || player_count == 0)
	{
		print_error(1, "Error\nInvalid map");
		safe_exit(data, 1);
	}
}

void	list_to_arr(t_list **map_list, t_data *data)
{
	int		row;
	t_list	*current;

	row = 0;
	data->map = ft_calloc(sizeof(char *), data->row + 1);
	if (!data->map)
	{
		print_error(1, "Error\nMalloc fail");
		free_list(map_list);
		safe_exit(data, EXIT_FAILURE);
	}
	current = *map_list;
	while (current)
	{
		if ((int)(ft_strlen(current->content)) > data->column)
			data->column = ft_strlen(current->content);
		current = current->next;
	}
	current = *map_list;
	while (current)
	{
		data->map[row] = malloc(data->column + 1);
		if (!data->map[row])
		{
			print_error(1, "Error\nMalloc fail");
			free_list(map_list);
			safe_exit(data, EXIT_FAILURE);
		}
		ft_memset(data->map[row], ' ', data->column);
		data->map[row][data->column] = '\0';
		ft_memcpy(data->map[row], current->content, ft_strlen(current->content));
		row++;
		current = current->next;
	}
	free_list(map_list);
}

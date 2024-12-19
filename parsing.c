/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:03 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 08:45:43 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_file(t_map *map);

void	parsing(int argc, char **argv, t_map *map)
{
	if (argc != 2)
	{
		print_error(3, "Error\nUsage: ", argv[0], " <map-filename>.cub");
		safe_exit(map, EXIT_FAILURE);
	}
	map->fd = safe_open(argv[1], true, map);
	parse_file(map);
	if (close(map->fd) == -1)
	{
		print_error(4, "Error\n", argv[1], " ", strerror(errno));
		safe_exit(map, EXIT_FAILURE);
	}
	check_map(map);
}

int	safe_open(char	*filename, bool is_cub, t_map *map)
{
	size_t	len;
	int		fd;

	len = ft_strlen(filename);
	if (is_cub && (len <= 4 || ft_strncmp(filename + len - 4, ".cub", 5) != 0))
	{
		print_error(1, "Error\nFilename should end with .cub");
		safe_exit(map, EXIT_FAILURE);
	}
	if (ft_strncmp(filename, "./", 2) == 0)
		fd = open(filename + 2, O_RDONLY, 0644);
	else
		fd = open(filename, O_RDONLY, 0644);
	if (fd == -1)
	{
		print_error(4, "Error\n", filename, " ", strerror(errno));
		safe_exit(map, EXIT_FAILURE);
	}
	return (fd);
}

void	parse_file(t_map *map)
{
	map->data->line = get_next_line(map->fd);
	while (map->data->line)
	{
		if (ft_strncmp(map->data->line, "NO ", 3) == 0)
			get_texture(&map->north, map);
		else if (ft_strncmp(map->data->line, "EA ", 3) == 0)
			get_texture(&map->east, map);
		else if (ft_strncmp(map->data->line, "WE ", 3) == 0)
			get_texture(&map->west, map);
		else if (ft_strncmp(map->data->line, "SO ", 3) == 0)
			get_texture(&map->south, map);
		else if (map->is_bonus && ft_strncmp(map->data->line, "DO ", 3) == 0)
			get_texture(&map->door, map);
		else if (ft_strncmp(map->data->line, "C ", 2) == 0)
			get_color(&map->ceiling, map);
		else if (ft_strncmp(map->data->line, "F ", 2) == 0)
			get_color(&map->floor, map);
		else if (ft_strncmp(map->data->line, "\n", 2) != 0)
			return (get_map(map));
		ft_free(&map->data->line);
		map->data->line = get_next_line(map->fd);
	}
}

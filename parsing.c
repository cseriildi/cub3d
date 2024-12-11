/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:03 by icseri            #+#    #+#             */
/*   Updated: 2024/12/10 10:49:29 by icseri           ###   ########.fr       */
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
	safe_open(argv[1], true, map);
	parse_file(map);
	close(map->fd);
}

void	safe_open(char	*filename, bool is_cub, t_map *map)
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
		else if (map->is_bonus && ft_strncmp(line, "DO ", 3) == 0)
			get_texture(line, &map->door, map);
		else if (ft_strncmp(line, "C ", 2) == 0)
			get_color(line, &map->ceiling, map);
		else if (ft_strncmp(line, "F ", 2) == 0)
			get_color(line, &map->floor, map);
		else if (ft_strncmp(line, "\n", 2) != 0)
			return (get_map(line, map));
		else
			ft_free(&line);
		line = get_next_line(map->fd);
	}
}

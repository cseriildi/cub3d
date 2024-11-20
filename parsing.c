/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:55:03 by icseri            #+#    #+#             */
/*   Updated: 2024/11/20 17:03:00 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_file(char	*filename, bool is_cub);
void	get_map(t_data *data, char *filename);

void	parsing(int argc, char **argv, t_data *data)
{
	if (argc != 2)
	{
		print_error(3, "Error\nUsage: ", argv[0], " <map-filename>.cub");
		exit(EXIT_FAILURE);
	}
	check_file(argv[1], true);
	get_map(data, argv[1]);
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
		print_error(3, "Error\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (access(filename, R_OK) == -1)
	{
		print_error(3, "Error\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void	get_map(t_data *data, char *filename)
{
	char	*line;

	data->fd = open(filename, O_RDONLY, 0644);
	if (data->fd == -1)
	{
		print_error(3, "Error\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	line = get_next_line(data->fd);
	while (line)
	{
		get_texture(line, data);
		ft_free(&line);
		line = get_next_line(data->fd);
	}
}

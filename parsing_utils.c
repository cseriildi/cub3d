/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:26:07 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 15:42:11 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void			set_colors(char **colors, int *color, t_map *map);
unsigned char	get_number(char *str, char **colors, t_map *map);

void	get_color(char *line, int *color, t_map *map)
{
	char	**color_list;

	if (*color)
	{
		ft_free(&line);
		print_error(1, "Error\nDuplicate color");
		safe_exit(map, EXIT_FAILURE);
	}
	if (ft_strlen(line) <= 2)
	{
		ft_free(&line);
		print_error(1, "Error\nInvalid color");
		safe_exit(map, EXIT_FAILURE);
	}
	color_list = ft_split(line + 2, ',');
	ft_free(&line);
	if (!color_list)
	{
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
	set_colors(color_list, color, map);
}

void	create_and_add(char *line, t_list **map_list, t_map *map)
{
	t_list	*new;
	char	*content;

	content = ft_strtrim(line, "\n");
	ft_free(&line);
	if (!content)
	{
		free_list(map_list);
		safe_exit(map, EXIT_FAILURE);
	}
	new = ft_lstnew(content);
	if (!new)
	{
		ft_free(&content);
		free_list(map_list);
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
	ft_lstadd_back(map_list, new);
}

void	get_map(char *line, t_map *map)
{
	t_list	**map_list;

	map_list = malloc(sizeof(t_list *));
	if (!map_list)
	{
		ft_free(&line);
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
	*map_list = NULL;
	while (line)
	{
		map->row++;
		create_and_add(line, map_list, map);
		line = get_next_line(map->fd);
	}
	list_to_arr(map_list, map);
	check_map(map);
}

void	set_colors(char **colors, int *color, t_map *map)
{
	int	r;
	int	g;
	int	b;
	
	if (!colors[0] || !colors[1] || !colors[2] || colors[3])
	{
		free_array(&colors);
		print_error(1, "Error\nInvalid color");
		safe_exit(map, EXIT_FAILURE);
	}
	r = get_number(colors[0], colors, map);
	g = get_number(colors[1], colors, map);
	b = get_number(colors[2], colors, map);
	*color = (r << 16) | (g << 8) | b;
	free_array(&colors);
}

unsigned char	get_number(char *str, char **colors, t_map *map)
{
	unsigned char	color_code;
	char			*trimmed_str;
	int				num;
	char			*new_str;

	trimmed_str = ft_strtrim(str, " \t\v\n\f\r");
	if (!trimmed_str)
	{
		free_array(&colors);
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
	num = ft_atoi(trimmed_str);
	color_code = num;
	new_str = ft_itoa(num);
	if (!new_str)
	{
		free_array(&colors);
		ft_free(&trimmed_str);
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
	if (ft_strncmp(trimmed_str, new_str, ft_strlen(new_str) + 1) != 0
		|| (int)color_code != num)
	{
		free_array(&colors);
		ft_free(&new_str);
		ft_free(&trimmed_str);
		print_error(1, "Error\nInvalid color");
		safe_exit(map, EXIT_FAILURE);
	}
	return (ft_free(&new_str), ft_free(&trimmed_str), color_code);
}

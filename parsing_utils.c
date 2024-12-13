/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:26:07 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 13:45:10 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_colors(char **colors, int *color, t_map *map);
int		get_number(char *str, char **colors, t_map *map);

void	get_color(char *line, int *color, t_map *map)
{
	char	**color_list;

	if (*color != -1 || ft_strlen(line) <= 2)
		return (ft_free(&line), safe_exit(map, COLOR));
	color_list = ft_split(line + 2, ',');
	ft_free(&line);
	if (!color_list)
		safe_exit(map, MALLOC);
	set_colors(color_list, color, map);
}

void	fix_map(t_list **map_list, t_map *map)
{
	t_list	*current;
	char	*fixed_content;

	current = *map_list;
	{
		if ((int)ft_strlen(current->content) < map->column)
		{
			fixed_content = ft_calloc(map->column + 1, sizeof(char));
			if (!fixed_content)
				return (ft_lstclear(map_list, &free), safe_exit(map, MALLOC));
			ft_memset(fixed_content, ' ', map->column);
			ft_memcpy(fixed_content, current->content,
				ft_strlen(current->content));
			free(current->content);
			current->content = fixed_content;
		}
		current = current->next;
	}
}

void	get_map(char *line, t_map *map)
{
	t_list	*map_list;

	check_textures(map);
	map_list = NULL;
	while (line)
	{
		if ((int)(ft_strlen(line)) > map->column)
			map->column = ft_strlen(line);
		map->row++;
		if (*line == '\n')
		{
			ft_free(&line);
			ft_lstclear(&map_list, &free);
			safe_exit(map, MAP);
		}
		create_and_add(line, &map_list, map);
		line = get_next_line(map->fd);
	}
	fix_map(&map_list, map);
	list_to_arr(&map_list, &(map->map), map);
	check_map(map);
}

void	set_colors(char **colors, int *color, t_map *map)
{
	char	*tmp;
	int		i;
	int		r;
	int		g;
	int		b;

	if (!colors[0] || !colors[1] || !colors[2] || colors[3])
		return (free_array(&colors), safe_exit(map, COLOR));
	i = -1;
	while (++i <= 2)
	{
		tmp = ft_strtrim(colors[i], " \t\v\n\f\r");
		if (!tmp)
			return (free_array(&colors), safe_exit(map, MALLOC));
		free(colors[i]);
		colors[i] = tmp;
	}
	r = get_number(colors[0], colors, map);
	g = get_number(colors[1], colors, map);
	b = get_number(colors[2], colors, map);
	*color = (r << 16) | (g << 8) | b;
	free_array(&colors);
}

int	get_number(char *str, char **colors, t_map *map)
{
	int		num;
	char	*new_str;

	num = ft_atoi(str);
	new_str = ft_itoa(num);
	if (!new_str)
	{
		free_array(&colors);
		ft_free(&str);
		safe_exit(map, MALLOC);
	}
	if (ft_strncmp(str, new_str, ft_strlen(new_str) + 1) != 0
		|| (int)(unsigned char)num != num)
	{
		free_array(&colors);
		ft_free(&new_str);
		safe_exit(map, COLOR);
	}
	return (ft_free(&new_str), num);
}

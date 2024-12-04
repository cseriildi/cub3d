/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:26:07 by icseri            #+#    #+#             */
/*   Updated: 2024/12/04 17:24:15 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char			*set_colors(char **colors, char *color, t_data *data);
unsigned char	get_number(char *str, char **colors, t_data *data);

char	*get_color(char *line, char *color, t_data *data)
{
	char	**color_list;

	if (color)
	{
		ft_free(&line);
		print_error(1, "Error\nDuplicate color");
		safe_exit(data, EXIT_FAILURE);
	}
	if (ft_strlen(line) <= 2)
	{
		ft_free(&line);
		print_error(1, "Error\nInvalid color");
		safe_exit(data, EXIT_FAILURE);
	}
	color_list = ft_split(line + 2, ',');
	ft_free(&line);
	if (!color_list)
	{
		print_error(1, "Error\nMalloc fail");
		safe_exit(data, EXIT_FAILURE);
	}
	color = ft_calloc(sizeof(char), 8);
	if (!color)
	{
		print_error(1, "Error\nMalloc failed");
		safe_exit(data, EXIT_FAILURE);
	}
	return (set_colors(color_list, color, data));
	//free_array(&color_list); //double free
}

int	create_and_add(char *line, t_list **map_list)
{
	t_list	*new;
	char	*content;

	content = ft_strtrim(line, "\n");
	ft_free(&line);
	if (!content)
		return (EXIT_FAILURE);
	new = ft_lstnew(content);
	if (!new)
	{
		print_error(1, "Error\nMalloc fail");
		//free list
		return (EXIT_FAILURE);
	}
	ft_lstadd_back(map_list, new);
	return (EXIT_SUCCESS);
}

void	get_map(char *line, t_data *data)
{
	t_list	**map_list;

	map_list = malloc(sizeof(t_list *));
	*map_list = NULL;
	while (line)
	{
		data->row++;
		create_and_add(line, map_list);
		line = get_next_line(data->fd);
	}
	list_to_arr(map_list, data);
	check_map(data);
}

char *set_colors(char **colors, char *color, t_data *data)
{
	int	r;
	int	g;
	int	b;
	
	if (!colors[0] || !colors[1] || !colors[2] || colors[3])
	{
		free_array(&colors);
		print_error(1, "Error\nInvalid color");
		safe_exit(data, EXIT_FAILURE);
	}
	r = get_number(colors[0], colors, data);
	g = get_number(colors[1], colors, data);
	b = get_number(colors[2], colors, data);
	color[0] = '#';
    color[1] = "0123456789ABCDEF"[r / 16];
    color[2] = "0123456789ABCDEF"[r % 16];
    color[3] = "0123456789ABCDEF"[g / 16];
    color[4] = "0123456789ABCDEF"[g % 16];
    color[5] = "0123456789ABCDEF"[b / 16];
    color[6] = "0123456789ABCDEF"[b % 16];
	return (color);
	//free_array(&colors); //double free
}

unsigned char	get_number(char *str, char **colors, t_data *data)
{
	unsigned char	color_code;
	char			*trimmed_str;
	int				num;
	char			*new_str;

	trimmed_str = ft_strtrim(str, " \t\v\n\f\r");
	ft_free(&str);
	if (!trimmed_str)
	{
		free_array(&colors);
		print_error(1, "Error\nMalloc fail");
		safe_exit(data, EXIT_FAILURE);
	}
	num = ft_atoi(trimmed_str);
	color_code = num;
	new_str = ft_itoa(num);
	if (ft_strncmp(trimmed_str, new_str, ft_strlen(new_str) + 1) != 0
		|| (int)color_code != num)
	{
		//free_array(&colors); //double free
		ft_free(&new_str);
		print_error(1, "Error\nInvalid color");
		safe_exit(data, EXIT_FAILURE);
	}
	return (ft_free(&new_str), color_code);
}

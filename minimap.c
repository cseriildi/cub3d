/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:54:17 by icseri            #+#    #+#             */
/*   Updated: 2024/12/10 09:57:10 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_tile(t_data *data, int row, int col)
{
	int	r;
	int	c;

	r = floor(data->player_y) - (data->minimap.size / 2) + row;
	c = floor(data->player_x) - (data->minimap.size / 2) + col;
	data->minimap.map_row = r;
	data->minimap.map_col = c;
	if (r >= 0 && r < data->map.row && c >= 0 && c < data->map.column)
		data->minimap.tile = data->map.map[r][c];
	else
		data->minimap.tile = ' ';
	data->rect.x = col * data->minimap.tile_size - data->minimap.offset_x;
	data->rect.y = row * data->minimap.tile_size - data->minimap.offset_y;
	data->rect.width = data->minimap.tile_size;
	data->rect.height = data->minimap.tile_size;
	if (data->minimap.tile == '1')
		data->rect.color = data->map.ceiling;
	else if (data->minimap.tile == 'D')
		data->rect.color = DOOR_COLOR;
	else if (ft_strchr("WENS0d", data->minimap.tile) != 0)
		data->rect.color = data->map.floor;
	else
		data->rect.color = 0;
	draw_rectangle(data, &data->rect);
}

void	draw_player(t_data *data, t_line *line)
{
	data->rect.x
		= ((data->minimap.size / 2) + 0.5) * data->minimap.tile_size - 3;
	data->rect.y
		= ((data->minimap.size / 2) + 0.5) * data->minimap.tile_size - 3;
	data->rect.width = 6;
	data->rect.height = 6;
	data->rect.color = PLAYER_COLOR;
	draw_rectangle(data, &data->rect);
	line->start_x = data->rect.x + 3;
	line->start_y = data->rect.y + 3;
	line->end_x = data->rect.x + 3 + cos(data->player_angle) * 10;
	line->end_y = data->rect.y + 3 + sin(data->player_angle) * 10;
	line->color = PLAYER_COLOR;
	draw_line(data, line);
}

void	draw_minimap(t_data *data)
{
	int			row;
	int			col;
	t_line		line;

	data->minimap.size = 7;
	data->minimap.tile_size = 25;
	data->minimap.offset_x
		= (data->player_x - floor(data->player_x)) * data->minimap.tile_size;
	data->minimap.offset_y
		= (data->player_y - floor(data->player_y)) * data->minimap.tile_size;
	row = -1;
	while (++row < data->minimap.size)
	{
		col = -1;
		while (++col < data->minimap.size)
			draw_tile(data, row, col);
	}
	draw_player(data, &line);
}

#include "cub3d.h"

void	draw_minimap(t_data *data)
{
	int			row;
	int			col;
	t_minimap	minimap;
	t_rectangle	rect;
	t_line		line;

	row = 0;
	minimap.size = 7;
	minimap.tile_size = 25;
	minimap.offset_x
		= (data->player_x - floor(data->player_x)) * minimap.tile_size;
	minimap.offset_y
		= (data->player_y - floor(data->player_y)) * minimap.tile_size;
	while (row < minimap.size)
	{
		col = 0;
		while (col < minimap.size)
		{
			minimap.map_row = floor(data->player_y) - (minimap.size / 2) + row;
			minimap.map_col = floor(data->player_x) - (minimap.size / 2) + col;
			if (minimap.map_row >= 0 && minimap.map_row < data->map.row
				&& minimap.map_col >= 0 && minimap.map_col < data->map.column)
				minimap.tile = data->map.map[minimap.map_row][minimap.map_col];
			else
				minimap.tile = ' ';
			rect.x = col * minimap.tile_size - minimap.offset_x;
			rect.y = row * minimap.tile_size - minimap.offset_y;
			rect.width = minimap.tile_size;
			rect.height = minimap.tile_size;
			if (minimap.tile == '1')
				rect.color = data->map.ceiling;
			else if (minimap.tile == 'D')
				rect.color = DOOR_COLOR;
			else if (minimap.tile == '0' || minimap.tile == 'N'
				|| minimap.tile == 'S' || minimap.tile == 'E'
				|| minimap.tile == 'W' || minimap.tile == 'd')
				rect.color = data->map.floor;
			else
				rect.color = 0;
			draw_rectangle(data, &rect);
			col++;
		}
		row++;
	}
	rect.x = ((minimap.size / 2) + 0.5) * minimap.tile_size - 3;
	rect.y = ((minimap.size / 2) + 0.5) * minimap.tile_size - 3;
	rect.width = 6;
	rect.height = 6;
	rect.color = PLAYER_COLOR;
	draw_rectangle(data, &rect);
	line.start_x = rect.x + 3;
	line.start_y = rect.y + 3;
	line.end_x = rect.x + 3 + cos(data->player_angle) * 10;
	line.end_y = rect.y + 3 + sin(data->player_angle) * 10;
	line.color = PLAYER_COLOR;
	draw_line(data, &line);
}

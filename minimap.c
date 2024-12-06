#include "cub3d.h"

void	draw_minimap(t_data *data)
{
	int			row;
	int			col;
	t_line		line;
	t_minimap	minimap;
	t_rectangle	rect;
	char		tile;

	minimap.map_rows = data->map.row;
	minimap.map_cols = data->map.column;
	if (WIDTH / 3 < HEIGHT / 3)
		minimap.tile_size = (WIDTH / 3) / minimap.map_cols;
	else
		minimap.tile_size = (HEIGHT / 3) / minimap.map_rows;
	minimap.player_x = (int)(data->player_x * minimap.tile_size)
		+ (minimap.tile_size / 2);
	minimap.player_y = (int)(data->player_y * minimap.tile_size)
		+ (minimap.tile_size / 2);
	row = 0;
	while (row < minimap.map_rows)
	{
		col = 0;
		while (col < minimap.map_cols)
		{
			tile = data->map.map[row][col];
			if (tile == '1' || tile == '0' || tile == 'N'
				|| tile == 'S' || tile == 'E' || tile == 'W')
			{
				rect.x = col * minimap.tile_size;
				rect.y = row * minimap.tile_size;
				rect.width = minimap.tile_size;
				rect.height = minimap.tile_size;
				if (tile == '1')
					rect.color = data->map.ceiling;
				else
					rect.color = data->map.floor;
				draw_rectangle(data, &rect);
				line.start_x = col * minimap.tile_size;
				line.start_y = row * minimap.tile_size;
				line.end_x = (col + 1) * minimap.tile_size;
				line.end_y = row * minimap.tile_size;
				line.color = GRID_COLOR;
				draw_line(data, &line);
				line.start_x = col * minimap.tile_size;
				line.start_y = (row + 1) * minimap.tile_size;
				line.end_x = (col + 1) * minimap.tile_size;
				line.end_y = (row + 1) * minimap.tile_size;
				draw_line(data, &line);
				line.start_x = col * minimap.tile_size;
				line.start_y = row * minimap.tile_size;
				line.end_x = col * minimap.tile_size;
				line.end_y = (row + 1) * minimap.tile_size;
				line.color = GRID_COLOR;
				draw_line(data, &line);
				line.start_x = (col + 1) * minimap.tile_size;
				line.start_y = row * minimap.tile_size;
				line.end_x = (col + 1) * minimap.tile_size;
				line.end_y = (row + 1) * minimap.tile_size;
				line.color = GRID_COLOR;
				draw_line(data, &line);
			}
			col++;
		}
		row++;
	}
	rect.x = minimap.player_x - 3;
	rect.y = minimap.player_y - 3;
	rect.width = 6;
	rect.height = 6;
	rect.color = PLAYER_COLOR;
	draw_rectangle(data, &rect);
	minimap.dir_x = minimap.player_x + cos(data->player_angle) * 10;
	minimap.dir_y = minimap.player_y + sin(data->player_angle) * 10;
	line.start_x = minimap.player_x;
	line.start_y = minimap.player_y;
	line.end_x = minimap.dir_x;
	line.end_y = minimap.dir_y;
	line.color = PLAYER_COLOR;
	draw_line(data, &line);
	cast_rays(data);
}

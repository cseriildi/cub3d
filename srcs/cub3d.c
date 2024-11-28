#include "../includes/cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_rectangle(t_data *data, int x, int y, int width, int height, int color)
{
	int	i;
	int	j;

	j = 0;
	while (j < height)
	{
		i = 0;
		while (i < width)
		{
			my_mlx_pixel_put(data, x + i, y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_line(t_data *data, int start_x, int start_y, int end_x, int end_y, int color)
{
	int	delta_x;
	int	delta_y;
	int	step_x;
	int	step_y;
	int	error2;
	int	error;

	delta_x = abs(end_x - start_x);
	delta_y = abs(end_y - start_y);
	if (start_x < end_x)
		step_x = 1;
	else
		step_x = -1;
	if (start_y < end_y)
		step_y = 1;
	else
		step_y = -1;
	error = delta_x - delta_y;
	while (1)
	{
		my_mlx_pixel_put(data, start_x, start_y, color);
		if (start_x == end_x && start_y == end_y)
			break ;
		error2 = error * 2;
		if (error2 > -delta_y)
		{
			error -= delta_y;
			start_x += step_x;
		}
		if (error2 < delta_x)
		{
			error += delta_x;
			start_y += step_y;
		}
	}
	my_mlx_pixel_put(data, end_x, end_y, color);
}

void	draw_minimap(t_data *data)
{
	int	map_rows = 5;
	int	map_cols = 8;
	int	tile_size = TILE_SIZE * MINIMAP_SCALE;
	int	row;
	int	col;
	int	color;
	int	x;
	int	y;
	int	player_x;
	int	player_y;
	int	dir_x;
	int	dir_y;

	row = 0;
	while (row < map_rows)
	{
		col = 0;
		while (col < map_cols)
		{
			if (data->map[row][col] == '1')
				color = WALL_COLOR;
			else
				color = FLOOR_COLOR;
			x = col * tile_size;
			y = row * tile_size;
			draw_rectangle(data, x, y, tile_size, tile_size, color);
			draw_line(data, x, y, x + tile_size, y, GRID_COLOR);
			draw_line(data, x, y, x, y + tile_size, GRID_COLOR);
			col++;
		}
		row++;
	}
	draw_line(data, 0, map_rows * tile_size, map_cols * tile_size, map_rows * tile_size, GRID_COLOR);
	draw_line(data, map_cols * tile_size, 0, map_cols * tile_size, map_rows * tile_size, GRID_COLOR);
	player_x = (int)(data->player_x * tile_size) + (tile_size / 2);
	player_y = (int)(data->player_y * tile_size) + (tile_size / 2);
	draw_rectangle(data, player_x - 3, player_y - 3, 6, 6, PLAYER_COLOR);
	dir_x = player_x + cos(data->player_angle) * tile_size;
	dir_y = player_y + sin(data->player_angle) * tile_size;
	draw_line(data, player_x, player_y, dir_x, dir_y, PLAYER_COLOR);
}

void	draw_vertical_line(t_data *data, int x, int wall_top, int wall_bottom, int wall_color)
{
	int	y;

	y = 0;
	while (y < wall_top)
	{
		my_mlx_pixel_put(data, x, y, CEIL_COLOR);
		y++;
	}
	y = wall_top;
	while (y <= wall_bottom)
	{
		my_mlx_pixel_put(data, x, y, wall_color);
		y++;
	}
	y = wall_bottom + 1;
	while (y < HEIGHT)
	{
		my_mlx_pixel_put(data, x, y, FLOOR_COLOR);
		y++;
	}
}

void	render_scene(t_data *data)
{
	int	x;
	int	wall_top;
	int	wall_bottom;

	x = 0;
	while (x < WIDTH)
	{
		wall_top = HEIGHT / 2;
		wall_bottom = HEIGHT / 2;
		draw_vertical_line(data, x, wall_top, wall_bottom, WALL_COLOR);
		x++;
	}
}

int	close_window(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	exit(0);
}

void	update_player_position(t_data *data, int keycode)
{
	if (keycode == KEY_W || keycode == ARROW_UP)
	{
		data->player_x += cos(data->player_angle) * MOVE_SPEED;
		data->player_y += sin(data->player_angle) * MOVE_SPEED;
	}
	else if (keycode == KEY_S || keycode == ARROW_DOWN)
	{
		data->player_x -= cos(data->player_angle) * MOVE_SPEED;
		data->player_y -= sin(data->player_angle) * MOVE_SPEED;
	}
	else if (keycode == KEY_A || keycode == ARROW_LEFT)
		data->player_angle -= TURN_SPEED;
	else if (keycode == KEY_D || keycode == ARROW_RIGHT)
		data->player_angle += TURN_SPEED;
	if (data->player_angle < 0)
		data->player_angle += 2 * M_PI;
	else if (data->player_angle >= 2 * M_PI)
		data->player_angle -= 2 * M_PI;
}

int key_hook(int keycode, t_data *data)
{
	if (keycode == KEY_ESC)
		close_window(data);
	mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
	{
		printf("Error: Failed to recreate image.\n");
		close_window(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len, &data->endian);
	update_player_position(data, keycode);
	render_scene(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}


void	initialize_map(t_data *data)
{
	int		i;
	int		j;
	char	temp_map[5][9] = {
		"11111111",
		"10000001",
		"111N0101",
		"10000101",
		"11111111"
	};

	i = 0;
	while (i < 5)
	{
		j = 0;
		while (j < 8)
		{
			data->map[i][j] = temp_map[i][j];
			if (temp_map[i][j] == 'N' || temp_map[i][j] == 'S' || temp_map[i][j] == 'E' || temp_map[i][j] == 'W')
			{
				data->player_x = j;
				data->player_y = i;
				if (temp_map[i][j] == 'N')
					data->player_angle = -M_PI / 2;
				else if (temp_map[i][j] == 'S')
					data->player_angle = M_PI / 2;
				else if (temp_map[i][j] == 'E')
					data->player_angle = 0;
				else if (temp_map[i][j] == 'W')
					data->player_angle = M_PI;
			}
			j++;
		}
		i++;
	}
}

int	main(void)
{
	t_data	data;

	data.mlx = mlx_init();
	if (!data.mlx)
	{
		printf("Error: MLX initialization failed.\n");
		return (1);
	}
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "cub3d");
	if (!data.win)
		return (1);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
		return (1);
	data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_len, &data.endian);
	initialize_map(&data);
	render_scene(&data);
	draw_minimap(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}

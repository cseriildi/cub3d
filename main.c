/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 17:10:03 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_map(t_map *map);

void	print_map(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->row)
	{
		j = 0;
		while (j < map->column)
		{
			printf("%c", map->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

int	close_window(t_data *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	safe_exit(&data->map, EXIT_SUCCESS);
	return (0);
}

static int	is_wall_at_position(t_data *data, double new_x, double new_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= data->map.column
		|| map_y < 0 || map_y >= data->map.row)
		return (1);
	if (data->map.map[map_y][map_x] == '1')
		return (1);
	return (0);
}

static void	move_player(t_data *data, double move_x, double move_y)
{
	double	new_player_x;
	double	new_player_y;

	new_player_x = data->player_x + move_x;
	new_player_y = data->player_y + move_y;
	if (!is_wall_at_position(data, new_player_x + 0.5, data->player_y + 0.5))
		data->player_x = new_player_x;
	if (!is_wall_at_position(data, data->player_x + 0.5, new_player_y + 0.5))
		data->player_y = new_player_y;
}

static void	turn_player(t_data *data, int keycode)
{
	if (keycode == ARROW_LEFT)
		data->player_angle -= TURN_SPEED;
	else if (keycode == ARROW_RIGHT)
		data->player_angle += TURN_SPEED;
	if (data->player_angle < 0)
		data->player_angle += 2 * M_PI;
	else if (data->player_angle >= 2 * M_PI)
		data->player_angle -= 2 * M_PI;
}

void	update_player_position(t_data *data, int keycode)
{
	double	move_x;
	double	move_y;
	double	strafe_x;
	double	strafe_y;

	move_x = cos(data->player_angle) * MOVE_SPEED;
	move_y = sin(data->player_angle) * MOVE_SPEED;
	strafe_x = cos(data->player_angle + M_PI_2) * MOVE_SPEED;
	strafe_y = sin(data->player_angle + M_PI_2) * MOVE_SPEED;

	if (keycode == KEY_W)
		move_player(data, move_x, move_y);
	else if (keycode == KEY_S)
		move_player(data, -move_x, -move_y);
	else if (keycode == KEY_A)
		move_player(data, -strafe_x, -strafe_y);
	else if (keycode == KEY_D)
		move_player(data, strafe_x, strafe_y);
	else
		turn_player(data, keycode);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_rectangle(t_data *data, t_rectangle *rect)
{
	int	i;
	int	j;

	j = 0;
	while (j < rect->height)
	{
		i = 0;
		while (i < rect->width)
		{
			my_mlx_pixel_put(data, rect->x + i, rect->y + j, rect->color);
			i++;
		}
		j++;
	}
}

static void	draw_line_with_params(t_data *data,
	t_line *line, t_line_params *params)
{
	while (1)
	{
		my_mlx_pixel_put(data, line->start_x, line->start_y, line->color);
		if (line->start_x == line->end_x && line->start_y == line->end_y)
			break ;
		params->error2 = params->error * 2;
		if (params->error2 > -params->delta_y)
		{
			params->error -= params->delta_y;
			line->start_x += params->step_x;
		}
		if (params->error2 < params->delta_x)
		{
			params->error += params->delta_x;
			line->start_y += params->step_y;
		}
	}
}

void	draw_line(t_data *data, t_line *line)
{
	t_line_params	params;

	params.delta_x = abs(line->end_x - line->start_x);
	params.delta_y = abs(line->end_y - line->start_y);
	if (line->start_x < line->end_x)
		params.step_x = 1;
	else
		params.step_x = -1;
	if (line->start_y < line->end_y)
		params.step_y = 1;
	else
		params.step_y = -1;
	params.error = params.delta_x - params.delta_y;
	draw_line_with_params(data, line, &params);
}

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
	minimap.player_x = (int)(data->player_x * minimap.tile_size) + (minimap.tile_size / 2);
	minimap.player_y = (int)(data->player_y * minimap.tile_size) + (minimap.tile_size / 2);
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
}

void	draw_vertical_line(t_data *data, int x, int wall_height, int wall_color)
{
	int		y;
	int		wall_top;
	int		wall_bottom;

	wall_top = (HEIGHT - wall_height) / 2;
	wall_bottom = wall_top + wall_height - 1;
	if (wall_top < 0)
		wall_top = 0;
	if (wall_bottom >= HEIGHT)
		wall_bottom = HEIGHT - 1;
	y = 0;
	while (y < wall_top)
		my_mlx_pixel_put(data, x, y++, data->map.ceiling);
	while (y <= wall_bottom)
		my_mlx_pixel_put(data, x, y++, wall_color);
	while (y < HEIGHT)
		my_mlx_pixel_put(data, x, y++, data->map.floor);
}

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	wall_height = 0;
	x = 0;
	while (x < WIDTH)
	{
		/*if (data->ray_distance[x] > 0)
		{
			wall_height = (TILE_SIZE / data->ray_distance[x]) * (HEIGHT / 55);
			draw_vertical_line(data, x, wall_height, data->ray_color[x]);
		}*/
		draw_vertical_line(data, x, wall_height, 0x000000);
		x++;
	}
}

int	key_hook(int keycode, t_data *data)
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
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	update_player_position(data, keycode);
	render_scene(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	data.map = (t_map){0};
	data.map.fd = -1;
	data.map.player[0] = -1;
	data.map.player[1] = -1;
	data.map.enemy[0] = -1;
	data.map.enemy[1] = -1;
	data.map.ceiling = -1;
	data.map.floor = -1;
	parsing(argc, argv, &data.map);
	printf("[%s], [%s], [%s], [%s], [%d], [%d], [%d], [%d], [%d], [%d]\n", data.map.north,
	data.map.south,
	data.map.west ,
	data.map.east ,
	data.map.floor,
	data.map.ceiling,
	data.map.row,
	data.map.column,
	data.map.player[0],
	data.map.player[1]);
	print_map(&data.map);
	data.player_x = data.map.player[1];
	data.player_y = data.map.player[0];
	data.player_angle = 0;
	data.mlx = mlx_init();
	if (!data.mlx)
	{
		printf("Error: MLX initialization failed.\n");
		safe_exit(&data.map, EXIT_FAILURE);
	}
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "cub3d");
	if (!data.win)
	{
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		safe_exit(&data.map, EXIT_FAILURE);
	}
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
	{
		mlx_destroy_window(data.mlx, data.win);
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		safe_exit(&data.map, EXIT_FAILURE);
	}
	data.addr = mlx_get_data_addr(data.img,
			&data.bpp, &data.line_len, &data.endian);
	render_scene(&data);
	draw_minimap(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
	return (0);
}

/*void	init_map(t_map *map)
{
	map->north = NULL;
	map->south = NULL;
	map->west = NULL;
	map->east = NULL;
	map->map = NULL;
	map->row = 0;
	map->column = 0;
	map->floor = 0;
	map->ceiling = 0;
	map->fd = -1;
	map->player[0] = -1;
	map->player[1] = -1;
	map->enemy[0] = -1;
	map->enemy[1] = -1;
	map->n_size[0] = 0;
	map->n_size[1] = 0;
	map->s_size[0] = 0;
	map->s_size[1] = 0;
	map->e_size[0] = 0;
	map->e_size[1] = 0;
	map->w_size[0] = 0;
	map->w_size[1] = 0;
}*/

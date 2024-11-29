#include "../includes/cub3d.h"

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
	while (1)
	{
		my_mlx_pixel_put(data, line->start_x, line->start_y, line->color);
		if (line->start_x == line->end_x && line->start_y == line->end_y)
			break ;
		params.error2 = params.error * 2;
		if (params.error2 > -params.delta_y)
		{
			params.error -= params.delta_y;
			line->start_x += params.step_x;
		}
		if (params.error2 < params.delta_x)
		{
			params.error += params.delta_x;
			line->start_y += params.step_y;
		}
	}
}

void	draw_vertical_line(t_data *data, int x, int wall_height, int wall_color)
{
	int	y;
	int	wall_top;
	int	wall_bottom;

	wall_top = (HEIGHT - wall_height) / 2;
	wall_bottom = wall_top + wall_height - 1;
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

static void	check_horizontal(t_data *data, double player_x,
	double player_y, t_ray *ray)
{
	t_ray_trace_state	ray_state;

	ray_state.direction = (ray->angle > 0 && ray->angle < M_PI);
	if (ray_state.direction)
	{
		ray_state.y_intercept = floor(player_y) + 1;
		ray_state.y_step = 1;
	}
	else
	{
		ray_state.y_intercept = floor(player_y);
		ray_state.y_step = -1;
	}
	ray_state.x_intercept
		= player_x + (ray_state.y_intercept - player_y) / tan(ray->angle);
	ray_state.x_step = ray_state.y_step / tan(ray->angle);
	ray_state.cur_x = ray_state.x_intercept;
	ray_state.cur_y = ray_state.y_intercept;
	while (ray_state.cur_x >= 0 && ray_state.cur_x < data->map_width
		&& ray_state.cur_y >= 0 && ray_state.cur_y < data->map_height)
	{
		ray_state.map_x = (int)ray_state.cur_x;
		if (ray_state.direction)
			ray_state.map_y = (int)ray_state.cur_y;
		else
			ray_state.map_y = (int)(ray_state.cur_y - 1);
		if (data->map[ray_state.map_y][ray_state.map_x] == '1')
		{
			ray_state.dist = sqrt(pow(ray_state.cur_x - player_x, 2)
					+ pow(ray_state.cur_y - player_y, 2));
			if (ray_state.dist < ray->closest_distance)
			{
				ray->closest_distance = ray_state.dist;
				ray->hit_x = ray_state.cur_x;
				ray->hit_y = ray_state.cur_y;
				ray->hit_color = HORIZONTAL_RAY_COLOR;
			}
			break ;
		}
		ray_state.cur_x += ray_state.x_step;
		ray_state.cur_y += ray_state.y_step;
	}
}

void	check_vertical(t_data *data, double player_x,
	double player_y, t_ray *ray)
{
	t_ray_trace_state	ray_state;

	ray_state.direction = (ray->angle < M_PI_2 || ray->angle > 3 * M_PI_2);
	if (ray_state.direction)
	{
		ray_state.x_intercept = floor(player_x) + 1;
		ray_state.x_step = 1;
	}
	else
	{
		ray_state.x_intercept = floor(player_x);
		ray_state.x_step = -1;
	}
	ray_state.y_intercept
		= player_y + (ray_state.x_intercept - player_x) * tan(ray->angle);
	ray_state.y_step = ray_state.x_step * tan(ray->angle);
	ray_state.cur_x = ray_state.x_intercept;
	ray_state.cur_y = ray_state.y_intercept;
	while (ray_state.cur_x >= 0 && ray_state.cur_x < data->map_width
		&& ray_state.cur_y >= 0 && ray_state.cur_y < data->map_height)
	{
		if (ray_state.direction)
			ray_state.map_x = (int)ray_state.cur_x;
		else
			ray_state.map_x = (int)(ray_state.cur_x - 1);
		ray_state.map_y = (int)ray_state.cur_y;
		if (data->map[ray_state.map_y][ray_state.map_x] == '1')
		{
			ray_state.dist = sqrt(pow(ray_state.cur_x - player_x, 2)
					+ pow(ray_state.cur_y - player_y, 2));
			if (ray_state.dist < ray->closest_distance)
			{
				ray->closest_distance = ray_state.dist;
				ray->hit_x = ray_state.cur_x;
				ray->hit_y = ray_state.cur_y;
				ray->hit_color = VERTICAL_RAY_COLOR;
			}
			break ;
		}
		ray_state.cur_x += ray_state.x_step;
		ray_state.cur_y += ray_state.y_step;
	}
}

void	cast_ray(t_data *data, double ray_angle, int ray_index)
{
	t_ray	ray;
	t_line	line;

	ray.angle = fmod(ray_angle, 2 * M_PI);
	if (ray.angle < 0)
		ray.angle += 2 * M_PI;
	ray.closest_distance = DBL_MAX;
	ray.hit_x = 0;
	ray.hit_y = 0;
	ray.hit_color = 0;
	check_horizontal(data, data->player_x + 0.5, data->player_y + 0.5, &ray);
	check_vertical(data, data->player_x + 0.5, data->player_y + 0.5, &ray);
	if (ray.closest_distance < DBL_MAX)
	{
		line.start_x = (data->player_x + 0.5) * TILE_SIZE * MINIMAP_SCALE;
		line.start_y = (data->player_y + 0.5) * TILE_SIZE * MINIMAP_SCALE;
		line.end_x = ray.hit_x * TILE_SIZE * MINIMAP_SCALE;
		line.end_y = ray.hit_y * TILE_SIZE * MINIMAP_SCALE;
		line.color = ray.hit_color;
		draw_line(data, &line);
		if (ray.closest_distance > 0.01)
		{
			data->ray_distance[ray_index]
				= ray.closest_distance * cos(ray.angle - data->player_angle);
			data->ray_color[ray_index] = ray.hit_color;
		}
		else
		{
			data->ray_distance[ray_index] = DBL_MAX;
			data->ray_color[ray_index] = 0;
		}
	}
	else
	{
		data->ray_distance[ray_index] = DBL_MAX;
		data->ray_color[ray_index] = 0;
	}
}

void	cast_rays(t_data *data)
{
	double	ray_angle;
	double	angle_increment;
	int		ray_index;

	ray_index = 0;
	angle_increment = FIELD_OF_VIEW / WIDTH;
	ray_angle = data->player_angle - FIELD_OF_VIEW / 2;
	while (ray_angle < data->player_angle + FIELD_OF_VIEW / 2)
	{
		cast_ray(data, ray_angle, ray_index);
		ray_angle += angle_increment;
		ray_index++;
	}
}

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	x = 0;
	while (x < WIDTH)
	{
		if (data->ray_distance[x] > 0)
		{
			wall_height = (TILE_SIZE / data->ray_distance[x]) * 10;
			if (wall_height > HEIGHT)
				wall_height = HEIGHT;
			draw_vertical_line(data, x, wall_height, data->ray_color[x]);
		}
		x++;
	}
}

void draw_minimap(t_data *data)
{
	int			row;
	int			col;
	t_line		line;
	t_minimap	minimap;
	t_rectangle	rect;

	minimap.map_rows = 5;
	minimap.map_cols = 8;
	minimap.tile_size = TILE_SIZE * MINIMAP_SCALE;
	minimap.player_x
		= (int)(data->player_x * minimap.tile_size) + (minimap.tile_size / 2);
	minimap.player_y
		= (int)(data->player_y * minimap.tile_size) + (minimap.tile_size / 2);
	row = 0;
	while (row < minimap.map_rows)
	{
		col = 0;
		while (col < minimap.map_cols)
		{
			if (data->map[row][col] == '1')
			{
				rect.x = col * minimap.tile_size;
				rect.y = row * minimap.tile_size;
				rect.width = minimap.tile_size;
				rect.height = minimap.tile_size;
				rect.color = WALL_COLOR;
				draw_rectangle(data, &rect);
			}
			else
			{
				rect.x = col * minimap.tile_size;
				rect.y = row * minimap.tile_size;
				rect.width = minimap.tile_size;
				rect.height = minimap.tile_size;
				rect.color = FLOOR_COLOR;
				draw_rectangle(data, &rect);
			}
			line.start_x = col * minimap.tile_size;
			line.start_y = row * minimap.tile_size;
			line.end_x = col * minimap.tile_size + minimap.tile_size;
			line.end_y = row * minimap.tile_size;
			line.color = GRID_COLOR;
			draw_line(data, &line);
			line.start_x = col * minimap.tile_size;
			line.start_y = row * minimap.tile_size;
			line.end_x = col * minimap.tile_size;
			line.end_y = row * minimap.tile_size + minimap.tile_size;
			draw_line(data, &line);
			col++;
		}
		row++;
	}
	line.start_x = 0;
	line.start_y = minimap.map_rows * minimap.tile_size;
	line.end_x = minimap.map_cols * minimap.tile_size;
	line.end_y = minimap.map_rows * minimap.tile_size;
	line.color = GRID_COLOR;
	draw_line(data, &line);
	line.start_x = minimap.map_cols * minimap.tile_size;
	line.start_y = 0;
	line.end_x = minimap.map_cols * minimap.tile_size;
	line.end_y = minimap.map_rows * minimap.tile_size;
	draw_line(data, &line);
	minimap.dir_x = minimap.player_x + cos(data->player_angle) * 10;
	minimap.dir_y = minimap.player_y + sin(data->player_angle) * 10;
	rect.x = minimap.player_x - 3;
	rect.y = minimap.player_y - 3;
	rect.width = 6;
	rect.height = 6;
	rect.color = PLAYER_COLOR;
	draw_rectangle(data, &rect);
	line.start_x = minimap.player_x;
	line.start_y = minimap.player_y;
	line.end_x = minimap.dir_x;
	line.end_y = minimap.dir_y;
	line.color = PLAYER_COLOR;
	draw_line(data, &line);
	cast_rays(data);
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
	cast_rays(data);
	render_scene(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

void	initialize_map(t_data *data)
{
	int		i;
	int		j;
	char	temp_map[MAP_HEIGHT][MAP_WIDTH] = {
		"11111111",
		"10000001",
		"111E0101",
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
			if (temp_map[i][j] == 'N' || temp_map[i][j] == 'S'
				|| temp_map[i][j] == 'E' || temp_map[i][j] == 'W')
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
	data->map_width = MAP_WIDTH;
	data->map_height = MAP_HEIGHT;
}

int	main(void)
{
	t_data	data;

	data = (t_data){0};
	data.mlx = mlx_init();
	if (!data.mlx)
	{
		printf("Error: MLX initialization failed.\n");
		return (1);
	}
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "cub3d");
	if (!data.win)
	{
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		return (1);
	}
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
	{
		mlx_destroy_window(data.mlx, data.win);
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		return (1);
	}
	data.addr = mlx_get_data_addr(data.img,
			&data.bpp, &data.line_len, &data.endian);
	initialize_map(&data);
	cast_rays(&data);
	render_scene(&data);
	draw_minimap(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}

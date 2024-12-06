/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/06 09:27:15 by dcsicsak         ###   ########.fr       */
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
	while (ray_state.cur_x >= 0 && ray_state.cur_x < data->map.column
		&& ray_state.cur_y >= 0 && ray_state.cur_y < data->map.row)
	{
		ray_state.map_x = (int)ray_state.cur_x;
		if (ray_state.direction)
			ray_state.map_y = (int)ray_state.cur_y;
		else
			ray_state.map_y = (int)(ray_state.cur_y - 1);
		if (ray_state.map_x < 0)
			ray_state.map_x = 0;
		else if (ray_state.map_x >= data->map.column)
			ray_state.map_x = data->map.column - 1;
		if (ray_state.map_y < 0)
			ray_state.map_y = 0;
		else if (ray_state.map_y >= data->map.row)
			ray_state.map_y = data->map.row - 1;
		if (data->map.map[ray_state.map_y][ray_state.map_x] == '1')
		{
			ray_state.dist = sqrt(pow(ray_state.cur_x - player_x, 2)
					+ pow(ray_state.cur_y - player_y, 2));
			if (ray_state.dist < ray->closest_distance)
			{
				ray->closest_distance = ray_state.dist;
				ray->hit_x = ray_state.cur_x;
				ray->hit_y = ray_state.cur_y;
				if (ray_state.direction)
				{
					data->ray_dir[ray->index] = SOUTH;
					data->texture_x[ray->index]
						= 1 - (ray->hit_x - floor(ray->hit_x));
				}
				else
				{
					data->ray_dir[ray->index] = NORTH;
					data->texture_x[ray->index]
						= ray->hit_x - floor(ray->hit_x);
				}
			}
			break ;
		}
		ray_state.cur_x += ray_state.x_step;
		ray_state.cur_y += ray_state.y_step;
	}
}

static void	check_vertical(t_data *data, double player_x,
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
	while (ray_state.cur_x >= 0 && ray_state.cur_x < data->map.column
		&& ray_state.cur_y >= 0 && ray_state.cur_y < data->map.row)
	{
		if (ray_state.direction)
			ray_state.map_x = (int)ray_state.cur_x;
		else
			ray_state.map_x = (int)(ray_state.cur_x - 1);
		ray_state.map_y = (int)ray_state.cur_y;
		if (ray_state.map_x < 0)
			ray_state.map_x = 0;
		else if (ray_state.map_x >= data->map.column)
			ray_state.map_x = data->map.column - 1;
		if (ray_state.map_y < 0)
			ray_state.map_y = 0;
		else if (ray_state.map_y >= data->map.row)
			ray_state.map_y = data->map.row - 1;
		if (data->map.map[ray_state.map_y][ray_state.map_x] == '1')
		{
			ray_state.dist = sqrt(pow(ray_state.cur_x - player_x, 2)
					+ pow(ray_state.cur_y - player_y, 2));
			if (ray_state.dist < ray->closest_distance)
			{
				ray->closest_distance = ray_state.dist;
				ray->hit_x = ray_state.cur_x;
				ray->hit_y = ray_state.cur_y;
				if (ray_state.direction)
				{
					data->ray_dir[ray->index] = EAST;
					data->texture_x[ray->index]
						= ray->hit_y - floor(ray->hit_y);
				}
				else
				{
					data->ray_dir[ray->index] = WEST;
					data->texture_x[ray->index]
						= 1 - (ray->hit_y - floor(ray->hit_y));
				}
			}
			break ;
		}
		ray_state.cur_x += ray_state.x_step;
		ray_state.cur_y += ray_state.y_step;
	}
}

static void	cast_ray(t_data *data, double ray_angle, int ray_index)
{
	t_ray	ray;
	t_line	line;
	int		tile_size;

	ray.index = ray_index;
	ray.angle = fmod(ray_angle, 2 * M_PI);
	if (ray.angle < 0)
		ray.angle += 2 * M_PI;
	ray.closest_distance = DBL_MAX;
	ray.hit_x = 0;
	ray.hit_y = 0;
	ray.hit_color = 0;
	if (WIDTH / 3 < HEIGHT / 3)
		tile_size = (WIDTH / 3) / data->map.column;
	else
		tile_size = (HEIGHT / 3) / data->map.row;
	check_horizontal(data, data->player_x + 0.5, data->player_y + 0.5, &ray);
	check_vertical(data, data->player_x + 0.5, data->player_y + 0.5, &ray);
	if (ray.closest_distance < DBL_MAX)
	{
		line.start_x = (data->player_x + 0.5) * tile_size;
		line.start_y = (data->player_y + 0.5) * tile_size;
		line.end_x = ray.hit_x * tile_size;
		line.end_y = ray.hit_y * tile_size;
		line.color = ray.hit_color;
		draw_line(data, &line);
		if (ray.closest_distance > 0.01)
		{
			data->ray_distance[ray_index]
				= ray.closest_distance * cos(ray.angle - data->player_angle);
		}
		else
		{
			data->ray_distance[ray_index] = DBL_MAX;
		}
	}
	else
	{
		data->ray_distance[ray_index] = DBL_MAX;
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

void	draw_vertical_line(t_data *data, int x, int wall_height)
{
	int			y;
	int			wall_top;
	int			wall_bottom;
	int			texture_y;
	double		step;
	double		texture_pos;
	t_texture	*texture;

	if (data->ray_dir[x] < 0 || data->ray_dir[x] >= 4)
		data->ray_dir[x] = NORTH;
	texture = &data->textures[data->ray_dir[x]];
	wall_top = (HEIGHT - wall_height) / 2;
	wall_bottom = wall_top + wall_height - 1;
	if (wall_top < 0)
		wall_top = 0;
	if (wall_bottom >= HEIGHT)
		wall_bottom = HEIGHT - 1;
	printf("wall_top: %d, wall_bottom: %d\n", wall_top, wall_bottom);
	step = (double)texture->height / wall_height;
	texture_pos = (wall_top < 0) ? -wall_top * step : 0;
	y = 0;
	while (y < wall_top)
		my_mlx_pixel_put(data, x, y++, data->map.ceiling);
	while (y <= wall_bottom)
	{
		texture_y = (int)texture_pos % texture->height;
		my_mlx_pixel_put(data, x, y++,
			texture->data[texture_y * texture->width
			+ ((int)(data->texture_x[x] * texture->width) % texture->width)]);
		texture_pos += step;
	}
	while (y < HEIGHT)
		my_mlx_pixel_put(data, x, y++, data->map.floor);
	printf("y: %d\n", y);
}

void	load_texture(t_data *data, t_texture *texture, char *file)
{
	texture->img = mlx_xpm_file_to_image(data->mlx, file,
			&texture->width, &texture->height);
	if (!texture->img)
	{
		ft_putstr_fd("Error: Failed to load texture\n", 2);
		exit(EXIT_FAILURE);
	}
	texture->data = (int *)mlx_get_data_addr(texture->img,
			&data->bpp, &data->line_len, &data->endian);
	if (!texture->data)
	{
		ft_putstr_fd("Error: Failed to retrieve texture data\n", 2);
		exit(EXIT_FAILURE);
	}
}


void	load_all_textures(t_data *data)
{
	load_texture(data, &data->textures[NORTH], data->map.north);
	load_texture(data, &data->textures[EAST], data->map.east);
	load_texture(data, &data->textures[SOUTH], data->map.south);
	load_texture(data, &data->textures[WEST], data->map.west);
}


void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	wall_height = 0;
	x = 0;
	while (x < WIDTH)
	{
		if (data->ray_distance[x] > 0)
		{
			printf("ray_distance[%d]: %f\n", x, data->ray_distance[x]);
			wall_height = (TILE_SIZE / data->ray_distance[x]) * (HEIGHT / 55);
			draw_vertical_line(data, x, wall_height);
		}
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
	cast_rays(data);
	render_scene(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

void	init_data(t_data *data)
{

	int	i;

	i = 0;
	while (i < WIDTH)
	{
		data->ray_dir[i] = -1;
		i++;
	}
	data->textures[0] = (t_texture){0};
	data->textures[1] = (t_texture){0};
	data->textures[2] = (t_texture){0};
	data->textures[3] = (t_texture){0};
	data->map = (t_map){0};
	data->map.fd = -1;
	data->map.player[0] = -1;
	data->map.player[1] = -1;
	data->map.enemy[0] = -1;
	data->map.enemy[1] = -1;
	data->map.ceiling = -1;
	data->map.floor = -1;
}


int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
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
	if (data.map.map[data.map.player[0]][data.map.player[1]] == 'N')
		data.player_angle = 3 * M_PI / 2;
	else if (data.map.map[data.map.player[0]][data.map.player[1]] == 'S')
		data.player_angle = M_PI / 2;
	else if (data.map.map[data.map.player[0]][data.map.player[1]] == 'E')
		data.player_angle = 0;
	else if (data.map.map[data.map.player[0]][data.map.player[1]] == 'W')
		data.player_angle = M_PI;
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
	cast_rays(&data);
	load_all_textures(&data);
	render_scene(&data);
	draw_minimap(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
	return (0);
}

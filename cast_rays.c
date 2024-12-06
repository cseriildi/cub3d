#include "cub3d.h"

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

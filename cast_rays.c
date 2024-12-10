/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:31:42 by icseri            #+#    #+#             */
/*   Updated: 2024/12/10 10:15:08 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_tilesize(t_ray *ray, double ray_angle,
	int ray_index, t_data *data)
{
	ray->index = ray_index;
	ray->angle = fmod(ray_angle, 2 * M_PI);
	if (ray->angle < 0)
		ray->angle += 2 * M_PI;
	ray->closest_distance = DBL_MAX;
	ray->hit_x = 0;
	ray->hit_y = 0;
	ray->hit_color = 0;
	if (WIDTH / 3 < HEIGHT / 3)
		return ((WIDTH / 3) / data->map.column);
	else
		return ((HEIGHT / 3) / data->map.row);
}

static void	cast_ray(t_data *data, double ray_angle, int ray_index)
{
	t_ray	ray;
	t_line	line;
	int		tile_size;

	tile_size = get_tilesize(&ray, ray_angle, ray_index, data);
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
			data->ray_distance[ray_index]
				= ray.closest_distance * cos(ray.angle - data->player_angle);
		else
			data->ray_distance[ray_index] = DBL_MAX;
	}
	else
		data->ray_distance[ray_index] = DBL_MAX;
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

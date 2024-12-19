/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:35 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 10:56:25 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>
# include <mlx.h>
# include <stdlib.h>
# include <math.h>
# include <sys/time.h>

# define WIDTH 800
# define HEIGHT 600
# define TILE_SIZE 64
# define MOVE_SPEED 0.2
# define TURN_SPEED 0.1
# define FIELD_OF_VIEW 1.047198
# define DBL_MAX 1.7976931348623157E+308
# define MOUSE_SENSITIVITY 0.002
# define NUM_FRAMES 100
# define FRAME_DURATION 0.1

# define PLAYER_COLOR 0xFF0000 // Red
# define GRID_COLOR 0xFF0000 // Red
# define DOOR_COLOR 0xA52A2A // Brown

# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_E 101
# define ARROW_LEFT 65361
# define ARROW_RIGHT 65363

typedef enum e_dir
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	DOOR
}	t_dir;

typedef enum e_error
{
	MALLOC = 2,
	MAP,
	TEXTURE,
	COLOR,
	WORNG_FILE
}	t_erorr;

typedef struct s_sprite
{
	char	**textures;
	int		**sizes;
	int		count;
	t_list	*texture_list;
}	t_sprite;

typedef struct s_data	t_data;

typedef struct s_map
{
	t_sprite	north;
	t_sprite	east;
	t_sprite	south;
	t_sprite	west;
	t_sprite	door;
	int			floor;
	int			ceiling;
	char		**map;
	int			row;
	int			column;
	int			player[2];
	int			fd;
	bool		is_bonus;
	t_data		*data;
}	t_map;

typedef struct s_texture
{
	void	*img;
	int		*data;
	int		width;
	int		height;
	double	position;
	t_data	*data_struct;
}	t_texture;

typedef struct s_line
{
	int	start_x;
	int	start_y;
	int	end_x;
	int	end_y;
	int	color;
}	t_line;

typedef struct s_line_params
{
	int	delta_x;
	int	delta_y;
	int	step_x;
	int	step_y;
	int	error;
	int	error2;
}	t_line_params;

typedef struct s_ray
{
	double	angle;
	double	closest_distance;
	double	hit_x;
	double	hit_y;
	int		hit_color;
	int		index;
}	t_ray;

typedef struct s_ray_trace_state
{
	int		direction;
	double	y_intercept;
	double	x_intercept;
	double	y_step;
	double	x_step;
	double	cur_x;
	double	cur_y;
	int		map_x;
	int		map_y;
	double	dist;
}	t_ray_trace_state;

typedef struct s_rectangle
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	color;
}	t_rectangle;

typedef struct s_minimap
{
	int		map_row;
	int		map_col;
	int		size;
	int		tile_size;
	int		player_x;
	int		player_y;
	int		dir_x;
	int		dir_y;
	char	tile;
	double	offset_x;
	double	offset_y;
}	t_minimap;

typedef struct s_proximity
{
	double	radius;
	double	dx;
	double	dy;
	double	check_x;
	double	check_y;
	int		adj_x;
	int		adj_y;
}	t_proximity;

typedef struct s_data
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			frame;
	double		last_frame;
	t_map		map;
	double		player_x;
	double		player_y;
	double		player_angle;
	double		ray_distance[WIDTH];
	double		texture_x[WIDTH];
	int			ray_dir[WIDTH];
	void		*texture_img;
	int			*texture_data;
	int			frame_count[5];
	t_texture	**textures;
	t_minimap	minimap;
	t_rectangle	rect;
	char		*line;
}	t_data;

//init
void	load_texture(t_data *data, t_texture *texture, char *file);
void	load_all_textures(t_data *data);
void	init_data(t_data *data);
void	init_mlx(t_data *data);
void	set_player(t_data *data);
void	set_counts(t_data *data);

//parsing
void	parsing(int argc, char **argv, t_map *map);
void	get_texture(t_sprite *sprite, t_map *map);
void	get_color(int *color, t_map *map);
void	get_map(t_map *map);
void	check_map(t_map *map);
void	list_to_arr(t_list **map_list, char ***arr, t_map *map);
void	check_textures(t_map *map);
int		safe_open(char	*filename, bool is_cub, t_map *map);
void	allocate_textures(t_data *data);

//mlx & mlx_monus
void	render_scene(t_data *data);
int		key_hook(int keycode, t_data *data);
double	get_time(void);

//mlx utils
int		close_window(t_data *data);
int		track_mouse(void *param);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

//cleanup
void	print_error(int count, ...);
void	safe_exit(t_map *map, int exit_code);
void	free_array(char ***arr);

//raycasting
void	cast_rays(t_data *data);
void	check_horizontal(t_data *data, double x, double y, t_ray *ray);
void	check_vertical(t_data *data, double x, double y, t_ray *ray);

//minimap
void	draw_minimap(t_data *data);

//draw_utils
void	draw_rectangle(t_data *data, t_rectangle *rect);
void	draw_line(t_data *data, t_line *line);
void	draw_vertical_line(t_data *data, int x, int wall_height);

//movement
void	move_player(t_data *data, double move_x, double move_y);

//movement utils
int		is_wall(t_data *data, double check_x, double check_y);
int		check_wall_in_radius(t_data *data, double new_x, double new_y,
			double radius);
int		is_wall_at_position(t_data *data, double new_x, double new_y);
void	turn_player(t_data *data, int keycode);
void	update_player_position(t_data *data, int keycode);

//movement bonus
void	check_and_open_door_nearby(t_data *data, double new_x, double new_y);

void	create_and_add(t_list **map_list, t_map *map);
void	free_textures(t_data *data);

#endif

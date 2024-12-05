#ifndef CUB3D_H
# define CUB3D_H

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define WIDTH 800
#define HEIGHT 600
#define TILE_SIZE 64
#define MINIMAP_SCALE 0.5
#define FIELD_OF_VIEW M_PI / 3
#define MOVE_SPEED 0.2
#define TURN_SPEED 0.1

#define MAP_WIDTH 8
#define MAP_HEIGHT 5

#define WALL_COLOR 0x444444 // Dark gray
#define PLAYER_COLOR 0xFF0000 // Red
#define GRID_COLOR 0x000000 // Black
#define FLOOR_COLOR 0x8B4513 // Brown
#define CEIL_COLOR 0x87CEEB // Light blue
#define SOUTH_RAY_COLOR 0xFF0000 // Red
#define EAST_RAY_COLOR 0x0000FF // Blue
#define NORTH_RAY_COLOR 0xFFFF00 // Yellow
#define WEST_RAY_COLOR 0xFF00FF // Magenta
#define TEXTURE_SIZE 8

# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define ARROW_UP 65362
# define ARROW_DOWN 65364
# define ARROW_LEFT 65361
# define ARROW_RIGHT 65363

typedef struct s_map
{
	int		map_width;
	int		map_height;
	int		floor_color;
	int		ceil_color;
}	t_map;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	char	map[MAP_HEIGHT][MAP_WIDTH];
	int		map_width;
	int		map_height;
	double	player_x;
	double	player_y;
	double	player_angle;
	double	ray_distance[WIDTH];
	int		ray_color[WIDTH];
	double	texture_x[WIDTH];
}	t_data;

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

typedef struct s_rectangle
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	color;
}	t_rectangle;

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

typedef struct s_minimap
{
	int	map_rows;
	int	map_cols;
	int	tile_size;
	int	player_x;
	int	player_y;
	int	dir_x;
	int	dir_y;
}	t_minimap;

void	cast_rays(t_data *data);
void	render_scene(t_data *data);
int		close_window(t_data *data);

//draw_utils.c
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void	draw_rectangle(t_data *data, t_rectangle *rect);
void	draw_line(t_data *data, t_line *line);
void	draw_vertical_line(t_data *data, int x, int wall_height, int wall_color);

//minimap.c
void	draw_minimap(t_data *data);

//movement.c
int		key_hook(int keycode, t_data *data);

//raycaster.c
void	cast_rays(t_data *data);




int	parse(int argc, char **argv);

#endif

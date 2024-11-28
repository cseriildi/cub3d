#ifndef CUB3D_H
# define CUB3D_H

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define TILE_SIZE 64
#define MINIMAP_SCALE 0.5
#define MAP_WIDTH 9
#define MAP_HEIGHT 5

#define WALL_COLOR 0x444444 // Dark gray
#define PLAYER_COLOR 0xFF0000 // Red
#define GRID_COLOR 0x000000 // Black
#define FLOOR_COLOR 0x8B4513 // Brown
#define CEIL_COLOR 0x87CEEB // Light blue

#define MOVE_SPEED 0.2
#define TURN_SPEED 0.1

# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define ARROW_UP 65362
# define ARROW_DOWN 65364
# define ARROW_LEFT 65361
# define ARROW_RIGHT 65363

typedef struct s_data
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	char	map[5][9];
	double	player_x;
	double	player_y;
	double	player_angle;
}	t_data;

#endif

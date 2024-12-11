/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:51:15 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 18:05:01 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_array(char ***arr)
{
	int	i;

	i = 0;
	if (arr && *arr)
	{
		while ((*arr)[i])
			ft_free(&(*arr)[i++]);
		free(*arr);
		*arr = NULL;
	}
}
/* 
void	free_texture(t_sprite *texture)
{
	int	i;

	free_array(&texture->textures);
	i = 0;
	if (texture)
	{
		while (i < texture->count)
			free(texture->sizes[i++]);
		free(*texture->sizes);
		*texture->sizes = NULL;
	}
	texture = NULL;
} */

void	free_list(t_list **list)
{
	t_list	*current;
	t_list	*next;

	if (list && *list)
	{
		current = *list;
		while (current != NULL)
		{
			next = current->next;
			ft_free((char **)&current->content);
			free(current);
			current = next;
		}
		*list = NULL;
	}
	free(list);
}

void	print_error(int count, ...)
{
	va_list	args;
	char	*str;

	va_start(args, count);
	while (count--)
	{
		str = va_arg(args, char *);
		if (str)
			ft_putstr_fd(str, STDERR_FILENO);
	}
	va_end(args);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	errors(int exit_code)
{
	if (exit_code == MALLOC)
		print_error(1, "Error\nMalloc fail");
	else if (exit_code == COLOR)
		print_error(1, "Error\nInvalid color");
	else if (exit_code == TEXTURE)
		print_error(1, "Error\nInvalid texture");
	else if (exit_code == MAP)
		print_error(1, "Error\nInvalid map");
}

void	safe_exit(t_map *map, int exit_code)
{
	free_array(&map->map);
	/* free_texture(&map->north);
	free_texture(&map->east);
	free_texture(&map->west);
	free_texture(&map->south);
	free_texture(&map->door);
	mlx_destroy_img
	*/
	if (map->fd != -1)
	{
		close(map->fd);
		map->fd = -1;
	}
	get_next_line(-1);
	errors(exit_code);
	exit(exit_code);
}

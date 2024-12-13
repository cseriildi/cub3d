/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:27:07 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 13:35:51 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	remove_node(t_gnl_list **list, t_gnl_list *node)
{
	t_gnl_list	*prev;
	t_gnl_list	*curr;

	prev = NULL;
	curr = *list;
	while (curr)
	{
		if (curr == node)
		{
			if (prev)
				prev->next = curr->next;
			else
				*list = curr->next;
			ft_free(&curr->content);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

static t_gnl_list	*get_node(t_gnl_list **list, int fd)
{
	t_gnl_list	*node;

	node = *list;
	while (node && node->fd != fd)
		node = node->next;
	if (node == NULL)
	{
		node = malloc(sizeof(t_gnl_list));
		if (node == NULL)
			return (NULL);
		node->fd = fd;
		node->content = NULL;
		node->next = *list;
		*list = node;
	}
	return (node);
}

static void	ft_listclear(t_gnl_list **lst, void (*del)(void *))
{
	t_gnl_list	*current;

	if (lst && del)
	{
		while (*lst)
		{
			current = (*lst)->next;
			del((*lst)->content);
			*lst = current;
		}
	}
	*lst = NULL;
}

static char	*read_fd(int fd, char *read_chars)
{
	char	*tmp;
	char	*buffer;
	long	read_size;

	buffer = malloc(BUFFER_SIZE + 1);
	if (buffer == NULL)
		return (ft_free(&read_chars), NULL);
	read_size = 1;
	while (read_size > 0 && is_nl(read_chars) == 0)
	{
		read_size = read(fd, buffer, BUFFER_SIZE);
		if (read_size == -1)
			return (ft_free(&buffer), ft_free(&read_chars), NULL);
		buffer[read_size] = '\0';
		tmp = ft_strjoin(read_chars, buffer);
		ft_free(&read_chars);
		read_chars = tmp;
		if (!read_chars)
			return (ft_free(&buffer), ft_free(&read_chars), NULL);
	}
	ft_free(&buffer);
	if (!read_chars || !read_chars[0])
		return (ft_free(&read_chars), NULL);
	return (read_chars);
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*list;
	t_gnl_list			*read_chars;
	char				*line;
	char				*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_listclear(&list, &free), NULL);
	read_chars = get_node(&list, fd);
	if (!read_chars)
		return (NULL);
	read_chars->content = read_fd(fd, read_chars->content);
	if (!read_chars->content)
		return (remove_node(&list, read_chars), NULL);
	line = line_search(read_chars->content);
	if (!line || !*line)
		return (ft_free(&line), remove_node(&list, read_chars), NULL);
	tmp = ft_strjoin(NULL, read_chars->content + ft_strlen(line));
	ft_free(&read_chars->content);
	read_chars->content = tmp;
	if (!read_chars->content)
		return (ft_free(&line), remove_node(&list, read_chars), NULL);
	return (line);
}

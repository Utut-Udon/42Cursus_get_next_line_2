/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fujitaharuki <fujitaharuki@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 10:23:56 by fujitaharuk       #+#    #+#             */
/*   Updated: 2024/06/07 16:31:51 by fujitaharuk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

char	*ft_strdup(const char *str)
{
	char	*dest;
	size_t	i;

	dest = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dest)
		return (NULL);
	i = -1;
	while (str[++i])
		dest[i] = str[i];
	dest[i] = '\0';
	return (dest);
}

char	*read_line(int fd, char *buf, char *backup)
{
	int		rd_size;
	char	*tmp;

	while (1)
	{
		rd_size = read(fd, buf, BUFFER_SIZE);
		if (rd_size == 0)
			break ;
		if (!backup)
			backup = ft_strdup("");
		if (rd_size == -1)
		{
			free(backup);
			return (NULL);
		}
		buf[rd_size] = '\0';
		tmp = backup;
		backup = ft_strjoin(tmp, buf);
		free(tmp);
		if (!backup)
			return (NULL);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (backup);
}

int	get_idx(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\0' || line[i + 1] == '\0')
		return (-1);
	return (i);
}

static char	*get_backup(char *line, int i)
{
	char	*str;

	str = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!str)
		return (NULL);
	line[i + 1] = '\0';
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*backup[OPEN_MAX];
	char		*line;
	char		*buf;
	int			i;

	if (BUFFER_SIZE <= 0 || fd < 0 || OPEN_MAX < fd)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	line = read_line(fd, buf, backup[fd]);
	free(buf);
	if (!line)
		return (NULL);
	i = get_idx(line);
	if (i == -1)
		backup[fd] = NULL;
	else
	{
		backup[fd] = get_backup(line, i);
		if (!backup[fd])
			return (NULL);
	}
	return (line);
}

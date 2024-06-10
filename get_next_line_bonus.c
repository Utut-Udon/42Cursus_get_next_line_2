/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fujitaharuki <fujitaharuki@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 03:06:55 by fujitaharuk       #+#    #+#             */
/*   Updated: 2024/06/11 03:08:08 by fujitaharuk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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

char	*read_line(int fd, char *buf, char **backup)
{
	int		rd_size;
	char	*tmp;

	rd_size = read(fd, buf, BUFFER_SIZE);
	if (rd_size == -1)
		return (NULL);
	if (rd_size == 0 && !*backup)
		return (NULL);
	while (rd_size > 0)
	{
		if (!*backup)
			*backup = ft_strdup("");
		buf[rd_size] = '\0';
		tmp = *backup;
		*backup = ft_strjoin(tmp, buf);
		free(tmp);
		if (!*backup)
			return (NULL);
		if (ft_strchr(buf, '\n'))
			break ;
		rd_size = read(fd, buf, BUFFER_SIZE);
	}
	return (ft_strdup(*backup));
}

int	get_idx(char *line, char **backup)
{
	size_t	i;
	char	*new_backup;

	i = 0;
	if (!line)
		return (-1);
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\0' || line[i + 1] == '\0')
		return (-1);
	new_backup = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!new_backup)
	{
		free(line);
		return (-1);
	}
	line[i + 1] = '\0';
	free(*backup);
	*backup = new_backup;
	return (0);
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
	line = read_line(fd, buf, &backup[fd]);
	free(buf);
	i = get_idx(line, &backup[fd]);
	if (i == -1)
	{
		if (backup[fd])
		{
			free(backup[fd]);
			backup[fd] = NULL;
		}
	}
	return (line);
}

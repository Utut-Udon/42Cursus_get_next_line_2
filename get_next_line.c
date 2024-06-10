/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fujitaharuki <fujitaharuki@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 01:35:56 by fujitaharuk       #+#    #+#             */
/*   Updated: 2024/06/11 03:16:19 by fujitaharuk      ###   ########.fr       */
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

int	get_backup(char *line, char **backup)
{
	size_t	i;
	char	*new_backup;

	i = 0;
	if (!line)
		return (1);
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\0' || line[i + 1] == '\0')
		return (1);
	new_backup = ft_substr(line, i + 1, ft_strlen(line) - i);
	free(*backup);
	if (!new_backup)
	{
		free(line);
		return (1);
	}
	line[i + 1] = '\0';
	*backup = new_backup;
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*backup;
	char		*line;
	char		*buf;

	if (BUFFER_SIZE <= 0 || fd < 0 || OPEN_MAX < fd)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	line = read_line(fd, buf, &backup);
	free(buf);
	if (get_backup(line, &backup))
	{
		if (backup)
		{
			free(backup);
			backup = NULL;
		}
	}
	return (line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brendos- <brendos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:45:32 by brendos-          #+#    #+#             */
/*   Updated: 2025/09/14 11:21:04 by brendos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_append_and_free(char *remainder, char *buffer)
{
	char	*tmp;

	if (!remainder)
		remainder = ft_strdup("");
	tmp = remainder;
	remainder = ft_strjoin(tmp, buffer);
	free(tmp);
	tmp = NULL;
	return (remainder);
}

static char	*ft_fill_buffer(int fd, char *remainder, char *buffer)
{
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		remainder = ft_append_and_free(remainder, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remainder);
}

static char	*ft_extract_line(char *line_buffer)
{
	ssize_t	i;
	char	*remainder;

	i = 0;
	while (line_buffer[i] && line_buffer[i] != '\n')
		i++;
	if (!line_buffer[i])
		return (NULL);
	remainder = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - i);
	if (*remainder == '\0')
	{
		free(remainder);
		remainder = NULL;
	}
	line_buffer[i + 1] = '\0';
	return (remainder);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*remainder;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	line = ft_fill_buffer(fd, remainder, buffer);
	free(buffer);
	buffer = NULL;
	if (!line)
	{
		if (remainder)
		{
			free(remainder);
			remainder = NULL;
		}
		return (NULL);
	}
	remainder = ft_extract_line(line);
	return (line);
}

/* int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Erro ao abrir o arquivo");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
} */

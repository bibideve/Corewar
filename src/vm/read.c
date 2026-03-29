/*
** read.c for tetris in src/vm/read.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Wed Mar 22 22:17:43 2017 Bilel Fourati
** Last update Wed Mar 22 22:17:43 2017 Bilel Fourati
*/

#include <stdlib.h>
#include <unistd.h>
#include "vm.h"

static bool	read_bytes(int fd, void *buffer, size_t size)
{
  ssize_t	read_size;
  size_t	total;

  total = 0;
  while (total < size)
  {
    read_size = read(fd, (char *)buffer + total, size - total);
    if (read_size <= 0)
      return (false);
    total += read_size;
  }
  return (true);
}

t_header	*read_header(int fd)
{
  t_header	*h;

  if ((h = malloc(sizeof(t_header))) == NULL)
    return (NULL);
  if (!read_bytes(fd, h, sizeof(t_header)))
  {
    free(h);
    return (NULL);
  }
  h->magic = swap_be(h->magic);
  h->prog_size = swap_be(h->prog_size);
  return (h);
}

char	*read_string(int fd, int length)
{
  char	*buf;

  if ((buf = malloc(length)) == NULL)
    return (NULL);
  if (!read_bytes(fd, buf, length))
  {
    free(buf);
    return (NULL);
  }
  return (buf);
}

int	read_int(int fd)
{
  int	mag;

  if (!read_bytes(fd, &mag, sizeof(int)))
    return (FAIL);
  return (mag);
}

char	read_char(int fd)
{
  char	mag;

  if (!read_bytes(fd, &mag, sizeof(char)))
    return (FAIL);
  return (mag);
}

/*
** loli.c for corewar in /home/zirk/Epitech/CPE_2016_corewar/src/asm
** 
** Made by Zirk
** Login   <christian.chaux@epitech.eu>
** 
** Started on  Thu Mar 30 23:13:15 2017 Zirk
** Last update Fri Mar 31 01:01:57 2017 Zirk
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "loli.h"
#include "my.h"

void	display_loli(void)
{
  int	fd;
  char	*line;

  fd = open("../loli/001.loli", O_RDONLY);
  if (fd < 0)
    my_putstr("Loli not found... What a sad time to be alive...\n");
  while ((line = my_gnl(fd)) != NULL)
    {
      my_putstr(line);
      my_putchar('\n');
    }
}

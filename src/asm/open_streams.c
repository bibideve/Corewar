/*
** open_streams.c for corewar in /home/zirk/Epitech/CPE_2016_corewar/src/asm
** 
** Made by Zirk
** Login   <zirk@epitech.net>
** 
** Started on  Wed Mar 15 11:13:44 2017 Zirk
** Last update Sun Apr  2 22:01:22 2017 Zirk
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "asm.h"
#include "my.h"

bool	open_streams(char *inputpath, char *outputpath, t_io *io)
{
  if (outputpath == NULL)
  {
    asm_error(MALLOC_FAIL);
    return (false);
  }
  io->input_fd = open(inputpath, O_RDONLY);
  if (io->input_fd < 0)
  {
    asm_error(INPUTFILE_OTHER);
    return (false);
  }
  io->input_name = my_strdup(inputpath);
  io->output_name = my_strdup(outputpath);
  if (io->input_name == NULL || io->output_name == NULL)
  {
    asm_error(MALLOC_FAIL);
    return (false);
  }
  return (true);
}

/*
** write_bytes.c for asm in src/asm/write_bytes.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Sun Apr  2 19:35:16 2017 Maxime Louet
** Last update Sun Apr  2 19:35:16 2017 Maxime Louet
*/

#include "asm.h"
#include "my.h"

void	write_reg(char *arg, t_io *io, t_options *options)
{
  int	regnumber;
  char	final;

  regnumber = my_atoi(&arg[1]);
  final = (char)regnumber;
  if (options->flags[DEBUG]->enabled)
    my_printf("\x1B[33m[DEBUG]\x1B[0m Writing register [%d]\n", final);
  if (!write_bytes(io->output_fd, &final, 1))
    options->error_encountered = true;
}

void	write_direct(char *arg, t_io *io, t_options *options)
{
  int	final;

  final = my_atoi(&arg[1]);
  if (options->flags[DEBUG]->enabled)
    my_printf("\x1B[33m[DEBUG]\x1B[0m Writing direct [%d]\n", final);
  final = swap_be_i(final);
  if (!write_bytes(io->output_fd, &final, DIR_SIZE))
    options->error_encountered = true;
}

void	write_indirect(char *arg, t_io *io, t_options *options)
{
  short int	final;

  if (arg[0] == DIRECT_CHAR)
    final = my_atoi(&(arg[1]));
  else
    final = my_atoi(arg);
  if (options->flags[DEBUG]->enabled)
    my_printf("\x1B[33m[DEBUG]\x1B[0m Writing indirect [%d]\n", final);
  final = swap_be_s(final);
  if (!write_bytes(io->output_fd, &final, IND_SIZE))
    options->error_encountered = true;
}

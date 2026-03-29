/*
** write.c for asm in src/asm/write.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Thu Mar 30 19:32:05 2017 Maxime Louet
** Last update Thu Mar 30 19:32:05 2017 Maxime Louet
*/

#include <stddef.h>
#include "asm.h"
#include "my.h"

static void	handle_parameters_description(t_line *line, int iarg,
					      char *dassaut)
{
  if (line->arguments[iarg].type == T_REG)
  {
    *dassaut <<= 1;
    *dassaut |= 1;
  }
  else if (line->arguments[iarg].type == T_DIR)
  {
    *dassaut |= 1;
    *dassaut <<= 1;
  }
  else
  {
    *dassaut |= 1;
    *dassaut <<= 1;
    *dassaut |= 1;
  }
}

static void	write_parameters_description(t_line *line, t_options *options,
					     t_io *io)
{
  int		i;
  char		dassaut;

  i = 0;
  dassaut = 0;
  while (i < line->nb_args)
  {
    handle_parameters_description(line, i, &dassaut);
    dassaut <<= 1;
    i++;
  }
  while (i++ < MAX_ARGS_NUMBER)
    dassaut <<= 1;
  if (line->instruction_code == 16)
    dassaut <<= 2;
  if (line->instruction_code == 2 ||
      line->instruction_code == 3)
    dassaut <<= 1;
  if (options->flags[DEBUG]->enabled)
    my_printf("\x1B[33m[DEBUG]\x1B[0m Writing parameters description [%d]\n",
	dassaut);
  write_bytes(io->output_fd, &dassaut, 1);
}

static void	write_parameters(t_line *line, t_io *io, t_options *options)
{
  int		i;

  i = 0;
  while (i < line->nb_args)
  {
    if (line->arguments[i].type == T_REG)
    {
      write_reg(line->arguments[i].arg, io, options);
    }
    else if (line->arguments[i].type == T_DIR &&
	     !argument_is_index(line->instruction_code, i))
    {
      write_direct(line->arguments[i].arg, io, options);
    }
    else
    {
      write_indirect(line->arguments[i].arg, io, options);
    }
    i++;
  }
}

static bool	write_line(t_line *line, t_options *options, t_io *io)
{
  if (line->instruction_code == 0)
    return (true);
  if (options->flags[DEBUG]->enabled)
    my_printf("\x1B[33m[DEBUG]\x1B[0m Writing instruction code [%d]\n",
	      line->instruction_code);
  if (!write_bytes(io->output_fd, &(line->instruction_code), 1))
    return (false);
  if (has_parameters_description(line->instruction_code))
    write_parameters_description(line, options, io);
  write_parameters(line, io, options);
  return (true);
}

bool		write_final_output(t_list *lines, t_header *header,
				   t_options *options, t_io *io)
{
  t_list	*tmp;

  if (!write_header(header, options, io))
    return (false);
  tmp = lines;
  while (tmp != NULL && tmp->next != NULL)
  {
    if (((t_line *)(tmp->data))->instruction_code > 0)
    {
      if (!write_line(tmp->data, options, io))
	return (false);
    }
    tmp = tmp->next;
  }
  if (tmp != NULL && ((t_line *)(tmp->data))->instruction_code > 0)
  {
    if (!write_line(tmp->data, options, io))
      return (false);
  }
  if (tmp != NULL &&
      !write_header_output(header, options, io,
			   ((t_line *)(tmp->data))->mempos
			   + ((t_line *)(tmp->data))->memsize))
    return (false);
  return (true);
}

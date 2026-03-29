/*
** clean.c for asm in src/asm/clean.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Wed Mar 22 22:32:10 2017 Maxime Louet
** Last update Thu Mar 23 22:56:42 2017 Zirk
*/

#include <stdlib.h>
#include <unistd.h>
#include "asm.h"

void	free_line_content(t_line_content *line_content)
{
  int	i;

  if (line_content == NULL)
    return ;
  free(line_content->line);
  if (line_content->instruction != NULL && line_content->instruction != line_content->label)
    free(line_content->instruction);
  free(line_content->label);
  i = 0;
  while (line_content->arguments != NULL && i < line_content->nb_args)
  {
    free(line_content->arguments[i].arg);
    i++;
  }
  free(line_content->arguments);
  free(line_content);
}

void	free_lines(t_list *lines)
{
  t_list	*next;

  while (lines != NULL)
  {
    next = lines->next;
    free_line_content(lines->data);
    free(lines);
    lines = next;
  }
}

void	asm_clean(t_options *options, t_io *io)
{
  int	i;

  i = 0;
  while (options != NULL && i < NB_FLAGS)
  {
    if (options->flags[i] != NULL)
    {
      free(options->flags[i]->name);
      free(options->flags[i]);
    }
    free(options->flags_description[i]);
    i++;
  }
  free(options);
  if (io != NULL)
  {
    if (io->input_fd >= 0)
      close(io->input_fd);
    if (io->output_fd >= 0)
      close(io->output_fd);
    free(io->input_name);
    free(io->output_name);
    free(io);
  }
}

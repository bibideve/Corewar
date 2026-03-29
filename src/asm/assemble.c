/*
** assemble.c for asm in src/asm/assemble.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Tue Mar 14 22:38:05 2017 Maxime Louet
** Last update Sun Apr  2 22:04:33 2017 Zirk
*/

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "asm.h"
#include "my.h"
#include "mylist.h"
#include "op.h"
#include "parse_string.h"
#include "loli.h"

static t_line_content	*ini_line_content(void)
{
  t_line_content	*line_content;

  if ((line_content = my_calloc(1, sizeof(*line_content))) == NULL)
    asm_error(MALLOC_FAIL);
  return (line_content);
}

static bool	cleanup_assemble(t_list *lines, t_header *header,
				 t_line_content *line_content)
{
  if (line_content != NULL)
    free_line_content(line_content);
  free_lines(lines);
  free(header);
  return (false);
}

static bool	finish_assemble(t_list *lines, t_header *header,
				t_options *options, t_io *io)
{
  if (options->error_encountered)
  {
    my_dprintf(2, "Compilation terminated due to errors.\n");
    return (cleanup_assemble(lines, header, NULL));
  }
  if (!resolve_check_labels(lines, options))
    return (cleanup_assemble(lines, header, NULL));
  io->output_fd = open(io->output_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (io->output_fd < 0)
  {
    asm_error(OUTPUTFILE_OTHER);
    return (cleanup_assemble(lines, header, NULL));
  }
  if (!write_final_output(lines, header, options, io))
    return (cleanup_assemble(lines, header, NULL));
  if (options->flags[LOLI]->enabled)
    display_loli();
  free_lines(lines);
  return (true);
}

static size_t	get_lastpos_size(t_list *lines)
{
  t_line	*line;

  while (lines != NULL && lines->next != NULL)
    lines = lines->next;
  line = lines->data;
  return (line->memsize + line->mempos);
}

static t_list	*do_stuff(t_list *lines, t_line_content *line_content)
{
  if (lines != NULL)
    line_content->mempos = get_lastpos_size(lines);
  if (line_content->instruction != NULL || line_content->label != NULL)
    lines = add_list(lines, line_content);
  else
    free_line_content(line_content);
  return (lines);
}

bool			assemble(t_options *options, t_io *io)
{
  char			*current_line;
  t_header		*header;
  t_line_content	*line_content;
  t_list		*lines;

  lines = NULL;
  if (!prepare_header(&header))
    return (false);
  while ((current_line = my_gnl(io->input_fd)) != NULL)
  {
    if ((line_content = ini_line_content()) == NULL)
      return (cleanup_assemble(lines, header, NULL));
    line_content->line = current_line;
    if (!assemble_line(options, io, header, line_content))
      return (cleanup_assemble(lines, header, line_content));
    if (options->error_encountered && options->flags[WFATAL_ERRORS]->enabled)
    {
      my_dprintf(2, "Compilation terminated due to %s.\n",
	  options->flags[WFATAL_ERRORS]->name);
      return (cleanup_assemble(lines, header, line_content));
    }
    lines = do_stuff(lines, line_content);
  }
  return (finish_assemble(lines, header, options, io));
}

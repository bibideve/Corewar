/*
** labels.c for asm in src/asm/labels.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Thu Mar 30 19:56:47 2017 Maxime Louet
** Last update Thu Mar 30 19:56:47 2017 Maxime Louet
*/

#include <stdlib.h>
#include "asm.h"
#include "my.h"

static void	free_label_list(t_list *labels)
{
  t_list	*tmp;

  while (labels != NULL)
  {
    tmp = labels->next;
    free(((t_label *)(labels->data))->name);
    free(labels->data);
    free(labels);
    labels = tmp;
  }
}

static bool	resolve_label(t_list *labels, char *name, size_t *resolvedpos)
{
  while (labels != NULL)
  {
    if (my_strcmp(((t_label *)(labels->data))->name, name) == 0)
    {
      *resolvedpos = ((t_label *)(labels->data))->mempos;
      return (true);
    }
    labels = labels->next;
  }
  return (false);
}

static bool	compute_distance_to_label(t_list *labels, t_line *line,
					  int iarg, short int *distance)
{
  size_t	resolvedpos;
  char		*label;

  if (line->arguments[iarg].arg[0] == DIRECT_CHAR)
    label = &(line->arguments[iarg].arg[2]);
  else
    label = &(line->arguments[iarg].arg[1]);
  if (!resolve_label(labels, label, &resolvedpos))
    return (false);
  if (resolvedpos == line->mempos)
    *distance = resolvedpos;
  else
    *distance = resolvedpos - line->mempos;
  return (true);
}

static bool	handle_label(t_list *labels, t_line *line, int iarg,
			     t_options *options)
{
  short int	distance;

  if (!compute_distance_to_label(labels, line, iarg, &distance))
    return (false);
  if (options->flags[DEBUG]->enabled)
    my_printf("\x1B[33m[DEBUG]\x1B[0m Replacing label %s with %d\n",
	      line->arguments[iarg].arg,
	      distance);
  if (line->arguments[iarg].arg[0] == DIRECT_CHAR)
    line->arguments[iarg].arg = my_concat("%", my_itoa(distance));
  else
    line->arguments[iarg].arg = my_itoa(distance);
  return (true);
}

static bool	resolve_labels(t_list *labels, t_list *lines,
			       t_options *options)
{
  t_line	*line;
  int		i;

  line = NULL;
  while (lines != NULL)
  {
    line = lines->data;
    i = 0;
    while (i < line->nb_args)
    {
      if (line->arguments[i].is_label)
      {
	if (!handle_label(labels, line, i, options))
	  return (false);
      }
      i++;
    }
    lines = lines->next;
  }
  return (true);
}

bool		resolve_check_labels(t_list *lines, t_options *options)
{
  t_list	*labels;

  labels = NULL;
  if (!store_labels(&labels, lines, options))
    return (false);
  if (!resolve_labels(labels, lines, options))
  {
    free_label_list(labels);
    return (false);
  }
  free_label_list(labels);
  return (true);
}

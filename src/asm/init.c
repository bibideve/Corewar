/*
** init.c for asm in src/asm/init.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Wed Mar 15 14:16:22 2017 Maxime Louet
** Last update Fri Mar 31 10:52:20 2017 Zirk
*/

#include <stdlib.h>
#include "asm.h"
#include "my.h"

static t_options	*malloc_options(void)
{
  t_options		*options;
  int			i;

  i = 0;
  if ((options = malloc(sizeof(*options))) == NULL)
  {
    asm_error(MALLOC_FAIL);
    return (NULL);
  }
  options->pos_inputfile = 0;
  options->pos_outputfile = 0;
  options->error_encountered = false;
  while (i < NB_FLAGS)
  {
    options->flags[i] = malloc(sizeof(t_stdpair));
    if (options->flags[i] == NULL)
    {
      asm_error(MALLOC_FAIL);
      return (NULL);
    }
    i++;
  }
  return (options);
}

static bool	fill_flags(t_options *options)
{
  int		i;
  const char		*flags[NB_FLAGS] = {
    "-Wunused-label", "-Winvalid-jump", "-Woverflow", "-loli", "-debug",
    "-Wall", "-Wfatal-errors", "-h", "--help", "-Werror"
  };

  i = 0;
  while (i < NB_FLAGS)
  {
    options->flags[i]->enabled = false;
    if ((options->flags[i]->name = my_strdup(flags[i])) == NULL)
    {
      asm_error(MALLOC_FAIL);
      return (false);
    }
    i++;
  }
  return (true);
}

static bool	fill_descriptions(t_options *options)
{
  int		i;
  const char		*description[NB_FLAGS] = {
    "Warn about an unused label.", "Warn about a jump that doesn't have an"
    " associated label.", "Warn when an overflow occured because of "
    "an instruction", "", "\tDisplay more informations about the compilation",
    "\tEnable -Wunusued-label, -Winvalid-jump and -Woverflow",
    "Stop the compilation when an error occured", "\tDisplay this help",
    "\tDisplay this help", "\tConvert all warnings to errors"
  };

  i = 0;
  while (i < NB_FLAGS)
  {
    if ((options->flags_description[i] = my_strdup(description[i])) == NULL)
    {
      asm_error(MALLOC_FAIL);
      return (false);
    }
    i++;
  }
  return (true);
}

static t_options	*init_options(void)
{
  t_options		*options;

  if ((options = malloc_options()) == NULL)
    return (NULL);
  if (!fill_flags(options))
    return (NULL);
  if (!fill_descriptions(options))
    return (NULL);
  return (options);
}

bool	init_structs(t_options **options, t_io **io)
{
  if ((*io = malloc(sizeof(t_io))) == NULL)
  {
    asm_error(MALLOC_FAIL);
    return (false);
  }
  (*io)->input_name = NULL;
  (*io)->output_name = NULL;
  (*io)->input_fd = -1;
  (*io)->output_fd = -1;
  (*io)->line_counter = 0;
  if ((*options = init_options()) == NULL)
  {
    free(*io);
    *io = NULL;
    return (false);
  }
  return (true);
}

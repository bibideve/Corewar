/*
** main.c for asm in src/asm/main.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Tue Mar 14 16:32:04 2017 Maxime Louet
** Last update Sun Apr  2 15:24:26 2017 Zirk
*/

#include <stdlib.h>
#include "asm.h"
#include "my.h"

static void	add_cor(char *str, int y)
{
  str[y] = '.';
  str[y + 1] = 'c';
  str[y + 2] = 'o';
  str[y + 3] = 'r';
  str[y + 4] = '\0';
}

static int	get_i(const char *output_name)
{
  int		i;

  i = my_strlen(output_name) - 1;
  while (i >= 0 && output_name[i] != '.')
    i--;
  if (i < 0)
    i = my_strlen(output_name) - 1;
  return (i);
}

static char	*get_output_name(char **av, int pos, int pos_s)
{
  int		i;
  int		y;
  char		*output;
  const char	*output_name = my_basename(av[pos_s]);

  if (pos == 0)
    {
      i = get_i(output_name);
      output = malloc(sizeof(char) * (i + 5));
      if (output == NULL)
	return (NULL);
      y = 0;
      while (y < i)
	{
	  output[y] = output_name[y];
	  y++;
	}
      add_cor(output, y);
      return (output);
    }
  else
    return (my_strdup(av[pos]));
}

int		main(int ac, char **av)
{
  t_options	*options;
  t_io		*io;
  char		*output_name;

  if (!init_structs(&options, &io))
    return (84);
  if (!parse_args(options, ac, av))
  {
    asm_clean(options, io);
    return (84);
  }
  output_name = get_output_name(av, options->pos_outputfile,
				options->pos_inputfile);
  if (!open_streams(av[options->pos_inputfile],
		    output_name, io))
  {
    free(output_name);
    asm_clean(options, io);
    return (84);
  }
  if (!assemble(options, io))
  {
    asm_clean(options, io);
    return (84);
  }
  asm_clean(options, io);
  return (0);
}

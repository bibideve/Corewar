/*
** flags_champs.c for corewar in src/vm/flags/flags_champs.c
**
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
**
** Started on  Sun Apr  2 20:09:31 2017 Bilel Fourati
** Last update Sun Apr  2 20:09:31 2017 Bilel Fourati
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "op.h"
#include "vm.h"
#include "my.h"

static t_fork	*init_pc(t_fork *f, t_champ *c)
{
  int		i;

  i = 0;
  if ((f = malloc(sizeof(t_fork))) == NULL)
    return (NULL);
  f->pc = c->add_start;
  f->pos = c->add_start;
  f->next = NULL;
  f->prev = NULL;
  f->cycle_before_ins = 0;
  f->pending_opcode = 0;
  f->carry = false;
  f->live_called = false;
  while (i < REG_NUMBER)
  {
    f->reg[i] = 0;
    i++;
  }
  return (f);
}

static int	is_file_champ(const char *file)
{
  int		i;
  int		j;

  i = my_strlen(file) - 1;
  j = my_strlen(FILE_EXT) - 1;
  while (i >= 0 && j >= 0)
  {
    if (file[i] != FILE_EXT[j])
      return (FAIL);
    i--;
    j--;
  }
  return (SUCCESS);
}

t_champ		*fill_champ(const char *file, t_champ *c, t_machine *mach,
    int *err)
{
  int		fd;
  char		buf[1];
  int		i;
  int		rt;

  i = 0;
  rt = 0;
  if ((fd = open(file, O_RDONLY)) == -1)
    *err = -1;
  if (*err != -1)
  {
    c->head = read_header(fd);
    if (c->head == NULL || c->head->magic != COREWAR_EXEC_MAGIC)
    {
      *err = -1;
      close(fd);
      return (c);
    }
    while ((rt = read(fd, buf, 1)) > 0)
    {
      mach->mem[wrap_pos(c->add_start + i)] = buf[0];
      i++;
    }
    c->fork = NULL;
    if ((c->fork = init_pc(c->fork, c)) == NULL || rt == -1)
      *err = -1;
  }
  close(fd);
  return (c);
}

static int	count_champ_file(char **av)
{
  int		i;
  int		c;

  i = 0;
  c = 0;
  while (av[i])
  {
    if (is_file_champ(av[i]) == SUCCESS)
      c++;
    i++;
  }
  return (c);
}

t_champ	**analyse_flags_champ(char **av, t_champ **c, t_machine *mach)
{
  int	i;
  bool	match;
  int	count;

  i = 1;
  count = count_champ_file(av);
  if ((count < 2 || count > 4)
      || ((c = malloc(sizeof(t_champ*) * (count + 1))) == NULL))
    return (NULL);
  c[count] = NULL;
  while (av[i])
  {
    match = false;
    if ((pars_flags(av, &i, &match) == FAIL)
	|| (dump_case_corflag(av, &i, &match) == FAIL)
	|| (!match && is_file_champ(av[i]) == FAIL))
      return (NULL);
    if (is_file_champ(av[i]) == SUCCESS)
      if (process_flag(av[i], c, mach, count) == FAIL)
	return (NULL);
    i++;
  }
  return (c);
}

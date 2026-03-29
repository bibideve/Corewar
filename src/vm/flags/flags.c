/*
** flags.c for asm in src/vm/flags/flags.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Sun Apr  2 23:41:52 2017 Maxime Louet
** Last update Sun Apr  2 23:41:52 2017 Maxime Louet
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "op.h"
#include "vm.h"
#include "my.h"

static t_flags	g_flags[NB_FLAGS] =
{
  {0, "-n", NULL},
  {0, "-a", NULL}
};

static bool	id_already_used(t_champ **c, int current, int id)
{
  int	i;

  i = 0;
  while (i < current)
  {
    if (c[i] != NULL && c[i]->id == id)
      return (true);
    i++;
  }
  return (false);
}

int	process_flag_verif(int i, t_champ **c)
{
  if (g_flags[N_FLAG].act != 0 && !my_str_isdigit(g_flags[N_FLAG].arg))
    return (FAIL);
  if (g_flags[A_FLAG].act != 0 && (!my_str_isdigit(g_flags[A_FLAG].arg)))
    return (FAIL);
  if (g_flags[N_FLAG].act > 0)
  {
    c[i]->id = my_atoi(g_flags[N_FLAG].arg);
    if (id_already_used(c, i, c[i]->id))
      return (FAIL);
  }
  if (g_flags[A_FLAG].act > 0)
    c[i]->add_start = my_atoi(g_flags[A_FLAG].arg) % MEM_SIZE;
  return (SUCCESS);
}

int		process_flag(const char *file, t_champ **c, t_machine *mach,
			     int n)
{
  static int	i;
  int		err;

  err = 0;
  if ((c[i] = my_calloc(sizeof(t_champ), 1)) == NULL)
    return (FAIL);
  c[i]->add_start = (MEM_SIZE / n * i) % MEM_SIZE;
  if (g_flags[N_FLAG].act != 0 || g_flags[A_FLAG].act != 0)
    if (process_flag_verif(i, c) == FAIL)
      return (FAIL);
  c[i] = fill_champ(file, c[i], mach, &err);
  if (err == -1 || c[i] == NULL || c[i]->fork == NULL)
    return (FAIL);
  if (g_flags[N_FLAG].act == 0)
  {
    c[i]->id = i + 1;
    if (id_already_used(c, i, c[i]->id))
      return (FAIL);
  }
  c[i]->fork->id = 1;
  c[i]->fork->reg[0] = c[i]->id;
  c[i]->fork->cycle_before_ins = 0;
  c[i]->alive = false;
  g_flags[N_FLAG].act = 0;
  g_flags[A_FLAG].act = 0;
  g_flags[N_FLAG].arg = NULL;
  g_flags[A_FLAG].arg = NULL;
  i++;
  return (SUCCESS);
}

int	pars_flags(char **av, int *i, bool *match)
{
  int	j;

  j = 0;
  while (j < NB_FLAGS)
  {
    if (my_strcmp(g_flags[j].id, av[*i]) == 0)
    {
      if (g_flags[j].act >= 1)
	return (FAIL);
      g_flags[j].act++;
      (*i)++;
      if (av[*i] == NULL)
	return (FAIL);
      g_flags[j].arg = av[*i];
      *match = true;
    }
    j++;
  }
  return (SUCCESS);
}

int	dump_case_corflag(char **av, int *i, bool *match)
{
  if (my_strcmp("-dump", av[*i]) == 0)
  {
    *match = true;
    (*i)++;
    if (av[*i] == NULL)
      return (FAIL);
  }
  return (SUCCESS);
}

t_champ		**analyse_flags(char **av, t_machine *mach)
{
  t_champ	**champs;

  champs = NULL;
  if (dump_f(av, mach) == FAIL)
    return (NULL);
  if ((champs = analyse_flags_champ(av, champs, mach)) == NULL)
    return (NULL);
  return (champs);
}

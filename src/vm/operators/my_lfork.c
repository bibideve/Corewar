/*
** my_lfork.c for corewar in src/vm/operators/my_lfork.c
**
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
**
** Started on  Sun Apr  2 01:00:13 2017 Bilel Fourati
** Last update Sun Apr  2 01:00:13 2017 Bilel Fourati
*/

#include <stdlib.h>
#include "operators.h"

extern t_op	g_op_tab[];

static t_fork	*append_fork(t_fork *fork)
{
  while (fork->next != NULL)
    fork = fork->next;
  return (fork);
}

void		my_lfork(t_machine *machine, t_champ *champ, t_fork *f,
			 int *reg)
{
  t_fork	*f_m;
  t_fork	*p;
  int		i;

  (void)champ;
  (void)reg;
  f->cycle_before_ins = g_op_tab[14].nbr_cycles;
  if ((f_m = malloc(sizeof(t_fork))) == NULL)
    return ;
  p = append_fork(f);
  p->next = f_m;
  f_m->next = NULL;
  f_m->prev = p;
  f_m->cycle_before_ins = 0;
  f_m->carry = f->carry;
  f_m->id = p->id + 1;
  i = 0;
  while (i < REG_NUMBER)
  {
    f_m->reg[i] = f->reg[i];
    i++;
  }
  f_m->pc = wrap_pos(f->pos + get_indirect(machine->mem, f->pos + 1));
  f_m->pos = f_m->pc;
}

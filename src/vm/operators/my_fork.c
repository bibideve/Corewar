/*
** my_fork.c for  in src/vm/operators/my_fork.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Sun Apr  2 00:59:02 2017 Bilel Fourati
** Last update Sun Apr  2 00:59:02 2017 Bilel Fourati
*/

#include <stdlib.h>
#include "operators.h"

extern t_op	g_op_tab[];

void		my_fork(t_machine *machine, t_champ *champ,
			t_fork *f, int *reg)
{
  t_fork	*f_m;
  t_fork	*p;
  int		i;

  (void)champ;
  (void)reg;
  f->cycle_before_ins = g_op_tab[11].nbr_cycles;
  i = 0;
  if ((f_m = malloc(sizeof(t_fork))) == NULL)
    return ;
  p = f;
  while (p->next != NULL)
    p = p->next;
  p->next = f_m;
  f_m->next = NULL;
  f_m->prev = p;
  f_m->cycle_before_ins = 0;
  f_m->pending_opcode = 0;
  f_m->carry = f->carry;
  f_m->live_called = f->live_called;
  f_m->id = p->id + 1;
  while (i < REG_NUMBER)
  {
    f_m->reg[i] = f->reg[i];
    i++;
  }
  f_m->pc = wrap_pos(f->pos + (get_indirect(machine->mem,
	      f->pos + 1) % IDX_MOD));
  f_m->pos = f_m->pc;
  return ;
}

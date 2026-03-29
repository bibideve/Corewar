/*
** sub.c for corewar in src/vm/operators/sub.c
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Fri Mar 31 14:12:17 2017 Robin Houssais
** Last update Fri Mar 31 14:12:17 2017 Robin Houssais
*/

#include "vm.h"
#include "op.h"
#include "operators.h"

extern t_op	g_op_tab[];

void	sub(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	r1;
  unsigned char	r2;
  unsigned char	r3;

  (void)machine;
  (void)champ;
  f->cycle_before_ins = g_op_tab[4].nbr_cycles;
  r1 = machine->mem[wrap_pos(f->pos + 2)];
  r2 = machine->mem[wrap_pos(f->pos + 3)];
  r3 = machine->mem[wrap_pos(f->pos + 4)];
  if (!is_valid_reg(r1) || !is_valid_reg(r2) || !is_valid_reg(r3))
    return ;
  reg[reg_index(r3)] = reg[reg_index(r1)] - reg[reg_index(r2)];
  f->carry = (reg[reg_index(r3)] == 0);
}

/*
** my_or.c for corewar in src/vm/operators/my_or.c
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Fri Mar 31 14:17:04 2017 Robin Houssais
** Last update Fri Mar 31 14:17:04 2017 Robin Houssais
*/

#include "vm.h"
#include "operators.h"

extern t_op	g_op_tab[];

void	my_or(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	reg_a;
  unsigned char	reg_b;
  unsigned char	reg_c;

  (void)machine;
  (void)champ;
  f->cycle_before_ins = g_op_tab[6].nbr_cycles;
  reg_a = machine->mem[wrap_pos(f->pos + 2)];
  reg_b = machine->mem[wrap_pos(f->pos + 3)];
  reg_c = machine->mem[wrap_pos(f->pos + 4)];
  if (!is_valid_reg(reg_a) || !is_valid_reg(reg_b) || !is_valid_reg(reg_c))
    return ;
  reg[reg_index(reg_c)] = reg[reg_index(reg_a)] | reg[reg_index(reg_b)];
  f->carry = (reg[reg_index(reg_c)] == 0);
}

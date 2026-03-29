/*
** my_xor.c for corewar in src/vm/operators/my_xor.c
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Fri Mar 31 14:17:36 2017 Robin Houssais
** Last update Fri Mar 31 14:17:36 2017 Robin Houssais
*/

#include "vm.h"
#include "operators.h"

extern t_op	g_op_tab[];

void	my_xor(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	dst;
  unsigned char	src1;
  unsigned char	src2;

  (void)machine;
  (void)champ;
  f->cycle_before_ins = g_op_tab[7].nbr_cycles;
  src1 = machine->mem[wrap_pos(f->pos + 2)];
  src2 = machine->mem[wrap_pos(f->pos + 3)];
  dst = machine->mem[wrap_pos(f->pos + 4)];
  if (!is_valid_reg(src1) || !is_valid_reg(src2) || !is_valid_reg(dst))
    return ;
  reg[reg_index(dst)] = reg[reg_index(src1)] ^ reg[reg_index(src2)];
  f->carry = (reg[reg_index(dst)] == 0);
}

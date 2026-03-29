/*
** lld.c for corewar in src/vm/operators/lld.c
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Fri Mar 31 14:10:08 2017 Robin Houssais
** Last update Fri Mar 31 14:10:08 2017 Robin Houssais
*/

#include "vm.h"
#include "op.h"
#include "operators.h"

extern t_op	g_op_tab[];

void	lld(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  int		arg_type;
  int		value;
  int		offset;
  unsigned char	reg_value;

  (void)champ;
  f->cycle_before_ins = g_op_tab[12].nbr_cycles;
  arg_type = get_cb_type(machine->mem[wrap_pos(f->pos + 1)], FIRST_ARG);
  if (arg_type != IND_TYPE && arg_type != DIR_TYPE)
    return ;
  offset = f->pos + 2;
  if (!read_arg_value(machine, f, reg, 0x0d, FIRST_ARG, arg_type,
		      offset, false, &value))
    return ;
  offset += get_arg_size(0x0d, arg_type, FIRST_ARG);
  reg_value = machine->mem[wrap_pos(offset)];
  if (!is_valid_reg(reg_value))
    return ;
  reg[reg_index(reg_value)] = value;
  f->carry = (value == 0);
}

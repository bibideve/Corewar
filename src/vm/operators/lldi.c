/*
** lldi.c for  in src/vm/operators/lldi.c
**
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
**
** Started on  Fri Mar 31 14:08:19 2017 Robin Houssais
** Last update Fri Mar 31 14:08:19 2017 Robin Houssais
*/

#include "vm.h"
#include "operators.h"

extern t_op	g_op_tab[];

void	lldi(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	cb;
  int		first_type;
  int		second_type;
  int		offset;
  int		first_value;
  int		second_value;
  unsigned char	dest_reg;

  (void)champ;
  f->cycle_before_ins = g_op_tab[13].nbr_cycles;
  cb = machine->mem[wrap_pos(f->pos + 1)];
  first_type = get_cb_type(cb, FIRST_ARG);
  second_type = get_cb_type(cb, SECOND_ARG);
  if ((first_type != REG_TYPE && first_type != DIR_TYPE && first_type != IND_TYPE)
      || (second_type != REG_TYPE && second_type != DIR_TYPE))
    return ;
  offset = f->pos + 2;
  if (!read_arg_value(machine, f, reg, 0x0e, 1, first_type, offset,
		      false, &first_value))
    return ;
  offset += get_arg_size(0x0e, first_type, 1);
  if (!read_arg_value(machine, f, reg, 0x0e, 2, second_type, offset,
		      false, &second_value))
    return ;
  offset += get_arg_size(0x0e, second_type, 2);
  dest_reg = machine->mem[wrap_pos(offset)];
  if (!is_valid_reg(dest_reg))
    return ;
  reg[reg_index(dest_reg)] = get_direct(machine->mem,
					wrap_pos(f->pos + first_value
						 + second_value));
  f->carry = (reg[reg_index(dest_reg)] == 0);
}

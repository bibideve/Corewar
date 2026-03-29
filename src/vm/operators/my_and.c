/*
** my_and.c for corewar in src/vm/operators/my_and.c
**
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
**
** Started on  Fri Mar 31 14:16:41 2017 Robin Houssais
** Last update Fri Mar 31 14:16:41 2017 Robin Houssais
*/

#include "vm.h"
#include "operators.h"

extern t_op	g_op_tab[];

void	my_and(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	cb;
  int		first_type;
  int		second_type;
  int		offset;
  int		first_value;
  int		second_value;
  unsigned char	dest;

  (void)champ;
  f->cycle_before_ins = g_op_tab[5].nbr_cycles;
  cb = machine->mem[wrap_pos(f->pos + 1)];
  first_type = get_cb_type(cb, FIRST_ARG);
  second_type = get_cb_type(cb, SECOND_ARG);
  offset = f->pos + 2;
  if (!read_arg_value(machine, f, reg, 0x06, 1, first_type, offset,
		      true, &first_value))
    return ;
  offset += get_arg_size(0x06, first_type, 1);
  if (!read_arg_value(machine, f, reg, 0x06, 2, second_type, offset,
		      true, &second_value))
    return ;
  offset += get_arg_size(0x06, second_type, 2);
  dest = machine->mem[wrap_pos(offset)];
  if (!is_valid_reg(dest))
    return ;
  reg[reg_index(dest)] = first_value & second_value;
  f->carry = (reg[reg_index(dest)] == 0);
}

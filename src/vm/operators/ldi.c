/*
** ldi.c for corewar in src/vm/operators/ldi.c
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

void	ldi(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	cb;
  int		first_type;
  int		second_type;
  int		offset;
  int		first_value;
  int		second_value;
  unsigned char	target_reg;

  (void)champ;
  f->cycle_before_ins = g_op_tab[9].nbr_cycles;
  cb = machine->mem[wrap_pos(f->pos + 1)];
  first_type = get_cb_type(cb, FIRST_ARG);
  second_type = get_cb_type(cb, SECOND_ARG);
  if (first_type == FAIL || second_type == FAIL)
    return ;
  offset = f->pos + 2;
  if (!read_arg_value(machine, f, reg, 0x0a, 1, first_type, offset,
		      true, &first_value))
    return ;
  offset += get_arg_size(0x0a, first_type, 1);
  if (!read_arg_value(machine, f, reg, 0x0a, 2, second_type, offset,
		      true, &second_value))
    return ;
  offset += get_arg_size(0x0a, second_type, 2);
  target_reg = machine->mem[wrap_pos(offset)];
  if (!is_valid_reg(target_reg))
    return ;
  reg[reg_index(target_reg)] = get_direct(machine->mem,
					  wrap_pos(f->pos
						   + ((first_value
						       + second_value)
						      % IDX_MOD)));
}

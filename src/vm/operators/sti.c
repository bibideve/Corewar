/*
** sti.c for corewar in src/vm/operators/sti.c
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Fri Mar 31 14:20:06 2017 Robin Houssais
** Last update Fri Mar 31 14:20:06 2017 Robin Houssais
*/

#include "operators.h"
#include "vm.h"

extern t_op	g_op_tab[];

void	sti(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	src_reg;
  unsigned char	cb;
  int		second_type;
  int		third_type;
  int		offset;
  int		n1;
  int		n2;

  (void)champ;
  n1 = 0;
  n2 = 0;
  f->cycle_before_ins = g_op_tab[10].nbr_cycles;
  cb = machine->mem[wrap_pos(f->pos + 1)];
  src_reg = machine->mem[wrap_pos(f->pos + 2)];
  if (!is_valid_reg(src_reg))
    return ;
  second_type = get_cb_type(cb, SECOND_ARG);
  third_type = get_cb_type(cb, THIRD_ARG);
  if ((second_type != REG_TYPE && second_type != DIR_TYPE
       && second_type != IND_TYPE)
      || (third_type != REG_TYPE && third_type != DIR_TYPE))
    return ;
  offset = f->pos + 3;
  if (!read_arg_value(machine, f, reg, 0x0b, 2, second_type,
		      offset, true, &n1))
    return ;
  offset += get_arg_size(0x0b, second_type, 2);
  if (!read_arg_value(machine, f, reg, 0x0b, 3, third_type,
		      offset, true, &n2))
    return ;
  put_direct(machine->mem, wrap_pos(f->pos + ((n1 + n2) % IDX_MOD)),
	     reg[reg_index(src_reg)]);
}

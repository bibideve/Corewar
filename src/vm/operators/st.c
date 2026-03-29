/*
** st.c for corewar in st.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Fri Mar 31 14:34:34 2017 Bilel Fourati
** Last update Fri Mar 31 14:34:34 2017 Bilel Fourati
*/

#include "operators.h"
#include "vm.h"

extern t_op	g_op_tab[];

void	st(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  int		getind;
  int		arg_type;
  unsigned char	src_reg;
  unsigned char	dst_reg;

  (void)champ;
  f->cycle_before_ins = g_op_tab[2].nbr_cycles;
  src_reg = machine->mem[wrap_pos(f->pos + 2)];
  if (!is_valid_reg(src_reg))
    return ;
  arg_type = get_cb_type(machine->mem[wrap_pos(f->pos + 1)], SECOND_ARG);
  if (arg_type == REG_TYPE)
  {
    dst_reg = machine->mem[wrap_pos(f->pos + 3)];
    if (!is_valid_reg(dst_reg))
      return ;
    reg[reg_index(dst_reg)] = reg[reg_index(src_reg)];
  }
  else if (arg_type == IND_TYPE)
  {
    getind = wrap_pos(f->pos
		      + (get_indirect(machine->mem, wrap_pos(f->pos + 3))
			 % IDX_MOD));
    put_direct(machine->mem, getind, reg[reg_index(src_reg)]);
  }
}

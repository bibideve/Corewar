/*
** aff.c for corewar in src/vm/aff.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Fri Mar 31 03:01:41 2017 Bilel Fourati
** Last update Fri Mar 31 03:01:41 2017 Bilel Fourati
*/

#include <unistd.h>
#include "operators.h"
#include "vm.h"

extern t_op	g_op_tab[];

void	aff(t_machine *machine, t_champ *champ, t_fork *f, int *reg)
{
  unsigned char	n;
  unsigned char	reg_value;

  (void)champ;
  f->cycle_before_ins = g_op_tab[15].nbr_cycles;
  reg_value = machine->mem[wrap_pos(f->pos + 2)];
  if (!is_valid_reg(reg_value))
    return ;
  n = reg[reg_index(reg_value)] % 256;
  (void)!write(1, &n, 1);
}

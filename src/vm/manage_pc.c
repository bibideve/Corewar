/*
** manage_pc.c for corewar in src/vm/manage_pc.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Sun Apr  2 19:46:57 2017 Bilel Fourati
** Last update Sun Apr  2 19:46:57 2017 Bilel Fourati
*/

#include <stdlib.h>
#include <stdio.h>
#include "vm.h"
#include "my.h"
#include "operators.h"
#include "op.h"

extern t_opcode	g_opcode[NB_OPCODE];

static void	set_pc_mem(unsigned char *mem, int *pc)
{
  int		i;

  i = 0;
  while (i < NB_OPCODE)
  {
    if (mem[wrap_pos(*pc)] == (unsigned char)g_opcode[i].id)
    {
      if (g_opcode[i].hav_cb == false)
	*pc = wrap_pos(*pc + 3);
      else
	*pc = wrap_pos(*pc + set_pc_dep_opcode(*pc, mem, g_opcode[i].id) + 1);
      return ;
    }
    i++;
  }
  *pc = wrap_pos(*pc + 1);
}

int	set_pc(int *pc, unsigned char *mem)
{
  while (!is_valid_opcode(mem[wrap_pos(*pc)]))
    *pc = wrap_pos(*pc + 1);
  if (mem[wrap_pos(*pc)] == 0x01)
    *pc = wrap_pos(*pc + 5);
  else if (mem[wrap_pos(*pc)] == 0x0c || mem[wrap_pos(*pc)] == 0x0f)
    *pc = wrap_pos(*pc + 3);
  else
    set_pc_mem(mem, pc);
  return (wrap_pos(*pc));
}

int	set_pos(int pc)
{
  return (pc);
}

/*
** dump.c for vm in src/vm/flags/dump.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Wed Mar 29 18:33:15 2017 Bilel Fourati
** Last update Wed Mar 29 18:33:15 2017 Bilel Fourati
*/

#include "vm.h"
#include "my.h"

void	show_dump(t_machine *machine)
{
  int	i;
  int	high;
  int	low;

  i = 0;
  while (i < MEM_SIZE)
  {
    high = machine->mem[i] / 16;
    low = machine->mem[i] % 16;
    my_putchar("0123456789ABCDEF"[high]);
    my_putchar("0123456789ABCDEF"[low]);
    my_printf(" ");
    if ((i + 1) % 32 == 0)
      my_printf("\n");
    i++;
  }
  if (MEM_SIZE % 32 != 0)
    my_printf("\n");
}

static int	is_dump_flag(char **av)
{
  int	i;
  int	ind;

  i = 1;
  ind = -1;
  while (av[i] != NULL)
  {
    if (my_strcmp("-dump", av[i]) == 0)
    {
      if (av[i + 1] == NULL || !my_str_isdigit(av[i + 1]))
	return (FAIL);
      ind = i + 1;
      break;
    }
    i++;
  }
  return (ind);
}

int	dump_f(char **av, t_machine *mach)
{
  int	ind_nf;

  if ((ind_nf = is_dump_flag(av)) == FAIL)
    mach->dump_cycle = -1;
  else
    mach->dump_cycle = my_atoi(av[ind_nf]);
  if (mach->dump_cycle != -1
      && (mach->dump_cycle <= 0 || mach->dump_cycle > CYCLE_TO_DIE))
    return (FAIL);
  return (SUCCESS);
}

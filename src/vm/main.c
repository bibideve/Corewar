/*
** main.c for corewar in src/vm/main.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Sun Apr  2 16:41:32 2017 Bilel Fourati
** Last update Sun Apr  2 16:41:32 2017 Bilel Fourati
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "op.h"
#include "vm.h"
#include "my.h"
#include "operators.h"

int		main(int ac, char **av)
{
  t_champ	**champs;
  t_machine	machine;

  if (ac < 3)
    return (ERROR);
  machine.mem = my_calloc(sizeof(unsigned char), MEM_SIZE);
  machine.last_live = NULL;
  machine.dump_cycle = -1;
  if ((champs = analyse_flags(av, &machine)) == NULL)
    return (ERROR);
  start(champs, &machine);
  return (0);
}

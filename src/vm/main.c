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
#include <string.h>
#include "op.h"
#include "vm.h"
#include "my.h"
#include "operators.h"

static void	init_replay_trace(t_machine *machine)
{
  char		*trace_path;
  char		*stride;

  trace_path = getenv("COREWAR_TRACE_FILE");
  machine->trace_stream = NULL;
  machine->trace_frame_stride = 1;
  machine->trace_frame_count = 0;
  if (trace_path == NULL || trace_path[0] == '\0')
    return ;
  machine->trace_stream = fopen(trace_path, "w");
  if (machine->trace_stream == NULL)
    return ;
  stride = getenv("COREWAR_TRACE_EVERY");
  if (stride != NULL && strlen(stride) > 0)
    machine->trace_frame_stride = atoi(stride);
  if (machine->trace_frame_stride <= 0)
    machine->trace_frame_stride = 1;
}

int		main(int ac, char **av)
{
  t_champ	**champs;
  t_machine	machine;

  if (ac < 3)
    return (ERROR);
  machine.mem = my_calloc(sizeof(unsigned char), MEM_SIZE);
  machine.owner = my_calloc(sizeof(unsigned char), MEM_SIZE);
  if (machine.mem == NULL || machine.owner == NULL)
    return (ERROR);
  machine.last_live = NULL;
  machine.dump_cycle = -1;
  init_replay_trace(&machine);
  if ((champs = analyse_flags(av, &machine)) == NULL)
    return (ERROR);
  start(champs, &machine);
  if (machine.trace_stream != NULL)
    fclose(machine.trace_stream);
  return (0);
}

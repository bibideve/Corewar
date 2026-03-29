/*
** start.c for corewar in src/vm/start.c
** 
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
** 
** Started on  Fri Mar 31 14:43:01 2017 Bilel Fourati
** Last update Fri Mar 31 14:43:01 2017 Bilel Fourati
*/

#include <stdlib.h>
#include <stdio.h>
#include "vm.h"
#include "my.h"
#include "operators.h"
#include "op.h"

t_opcode	g_opcode[NB_OPCODE] =
{
  {0x01, false, NULL},
  {0x02, true, &ld},
  {0x03, true, &st},
  {0x04, true, &add},
  {0x05, true, &sub},
  {0x06, true, &my_and},
  {0x07, true, &my_or},
  {0x08, true, &my_xor},
  {0x09, false, &zjmp},
  {0x0a, true, &ldi},
  {0x0b, true, &sti},
  {0x0c, false, &my_fork},
  {0x0d, true, &lld},
  {0x0e, true, &lldi},
  {0x0f, false, &my_lfork},
  {0x10, true, &aff}
};
int	g_live_counter = 0;

static int	count_total_forks(t_champ **champs)
{
  int		i;
  int		count;
  t_fork	*fork;

  i = 0;
  count = 0;
  while (champs[i] != NULL)
  {
    fork = champs[i]->fork;
    while (fork != NULL)
    {
      count++;
      fork = fork->next;
    }
    i++;
  }
  return (count);
}

static void	write_json_string(FILE *stream, const char *value)
{
  const unsigned char	*ptr;

  fputc('"', stream);
  if (value != NULL)
  {
    ptr = (const unsigned char *)value;
    while (*ptr != '\0')
    {
      if (*ptr == '"' || *ptr == '\\')
      {
	fputc('\\', stream);
	fputc(*ptr, stream);
      }
      else if (*ptr == '\n')
	fputs("\\n", stream);
      else if (*ptr == '\r')
	fputs("\\r", stream);
      else if (*ptr == '\t')
	fputs("\\t", stream);
      else if (*ptr < 32)
	fprintf(stream, "\\u%04x", *ptr);
      else
	fputc(*ptr, stream);
      ptr++;
    }
  }
  fputc('"', stream);
}

static void	write_trace_metadata(FILE *stream, t_champ **champs)
{
  int	i;

  fprintf(stream, "{\n");
  fprintf(stream, "  \"memSize\": %d,\n", MEM_SIZE);
  fprintf(stream, "  \"rowSize\": 32,\n");
  fprintf(stream, "  \"champions\": [\n");
  i = 0;
  while (champs[i] != NULL)
  {
    fprintf(stream, "    {\"id\": %d, \"name\": ",
	    champs[i]->id);
    write_json_string(stream, champs[i]->head->prog_name);
    fprintf(stream, ", \"start\": %d}", champs[i]->add_start);
    if (champs[i + 1] != NULL)
      fprintf(stream, ",");
    fprintf(stream, "\n");
    i++;
  }
  fprintf(stream, "  ],\n");
  fprintf(stream, "  \"frames\": [\n");
}

static void	write_trace_hex(FILE *stream, unsigned char *bytes, int size)
{
  int	i;

  i = 0;
  while (i < size)
  {
    fprintf(stream, "%02X", bytes[i]);
    i++;
  }
}

static void	write_trace_frame(t_machine *machine, t_champ **champs, int cycle)
{
  FILE		*stream;
  int		i;
  t_fork	*fork;
  int		first_fork;

  if (machine->trace_stream == NULL)
    return ;
  stream = machine->trace_stream;
  if (machine->trace_frame_count > 0)
    fprintf(stream, ",\n");
  fprintf(stream,
	  "    {\"cycle\": %d, \"aliveProcesses\": %d, \"memory\": \"",
	  cycle, count_total_forks(champs));
  write_trace_hex(stream, machine->mem, MEM_SIZE);
  fprintf(stream, "\", \"owners\": \"");
  write_trace_hex(stream, machine->owner, MEM_SIZE);
  fprintf(stream, "\", \"processes\": [");
  i = 0;
  first_fork = 1;
  while (champs[i] != NULL)
  {
    fork = champs[i]->fork;
    while (fork != NULL)
    {
      if (!first_fork)
	fprintf(stream, ", ");
      fprintf(stream, "{\"championId\": %d, \"pc\": %d, \"carry\": %s}",
	      champs[i]->id, wrap_pos(fork->pc),
	      fork->carry ? "true" : "false");
      first_fork = 0;
      fork = fork->next;
    }
    i++;
  }
  fprintf(stream, "]}");
  machine->trace_frame_count++;
}

static int	init_trace_file(t_machine *machine, t_champ **champs)
{
  if (machine->trace_stream == NULL)
    return (SUCCESS);
  machine->trace_frame_count = 0;
  write_trace_metadata(machine->trace_stream, champs);
  return (SUCCESS);
}

static void	finalize_trace_file(t_machine *machine)
{
  if (machine->trace_stream == NULL)
    return ;
  fprintf(machine->trace_stream, "\n  ],\n");
  fprintf(machine->trace_stream, "  \"winner\": ");
  if (machine->last_live == NULL)
    fprintf(machine->trace_stream, "null\n");
  else
  {
    fprintf(machine->trace_stream, "{\"id\": %d, \"name\": ",
	    machine->last_live->id);
    write_json_string(machine->trace_stream,
		      machine->last_live->head->prog_name);
    fprintf(machine->trace_stream, "}\n");
  }
  fprintf(machine->trace_stream, "}\n");
}

bool	is_valid_opcode(unsigned char b)
{
  int	i;

  i = 0;
  while (i < NB_OPCODE)
  {
    if ((unsigned char)g_opcode[i].id == b)
      return (true);
    i++;
  }
  return (false);
}

static int	count_alive_processes(t_champ **champs)
{
  int		i;
  int		count;
  t_fork	*fork;

  i = 0;
  count = 0;
  while (champs[i] != NULL)
  {
    fork = champs[i]->fork;
    while (fork != NULL)
    {
      count++;
      fork = fork->next;
    }
    i++;
  }
  return (count);
}

static void	reset_alive_states(t_champ **champs)
{
  int		i;
  t_fork	*fork;

  i = 0;
  while (champs[i] != NULL)
  {
    champs[i]->alive = false;
    fork = champs[i]->fork;
    while (fork != NULL)
    {
      fork->live_called = false;
      fork = fork->next;
    }
    i++;
  }
}

static int	get_instruction_cycles(unsigned char opcode)
{
  int	i;

  i = 0;
  while (g_op_tab[i].mnemonique != 0)
  {
    if ((unsigned char)g_op_tab[i].code == opcode)
      return (g_op_tab[i].nbr_cycles);
    i++;
  }
  return (1);
}

static void	kill_dead_forks(t_champ *champ)
{
  t_fork	*ptr;
  t_fork	*next;

  ptr = champ->fork;
  while (ptr != NULL)
  {
    next = ptr->next;
    if (!ptr->live_called)
    {
      if (ptr->prev != NULL)
	ptr->prev->next = ptr->next;
      else
	champ->fork = ptr->next;
      if (ptr->next != NULL)
	ptr->next->prev = ptr->prev;
      free(ptr);
    }
    ptr = next;
  }
}

static void	load_instruction(t_machine *machine, t_fork *fork)
{
  unsigned char	opcode;

  opcode = machine->mem[wrap_pos(fork->pc)];
  if (!is_valid_opcode(opcode))
  {
    fork->pc = wrap_pos(fork->pc + 1);
    return ;
  }
  fork->pos = fork->pc;
  fork->pending_opcode = opcode;
  fork->cycle_before_ins = get_instruction_cycles(opcode);
  set_pc(&fork->pc, machine->mem);
}

static void	execute_loaded_instruction(t_machine *machine, t_champ **champs,
					   int champ_index, t_fork *fork)
{
  int	j;

  if (fork->pending_opcode == 0)
    return ;
  if (fork->pending_opcode == 0x01)
  {
    live(machine, champs, fork);
    fork->pending_opcode = 0;
    return ;
  }
  j = 0;
  while (j < NB_OPCODE)
  {
    if ((unsigned char)g_opcode[j].id == fork->pending_opcode
	&& g_opcode[j].fnc != NULL)
    {
      g_opcode[j].fnc(machine, champs[champ_index], fork, fork->reg);
      fork->pending_opcode = 0;
      return ;
    }
    j++;
  }
  fork->pending_opcode = 0;
}

static void	run_cycle(t_champ **champs, t_machine *machine)
{
  int		i;
  t_fork	*fork;
  t_fork	*next;

  i = 0;
  while (champs[i] != NULL)
  {
    fork = champs[i]->fork;
    while (fork != NULL)
    {
      next = fork->next;
      if (fork->pending_opcode == 0)
	load_instruction(machine, fork);
      if (fork->pending_opcode != 0)
      {
	fork->cycle_before_ins--;
	if (fork->cycle_before_ins <= 0)
	  execute_loaded_instruction(machine, champs, i, fork);
      }
      fork = next;
    }
    i++;
  }
}

static bool	handle_cycle_to_die(t_champ **champs, t_machine *machine,
				    int *cycle_to_die)
{
  int	i;

  i = 0;
  while (champs[i] != NULL)
  {
    kill_dead_forks(champs[i]);
    i++;
  }
  if (g_live_counter >= NBR_LIVE && *cycle_to_die > CYCLE_DELTA)
    *cycle_to_die -= CYCLE_DELTA;
  g_live_counter = 0;
  if (count_alive_processes(champs) == 0)
    return (true);
  reset_alive_states(champs);
  (void)machine;
  return (false);
}

int	start(t_champ **champs, t_machine *machine)
{
  bool	win;
  int	cycle_to_die;
  int	nb_cycle;
  int	period_cycle;

  win = false;
  cycle_to_die = CYCLE_TO_DIE;
  nb_cycle = 0;
  period_cycle = 0;
  machine->last_live = NULL;
  if (init_trace_file(machine, champs) == FAIL)
    return (ERROR);
  reset_alive_states(champs);
  if (machine->trace_stream != NULL)
    write_trace_frame(machine, champs, nb_cycle);
  while (!win && cycle_to_die > 0)
  {
    run_cycle(champs, machine);
    nb_cycle++;
    period_cycle++;
    if (machine->trace_stream != NULL
	&& (nb_cycle % machine->trace_frame_stride) == 0)
      write_trace_frame(machine, champs, nb_cycle);
    if (machine->dump_cycle > 0 && nb_cycle >= machine->dump_cycle)
    {
      show_dump(machine);
      finalize_trace_file(machine);
      return (SUCCESS);
    }
    if (period_cycle >= cycle_to_die)
    {
      win = handle_cycle_to_die(champs, machine, &cycle_to_die);
      period_cycle = 0;
    }
  }
  if (machine->last_live != NULL)
    my_printf("The player %d(%s) has won.\n", machine->last_live->id,
	      machine->last_live->head->prog_name);
  if (machine->trace_stream != NULL
      && (machine->trace_frame_count == 0
	  || ((nb_cycle % machine->trace_frame_stride) != 0)))
    write_trace_frame(machine, champs, nb_cycle);
  finalize_trace_file(machine);
  return (SUCCESS);
}

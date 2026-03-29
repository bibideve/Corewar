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
  reset_alive_states(champs);
  while (!win && cycle_to_die > 0)
  {
    run_cycle(champs, machine);
    nb_cycle++;
    period_cycle++;
    if (machine->dump_cycle > 0 && nb_cycle >= machine->dump_cycle)
    {
      show_dump(machine);
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
  return (SUCCESS);
}

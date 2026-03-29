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

static int	count_alive(t_champ **champs, int *winner)
{
  int	i;
  int	nb_alive;

  i = 0;
  nb_alive = 0;
  *winner = -1;
  while (champs[i] != NULL)
  {
    if (champs[i]->alive)
    {
      nb_alive++;
      *winner = i;
    }
    i++;
  }
  return (nb_alive);
}

static void	reset_alive_states(t_champ **champs)
{
  int	i;

  i = 0;
  while (champs[i] != NULL)
  {
    champs[i]->alive = false;
    i++;
  }
}

static void	execute_instruction(t_champ **champs, int i,
				    t_machine *machine, t_fork *ptr)
{
  int		j;
  unsigned char	opcode;

  opcode = machine->mem[ptr->pos % MEM_SIZE];
  if (ptr->cycle_before_ins > 0)
    return ;
  if (opcode == 0x01)
  {
    live(machine, champs, ptr);
    return ;
  }
  j = 0;
  while (j < NB_OPCODE)
  {
    if ((unsigned char)g_opcode[j].id == opcode && g_opcode[j].fnc != NULL)
    {
      g_opcode[j].fnc(machine, champs[i], ptr, ptr->reg);
      return ;
    }
    j++;
  }
}

static void	fork_manager(t_champ **champs, int i, t_machine *machine)
{
  t_fork	*ptr;

  ptr = champs[i]->fork;
  while (ptr != NULL)
  {
    if (ptr->cycle_before_ins <= 0)
    {
      ptr->pos = set_pos(ptr->pc);
      ptr->pc = set_pc(&ptr->pc, machine->mem);
    }
    execute_instruction(champs, i, machine, ptr);
    if (ptr->cycle_before_ins > 0)
      ptr->cycle_before_ins--;
    ptr = ptr->next;
  }
}

static int	run_cycle(t_champ **champs, t_machine *machine)
{
  int	i;

  i = 0;
  while (champs[i] != NULL)
  {
    fork_manager(champs, i, machine);
    i++;
  }
  return (SUCCESS);
}

static bool	handle_cycle_to_die(int *cycle_to_die, t_champ **champs,
				    int *winner)
{
  int	nb_alive;

  nb_alive = count_alive(champs, winner);
  if (g_live_counter >= NBR_LIVE && *cycle_to_die > CYCLE_DELTA)
    *cycle_to_die -= CYCLE_DELTA;
  g_live_counter = 0;
  if (nb_alive <= 1)
    return (true);
  reset_alive_states(champs);
  return (false);
}

static void	print_winner(t_champ **champs, int winner)
{
  if (winner >= 0)
    my_printf("The player %d(%s) has won.\n", champs[winner]->id,
	      champs[winner]->head->prog_name);
}

int	start(t_champ **champs, t_machine *machine)
{
  bool	win;
  int	cycle_to_die;
  int	nb_cycle;
  int	period_cycle;
  int	winner;

  win = false;
  cycle_to_die = CYCLE_TO_DIE;
  nb_cycle = 0;
  period_cycle = 0;
  winner = -1;
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
      win = handle_cycle_to_die(&cycle_to_die, champs, &winner);
      period_cycle = 0;
    }
  }
  if (!win)
    count_alive(champs, &winner);
  print_winner(champs, winner);
  return (SUCCESS);
}

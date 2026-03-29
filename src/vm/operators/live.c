/*
** live.c for corewar in src/vm/operators/live.c
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Fri Mar 31 14:10:27 2017 Robin Houssais
** Last update Fri Mar 31 14:10:27 2017 Robin Houssais
*/

#include "vm.h"
#include "my.h"
#include "operators.h"

extern int g_live_counter;
extern t_op	g_op_tab[];

t_champ	*exist_id(t_champ **champs, int id)
{
  int	i;

  i = 0;
  while (champs[i])
  {
    if (champs[i]->id == id)
      return (champs[i]);
    i++;
  }
  return (NULL);
}

void		live(t_machine *machine, t_champ **champs, t_fork *f)
{
  t_champ	*c;

  (void)machine;
  (void)champs;
  f->cycle_before_ins = g_op_tab[0].nbr_cycles;
  g_live_counter++;
  if ((c = exist_id(champs, get_direct(machine->mem, f->pos + 1))))
  {
    my_printf("The player %d(%s) is alive.\n", c->id,
	c->head->prog_name);
    c->alive = true;
  }
  f->carry = false;
}

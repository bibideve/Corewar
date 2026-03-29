/*
** getters.c for vm in src/vm/getters.c
**
** Made by Bilel Fourati
** Login   <bilel.fourati@epitech.eu>
**
** Started on  Wed Mar 29 16:50:31 2017 Bilel Fourati
** Last update Wed Mar 29 16:50:31 2017 Bilel Fourati
*/

#include "op.h"
#include "vm.h"

int	wrap_pos(int pos)
{
  while (pos < 0)
    pos += MEM_SIZE;
  return (pos % MEM_SIZE);
}

bool	is_valid_reg(unsigned char reg_value)
{
  return (reg_value >= 1 && reg_value <= REG_NUMBER);
}

int	reg_index(unsigned char reg_value)
{
  return (reg_value - 1);
}

int	get_arg_size(unsigned char opcode, int arg_type, int arg_number)
{
  (void)arg_number;
  if (arg_type == REG_TYPE)
    return (1);
  if (arg_type == IND_TYPE)
    return (IND_SIZE);
  if (arg_type != DIR_TYPE)
    return (0);
  if (opcode == 0x09 || opcode == 0x0a || opcode == 0x0b
      || opcode == 0x0c || opcode == 0x0e || opcode == 0x0f)
    return (IND_SIZE);
  return (DIR_SIZE);
}

bool	read_arg_value(t_machine *machine, t_fork *fork, int *reg,
		       unsigned char opcode, int arg_number, int arg_type,
		       int pos, bool apply_idx_mod, int *value)
{
  int		address;
  unsigned char	reg_value;

  if (arg_type == REG_TYPE)
  {
    reg_value = machine->mem[wrap_pos(pos)];
    if (!is_valid_reg(reg_value))
      return (false);
    *value = reg[reg_index(reg_value)];
    return (true);
  }
  if (arg_type == DIR_TYPE)
  {
    if (get_arg_size(opcode, arg_type, arg_number) == IND_SIZE)
      *value = get_indirect(machine->mem, pos);
    else
      *value = get_direct(machine->mem, pos);
    return (true);
  }
  if (arg_type != IND_TYPE)
    return (false);
  address = get_indirect(machine->mem, pos);
  if (apply_idx_mod)
    address %= IDX_MOD;
  *value = get_direct(machine->mem, wrap_pos(fork->pos + address));
  return (true);
}

int		get_direct(unsigned char *mem, int pos)
{
  int	value;
  int	i;

  value = 0;
  i = 0;
  while (i < DIR_SIZE)
  {
    value <<= 8;
    value |= mem[wrap_pos(pos + i)];
    i++;
  }
  return (value);
}

int		get_indirect(unsigned char *mem, int pos)
{
  short	value;
  int	i;

  value = 0;
  i = 0;
  while (i < IND_SIZE)
  {
    value <<= 8;
    value |= mem[wrap_pos(pos + i)];
    i++;
  }
  return (value);
}

static void	write_owner(unsigned char *owners, int pos, int size,
			    unsigned char owner_id)
{
  int	i;

  if (owners == NULL)
    return ;
  i = 0;
  while (i < size)
  {
    owners[wrap_pos(pos + i)] = owner_id;
    i++;
  }
}

void		put_direct(unsigned char *mem, int pos, int nb)
{
  unsigned int	value;
  int		i;

  value = (unsigned int)nb;
  i = 0;
  while (i < DIR_SIZE)
  {
    mem[wrap_pos(pos + i)] = (value >> (8 * (DIR_SIZE - 1 - i))) & 0xFF;
    i++;
  }
}

void		put_direct_owner(t_machine *machine, int pos, int nb,
			       unsigned char owner_id)
{
  put_direct(machine->mem, pos, nb);
  write_owner(machine->owner, pos, DIR_SIZE, owner_id);
}

void		put_indirect(unsigned char *mem, int pos, short nb)
{
  unsigned short	value;
  int			i;

  value = (unsigned short)nb;
  i = 0;
  while (i < IND_SIZE)
  {
    mem[wrap_pos(pos + i)] = (value >> (8 * (IND_SIZE - 1 - i))) & 0xFF;
    i++;
  }
}

void		put_indirect_owner(t_machine *machine, int pos, short nb,
				 unsigned char owner_id)
{
  put_indirect(machine->mem, pos, nb);
  write_owner(machine->owner, pos, IND_SIZE, owner_id);
}

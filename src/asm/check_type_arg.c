/*
** assemble_instruction.c for asm in src/asm/assemble_instruction.c
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Thu Mar 23 14:02:50 2017 Maxime Louet
** Last update Sun Apr  2 20:34:12 2017 Zirk
*/

#include "asm.h"
#include "parse_string.h"
#include "my.h"

bool	is_label(const char *instruction)
{
  if (instruction == NULL || instruction[0] == '\0')
    return (false);
  if (instruction[0] == LABEL_CHAR)
  {
    if (instruction[1] == '\0')
      return (false);
    if (is_label_chars(instruction, 1) != -2)
      return (false);
    return (true);
  }
  else if (instruction[0] == DIRECT_CHAR && instruction[1] == LABEL_CHAR)
  {
    if (instruction[2] == '\0')
      return (false);
    if (is_label_chars(instruction, 2) != -2)
      return (false);
    return (true);
  }
  else
    return (false);
}

bool	is_indirect(const char *instruction)
{
  if (instruction == NULL || instruction[0] == '\0')
    return (false);
  if (instruction[0] == LABEL_CHAR)
  {
    if (instruction[1] == '\0')
      return (false);
    if (is_label_chars(instruction, 1) != -2)
      return (false);
    return (true);
  }
  else if (is_strn_number(instruction, 0) != -2)
    return (false);
  return (true);
}

bool	is_direct(const char *instruction)
{
  int	i;

  if (instruction == NULL || instruction[0] == '\0' ||
      instruction[1] == '\0' || instruction[0] != DIRECT_CHAR)
    return (false);
  if (instruction[1] == LABEL_CHAR)
    {
      i = 2;
      if (!instruction[2])
	return (false);
      while (instruction[i] != '\0')
	{
	  if (is_label_chars(instruction, i) != -2)
	    return (false);
	  i++;
	}
    }
  else
    {
      if ((i = is_strn_number(instruction, 1)) != -2)
	return (false);
    }
  return (true);
}

bool	is_register(const char *instruction)
{
  int	i;
  int	reg_number;

  if (instruction == NULL || instruction[0] == '\0' ||
      instruction[1] == '\0')
    return (false);
  if (instruction[0] != 'r')
    return (false);
  i = 1;
  while (instruction[i] != '\0')
  {
    if (!my_isdigit(instruction[i]))
      return (false);
    i++;
  }
  reg_number = my_atoi(&instruction[1]);
  if (reg_number < 1 || reg_number > REG_NUMBER)
    return (false);
  return (true);
}

/*
** asm_error.c for corewar in /home/zirk/Epitech/CPE_2016_corewar/src/asm
** 
** Made by Zirk
** Login   <zirk@epitech.net>
** 
** Started on  Tue Mar 14 23:04:13 2017 Zirk
** Last update Fri Mar 31 01:01:04 2017 Zirk
*/

#include "asm.h"
#include "my.h"

void		asm_warning(int warcode, int line, t_options *options)
{
  const char	*war_msgs[] = {
    "Unhandled dot-instruction"
  };

  if (options->flags[WERROR]->enabled)
    {
      my_dprintf(2, "\x1B[31mERROR\x1B[0m line %d:\n", line);
      my_dprintf(2, "\t%s\n\n", war_msgs[warcode]);
    }
  else
    {
      my_dprintf(2, "\x1B[34mWARNING\x1B[0m line %d:\n", line);
      my_dprintf(2, "\t%s\n\n", war_msgs[warcode]);
    }
}

int		asm_error(int errcode)
{
  const char	*err_msgs[] = {
    "Dynamic memory allocation failed. Please try to restart your computer",
    "Cannot open input file. "
    "You don't have enough permissions to open this file.",
    "Cannot open input file: the specified file does not exist.",
    "Cannot open input file: unknown error.",
    "Cannot create output file: file already exists.",
    "Cannot create output file: unknown error.",
    "Cannot read input file: unknown error.",
    "Cannot write output file: unknown error.",
    "OHMYGOSH DONT TOUCH OR EVERYTHING WILL BURN, YA FOOL",
  };
  my_dprintf(2, "\x1B[31mFATAL ERROR\x1B[0m\n");
  my_dprintf(2, "\t%s\n", err_msgs[errcode]);
  return (84);
}

bool		code_error(int errcode, int line, t_options *options)
{
  const char	*err_msgs[] = {
    "Name is too long",
    "Comment is too long",
    "Too many arguments for the instruction",
    "Not enough arguments for the instruction",
    "Variable is neither a direct value, an indirect value"
    " or a register",
    "Unknown instruction",
    "This instruction can't take this type of argument",
    "No such register"
  };

  options->error_encountered = true;
  my_dprintf(2, "\x1B[31mERROR\x1B[0m line %d:\n", line);
  my_dprintf(2, "\t%s\n\n", err_msgs[errcode]);
  return (false);
}

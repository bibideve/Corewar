/*
** vm.h for corewar in include/vm.h
** 
** Made by Robin Houssais
** Login   <robin.houssais@epitech.eu>
** 
** Started on  Tue Mar 21 10:59:27 2017 Robin Houssais
** Last update Tue Mar 21 10:59:27 2017 Robin Houssais
*/

#ifndef VM_H_
# define VM_H_

# include <stdbool.h>
# include "op.h"

# define ERROR 84
# define NB_CHAMP 4
# define FAIL (-1)
# define SUCCESS 0
# define NB_FLAGS 2
# define NB_OPCODE 16
# define FILE_EXT ".cor"

/*
**  Enum for an access to the flags -n or -a
*/
enum e_n_flag { N_FLAG, A_FLAG };

/*
**  Enum for an access to the flags -n or -a
*/
enum e_get_cod_type { FIRST_ARG, SECOND_ARG, THIRD_ARG };

/*
**  Enum for an access to the flags -n or -a
*/
enum e_cod_type { IND_TYPE, DIR_TYPE, REG_TYPE };

typedef struct	s_flags
{
  int		act;
  char		*id;
  char		*arg;
}		t_flags;

typedef struct	s_fork
{
  struct s_fork	*prev;
  struct s_fork	*next;
  int		id;
  int		pos;
  int		pc;
  int		reg[REG_NUMBER];
  int		cycle_before_ins;
  bool		carry;
}		t_fork;

typedef struct	s_champ
{
  int		id;
  bool		carry;
  bool		alive;
  t_fork	*fork;
  t_header	*head;
  int		add_start;
}		t_champ;

typedef struct	s_machine
{
  unsigned char	*mem;
  int		dump_cycle;
}		t_machine;

typedef struct	s_opcode
{
  char		id;
  bool		hav_cb;
  void		(*fnc)(t_machine*, t_champ*, t_fork*, int*);
}		t_opcode;

/*
** Swab Big endian / Little Endian
*/
unsigned int	swap_be(unsigned int);
unsigned int	swap_be_ui(unsigned int);
int		swap_be_i(int);
short		swap_be_s(short);

int		binary(unsigned char decimal);
int		get_direct(unsigned char *mem, int pos);
int		get_indirect(unsigned char *mem, int pos);
void		put_direct(unsigned char *mem, int pos, int nb);
void		put_indirect(unsigned char *mem, int pos, short nb);
int		get_cb_type(char, int);
int		get_arg_size(unsigned char opcode, int arg_type, int arg_number);
bool		read_arg_value(t_machine *machine, t_fork *fork, int *reg,
			       unsigned char opcode, int arg_number, int arg_type,
			       int pos, bool apply_idx_mod, int *value);
int		wrap_pos(int pos);
bool		is_valid_reg(unsigned char reg_value);
int		reg_index(unsigned char reg_value);
int		start(t_champ**, t_machine*);
int		set_pc_dep_opcode(int, unsigned char*, unsigned char);
void		show_dump(t_machine*);

/*
** Read.c
*/
t_header	*read_header(int);
char		*read_string(int, int);
int		read_int(int);
char		read_char(int);
/*
** Flag.c
*/
t_champ		**analyse_flags(char**, t_machine*);
int		dump_f(char**, t_machine*);
int		dump_case_corflag(char**, int*, bool*);
int		pars_flags(char**, int*, bool*);
int		process_flag(const char*, t_champ**, t_machine*, int);
int		process_flag_verif(int, t_champ**);
t_champ		*fill_champ(const char*, t_champ*, t_machine*, int*);
t_champ		**analyse_flags_champ(char**, t_champ**, t_machine*);
/*
** Manage_pc.c
*/
int		set_pc(int*, unsigned char*);
int		set_pos(int);
/*
** Start.c
*/
bool		is_valid_opcode(unsigned char);

#endif /* !VM_H_ */

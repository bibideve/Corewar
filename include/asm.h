/*
** asm.h for asm in include/asm.h
**
** Made by Maxime Louet
** Login   <maxime.louet@epitech.eu>
**
** Started on  Tue Mar  7 19:25:22 2017 Maxime Louet
** Last update Sun Apr  2 20:53:08 2017 Zirk
*/

#ifndef ASM_H_
# define ASM_H_

# include <stdbool.h>
# include <stddef.h>

# include "op.h"
# include "stdpair.h"
# include "mylist.h"

/*
** Warnings
*/
# define UNHANDLED_INSTRUCTION	0

/*
** Errors
*/
# define MALLOC_FAIL		0
# define INPUTFILE_PERMS	1
# define INPUTFILE_EXISTS	2
# define INPUTFILE_OTHER	3
# define OUTPUTFILE_EXISTS	4
# define OUTPUTFILE_OTHER	5
# define READ_OTHER		6
# define WRITE_OTHER		7
# define HIGHLY_INFLAMMABLE	8

/*
** Code errors
*/
# define NAME_TOO_LONG		0
# define COMMENT_TOO_LONG	1
# define TOO_MANY_ARGS		2
# define NOT_ENOUGH_ARGS	3
# define INVALID_ARGUMENT	4
# define INVALID_FUNCTION	5
# define WRONG_ARGUMENT		6

/*
** Option flags
*/
# define NB_FLAGS		10
# define WUNUSED_LABEL		0
# define WINVALID_JUMP		1
# define WOVERFLOW		2
# define LOLI			3
# define DEBUG			4
# define WALL			5
# define WFATAL_ERRORS		6
# define HELP1			7
# define HELP2			8
# define WERROR			9

# define ERROR			84

/*
** Check if arguments in t_line_content is not equal
** at NULL before accessing this struct !
*/
typedef struct	s_arg
{
  int		type;
  char		*arg;
  bool		is_label;
}		t_arg;

typedef struct	s_line_content
{
  char		*line;
  char		*label;
  char		*instruction;
  int		instruction_code;
  int		nb_args;
  t_arg		*arguments;
  size_t	mempos;
  size_t	memsize;
  int		prog_arg;
}		t_line_content;

typedef struct s_line_content	t_line;

typedef struct	s_options
{
  int		pos_inputfile;
  int		pos_outputfile;
  t_stdpair	*flags[NB_FLAGS];
  char		*flags_description[NB_FLAGS];
  bool		error_encountered;
}		t_options;

typedef struct	s_io
{
  char		*input_name;
  char		*output_name;
  int		input_fd;
  int		output_fd;
  int		line_counter;
}		t_io;

typedef struct	s_label
{
  char		*name;
  size_t	mempos;
}		t_label;

/*
** Swab Big endian / Little Endian
*/
unsigned int	swap_be(unsigned int);
unsigned int	swap_be_ui(unsigned int);
int		swap_be_i(int);
short		swap_be_s(short);

void		asm_warning(int warcode, int line, t_options *options);
int		asm_error(int errcode);
bool		code_error(int errcode, int line, t_options *options);

bool		parse_args(t_options *options, int ac, char **av);
bool		open_streams(char *inputpath, char *outputpath, t_io *io);
bool		assemble(t_options *options, t_io *io);
bool		init_structs(t_options **options, t_io **io);
bool		write_bytes(int fd, const void *buf, size_t size);
void		free_line_content(t_line_content *line_content);
void		free_lines(t_list *lines);

/*
** assemble_dot_instruction.c
*/
bool	assemble_dot_instruction(char *line, t_header *header, t_io *io,
				 t_options *options);

/*
** assemble_instruction.c
*/
bool	assemble_instruction(t_line_content *line_content, t_options *options,
			     t_io *io);

/*
** check_type_instruction.c
*/
int	get_arg(t_line_content *line_content, int which_arg,
		bool debug_mode, int nb_line);

/*
** is_label_chars.c
*/
int	is_label_chars(const char *str, int ini_pos);

/*
** header.c
*/
bool	write_header(t_header *header, t_options *options, t_io *io);
bool	prepare_header(t_header **header);
bool	write_header_output(t_header *header, t_options *options, t_io *io,
			    size_t final_size);

/*
** clean.c
*/
void	asm_clean(t_options *options, t_io *io);

/*
** swap_be.c
*/
void	check_if_arg_valid(t_options *options, t_io *io,
			   int arg[2], char *instruction);
bool	check_arg(t_options *options, t_io *io,
		  t_line_content *line_content, int nb_args);

/*
** usage.c
*/
void	usage(const t_options *options, const char *progname);

/*
** write.c
*/
bool	argument_is_index(int instruction_code, int iarg);
bool	write_final_output(t_list *lines, t_header *header,
			   t_options *options, t_io *io);
/*
** write_bytes.c
*/
void	write_reg(char *arg, t_io *io, t_options *options);
void	write_direct(char *arg, t_io *io, t_options *options);
void	write_indirect(char *arg, t_io *io, t_options *options);

/*
** labels.c
*/
bool	resolve_check_labels(t_list *lines, t_options *options);

/*
** labels_store.c
*/
bool	store_labels(t_list **labels, t_list *lines, t_options *options);

/*
** compute_size.c
*/
bool	argument_is_index(int instruction_code, int iarg);
bool	has_parameters_description(char code);
size_t	compute_line_size(t_line *line);

/*
** check_type_arg.c
*/
bool	is_label(const char *instruction);
bool	is_indirect(const char *instruction);
bool	is_register(const char *instruction);
bool	is_direct(const char *instruction);

/*
** assemble_line.c
*/
bool assemble_line(t_options *options, t_io *io,
		   t_header *header, t_line_content *line_content);

#endif /* !ASM_H_ */

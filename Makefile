##
## Makefile for caviardecanard in Makefile
## 
## Made by Robin Houssais
## Login   <robin.houssais@epitech.eu>
## 
## Started on  Tue Feb 14 09:28:41 2017 Robin Houssais
## Last update Sun Apr  2 20:53:30 2017 Zirk
##

MAKEFLAGS = --no-print-directory

CC	= gcc

CFLAGS	+= -Wall -Wextra -O3
#CFLAGS  += -ansi -pedantic
#CFLAGS  += -std=c99
#CFLAGS	+= -Wmissing-prototypes -Wfloat-equal -Wstrict-prototypes -Wshadow
#CFLAGS	+= -Winit-self -Wcast-align -Waggregate-return -Wunreachable-code
#CFLAGS	+= -Wlogical-op -Wdisabled-optimization -Wunused-result
#CFLAGS  += -fstrict-overflow -Wstrict-overflow=4
#CFLAGS	+= -fstack-protector-strong -Wstack-protector

CPPFLAGS	+= -Iinclude/
DEPFLAGS	= -MMD -MP
CPPFLAGS	+= $(DEPFLAGS)

SRCDIRCOR	= ./src/vm/
SRCCOR	= flags/dump.c \
	  flags/flags.c \
	  flags/flags_champs.c \
	  getters.c \
	  main.c \
	  get_type.c \
	  operators/add.c \
	  operators/ld.c \
	  operators/lld.c \
	  operators/ldi.c \
	  operators/lldi.c \
	  operators/live.c \
	  operators/my_and.c \
	  operators/my_or.c \
	  operators/my_xor.c \
	  operators/sti.c \
	  operators/my_fork.c \
	  operators/st.c \
	  operators/sub.c \
	  operators/zjmp.c \
	  operators/my_lfork.c \
	  operators/aff.c \
	  start.c \
	  read.c \
	  manage_pc.c \
	  virt_move.c \
	  ../op.c \
	  ../swap_be.c

SRCDIRASM	= ./src/asm/
SRCASM	= asm_error.c \
	  assemble.c \
	  assemble_dot_instruction.c \
	  assemble_instruction.c \
	  check_arg_valid.c \
	  check_type_instruction.c \
	  clean.c \
	  compute_size.c \
	  get_word.c \
	  header.c \
	  init.c \
	  is_label_chars.c \
	  is_strn_number.c \
	  labels.c \
	  labels_store.c \
	  list_add.c \
	  list_remove.c \
	  main.c \
	  open_streams.c \
	  parse_args.c \
	  parse_string.c \
	  usage.c \
	  write.c \
	  write_bytes.c \
	  ../op.c \
	  ../swap_be.c \
	  loli.c \
	  check_type_arg.c \
	  assemble_line.c

SRCCOR	:= $(addprefix $(SRCDIRCOR), $(SRCCOR))
SRCASM	:= $(addprefix $(SRCDIRASM), $(SRCASM))

OBJCOR	= $(SRCCOR:.c=.o)
OBJASM	= $(SRCASM:.c=.o)
DEPCOR	= $(OBJCOR:.o=.d)
DEPASM	= $(OBJASM:.o=.d)

LDFLAGS	+= -Llibmy/ -lmy

RM	= rm -rf

MKDIR	= mkdir -p

LIB	= $(LIBDIR)libmy.a
LIBDIR	= libmy/

DIRCOR	= ./corewar/
DIRASM	= ./asm/
NAMECOR	= corewar
NAMEASM	= asm


all:
	$(MAKE) -s -C $(LIBDIR)
	$(MAKE) $(NAMECOR)
	$(MAKE) $(NAMEASM)

$(NAMECOR): $(LIB) $(OBJCOR)
	$(MKDIR) $(DIRCOR)
	$(CC) $(OBJCOR) -o $(DIRCOR)$(NAMECOR) $(LDFLAGS)

$(NAMEASM): $(LIB) $(OBJASM)
	$(MKDIR) $(DIRASM)
	$(CC) $(OBJASM) -o $(DIRASM)$(NAMEASM) $(LDFLAGS)

clean:
	$(RM) $(OBJCOR)
	$(RM) $(OBJASM)
	$(RM) $(DEPCOR)
	$(RM) $(DEPASM)
	$(MAKE) -C $(LIBDIR) clean

fclean: clean
	$(RM) $(NAMECOR)
	$(RM) $(NAMEASM)
	$(MAKE) -C $(LIBDIR) fclean

re:	fclean all

.PHONY:	all clean fclean re

-include $(DEPCOR) $(DEPASM)

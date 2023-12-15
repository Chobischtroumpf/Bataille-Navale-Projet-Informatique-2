# ================================ VARIABLES ================================= #

# The name of your executable
CLIENT	= battleship
<<<<<<< HEAD
SERVER = 
=======
# SERVER = 
>>>>>>> master
# Compiler and compiling flags
CXX			= g++
CXXFLAGS	=	-Wall -Werror -Wextra -std='c++20' -pedantic -g \
				-Wall -Wextra -Wsign-compare -fmax-errors=2 \

ifeq ($(shell uname),Darwin)


# Debug, use with`make DEBUG=1`
ifeq ($(DEBUG),1)
CFLAGS	+= -g3 -fsanitize=address
endif

# Folder name
SRCDIR_CLIENT	= src/client
INCDIR_CLIENT	= $(SRCDIR_CLIENT)/includes/
OBJDIR	= build/

<<<<<<< HEAD
=======
SRCDIR_SERVER	= src/server

>>>>>>> master
# Add include folder
CXXFLAGS	+= -I $(INCDIR_CLIENT)

# Linking stage flags
LDFLAGS =

###▼▼▼<src-updater-do-not-edit-or-remove>▼▼▼
# **************************************************************************** #
# **   Generated with https://github.com/lorenuars19/makefile-src-updater   ** #
# **************************************************************************** #

SRCS_CLIENT =\


HEADERS_CLIENT =\



###▲▲▲<src-updater-do-not-edit-or-remove>▲▲▲

# String manipulation magic
SRC		:= $(notdir $(SRCS))
OBJ		:= $(SRC:.cpp=.o)
OBJS	:= $(addprefix $(OBJDIR), $(OBJ))

# Colors
GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m

# Implicit rules
VPATH := $(SRCDIR) $(OBJDIR) $(shell find $(SRCDIR) -type d)

# ================================== RULES =================================== #

all : $(NAME)

# Compiling
$(OBJDIR)%.o : %.cpp
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Linking
$(NAME)	: $(SRCS) $(HEADERS) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

# Cleaning
clean :
	@rm -rf $(OBJDIR)

fclean : clean
	@rm -f $(NAME)

# Special rule to force to remake everything
re : fclean all

# This runs the program
run : $(NAME)
	@printf "$(CY)>>> Running $(NAME)$(RC)"
	./$(NAME)

# This specifies the rules that does not correspond to any filename
.PHONY	= all run clean fclean re
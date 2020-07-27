NAME	= plazza 

CC	= g++ -pthread -g
 
RM	= rm -f

SRCS	= ./srcs/APizza.cpp \
	  ./srcs/Kitchen.cpp \
	  ./srcs/Cook.cpp \
	  ./srcs/Regina.cpp \
	  ./srcs/Margarita.cpp \
	  ./srcs/Americana.cpp \
	  ./srcs/Fantasia.cpp \
	  ./srcs/Reception.cpp \
	  ./srcs/Stock.cpp \
	  ./srcs/StringUtils.cpp \
	  ./srcs/Utils.cpp \
	  ./srcs/main.cpp 

OBJS	= $(SRCS:.cpp=.o)

CPPFLAGS = -I ./include/pp
CPPFLAGS += -Wall -Wextra 

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

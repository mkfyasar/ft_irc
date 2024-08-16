NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra  -std=c++98
SRC = test/main.cpp test/Server.cpp 
OBJ = $(SRC:test/%.cpp=obj/%.o)
INCLUDES = -Iinclude

all: obj $(NAME)

obj:
	mkdir -p obj

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re obj



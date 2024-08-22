NAME = ircserv
CXX = c++
CXXFLAGS =
SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/SignalHandler.cpp src/PollManager.cpp
OBJS = $(SRCS:src/%.cpp=obj/%.o)
INCLUDES = -Iinclude

all: obj $(NAME)

obj:
	mkdir -p obj

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re obj
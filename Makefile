NAME = ircserv
CXX = c++

MAIN_CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude -Isrc
LDFLAGS = -L$(GTEST_LIB) -lgtest -lgtest_main -pthread
MAIN_SRCS = src/main.cpp src/parseArguments.cpp src/Server.cpp src/Client.cpp \
src/NetworkManager.cpp src/ClientManager.cpp src/EpollManager.cpp src/MessageParser.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
RM = rm -f
HEADERS = src/utils.hpp include/ft_irc/Server.hpp src/Client.hpp src/NetworkManager.hpp \
src/ClientManager.hpp src/EpollManager.hpp src/MessageParser.hpp

all: $(NAME)

$(NAME): $(MAIN_OBJS)
	$(CXX) $(MAIN_CXXFLAGS) -o $(NAME) $(MAIN_OBJS)

%.o: %.cpp  $(HEADERS)
	$(CXX) $(MAIN_CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(MAIN_OBJS) $(MAIN_DEP)

clean_tests:
	$(RM) $(TEST_OBJS) $(TEST_DEP)

fclean: clean clean_tests
	$(RM) $(NAME) $(TEST_NAME)

re: fclean all




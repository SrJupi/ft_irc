NAME = ircserv
CXX = c++

MAIN_CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude -Isrc -g
LDFLAGS = -L$(GTEST_LIB) -lgtest -lgtest_main -pthread
MAIN_SRCS = src/main.cpp src/Server.cpp src/user/User.cpp \
src/NetworkManager.cpp src/user/UserManager.cpp src/EpollManager.cpp src/Parser.cpp \
src/command/CommandManager.cpp src/channel/Channel.cpp src/channel/ChannelManager.cpp \
src/command/HandleCap.cpp   src/command/HandleKick.cpp  src/command/HandleNick.cpp src/command/HandlePass.cpp  \
src/command/HandleQuit.cpp  src/command/HandleTopic.cpp src/command/HandleJoin.cpp src/command/HandleMode.cpp \
src/command/HandlePart.cpp  src/command/HandlePrivmsg.cpp src/command/HandleExit.cpp src/command/HandleUser.cpp \
src/command/HandleInvite.cpp src/command/sendResponse.cpp src/command/HandleWho.cpp src/command/HandlePing.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
RM = rm -f
HEADERS = include/ft_irc/Server.hpp src/user/User.hpp src/NetworkManager.hpp \
src/user/UserManager.hpp src/EpollManager.hpp src/Parser.hpp src/command/CommandManager.hpp \
src/ServerReplyMessages.hpp src/channel/Channel.hpp src/channel/ChannelManager.hpp \
src/command/Handlers.hpp

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




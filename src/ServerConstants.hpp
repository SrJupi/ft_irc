#ifndef SERVERCONSTANTS_HPP
# define SERVERCONSTANTS_HPP

#include <string>

//ERRORS RETURNS
#define GETADDRINFO_FAILURE 1
#define CREATE_SOCKET_FAILURE 2
#define BIND_FAILURE 3
#define LISTEN_FAILURE 4
#define CREATE_EPOLL_FAILURE 5
#define ADD_TO_EPOLL_FAILURE 6
#define LOAD_MOTD_FAILURE 7
#define SIGNAL_FAILURE 8

const int MAX_EVENTS = 128;
const int BUFFER_SIZE = 512;
const std::string CRLF = "\r\n";

const std::string SERVER_NAME = "ft_irc.server.br";
const std::string NETWORK_NAME = "Brazilian IRC";
const std::string IRC_VERSION = "0.0.1-ALPHA";

const std::string USER_MODES = "";
const std::string CHAN_MODES = "iklot";

static const char MOTD_FILE[] = "src/.conf/motd.irc";

static const char OPER_CONFIG_FILE[] = "src/.conf/oper.irc";

//IRC definitions
static std::string SPECIAL_CHARS = "-[]\\`_^{|}";

//Size definitions
const int   NICKLEN = 16;
const int   TOPICLEN = 300;

//Quit messages
const std::string QUIT_NOMSG = "Leaving";
const std::string QUIT_CTRLC = "Connection reset by peer";
const std::string QUIT_ERROR = "Connection closed";



#endif

#ifndef SERVERCONSTANTS_HPP
# define SERVERCONSTANTS_HPP

#include <string>

//ERRORS RETURNS
/* TODO */


const int MAX_EVENTS = 128;
const int BUFFER_SIZE = 512;
const std::string CRLF = "\r\n";

const std::string SERVER_NAME = "ft_irc.server.br";
const std::string NETWORK_NAME = "Brazilian IRC";
const std::string IRC_VERSION = "0.0.1-ALPHA";

const std::string USER_MODES = "";
const std::string CHAN_MODES = "iklot";

static const char MOTD_FILE[] = "src/.conf/motd.irc";


#endif

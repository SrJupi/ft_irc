#ifndef HANDLERS_HPP
# define HANDLERS_HPP

#include <user/User.hpp>
#include <ft_irc/Server.hpp>

/* TODO:
4. OPER - David
6. "LOGIN" - Lucas
Info pro Login:
Sequence of Responses After a User Logs In

Once a user successfully logs in with PASS, NICK, and USER, the server sends a sequence of numeric replies to complete the registration process.

    RPL_WELCOME (001) – Welcome message
    RPL_YOURHOST (002) – Server information
    RPL_CREATED (003) – Server creation time
    RPL_MYINFO (004) – Server details
    RPL_ISUPPORT (005) – Supported server features (optional)
    RPL_MOTDSTART (375) – Start of the MOTD (Message of the Day)
    RPL_MOTD (372) – MOTD line (repeated for each line)
    RPL_ENDOFMOTD (376) – End of MOTD
*/

void handleCap(User &user, Server &server, const std::vector<std::string> &args);
void handleExit(User &user, Server &server, const std::vector<std::string> &args);
void handleInvite(User &user, Server &server, const std::vector<std::string> &args);
void handleJoin(User &user, Server &server, const std::vector<std::string> &args);
void handleKick(User &user, Server &server, const std::vector<std::string> &args);
void handleMode(User &user, Server &server, const std::vector<std::string> &args);
void handleNick(User &user, Server &server, const std::vector<std::string> &args);
void handlePart(User &user, Server &server, const std::vector<std::string> &args);
void handlePass(User &user, Server &server, const std::vector<std::string> &args);
void handlePing(User &user, Server &server, const std::vector<std::string> &args);
void handlePong(User &user, Server &server, const std::vector<std::string> &args);
void handlePrivmsg(User &user, Server &server, const std::vector<std::string> &args);
void handleQuit(User &user, Server &server, const std::vector<std::string> &args);
void handleTopic(User &user, Server &server, const std::vector<std::string> &args);
void handleUser(User &user, Server &server, const std::vector<std::string> &args);
void handleWho(User &user, Server &server, const std::vector<std::string> &args);

//Utils

void sendResponse(const std::string &response, int receiver);


#endif

#ifndef HANDLERS_HPP
# define HANDLERS_HPP

#include <user/User.hpp>
#include <ft_irc/Server.hpp>

/* TODO:
4. OPER - David
5. JOIN - Lucas
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
void handlePrivmsg(User &user, Server &server, const std::vector<std::string> &args);
void handleQuit(User &user, Server &server, const std::vector<std::string> &args);
void handleTopic(User &user, Server &server, const std::vector<std::string> &args);
void handleUser(User &user, Server &server, const std::vector<std::string> &args);
void handleWho(User &user, Server &server, const std::vector<std::string> &args);

//Utils

void sendResponse(const std::string &response, int receiver);


#endif

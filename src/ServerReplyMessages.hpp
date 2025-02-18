#ifndef SERVERREPLYMESSAGES_HPP
# define SERVERREPLYMESSAGES_HPP

#define RPL_WELCOME(server, network, nick, user, host) \
    ":" + (server) + " 001 " + (nick) + " :Welcome to the " + (network) + \
    " Network, " + (nick) + "!" + (user) + "@" + (host) + "\r\n"

#define RPL_YOURHOST(server, nick, version) \
    ":" + (server) + " 002 " + (nick) + " :Your host is " + (server) + ", running version " + (version) + "\r\n"

#define RPL_CREATED(server, nick, datetime) \
    ":" + (server) + " 003 " + (nick) + " :This server was created " + (datetime) + "\r\n"

#define RPL_MYINFO(server, nick, version, usermodes, chanmodes) \
    ":" + (server) + " 004 " + (nick) + " " + (server) + " " + (version) + " " + (usermodes) + " " + (chanmodes) + "\r\n"

#define RPL_ISUPPORT(server, nick, features) \
    ":" + (server) + " 005 " + (nick) + " " + (features) + " :are supported by this server\r\n"

#define RPL_CHANNELMODEIS(server, nick, channel, modesAndParams) \
    ":" + (server) + " 324 " + (nick) + " " + (channel) + " " + (modesAndParams) + "\r\n"

#define RPL_CREATIONTIME(server, nick, channel, timestamp) \
    ":" + (server) + " 329 " + (nick) + " " + (channel) + " " + (timestamp) + "\r\n"

#define RPL_NOTOPIC(server, nick, channel) \
    ":" + (server) + " 331 " + (nick) + " " + (channel) + " :No topic is set\r\n"

#define RPL_TOPIC(server, nick, channel, topic) \
    ":" + (server) + " 332 " + (nick) + " " + (channel) + " :" + (topic) + "\r\n"

#define RPL_TOPICWHOTIME(server, nick, channel, setBy, timestamp) \
    ":" + (server) + " 333 " + (nick) + " " + (channel) + " " + (setBy) + " " + (timestamp) + "\r\n"

#define RPL_NAMREPLY(server, nick, channel, nicks) \
    ":" + (server) + " 353 " + (nick) + " = " + (channel) + " :" + (nicks) + "\r\n"

#define RPL_ENDOFNAMES(server, nick, channel) \
    ":" + (server) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n"

#define RPL_NICK(oldNick, newNick) \
    ":" + (oldNick) + " NICK :" + (newNick) + "\r\n"

#define RPL_CAP_LS(server) \
    ":" + (server) + " CAP * LS :\r\n"

#define RPL_MOTD(server, nick, message) \
    ":" + (server) + " 372 " + (nick) + " :- " + (message) + "\r\n"

#define RPL_MOTDSTART(server, nick) \
    ":" + (server) + " 375 " + (nick) + " :- Message of the day -\r\n"

#define RPL_ENDOFMOTD(server, nick) \
    ":" + (server) + " 376 " + (nick) + " :End of MOTD\r\n"

#define ERR_NOSUCHNICK(nick, receiver) \
    "401 " + (nick) + " " + (receiver) + " :No such nick/channel\r\n"

#define ERR_NOSUCHCHANNEL(server, nick, channel) \
    ":" + (server) + " 403 " + (nick) + " " + (channel) + " :No such channel\r\n"   

#define ERR_CANNOTSENDTOCHAN(nick, channel) \
    "404 " + (nick) + " " + (channel) + " :Cannot send to channel\r\n"

#define ERR_NORECIPIENT(nick, command) \
    "411 " + (nick) + " :No recipient given (" + (command) + ")\r\n"

#define ERR_NOTEXTTOSEND(server, nick) \
    ":" + (server) + " 412 " + (nick) + " :No text to send\r\n"

#define ERR_UNKNOWNCOMMAND(server, nick, command) \
    ":" + (server) + " 421 " + (nick) + " " + (command) + " :Unknown command\r\n"

#define ERR_NONICKNAMEGIVEN(server) \
    ":" + (server) + " 431 * :No nickname given\r\n"

#define ERR_ERRONEUSNICKNAME(server, nick, attempted_nick) \
    ":" + (server) + " 432 " + (nick) + " " + (attempted_nick) + " :Erroneous nickname\r\n"

#define ERR_NICKNAMEINUSE(server, nick) \
    ":" + (server) + " 433 " + (nick) + " :Nickname is already in use\r\n"
    
#define ERR_USERNOTINCHANNEL(server, nick, channel) \
    ":" + (server) + " 441 " + (nick) + (channel) + " :They aren't on that channel\r\n"

#define ERR_NOTONCHANNEL(server, nick, channel) \
    ":" + (server) + " 442 " + (nick) + " " + (channel) + " :You're not on that channel\r\n"

#define ERR_NOTREGISTERED(server) \
    ":" + (server) + " 451 :You have not registered\r\n"

#define ERR_NEEDMOREPARAMS(server, nick, command) \
    ":" + (server) + " 461 " + (nick) + " " + (command) + " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTRED(nick) \
    "462 " + nick + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(nick, command) \
    "464 " + (nick) + " " + (command) + " :Password incorrect\r\n"

#define ERR_CHANNELISFULL(server, nick, channel) \
    ":" + (server) + " 471 " + (nick) + " " + (channel) + " :Cannot join channel (+l)\r\n"

#define ERR_UNKNOWNMODE(server, nick, mode) \
    ":" + (server) + " 472 " + (nick) + " " + (mode) + " :is unknown mode char to me\r\n"

#define ERR_INVITEONLYCHAN(server, nick, channel) \
    ":" + (server) + " 473 " + (nick) + " " + (channel) + " :Cannot join channel (+i)\r\n"

#define ERR_BADCHANNELKEY(server, nick, channel) \
    ":" + (server) + " 475 " + (nick) + " " + (channel) + " :Cannot join channel (+k)\r\n"

#define ERR_BADCHANMASK(server, nick, channel) \
    ":" + (server) + " 476 " + (nick) + " " + (channel) + " :Bad Channel Mask\r\n"

#define ERR_NOPRIVILEGES(server, nick) \
    ":" + (server) + " 481 " + (nick) + " :Permission Denied - You're not an IRC operator\r\n"

#define ERR_CHANOPRIVSNEEDED(server, nick, channel) \
    ":" + (server) + " 482 " + (nick) + " " + (channel) + " :You're not a channel operator\r\n"

// WHO repply messages
#define RPL_WHOREPLY(server, nick, channel, user, host, srv, targetNick, status, hopcount, realname) \
    ":" + (server) + " 352 " + (nick) + " " + (channel) + " " + (user) + " " + (host) + " " + (srv) + " " + (targetNick) + \
    " " + (status) + " :" + (hopcount) + " " + (realname) + "\r\n"

#define RPL_ENDOFWHO(server, nick, target) \
    ":" + (server) + " 315 " + (nick) + " " + (target) + " :End of WHO list\r\n"

// Forwarded message format for PRIVMSG
#define PRIVMSG(sender, receiver, message) \
    ":" + (sender) + " PRIVMSG " + (receiver) + " :" + (message) + "\r\n"

// Join Reply
#define RPL_JOIN(nick, user, host, channel) \
    ":" + (nick) + "!" + (user) + "@" + (host) + " JOIN " + (channel) + "\r\n"

// Mode Replies

#define RPL_MODES(server, nick, channel, sign, mode, message) \
":" + (nick) + "!" + (server) + " MODE " + (channel) + " " + (sign) + (mode) + " " + (message) + "\r\n"

#define RPL_MODE_OP(server, nick, channel, target, sign) \
":" + (nick) + "!" + (server) + " MODE " + (channel) + " " + (sign) + "o " + (target) + "\r\n"

#define RPL_MODE_INVITE(server, nick, channel, sign) \
":" + (nick) + "!" + (server) + " MODE " + (channel) + " " + (sign) + "i\r\n"

#define RPL_MODE_KEY(server, nick, channel, sign, key) \
":" + (nick) + "!" + (server) + " MODE " + (channel) + " " + (sign) + "k " + (key) + "\r\n"

#define RPL_MODE_LIMIT(server, nick, channel, sign, limit) \
":" + (nick) + "!" + (server) + " MODE " + (channel) + " " + (sign) + "l " + (limit) + "\r\n"

#define RPL_MODE_TOPIC(server, nick, channel, sign) \
":" + (nick) + "!" + (server) + " MODE " + (channel) + " " + (sign) + "t\r\n"

//Part message
#define RPL_PART(nick, user, host, channel, reason) \
    ":" + (nick) + "!" + (user) + "@" + (host) + " PART " + (channel) + " :" + (reason) + "\r\n"


//Quit message
#define RPL_QUIT(usermask, message) \
    ":" + (usermask) + " QUIT :" + (message) + "\r\n"

//Ping Pong

#define RPL_PING(server, token) \
    ":" + (server) + " PING :" + (token) + "\r\n"

#define RPL_PONG(server, token) \
    ":" + (server) + " PONG :" + (token) + "\r\n"

#define IRC_ERROR(nick, reason) \
    "ERROR :Closing Link: (" + (nick) + ") [" + (reason) + "]\r\n"

#define RPL_DIE_SHUTDOWN(server) \
    "ERROR :" + (server) + " shutting down\r\n"

#define RPL_KILL_TARGET(operNick, reason) \
    "ERROR :Killed by " + (operNick) + " (" + (reason) + ")\r\n"

#endif

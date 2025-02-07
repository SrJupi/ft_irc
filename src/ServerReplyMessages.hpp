#ifndef SERVERREPLYMESSAGES_HPP
# define SERVERREPLYMESSAGES_HPP

#define RPL_WELCOME(network, nick, user, host) \
    "001 " + (user) + " :Welcome to the " + (network) + \
    " Network, " + (nick) + "!" + (user) + "@" + (host) + "\r\n"

#define RPL_CHANNELMODEIS(server, nick, channel, modes, params) \
    ":" + (server) + " 324 " + (nick) + " " + (channel) + " " + (modes) + " " + (params) + "\r\n"

#define RPL_TOPIC(server, nick, channel, topic) \
    ":" + (server) + " 332 " + (nick) + " " + (channel) + " :" + (topic) + "\r\n"

#define RPL_NAMREPLY(server, nick, channel, nicks) \
    ":" + (server) + " 353 " + (nick) + " = " + (channel) + " :" + (nicks) + "\r\n"

#define RPL_ENDOFNAMES(server, nick, channel) \
    ":" + (server) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n"

#define RPL_NICK(oldNick, newNick) \
    ":" + (oldNick) + " NICK :" + (newNick) + "\r\n"

#define RPL_CAP_LS(server) \
    ":" + (server) + " CAP * LS :\r\n"

#define ERR_NOSUCHNICK(nick, receiver) \
    "401 " + (nick) + " " + (receiver) + " :No such nick/channel\r\n"

#define ERR_NOSUCHCHANNEL(server, nick, channel) \
    ":" + (server) + " 403 " + (nick) + " " + (channel) + " :No such channel\r\n"   

#define ERR_CANNOTSENDTOCHAN(nick, channel) \
    "404 " + (nick) + " " + (channel) + " :Cannot send to channel\r\n"

#define ERR_NORECIPIENT(nick, command) \
    "411 " + (nick) + " :No recipient given (" + (command) + ")\r\n"

#define ERR_NOTEXTTOSEND(nick) \
    "412 " + (nick) + " :No text to send\r\n"

#define ERR_NONICKNAMEGIVEN(server) \
    ":" + (server) + " 431 :No nickname given\r\n"

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

#define ERR_UNKNOWNMODE(server, nick, mode) \
    ":" + (server) + " 472 " + (nick) + " " + (mode) + " :is unknown mode char to me\r\n"

#define ERR_CHANOPRIVSNEEDED(server, nick, channel) \
    ":" + (server) + " 482 " + (nick) + " " + (channel) + " :You're not a channel operator\r\n"



// Forwarded message format for PRIVMSG
#define PRIVMSG(sender, receiver, message) \
    ":" + (sender) + " PRIVMSG " + (receiver) + " :" + (message) + "\r\n"

// Join Reply
#define RPL_JOIN(nick, channel) \
    ":" + (nick) + " JOIN " + (channel) + "\r\n"

// Mode Replies

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



#endif

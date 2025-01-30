#ifndef SERVERREPLYMESSAGES_HPP
# define SERVERREPLYMESSAGES_HPP

#define RPL_WELCOME(client, network, nick, user, host) \
    "001 " + (client) + " :Welcome to the " + (network) + \
    " Network, " + (nick) + "!" + (user) + "@" + (host) + "\r\n"

#define RPL_TOPIC(server, nick, channel, topic) \
    ":" + (server) + " 332 " + (nick) + " " + (channel) + " :" + (topic) + "\r\n"

#define RPL_NAMREPLY(server, nick, channel, nicks) \
    ":" + (server) + " 353 " + (nick) + " = " + (channel) + " :" + (nicks) + "\r\n"

#define RPL_ENDOFNAMES(server, nick, channel) \
    ":" + (server) + " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n"

#define ERR_NEEDMOREPARAMS(nick, command) \
    "461 " + nick + " " + command + " :Not enough parameters\r\n"

#define ERR_NOSUCHNICK(nick, receiver) \
    "401 " + (nick) + " " + (receiver) + " :No such nick/channel\r\n"

#define ERR_NOSUCHCHANNEL(nick, channelName) \
    "403 " + (nick) + " " + (channelName) + " :No such channel\r\n"    

#define ERR_CANNOTSENDTOCHAN(nick, channel) \
    "404 " + (nick) + " " + (channel) + " :Cannot send to channel\r\n"

#define ERR_NORECIPIENT(nick, command) \
    "411 " + (nick) + " :No recipient given (" + (command) + ")\r\n"

#define ERR_NOTEXTTOSEND(nick) \
    "412 " + (nick) + " :No text to send\r\n"

#define ERR_ALREADYREGISTRED(nick) \
    "462 " + nick + " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(nick, command) \
    "464 " + (nick) + " " + (command) + " :Password incorrect\r\n"


// Forwarded message format for PRIVMSG
#define PRIVMSG(sender, receiver, message) \
    ":" + (sender) + " PRIVMSG " + (receiver) + " :" + (message) + "\r\n"

// Join Reply
#define RPL_JOIN(nick, channel) \
    ":" + (nick) + " JOIN " + (channel) + "\r\n"


#endif

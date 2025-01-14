#ifndef SERVERREPLYMESSAGES_HPP
# define SERVERREPLYMESSAGES_HPP

#define RPL_WELCOME(client, network, nick, user, host) \
    (std::string(client) + " :Welcome to the " + std::string(network) + \
    " Network, " + std::string(nick) + "!" + std::string(user) + "@" + std::string(host))

#endif

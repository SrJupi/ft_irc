#include <command/Handlers.hpp>

//TODO[1]
void handleInvite(User &user, Server &server, const std::vector<std::string> &args)
{
    std::string message = ": david INVITE guido : #waka\r\n";
    send(6, message.c_str(), message.length(), 0);
    message = "341 david INVITE guido : #waka :Invitation sent\r\n";
    send(user.getFd(), message.c_str(), message.length(), 0);
    (void)server;
    (void)args;
}

/*
void CommandManager::handleInvite(int fd, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::string errorMessage = ERR_NEEDMOREPARAMS(server_ptr->getServerName(), 
                            server_ptr->getUserManager().getUserByFd(fd)->getNickname(), "INVITE");
        send(fd, errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    std::string targetNick = args[0];
    std::string channelName = args[1];

    User* inviter = server_ptr->getUserManager().getUserByFd(fd);
    Channel* channel = server_ptr->getChannelManager().getChannelByName(channelName);
    User* invitee = server_ptr->getUserManager().getUserByNick(targetNick);

    // Check if the channel exists
    if (!channel) {
        std::string errorMessage = ERR_NOSUCHCHANNEL(server_ptr->getServerName(), inviter->getNickname(), channelName);
        send(fd, errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    // Check if the inviting user is in the channel
    if (!channel->isUserInChannel(inviter->getFd())) {
        std::string errorMessage = "442 " + inviter->getNickname() + " " + channelName + " :You're not on that channel\r\n";
        send(fd, errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    // Check if the inviting user has operator privileges if the channel is invite-only (+i)
    if (channel->isInviteOnly() && !channel->isOperator(inviter->getFd())) {
        std::string errorMessage = ERR_CHANOPRIVSNEEDED(server_ptr->getServerName(), inviter->getNickname(), channelName);
        send(fd, errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    // Check if the user being invited exists
    if (!invitee) {
        std::string errorMessage = ERR_NOSUCHNICK(inviter->getNickname(), targetNick);
        send(fd, errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    // Check if the user is already in the channel
    if (channel->isUserInChannel(invitee->getFd())) {
        std::string errorMessage = "443 " + inviter->getNickname() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
        send(fd, errorMessage.c_str(), errorMessage.length(), 0);
        return;
    }

    // Send an invite message to the user
    std::string inviteMessage = ":" + inviter->getNickname() + " INVITE " + invitee->getNickname() + " :" + channelName + "\r\n";
    send(invitee->getFd(), inviteMessage.c_str(), inviteMessage.length(), 0);

    // Send confirmation to the inviter
    std::string confirmation = "341 " + inviter->getNickname() + " " + invitee->getNickname() + " " + channelName + " :Invitation sent\r\n";
    send(fd, confirmation.c_str(), confirmation.length(), 0);
}


*/
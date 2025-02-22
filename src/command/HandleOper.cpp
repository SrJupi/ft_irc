#include <command/Handlers.hpp>

#include <fstream>
#include <sstream>

// Function to get a specific value from the loaded config map
std::string getConfigValue(const std::string &key, Server &server) {
    std::map<std::string, std::string>::const_iterator it = server.getOpersData().find(key);
    if (it != server.getOpersData().end()) {
        return it->second; 
    }
    return ""; 
}

void handleOper(User &user, Server &server, const std::vector<std::string> &args)
{
    if (args.size() < 2) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "OPER"), user.getFd());
    }

    std::string userName = args[0];
    std::string passWord = args[1];

    if (passWord == getConfigValue(userName, server)) {
        server.addServerOperator(user.getFd());
        user.setOperUserName(userName);
        sendResponse(RPL_YOUREOPER(SERVER_NAME, user.getNickname()), user.getFd()); 
    } else {
        return sendResponse(ERR_PASSWDMISMATCH(user.getNickname(), "OPER"), user.getFd());
    }
}

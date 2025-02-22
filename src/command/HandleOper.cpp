#include <command/Handlers.hpp>

#include <fstream>
#include <sstream>

// Function to load a specific key from the configuration file
std::string loadConfigValue(const std::string &key) {
    std::ifstream configFile(OPER_CONFIG_FILE);
    if (!configFile.is_open()) {
        std::cerr << "Error: Could not open operator config file: " << OPER_CONFIG_FILE << std::endl;
        return "";
    }
    std::string line, foundKey, value;
    while (std::getline(configFile, line)) {
        // std::cout << line << std::endl;
        std::istringstream iss(line);
        if (std::getline(iss, foundKey, '=') && std::getline(iss, value)) {
            if (foundKey == key) {
                configFile.close();
                return value;
            }
        }
    }
    configFile.close();
    return "";
}

// Load operator username from the config file oper.irc
std::string loadOperUserName() {
    return loadConfigValue("oper_user_name");
}

// Load operator password from the config file oper.irc
std::string loadOperPassword() {
    return loadConfigValue("oper_password");
}

void handleOper(User &user, Server &server, const std::vector<std::string> &args)
{
    if (args.size() < 2) {
        return sendResponse(ERR_NEEDMOREPARAMS(SERVER_NAME, user.getNickname(), "OPER"), user.getFd());
    }

    std::string userName = args[0];
    std::string passWord = args[1];

    if (userName == loadOperUserName() && passWord == loadOperPassword()) {
        server.addServerOperator(user.getFd());
        sendResponse(RPL_YOUREOPER(SERVER_NAME, user.getNickname()), user.getFd()); 
    } else {
        return sendResponse(ERR_PASSWDMISMATCH(user.getNickname(), "OPER"), user.getFd());
    }
}

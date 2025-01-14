

#include "ClientManager.hpp"

ClientManager::ClientManager()
{
}

ClientManager::ClientManager(const ClientManager& ref)
{
    *this=ref;
}

ClientManager::~ClientManager()
{
}

ClientManager&	ClientManager::operator=(const ClientManager& ref)
{
    if (this != &ref) {
        *this=ref;
    }
    return (*this);
}

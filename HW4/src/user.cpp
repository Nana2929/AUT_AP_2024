#include "user.hpp"
#include "crypto.hpp"
#include "message.hpp"
User::User(std::string username, std::string private_key, Server *server) : username(username), private_key(private_key), server(server){};

std::string User::get_username() const { return username; }
std::string User::get_private_key() const { return private_key; }
bool User::send_text_message(std::string text, std::string receiver)
{
    // first check if receiver is in the server
    std::vector<User> registered_users = server->get_users();
    auto it = std::find_if(registered_users.begin(), registered_users.end(),
                           [&](const User &user)
                           {
                               return user.username == receiver;
                           });
    if (it == registered_users.end())
    {
        return false;
    }
    // through server
    TextMessage *msg = new TextMessage(text, this->username, receiver);
    std::string signature = crypto::signMessage(this->private_key, text);
    bool auth = server->create_message(msg, signature);
    return auth;
}

bool User::send_voice_message(std::string receiver)
{
    // first check if receiver is in the server
    std::vector<User> registered_users = server->get_users();
    auto it = std::find_if(registered_users.begin(), registered_users.end(),
                           [&](const User &user)
                           {
                               return user.username == receiver;
                           });
    if (it == registered_users.end())
    {
        return false;
    }
    VoiceMessage *vmsg = new VoiceMessage(this->username, receiver);
    std::string signature = crypto::signMessage(this->private_key, "");
    bool auth = server->create_message(vmsg, signature);
    return auth;
}

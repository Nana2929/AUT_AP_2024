#include "server.hpp"
#include "crypto.hpp"
#include "message.hpp"
#include <algorithm> // std::copy_if
#include <ctime>
#include <exception>

Server::Server(){};

std::vector<User> Server::get_users() const { return users; }
std::map<std::string, std::string> Server::get_public_keys() const
{
    return public_keys;
}
std::vector<Message *> Server::get_messages() const { return messages; }

User Server::create_user(std::string username)
{
    //!! first check if the username is taken !!
    auto it = std::find_if(this->users.begin(), this->users.end(),
                           [&](const User &user)
                           { return user.get_username() == username; });

    if (it != this->users.end())
    {
        throw std::logic_error("User name: " + username + " has been taken.");
    }
    std::string public_key, private_key;
    crypto::generate_key(public_key, private_key);
    // create the user
    User user(username, private_key, this);
    // put the user into `user` vector and public_key to `public_keys` vector
    users.push_back(user);
    public_keys[username] = public_key;
    return user;
}

bool Server::create_message(Message *msg, std::string signature)
{
    std::string sender_name = msg->get_sender();
    std::string sender_public_key = public_keys[sender_name];
    // verifySignature() needs a plain_text, but VoiceMessage does not have that

    std::string plain_text;
    if (TextMessage *text_msg = dynamic_cast<TextMessage *>(msg))
    {
        // msg is of type TextMessage, so you can safely call get_text()
        plain_text = text_msg->get_text();
    }
    else
    {
        plain_text = "";
    }

    if (!crypto::verifySignature(sender_public_key, plain_text, signature))
    {
        // abort this message
        return false;
    }
    else
    {
        // save the verified message to the Message vector
        messages.push_back(msg);
    }
    return true;
}

/* Message Retrieval */
std::vector<Message *> Server::get_all_messages_from(std::string username)
{
    std::vector<Message *> filtered_msgs;
    std::copy_if(
        this->messages.begin(), this->messages.end(),
        std::back_inserter(filtered_msgs),
        [&](Message *message)
        { return message->get_sender() == username; });
    return filtered_msgs;
};

std::vector<Message *> Server::get_all_messages_to(std::string username)
{
    std::vector<Message *> filtered_msgs;
    std::copy_if(
        this->messages.begin(),
        this->messages.end(),
        std::back_inserter(filtered_msgs),
        [&](Message *message)
        {
            return message->get_receiver() == username;
        });
    return filtered_msgs;
};

std::vector<Message *> Server::get_chat(std::string user1, std::string user2)
{
    std::vector<Message *> chat;
    std::copy_if(
        this->messages.begin(),
        this->messages.end(),
        std::back_inserter(chat),
        [=](Message *message)
        {
            return ((message->get_sender() == user1 && message->get_receiver() == user2) ||
                    (message->get_sender() == user2 && message->get_receiver() == user1));
        });
    return chat;
};

/* utility functions */
// sort by time&
void Server::sort_msgs(std::vector<Message *> &msgs)
{

    std::sort(
        msgs.begin(), msgs.end(),
        [&](Message *msg1, Message *msg2)
        {
            if (!msg1 || !msg2)
                return false;
            try
            {
                return String2Time(msg1->get_time()) < String2Time(msg2->get_time());
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error comparing times: " << e.what() << std::endl;
                return false;
            }
        });
}

time_t String2Time(std::string timeStr)
{
    struct tm tm = {};
    // Parse the string
    if (strptime(timeStr.c_str(), "%a %b %d %H:%M:%S %Y", &tm) == nullptr)
    {
        throw std::runtime_error("Failed to parse time string");
    }
    // Convert to time_t
    time_t time = mktime(&tm);
    if (time == -1)
    {
        throw std::runtime_error("Failed to parse time string");
    }
    return time;
}

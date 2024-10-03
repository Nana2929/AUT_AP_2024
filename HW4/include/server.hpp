#ifndef SERVER_H
#define SERVER_H

#include "Message.hpp"
#include "User.hpp"
#include <map>
#include <string>
#include <vector>
// forward declare

class User;

class Server
{
public:
    // Default constructor
    Server();

    // Getter functions
    std::vector<User> get_users() const;
    std::map<std::string, std::string> get_public_keys() const;
    std::vector<Message *> get_messages() const;

    // User and message management
    User create_user(std::string username); // put the user into the `users` vector
    bool create_message(Message *msg, std::string signature);

    // Message retrieval
    std::vector<Message *> get_all_messages_from(std::string username);
    std::vector<Message *> get_all_messages_to(std::string username);
    std::vector<Message *> get_chat(std::string user1, std::string user2);

    // Utility functions
    static void sort_msgs(std::vector<Message *> &msgs);

private:
    std::vector<User> users;                        // Stores all users
    std::map<std::string, std::string> public_keys; // Maps usernames to their public keys
    std::vector<Message *> messages;                // Stores all messages sent by users
};
time_t String2Time(std::string);
#endif // SERVER_H
#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class Message {
public:
  Message(std::string type, std::string sender, std::string receiver);
  Message() {
    // involving complex logic and hence cannot use initializer list
    std::time_t now = std::time(0);
    this->time = std::ctime(&now);
    this->time.erase(this->time.find_last_not_of(" \n\r\t") +
                     1); // trim the string
  };
  std::string get_type() const;
  std::string get_sender() const;
  std::string get_receiver() const;
  std::string get_time() const;

  virtual void print(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const Message &msg);

private:
  std::string type = "";
  std::string sender = "";
  std::string receiver = "";
  std::string time;
};

/* Derived: TextMessage */
class TextMessage : public Message {
public:
  TextMessage(std::string text, std::string sender, std::string receiver);
  void print(std::ostream &os) const;
  // overload the << operator
  friend std::ostream &operator<<(std::ostream &os, const Message &msg);
  std::string get_text() const;

private:
  std::string text = "";
};

/* Derived: VoiceMessage */

class VoiceMessage : public Message {
public:
  // Constructor
  VoiceMessage(std::string sender, std::string receiver);
  VoiceMessage(std::vector<unsigned char> voice, std::string sender,
               std::string receiver);

  // Member function for printing
  void print(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os,
                                  const VoiceMessage &voicemsg);

  // Getter function for the voice data
  std::vector<unsigned char> get_voice() const;

private:
  std::vector<unsigned char> voice; // Array of bytes to store the voice data
};
// overload function for std::vector<unsigned char>

std::ostream &operator<<(std::ostream &os,
                         const std::vector<unsigned char> &char_arr);

#endif // MESSAGE_HPP
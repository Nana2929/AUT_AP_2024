#include "message.hpp"
#include <algorithm> // `std::generate`
#include <climits>   // `CHAR_BIT`
#include <random>
#include <string>
using random_bytes_engine =
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT,
                                 unsigned char>;

/* Message */
Message::Message(std::string type, std::string sender, std::string receiver)
    : type(type), sender(sender), receiver(receiver)
{
    std::time_t now = std::time(0);
    this->time = std::ctime(&now);
    this->time.erase(this->time.find_last_not_of(" \n\r\t") +
                     1); // trim the string
}
std::string Message::get_type() const { return type; }
std::string Message::get_sender() const { return sender; }
std::string Message::get_receiver() const { return receiver; }
std::string Message::get_time() const { return time; }

void Message::print(std::ostream &os) const
{
    std::string bound = std::string(15, '*');
    os << bound << std::endl
       << this->sender << " -> " << this->receiver << std::endl;
    os << "message type: " << this->type << std::endl;
    os << "message time: " << this->time << std::endl;
    os << bound << std::endl;
}

/* Derived: TextMessage */
TextMessage::TextMessage(std::string text, std::string sender,
                         std::string receiver)
    : Message("text", sender, receiver), text(text){};

std::string TextMessage::get_text() const { return text; }

void TextMessage::print(std::ostream &os) const
{
    std::string bound = std::string(15, '*');
    os << bound << std::endl
       << this->get_sender() << " -> " << this->get_receiver() << std::endl;
    os << "message type: " << this->get_type() << std::endl;
    os << "message time: " << this->get_time() << std::endl;
    os << "text: " << this->text << std::endl;
    os << bound << std::endl;
}

/* Derived: VoiceMessage */
VoiceMessage::VoiceMessage(std::string sender, std::string receiver)
    : Message("voice", sender, receiver)
{
    random_bytes_engine rbe;
    std::vector<unsigned char> data(5);
    std::generate(begin(data), end(data), std::ref(rbe));
    this->voice = data;
};

VoiceMessage::VoiceMessage(std::vector<unsigned char> voice, std::string sender,
                           std::string receiver)
    : Message("voice", sender, receiver)
{

    // check if the voice array is 5 bytes (for simplicity)
    if (voice.size() != 5)
    {
        throw std::invalid_argument("voice size should be 5");
    }
    this->voice = voice;
}

void VoiceMessage::print(std::ostream &os) const
{
    std::string bound = std::string(15, '*');
    os << bound << std::endl
       << this->get_sender() << " -> " << this->get_receiver() << std::endl;
    os << "message type: " << this->get_type() << std::endl;
    os << "message time: " << this->get_time() << std::endl;
    os << "voice: " << this->get_voice() << std::endl;
    os << bound << std::endl;
}
std::vector<unsigned char> VoiceMessage::get_voice() const
{
    return this->voice;
}

/* friend functions */
std::ostream &operator<<(std::ostream &os, const VoiceMessage &voicemsg)
{
    voicemsg.print(os);
    return os;
}
std::ostream &operator<<(std::ostream &os, const TextMessage &textmsg)
{
    textmsg.print(os);
    return os;
}
std::ostream &operator<<(std::ostream &os, const Message &msg)
{
    msg.print(os);
    return os;
}

// overload function for std::vector<unsigned char>
std::ostream &operator<<(std::ostream &os,
                         const std::vector<unsigned char> &char_arr)
{
    for (size_t i = 0; i < char_arr.size(); i++)
    {
        os << static_cast<int>(char_arr[i])
           << (i < char_arr.size() - 1 ? ", " : "");
    }
    return os;
}
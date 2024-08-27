#include "Person.h"
#include "Utils.h"
#include <functional>
#include <stdexcept> // For std::invalid_argument
#include <string>
#include <compare> // For std::strong_ordering
// Constructo
Person::Person(std::string &name, size_t age, std::string &gender,
               std::string &fingerprint, size_t socioeconomic_rank, bool is_alive) : name(name), gender(gender),
                                                                                     hashed_fingerprint(std::hash<std::string>{}(fingerprint))

// const member variables are initialized in the member initializer list
{
    // validate gender
    if (!set_gender(gender))
    {
        throw std::invalid_argument("Gender is restricted to Male or Female");
    }
    if (!set_socioeconomic_rank(socioeconomic_rank))
    {
        throw std::invalid_argument("Socioeconomic rank (integer) should be in the range [1, 10]");
    }
    set_age(age);
    set_is_alive(is_alive);
};

// ** Getters **
// getters are const functions that do not modify the object

std::string Person::get_name() const
{
    return this->name;
}
size_t Person::get_age() const
{
    return this->age;
}
std::string Person::get_gender() const
{
    return this->gender;
}
size_t Person::get_hashed_fingerprint() const
{
    return this->hashed_fingerprint;
}
size_t Person::get_socioeconomic_rank() const
{
    return this->socioeconomic_rank;
}
bool Person::get_is_alive() const
{
    return this->is_alive;
}

// ** Setters **
// Setters involve validation of the input and then setting the value

bool Person::set_age(size_t age)
{
    this->age = age;
    return true;
}

bool Person::set_socioeconomic_rank(size_t rank)
{
    if (rank > 10 || rank < 1)
    {
        throw std::invalid_argument("Socioeconomic rank (integer) should be in the range [1, 10]");
        return false;
    }
    this->socioeconomic_rank = rank;
    return true;
}

bool Person::set_is_alive(bool is_alive)
{
    this->is_alive = is_alive;
    return true;
}

bool Person::set_gender(const std::string gender)
{
    if (gender != "Female" && gender != "Male")
    {
        return false;
    }
    return true;
}

// Spaceship implementation
// (since C++20)
// a non-static method cannot be declared inline?
std::strong_ordering Person::operator<=>(const Person &other) const
{
    return this->hashed_fingerprint <=> other.hashed_fingerprint; // the comparison is based on the fingerprint (lexicographical)
}

// Outputs person information, supports writing to file

void Person::get_info(std::optional<std::string> file_name) const
{
    std::string info = "Name: " + this->name + "\n" +
                       "Age: " + std::to_string(this->age) + "\n" +
                       "Gender: " + this->gender + "\n" +
                       "Fingerprint: " + std::to_string(this->hashed_fingerprint) + "\n" +
                       "Socioeconomic Rank: " + std::to_string(this->socioeconomic_rank) + "\n" +
                       "Is Alive: " + std::to_string(this->is_alive) + "\n";
    if (!file_name.has_value())
    {
        file_name = "test_person_info.txt";
    }
    Utils::write_to_file(file_name.value(), info);
}
#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"
#include <random>
#include <string>
#include <unordered_set>
#include <stdexcept> // For std::invalid_argument
#include <compare>   // For std::strong_ordering
#include <regex>     // For std::regex

//  In C++, static members need to be defined outside the class declaration, even though they are declared inside the class.
std::unordered_set<std::string> Account::used_account_ids;


Account::Account(const Person *const owner, const Bank *const bank, std::string &password) : bank(bank),account_number(generate_account_id()),
balance(0.0), loan(0.0),
account_status(true), CVV2("0000"), password(password), exp_date("24-08")
{
    this->owner = const_cast<Person *>(owner);
    // !!why passing in a const pointer if you're going to cast it to a non-const pointer??
};

Account::~Account()
{
    used_account_ids.erase(account_number);
}

// * Account id generator
std::string Account::generate_account_id()
{
    // a 16-digit string
    std::string account_id;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 9);
    while (true)
    {
        account_id = "";
        for (int i = 0; i < 16; i++)
        {
            account_id += std::to_string(dis(gen));
        }
        if (used_account_ids.find(account_id) == used_account_ids.end())
        {
            break;
        }
    }
    used_account_ids.insert(account_id);
    std::cout << "Account ID: " << account_id << std::endl;
    return account_id;
}

// * Getters
const Person *Account::get_owner() const
{
    return this->owner;
}
double Account::get_balance() const
{
    return this->balance;
}
double Account::get_loan() const
{
    return this->loan;
}


std::string Account::get_account_number() const
{
    return this->account_number;
}

bool Account::get_status() const
{
    return this->account_status;
}
// * Certified getters

std::string Account::get_CVV2(std::string &owner_fingerprint) const
{
    // get the hashed finger print
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        return this->CVV2;
    }
    throw std::invalid_argument("[CVV2] Owner fingerprint does not pass authentication");
}

std::string Account::get_password(std::string &owner_fingerprint) const
{
    // get the hashed finger print
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        return this->password;
    }
    throw std::invalid_argument("[Password] Owner fingerprint does not pass authentication");
}

std::string Account::get_exp_date(std::string &owner_fingerprint) const
{
    // get the hashed finger print
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        return this->exp_date;
    }
    throw std::invalid_argument("[Expiration Date] Owner fingerprint does not pass authentication");
}

// * Setters

bool Account::set_password(std::string &password, std::string &owner_fingerprint)
{
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        this->password = password;
        return true;
    }
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
}

bool Account::set_balance(double amount, std::string &owner_fingerprint)
{
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        if (amount < 0)
        {
            throw std::invalid_argument("Amount cannot be negative");
        }
        this->balance = amount;
        return true;
    }
    throw std::invalid_argument("[Setting balance] Owner fingerprint does not pass authentication");
}

// for Bank::transfer
bool Account::increase_balance(double increase)
{
    // check if increase is negative
    if (increase < 0)
    {
        throw std::invalid_argument("Amount cannot be negative");
    }
    this->balance += increase;
    return true;
}


bool Account::set_loan(double amount, std::string &owner_fingerprint)
{
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        if (amount < 0)
        {
            throw std::invalid_argument("Amount cannot be negative");
        }
        this->loan = amount;
        return true;
    }
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
}

bool Account::set_owner(const Person *new_owner, size_t hashed_fingerprint)
{
    if (hashed_fingerprint == this->owner->get_hashed_fingerprint())
    {
        this->owner = const_cast<Person *>(new_owner);
        return true;
    }
    throw std::invalid_argument("Original owner fingerprint does not pass authentication");
}

bool Account::set_status(bool status, size_t owner_fingerprint)
{
    if (owner_fingerprint == this->owner->get_hashed_fingerprint())
    {
        this->account_status = status;
        return true;
    }
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
}

bool Account::set_exp_date(std::string &exp_date, size_t owner_fingerprint)
{

    if (owner_fingerprint == this->owner->get_hashed_fingerprint())
    {
        std::regex expDatePattern(R"(\d{2}-\d{2})");
        if (!std::regex_match(exp_date, expDatePattern))
        {
            throw std::invalid_argument("Invalid expiration date format");
        }
        this->exp_date = exp_date;
        return true;
    }
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
}


// * spaceship Operator
std::strong_ordering Account::operator<=>(const Account &other) const
{
    return account_number <=> other.account_number;
}

// * Outputs account information
void Account::get_info(std::optional<std::string> file_name) const
{
    // write to file
    std::string info;
    info += "Account Number: " + this->account_number + "\n";
    info += "Owner: " + this->owner->get_name() + "\n";
    info += "Bank: " + this->bank->get_bank_name() + "\n";
    info += "Balance: " + std::to_string(this->balance) + "\n";
    info += "Status: " + (this->account_status == true) ? "Active" : "Inactive";
    // Exclude credentials from the output. If a file name is provided, write the information to a file; otherwise, print it to the terminal. Ensure the output is well-formatted and informative.

    if (file_name.has_value())
    {
        Utils::write_to_file(file_name.value(), info);
    }
    else
    {
        std::cout << info;
    }
}
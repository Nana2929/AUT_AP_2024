#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"
#include <random>
#include <string>
#include <unordered_set>
#include <stdexcept> // For std::invalid_argument
#include <compare>   // For std::strong_ordering

Account::Account(const Person *const owner, const Bank *const bank, std::string &password) : bank(bank), password(password), balance(0), account_status(true)
{
    this->owner = const_cast<Person *>(owner);
    // !!why passing in a const pointer if you're going to cast it to a non-const pointer??
    generate_account_id();
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
    while (used_account_ids.find(account_id) != used_account_ids.end())
    {
        account_id = "";
        for (int i = 0; i < 16; i++)
        {
            account_id += std::to_string(dis(gen));
        }
    }
    // add the account id to the set
    used_account_ids.insert(account_id);
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
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
}

std::string Account::get_password(std::string &owner_fingerprint) const
{
    // get the hashed finger print
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        return this->password;
    }
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
}

std::string Account::get_exp_date(std::string &owner_fingerprint) const
{
    // get the hashed finger print
    if (std::hash<std::string>{}(owner_fingerprint) == this->owner->get_hashed_fingerprint())
    {
        return this->exp_date;
    }
    throw std::invalid_argument("Owner fingerprint does not pass authentication");
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
    info += "Status: " + (this->account_status) ? "Active" : "Inactive"; 
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
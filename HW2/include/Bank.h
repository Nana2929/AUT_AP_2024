#ifndef BANK_H // Prevents double inclusion of this header
#define BANK_H

#include <compare>  // For std::strong_ordering
#include <map>      // For std::map
#include <optional> // For std::optional
#include <string>   // For std::string
#include <vector>   // For std::vector

class Account; // Forward declaration of Account
class Person; // Forward declaration of Person

// Represents a banking institution
class Bank {
public:
    // Constructor with bank name and security fingerprint
    Bank(const std::string& bank_name, const std::string& bank_fingerprint);

    ~Bank(); // Destructor

    // Bank operations
    Account* create_account(Person& owner, const std::string& owner_fingerprint, std::string password);
    bool delete_account(Account& account, const std::string& owner_fingerprint);

    // Financial operations
    bool deposit(Account& account, const std::string& owner_fingerprint, double amount);
    bool withdraw(Account& account, const std::string& owner_fingerprint, double amount);
    // owner_fingerprint, CVV2, password, exp_date belongs to the source account
    bool transfer(Account& source, Account& destination, const std::string& owner_fingerprint,
                  const std::string& CVV2, const std::string& password, const std::string& exp_date, double amount);
    bool take_loan(Account& account, const std::string& owner_fingerprint, double amount);
    bool pay_loan(Account& account, double amount);
    // Deleting Customers: Remove all information related to a person from the bank's records without "deleting the person.", Remember, deleting a customer's account doesn't mean we're deleting the person!


    bool delete_customer(Person& owner, const std::string& owner_fingerprint);


    // Bank authentication
    bool authenticate_bank(std::string& bank_fingerprint) const;

    // Getters
    const std::string& get_bank_name() const;
    size_t get_hashed_bank_fingerprint() const;

    // Getters requiring bank authentication
    const std::vector<Person*>& get_bank_customers(std::string& bank_fingerprint) const;
    const std::vector<Account*>& get_bank_accounts(std::string& bank_fingerprint) const;
    const std::map<Account*, Person*>& get_account_2_customer_map(std::string& bank_fingerprint) const;
    const std::map<Person*, std::vector<Account*>>& get_customer_2_accounts_map(std::string& bank_fingerprint) const;
    const std::map<Person*, double>& get_customer_2_paid_loan_map(std::string& bank_fingerprint) const;
    const std::map<Person*, double>& get_customer_2_unpaid_loan_map(std::string& bank_fingerprint) const;
    double get_bank_total_balance(std::string& bank_fingerprint) const;
    double get_bank_total_loan(std::string& bank_fingerprint) const;

    // Account Setters requiring owner's and bank's authentication
    // 好像沒有 unittest 來測試這個函數
    // bool set_owner(Account& account, const Person* new_owner, std::string& owner_fingerprint, std::string& bank_fingerprint);
    // Account Setters requiring bank authentication
    bool set_account_status(Account& account, bool status, std::string& bank_fingerprint);
    bool set_exp_date(Account& account, std::string& exp_date, std::string& bank_fingerprint);

    // Outputs bank information, supports writing to file
    void get_info(std::optional<std::string> file_name = std::nullopt) const;

private:
    // Private member variables
    const std::string bank_name;
    const size_t hashed_bank_fingerprint;
    // let’s note that a vector of owning raw pointers is not recommended in modern C++ (even using owning raw pointers without a vector are not recommended in modern C++). std::unique_ptr and other smart pointers offer safer and more expressive alternative since C++11.
    std::vector<Person*> bank_customers;
    std::vector<Account*> bank_accounts; // if we can use Linked List to replace vector many operations could be more efficient
    std::map<Account*, Person*> account_2_customer;
    std::map<Person*, std::vector<Account*>> customer_2_accounts;
    // _paid_loan tracks the total amount paid by each customer to the bank; used for upgrading customer's socioeconomic rank
    std::map<Person*, double> customer_2_paid_loan;
    // _unpaid_loan tracks the current unpaid loan amount for each customer; used for bank to 討債
    std::map<Person*, double> customer_2_unpaid_loan;
    double bank_total_balance = 0.0; // Total bank profit
    double bank_total_loan = 0.0; // Total loans issued
};

#endif // BANK_H

#include "Bank.h"
#include "Person.h"
#include "Account.h"
#include "Utils.h"
#include <functional>

Bank::Bank(const std::string &bank_name, const std::string &bank_fingerprint) : bank_name(bank_name),
                                                                                hashed_bank_fingerprint(std::hash<std::string>{}(bank_fingerprint)) {};
Bank::~Bank() {};

bool Bank::authenticate_bank(const std::string &bank_fingerprint)
{
    bool res = std::hash<std::string>{}(bank_fingerprint) == this->hashed_bank_fingerprint;
    return res;
}

//* Bank operations
Account *Bank::create_account(Person &owner, const std::string &owner_fingerprint, std::string password)
{

    // Naming convention: always mark a pointer with {variable name]_ptr or p{variable_name}
    if (!authenticate_bank(owner_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    Account *act_ptr = new Account(&owner, this, password);
    this->bank_accounts.push_back(act_ptr);
    this->bank_customers.push_back(&owner);

    this->account_2_customer[act_ptr] = &owner;
    this->customer_2_accounts[&owner].push_back(act_ptr);
    return act_ptr;
}

bool Bank::delete_account(Account& account, const std::string& owner_fingerprint)
{
    if (!authenticate_bank(owner_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    auto it = std::find(this->bank_accounts.begin(), this->bank_accounts.end(), &account);
    auto act_owner = this->account_2_customer[&account];
    if (it != this->bank_accounts.end())
    {
        this->bank_accounts.erase(it);
        this->account_2_customer.erase(&account);
        auto it2 = std::find(this->customer_2_accounts[act_owner].begin(), this->customer_2_accounts[act_owner].end(), &account);
        this->customer_2_accounts[act_owner].erase(it2);
        return true;
    }
    return false;
}

// Financial operations
// 存錢
bool Bank::deposit(Account& accout, const std::string& owner_fingerprint, double amount){
    if (!authenticate_bank(owner_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    accout.set_balance(accout.get_balance() + amount, owner_fingerprint);
}

// 領錢
bool Bank::withdraw(Account& account, const std::string& owner_fingerprint, double amount){
    if (!authenticate_bank(owner_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    // check if the account has enough balance
    if (account.get_balance() < amount)
    {
        throw std::invalid_argument("Account does not have enough balance: " +
             std::to_string(account.get_balance()) + " < withdrawal request " + std::to_string(amount));
    }
    account.set_balance(account.get_balance() - amount, owner_fingerprint);
}
// transfer

bool Bank::transfer(Account& source, Account& destination,
const std::string& owner_fingerprint, const std::string& CVV2,
const std::string& password, const std::string& exp_date, double amount){

    if (!authenticate_bank(owner_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    // make a copy of owner_fingerprint
    std::string owner_fingerprint_copy = owner_fingerprint;
    std::string acc_CVV2 = source.get_CVV2(owner_fingerprint_copy);
    std::string acc_password = source.get_password(owner_fingerprint_copy);
    std::string acc_exp_date = source.get_exp_date(owner_fingerprint_copy);
    if (acc_CVV2 == CVV2 && acc_password == password && acc_exp_date == exp_date)
    {
        // withdraw from source (exception will be thrown if not enough balance)
        withdraw(source, owner_fingerprint, amount);
        // deposit to destination
        deposit(destination, owner_fingerprint, amount);
        return true;
    }
    else{
        // else the authentication failed
        throw std::invalid_argument("At least one of owner fingerprint, CVV2, password, or exp_date does not pass authentication");
    }
}

/*
Owner Authentication: Verify the owner's identity by comparing the hash of the provided fingerprint against the owner's stored hashed fingerprint.
- Loan Amount Calculation: The maximum loan amount a customer can request is determined by their socioeconomic rank. Specifically, a customer can borrow up to (10 * rank)% of the total balance across all their accounts.
- For example, if a customer has a socioeconomic rank of 8 and a total balance of $10,000 across all accounts, they can request a loan of up to 80% of $10,000, which equals $8,000.
- Total Loan Limit: Ensure the requested loan does not exceed the customer's loan limit, considering their current unpaid loans.
- Loan Distribution: If the loan request is approved, distribute the loan amount to the specified account and update the relevant loan tracking variables (bank_total_loan, customer_2_unpaid_loan).*/
bool Bank::take_loan(Account& account, const std::string& owner_fingerprint, double amount){
    // !! I acutally don't know how to `distribute the loan amount to the specified account`
    // because in my opinion, the loan amount shall be keeping under `customer` instead of `account` 

}



//* getters */

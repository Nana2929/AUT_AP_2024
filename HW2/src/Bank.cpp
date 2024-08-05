#include "Bank.h"
#include "Person.h"
#include "Account.h"
#include "Utils.h"
#include <functional>
#include <algorithm>
#include <cmath>

Bank::Bank(const std::string &bank_name, const std::string &bank_fingerprint) : bank_name(bank_name),
                                                                                hashed_bank_fingerprint(std::hash<std::string>{}(bank_fingerprint)) {};
Bank::~Bank() {};

bool Bank::authenticate_bank(std::string &bank_fingerprint) const
{
    bool res = std::hash<std::string>{}(bank_fingerprint) == this->hashed_bank_fingerprint;
    return res;
}

//* Bank operations
Account *Bank::create_account(Person &owner, const std::string &owner_fingerprint, std::string password)
{

    // Naming convention: always mark a pointer with {variable name]_ptr or p{variable_name}
    if (owner.get_hashed_fingerprint() != std::hash<std::string>{}(owner_fingerprint))
    {
        throw std::invalid_argument("Owner fingerprint does not pass authentication");
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

    auto it = std::find(this->bank_accounts.begin(), this->bank_accounts.end(), &account);
    Person* act_owner = this->account_2_customer[&account];

    if (act_owner->get_hashed_fingerprint() != std::hash<std::string>{}(owner_fingerprint))
    {
        throw std::invalid_argument("Account Deletion Failed. Owner fingerprint does not pass authentication");
    }
    auto it2 = this->customer_2_unpaid_loan.find(act_owner);
    if (it2->second > 0)
    {
        throw std::invalid_argument("Account Deletion Failed. Owner has unpaid loans: " + std::to_string(it2->second));
    }

    if (it != this->bank_accounts.end())
    {
        this->bank_accounts.erase(it);
        this->account_2_customer.erase(&account);
        auto it2 = std::find(this->customer_2_accounts[act_owner].begin(), this->customer_2_accounts[act_owner].end(), &account);
        this->customer_2_accounts[act_owner].erase(it2);
        return true;
    }
    return false;// if the account is not found
}

bool Bank::delete_customer(Person& owner, const std::string& owner_fingerprint){
    // * check if this owner belongs to the bank
    auto it = std::find(this->bank_customers.begin(), this->bank_customers.end(), &owner);
    if (it == this->bank_customers.end())
    {
        throw std::invalid_argument("Owner does not belong to the bank");
    }
    // check if the owner fingerprint is correct
    if (owner.get_hashed_fingerprint() != std::hash<std::string>{}(owner_fingerprint))
    {
        throw std::invalid_argument("Owner fingerprint does not pass authentication");
    }
    // * check if the owner has unpaid loans
    // std::map<Person*, double> customer_2_unpaid_loan;
    auto it2 = this->customer_2_unpaid_loan.find(&owner);
    if (it2 != this->customer_2_unpaid_loan.end() && it2->second > 0)
    {
        std::string msg = "Owner has unpaid loans: " + std::to_string(it2->second);
        throw std::invalid_argument(msg);
    }
    //* Delete all accounts of the owner (account_2_customer, customer_2_accounts)
    for (auto acc: this->customer_2_accounts[&owner]){
        // remove the acc key in account_2_customer
        this->account_2_customer.erase(acc);
        // remove the acc from bank_accounts
    }
    std::unordered_set<Account*> acc_to_del(
        this->customer_2_accounts[&owner].begin(), this->customer_2_accounts[&owner].end());
    // !! the very convenient erase + remove(if) idiom!
    this->bank_accounts.erase(std::remove_if(this->bank_accounts.begin(),
        this->bank_accounts.end(),
        [&acc_to_del](Account* acc){
            return acc_to_del.find(acc) != acc_to_del.end();
        }), this->bank_accounts.end());

    // 新知識...
    // !! in C++, when you have a reference to an object and you use the address-of operator (&) on that reference,
    // !! you get a pointer to the object.
    customer_2_accounts.erase(&owner); // remove the owner key in customer_2_accounts

    // * Remove key of the customer of customer_2_paid_loan, customer_2_unpaid_loan
    this->customer_2_paid_loan.erase(&owner);
    this->customer_2_unpaid_loan.erase(&owner);
    // * Remove owner from bank customers
    // https://www.reddit.com/r/cpp_questions/comments/15smdyk/question_about_vectors_of_pointers_and_erase/
    // move all elements pointe to owner to the end of the vector
    this->bank_customers.erase(std::remove(this->bank_customers.begin(), this->bank_customers.end(), &owner),
                    this->bank_customers.end());
    return true;
}



// ** Financial operations */
// 存錢
bool Bank::deposit(Account& accout, const std::string& owner_fingerprint, double amount){
    auto owner_fingerprint_copy = owner_fingerprint;
    // exception will be thrown if the owner_fingerprint is wrong
    accout.set_balance(accout.get_balance() + amount, owner_fingerprint_copy);
    return true;
}

// 領錢
bool Bank::withdraw(Account& account, const std::string& owner_fingerprint, double amount){
    // check if the account has enough balance
    if (account.get_balance() < amount)
    {
        throw std::invalid_argument("Account does not have enough balance: " +
             std::to_string(account.get_balance()) + " < withdrawal request " + std::to_string(amount));
    }
    std::string owner_fingerprint_copy = owner_fingerprint;
    account.set_balance(account.get_balance() - amount, owner_fingerprint_copy);
    return true;
}
// transfer

bool Bank::transfer(Account& source, Account& destination,
const std::string& owner_fingerprint, const std::string& CVV2,
const std::string& password, const std::string& exp_date, double amount){

    // make a copy of owner_fingerprint
    std::string src_fingerprint_copy = owner_fingerprint;
    std::string acc_CVV2 = source.get_CVV2(src_fingerprint_copy );
    std::string acc_password = source.get_password(src_fingerprint_copy);
    std::string acc_exp_date = source.get_exp_date(src_fingerprint_copy );
    if (acc_CVV2 == CVV2 && acc_password == password && acc_exp_date == exp_date)
    {
        // withdraw from source (exception will be thrown if not enough balance)
        withdraw(source, owner_fingerprint, amount);
        // deposit to destination but NO NEED to authenticate the fingerprint (logic is a bit weird though)
        destination.increase_balance(amount);
        return true;
    }
    else{
        // else the authentication failed
        throw std::invalid_argument("At least one of owner fingerprint, CVV2, password, or exp_date does not pass authentication");
    }
}

// ** Loaning */
/*
12. Taking a Loan
Owner Authentication: Verify the owner's identity by comparing the hash of the provided fingerprint against the owner's stored hashed fingerprint.
- Loan Amount Calculation: The maximum loan amount a customer can request is determined by their socioeconomic rank. Specifically, a customer can borrow up to (10 * rank)% of the total balance across all their accounts.
- For example, if a customer has a socioeconomic rank of 8 and a total balance of $10,000 across all accounts, they can request a loan of up to 80% of $10,000, which equals $8,000.
- Total Loan Limit: Ensure the requested loan does not exceed the customer's loan limit, considering their current unpaid loans.
- Loan Distribution: If the loan request is approved, distribute the loan amount to the specified account and update the relevant loan tracking variables (bank_total_loan, customer_2_unpaid_loan).*/
bool Bank::take_loan(Account& account, const std::string& owner_fingerprint, double amount){

    // make a copy of owner_fingerprint
    std::string owner_fingerprint_copy = owner_fingerprint;
    // check if the account belongs to the bank
    if (this->account_2_customer.find(&account) == this->account_2_customer.end())
    {
        throw std::invalid_argument("Account does not belong to the bank");
    }
    // should not happen though, as the account_2_customer and customer_2_accounts should be in sync
    auto it = this->account_2_customer.find(&account);
    if (it == this->account_2_customer.end()) {
        throw std::invalid_argument("No customers found for the account");
    }
    Person* customer = it->second;
    // identity checks
    if (this->account_2_customer[&account]->get_hashed_fingerprint() != std::hash<std::string>{}(owner_fingerprint_copy))
    {
        throw std::invalid_argument("Owner fingerprint does not pass authentication");
    }
    // calculate the maximum loan amount
    size_t socioeconomic_rank = customer->get_socioeconomic_rank();
    // (10 / rank)%
    double total_balances = 0.;
    for (auto acc: this->customer_2_accounts[customer]){
        total_balances += acc->get_balance();
    }
    double currUnpaidLoan;
    if (this->customer_2_unpaid_loan.find(customer) == this->customer_2_unpaid_loan.end())
    {
        currUnpaidLoan = 0.;
        // No loan record should exist for the customer after failed loan attempt.
    }
    else{
        currUnpaidLoan = this->customer_2_unpaid_loan[customer];
    }
    // currently allowed maximum loan
    double requestedLoanAmtWithInterest = amount * (1 + (10. / (double) socioeconomic_rank / 100.));
    double currMaxLoan = total_balances * 10 * (double) socioeconomic_rank / 100.;
    // check if the requested loan exceeds the customer's maximum loan amount
    // Attempt to take a loan exceeding the eligibility
    if (requestedLoanAmtWithInterest > (currMaxLoan - currUnpaidLoan))
    {
        throw std::invalid_argument("Requested loan (with interest) " + std::to_string(requestedLoanAmtWithInterest) +
        " exceeds the remaining loan amount: " + std::to_string(currMaxLoan - currUnpaidLoan));
    }

    // update the loan tracking variables
    this->bank_total_loan += requestedLoanAmtWithInterest;
    this->customer_2_unpaid_loan[this->account_2_customer[&account]] += requestedLoanAmtWithInterest;
    // update the account's loan
    account.set_loan(account.get_loan() + requestedLoanAmtWithInterest, owner_fingerprint_copy);
    return true;
}
/*### 13. Paying a Loan

When a customer pays back part or all of a loan, the bank processes the payment as follows:

- **Interest**: Incorporate an additional `10/rank` percent interest charge on the loan payment. The interest serves as the bank's profit and is added to `bank_total_balance`.
    - this amount is calculated when you take a loan, not when you pay it.
- **Loan Update**: Deduct the payment from the customer's total unpaid loan and update `customer_2_paid_loan` and `customer_2_unpaid_loan` accordingly.
- **Socioeconomic Rank Upgrade**: After the payment, check if the customer's total paid loan reaches a threshold for upgrading their socioeconomic rank. The threshold is `10^rank`. If the threshold is met, increase the customer's rank by one.
  - For instance, if a customer with a rank of 4 pays off a total loan amount reaching $10,000, their rank is upgraded to 5.
*/
bool Bank::pay_loan(Account& account, double amount){
    //* Checking
    // check if the account belongs to the bank
    if (this->account_2_customer.find(&account) == this->account_2_customer.end())
    {
        throw std::invalid_argument("Account does not belong to the bank");
    }
    // should not happen though, as the account_2_customer and customer_2_accounts should be in sync
    auto it = this->account_2_customer.find(&account);
    if (it == this->account_2_customer.end()) {
        throw std::invalid_argument("No customers found for the account");
    }

    Person* customer = it->second;
    double curr_unpaid_loan = this->customer_2_unpaid_loan[customer];
    // you only pay back as much or less
    double amt = std::min(amount, curr_unpaid_loan);
    // * Updating customer's paid and unpaid loan
    this->customer_2_unpaid_loan[customer] -= amt;
    this->customer_2_paid_loan[customer] += amt;
    // * Updating bank's total balance and loan
    // !! Different Banks create different Persons for "Emma".
    // !! so Emma can have different socio-economic ranks in different banks.
    size_t socio_ecorank = customer->get_socioeconomic_rank();
    this->bank_total_loan -= amt;
    // * Customer's Socioeconomic Rank Upgrade
    double threshold = std::pow(10, socio_ecorank);
    if (this->customer_2_paid_loan[customer] >= threshold)
    {
        customer->set_socioeconomic_rank(socio_ecorank + 1);
    }
    return true;
}


//* getters */

const std::string& Bank::get_bank_name() const{
    return this->bank_name;
}
size_t Bank::get_hashed_bank_fingerprint() const{
    return this->hashed_bank_fingerprint;
}

// Getters requiring bank authentication
const std::vector<Person*>& Bank::get_bank_customers(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->bank_customers;
}
const std::vector<Account*>& Bank::get_bank_accounts(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->bank_accounts;

}
const std::map<Account*, Person*>& Bank::get_account_2_customer_map(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->account_2_customer;
}
const std::map<Person*, std::vector<Account*>>& Bank::get_customer_2_accounts_map(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->customer_2_accounts;
}
const std::map<Person*, double>& Bank::get_customer_2_paid_loan_map(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->customer_2_paid_loan;
}
const std::map<Person*, double>& Bank::get_customer_2_unpaid_loan_map(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->customer_2_unpaid_loan;
}
double Bank::get_bank_total_balance(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->bank_total_balance;
}
double Bank::get_bank_total_loan(std::string& bank_fingerprint) const{
    if (!authenticate_bank(bank_fingerprint))
    {
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    return this->bank_total_loan;
}

// Account Setters requiring owner and bank authentication

// Account Setters requiring bank authentication
bool Bank::set_account_status(Account& account, bool status, std::string& bank_fingerprint){
    if (!authenticate_bank(bank_fingerprint)){
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    // account to customer
    Person* customer = this->account_2_customer[&account];
    size_t owner_hashed_fingerprint = customer->get_hashed_fingerprint();
    account.set_status(status, owner_hashed_fingerprint);
    return true;
}
bool Bank::set_exp_date(Account& account, std::string& exp_date, std::string& bank_fingerprint){
    if (!authenticate_bank(bank_fingerprint)){
        throw std::invalid_argument("Bank fingerprint does not pass authentication");
    }
    // account to customer
    Person* customer = this->account_2_customer[&account];
    size_t owner_hashed_fingerprint = customer->get_hashed_fingerprint();
    account.set_exp_date(exp_date, owner_hashed_fingerprint);
    return true;
}

// Outputs bank information, supports writing to file
void Bank::get_info(std::optional<std::string> file_name) const{
    std::string info = "Bank Name: " + this->bank_name + "\n";
    info += "Bank Fingerprint: " + std::to_string(this->hashed_bank_fingerprint) + "\n";
    info += "Bank Total Balance: " + std::to_string(this->bank_total_balance) + "\n";
    info += "Bank Total Loan: " + std::to_string(this->bank_total_loan) + "\n";
    if (file_name.has_value())
    {
        Utils::write_to_file(file_name.value(), info);
    }
    else{
        std::cout << info;
    }

}


// 這個的邏輯很麻煩而且好像 spec / unit-tests 都不太清楚，所以先跳過
// bool Bank::set_owner(Account& account, const Person* new_owner, std::string& owner_fingerprint, std::string& bank_fingerprint){
//     // if (!authenticate_bank(bank_fingerprint))
//     // {
//     //     throw std::invalid_argument("Bank fingerprint does not pass authentication");
//     // }
//     // if (this->account_2_customer.find(&account) == this->account_2_customer.end())
//     // {
//     //     throw std::invalid_argument("Account does not belong to the bank");
//     // }
//     // // set the owner
//     // account.set_owner(new_owner, owner_fingerprint); // this will throw an exception if the owner_fingerprint is wrong
//     // update the account_2_customer and customer_2_accounts

//     // add to the bank_customers if not already there

//     // fuck updating ctomer_2-unpiad_loan is an effort
//     return true;
// }


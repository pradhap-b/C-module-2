
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sstream>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    string customerName;
    long long balance; // Balance stored in paise

public:
    BankAccount(int accNo, string name, long long amount) {
        accountNumber = accNo;
        customerName = name;
        balance = amount;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    string getCustomerName() const {
        return customerName;
    }

    long long getBalance() const {
        return balance;
    }

    void deposit(long long amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!\n";
            return;
        }

        if (balance > numeric_limits<long long>::max() - amount) {
            cout << "Amount exceeds account limit!\n";
            return;
        }

        balance += amount;
        cout << "Deposit successful!\n";
    }

    void withdraw(long long amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!\n";
        }
        else if (amount > balance) {
            cout << "Insufficient balance!\n";
        }
        else {
            balance -= amount;
            cout << "Withdrawal successful!\n";
        }
    }

    void displayBalance() const {
        cout << "\nAccount Number : " << accountNumber;
        cout << "\nCustomer Name  : " << customerName;
        cout << "\nBalance        : Rs. "
             << fixed << setprecision(2)
             << balance / 100.0 << "\n";
    }
};

class Bank {
private:
    vector<BankAccount> accounts;
    const string filename = "accounts.txt";

    void saveToFile() {
        ofstream file(filename);

        if (!file) {
            cout << "Error saving account records!\n";
            return;
        }

        for (const auto& account : accounts) {
            file << account.getAccountNumber() << " "
                 << quoted(account.getCustomerName()) << " "
                 << account.getBalance() << "\n";
        }

        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);

        if (!file)
            return;

        int accNo;
        string name;
        long long balance;

        while (file >> accNo >> quoted(name) >> balance) {
            if (accNo > 0 && balance >= 0) {
                accounts.emplace_back(accNo, name, balance);
            }
        }

        file.close();
    }

    BankAccount* findAccount(int accNo) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accNo)
                return &account;
        }

        return nullptr;
    }

    long long getAmount() {
        double amount;

        while (true) {
            cout << "Enter amount: Rs. ";

            if (cin >> amount &&
                amount > 0 &&
                amount <= 90000000000000.0) {

                long long paise =
                    static_cast<long long>(amount * 100.0 + 0.5);

                if (paise > 0)
                    return paise;
            }

            cout << "Please enter a valid positive amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

public:
    Bank() {
        loadFromFile();
    }

    void createAccount() {
        int accNo = 1001;

        for (const auto& account : accounts) {
            accNo = max(accNo, account.getAccountNumber() + 1);
        }

        string name;

        cout << "\nEnter Customer Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);

        if (name.empty()) {
            cout << "Customer name cannot be empty!\n";
            return;
        }

        cout << "Enter Initial Deposit: Rs. ";
        long long amount = getAmount();

        accounts.emplace_back(accNo, name, amount);

        saveToFile();

        cout << "\nAccount Created Successfully!\n";
        cout << "Your Account Number: " << accNo << "\n";
    }

    void depositMoney() {
        int accNo;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        BankAccount* account = findAccount(accNo);

        if (account == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        long long amount = getAmount();

        account->deposit(amount);
        saveToFile();
    }

    void withdrawMoney() {
        int accNo;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        BankAccount* account = findAccount(accNo);

        if (account == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        long long amount = getAmount();

        account->withdraw(amount);
        saveToFile();
    }

    void checkBalance() {
        int accNo;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        BankAccount* account = findAccount(accNo);

        if (account == nullptr) {
            cout << "Account not found!\n";
            return;
        }

        account->displayBalance();
    }

    void displayAllAccounts() {
        if (accounts.empty()) {
            cout << "\nNo accounts available!\n";
            return;
        }

        cout << "\n========== ALL BANK ACCOUNTS ==========\n";

        for (const auto& account : accounts) {
            account.displayBalance();
            cout << "---------------------------------------\n";
        }
    }

    void menu() {
        int choice;

        do {
            cout << "\n====================================\n";
            cout << "       BANK MANAGEMENT SYSTEM       \n";
            cout << "====================================\n";
            cout << "1. Create New Account\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Check Balance\n";
            cout << "5. Display All Accounts\n";
            cout << "6. Exit\n";
            cout << "====================================\n";
            cout << "Enter Your Choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input! Enter a number.\n";
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );
                continue;
            }

            switch (choice) {
                case 1:
                    createAccount();
                    break;

                case 2:
                    depositMoney();
                    break;

                case 3:
                    withdrawMoney();
                    break;

                case 4:
                    checkBalance();
                    break;

                case 5:
                    displayAllAccounts();
                    break;

                case 6:
                    cout << "\nThank you for using our bank!\n";
                    break;

                default:
                    cout << "Invalid choice! Try again.\n";
            }

        } while (choice != 6);
    }
};

int main() {
    Bank bank;

    cout << "Welcome to Bank Management System!\n";

    bank.menu();

    return 0;
}

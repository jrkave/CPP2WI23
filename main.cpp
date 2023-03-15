#include "inventory.h"
#include "userOps.h"
#include "menus.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {

    // Create class objects
    Inventory inventory; // deals with inventory
    UserOps users; // deals with login functionality
    Customer customer; // deals with purchases/shopping list

    /*
    // Initialiaze database
    inventory.writeInventoryToDatabase(inventory.inventory);
    */

    // Login functionality
    printLoginMenu();
    int loginChoice = 0;
    string username, password;
    std::cout << std::endl;
    std::cout << "Select an option:" << std::endl;
    std::cin >> loginChoice;
    while (std::cin.fail() || (loginChoice != 1 && loginChoice != 2)) {
        std::cout << "Invalid choice. Enter 1 to create account, 2 to login." << std::endl;
        std::cin.clear();
        std::cin.ignore();
        std::cin >> loginChoice;
    }
    std::cout << "Enter username: " << std::endl;
    std::cin >> username;
    std::cout << "Enter password: " << std::endl;
    std::cin >> password;
    if (loginChoice == 1) {
        users.createNewAccount(username, password);
    }
    else if (loginChoice == 2) {
        if (users.verifyReturningUser(username, password) == false) {
            exit(0);
        }
    }

    // Main program variables
    int option, amount;
    double total = 0.0;
    char quit = 'm';
    char couponChoice;
    bool validCoupon = false;
    string title, fname, lname, wname, email, newPassword, shopList, purchaseList, endProgram, coupon;
    shopList = "csvshoplist.csv";
    purchaseList = "csvpurchases.csv";
    Book newBook, purchaseBook;
    vector<Book> newBooks;

    // Open databasse
    sqlite3 *db;
    int rc = sqlite3_open("books.db", &db);
    if (rc != SQLITE_OK) {
        std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }

    // Main program start
    while (endProgram != "stop") {
        printMenu();
        std::cout << std::endl;
        std::cout << "Select an option:" << std::endl;
        std::cin >> option;
        while (std::cin.fail() || (option < 1 || option > 6)) {
            std::cout << "Invalid selection. Enter a number between 1 and 6." << std::endl;
            std::cin.clear();
            std::cin.ignore();
            std::cin >> option;
        }

        // Executes functions based on user input
        switch (option) {

            // Retrieves a book; requires title
            case 1:
                while (quit != 'q') {
                    std::cout << "Enter title:" << std::endl;
                    std::cin.ignore();
                    getline(std::cin, title);
                    std::cout << std::endl;
                    inventory.retrieveBookInformation(title);
                    // Continue prompt
                    std::cout << std::endl;
                    std::cout << "Enter 'q' to stop or any other key to continue." << std::endl;
                    std::cin >> quit;
                }
                break;
            

            // Add to inventory; requires: Book obj and vector<Book> 
            case 2:
                while (quit != 'q') {
                    newBook = inventory.createBook(); // makes new Book obj
                    newBooks.push_back(newBook); // adds Book obj to vector<Book> 
                    // Continue prompt 
                    std::cout << std::endl;
                    std::cout << "Enter 'q' to stop or any other key to continue." << std::endl;
                    std::cin >> quit;
                }
                inventory.addToInventory(newBooks, db); // adds vector<Book> to inventory and database
                inventory.writeUserListToCSV(newBooks); // writes vector<Book> to CSV
                break;
            

            // Remove from inventory; requires title
            case 3:
                while (quit != 'q') {
                    std::cout << "Enter title:" << std::endl;
                    std::cin.ignore();
                    getline(std::cin, title);
                    inventory.removeFromInventory(title);
                    inventory.removeData(db, title.c_str()); 
                    // Continue prompt
                    std::cout << "Enter 'q' to stop or any other key to continue." << std::endl;
                    std::cin >> quit;
                }
                break;
            

            // Create shopping list; requires title, quantity, Book obj
            case 4:
                while (quit != 'q') {
                    std::cout << "Enter title:" << std::endl;
                    std::cin.ignore();
                    getline(std::cin, title);
                    purchaseBook = inventory.retrieveBook(title);
                    std::cout << "Amount to purchase:" << std::endl;
                    std::cin >> amount;
                    while (std::cin.fail()) {
                        std::cout << "Input must be an integer." << std::endl;
                        std::cin >> amount;
                    }
                    customer.addPurchase(purchaseBook, amount);
                    // Continue prompt
                    std::cout << "Enter 'q' to stop or any other key to continue." << std::endl;
                    std::cin >> quit;
                }
                customer.writePurchasesToCSV(shopList);
                break;


            // View admin menu; requires option, username, password
            case 5:
                printAdminMenu();
                std::cout << std::endl;
                std::cout << "Select an option:" << std::endl;
                std::cin >> option;
                while (std::cin.fail() || (option != 1 && option != 2 && option != 3)) {
                    std::cout << "Invalid selection. Enter 1 to create an account, 2 to change passwords, or 3 to return to main menu." << std::endl;
                    std::cin.clear();
                    std::cin.ignore();
                    std::cin >> option;
                }
                if (option == 3) {
                    break;
                }
                std::cout << "Enter username:" << std::endl;
                std::cin >> username;
                std::cout << "Enter password:" << std::endl;
                std::cin >> password;
                if (option == 1) {
                    users.createNewAccount(username, password);
                }
                else {
                    std::cout << "New password:" << std::endl;
                    std::cin >> newPassword;
                    users.changeUserPassword(username, password, newPassword);
                }
                break;
            

            // Purchase books; requires name, email, title, amount
            case 6:
                // Obtain info about customer
                std::cout << "Enter first name:" << std::endl;
                std::cin >> fname;
                std::cout << "Enter last name:" << std::endl;
                std::cin >> lname;
                std::cout << "Enter email:" << std::endl;
                std::cin >> email;
                wname = fname + " " + lname;
                customer.setName(wname);
                customer.setEmail(email);
                customer.addShopper(db, wname.c_str(), email.c_str());
                std::cout << std::endl;

                while (quit != 'q') {
                    // Obtain info about books to purchase
                    std::cout << "Enter title:" << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, title);
                    purchaseBook = inventory.retrieveBook(title);
                    if (purchaseBook.isbn != "isbn") {
                        std::cout << "Amount to purchase:" << std::endl;
                        std::cin >> amount;
                        while (std::cin.fail()) {
                            std::cout << "Amount must be an integer." << std::endl;
                            std::cin.clear();
                            std::cin.ignore();
                            std::cin >> amount;
                        }
                        customer.addPurchase(purchaseBook, amount);
                    }
                    // Continue prompt
                    std::cout << "Enter 'q' to stop or any other key to continue." << std::endl;
                    std::cin >> quit;
                }

                // Check for coupon code
                std::cout << "Do you have a coupon code? Enter y/n." << std::endl;
                cin >> couponChoice;
                while (couponChoice != 'y' && couponChoice != 'n') {
                    std::cout << "Invalid entry. Enter y for yes, n for no." << std::endl;
                    cin >> couponChoice;
                }

                // Apply coupon or not
                if (couponChoice == 'y') {
                    std::cout << "Please enter code:" << std::endl;
                    std::cin >> coupon;
                    validCoupon = customer.hasValidCoupon(coupon);
                    if (validCoupon == false) {
                        std::cout << "Sorry, that is not a valid coupon code." << std::endl;
                    }
                    else {
                        std::cout << "Your coupon has been applied." << std::endl;
                    }
                }

                customer.writePurchasesToCSV(purchaseList);
                total = customer.displayReceipt();

                if (validCoupon == true) {
                   total *= 0.80;
                   std::cout << "Coupon Applied: 20% OFF" << std::endl;
                   std::cout << "Final Total: $" << total << std::endl;
                }
                customer.addShopperTotal(db, wname.c_str(), total);
                break;
            
            
            default:
                break;
        }
        // Continue prompt
        std::cout << "To end the program, enter 'stop'. To continue, enter any key." << std::endl;
        std::cin.ignore();
        std::cin >> endProgram;
        
        // Reset variables
        quit = 'm'; // reset char 'quit'
        customer.resetPurchases(); // resets customer's purchases
    }
}
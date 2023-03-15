#include <iostream>
#include <string>
#include <regex>
using namespace std;

// TO DO:
// Figure out shopper / remove lists
// Validation
// DatabaseOps

/* PRINT MENUS */
void printLoginMenu() {
    std::cout << std::endl;
    std::cout << " ====== LOGIN MENU ====== " << std::endl;
    std::cout << "Option 1: Create Account" << std::endl;
    std::cout << "Option 2: Login to Existing Account" << std::endl;
}

// Main menu
void printMenu() {
    std::cout << std::endl;
    std::cout << " ====== MAIN MENU ====== " << std::endl;
    std::cout << "Option 1: Retrieve Book Information" << std::endl;
    std::cout << "Option 2: Add to Inventory" << std::endl;
    std::cout << "Option 3: Remove from Inventory" << std::endl;
    std::cout << "Option 4: Create Shopping List" << std::endl;
    std::cout << "Option 5: View Admin Menu" << std::endl;
    std::cout << "Option 6: Purchase Book(s)" << std::endl;
}

// Admin menu
void printAdminMenu() {
    std::cout << std::endl;
    std::cout << " ====== ADMIN MENU ====== " << std::endl;
    std::cout << "Option 1: Add User" << std::endl;
    std::cout << "Option 2: Change Password" << std::endl;
    std::cout << "Option 3: Return to Main Menu" << std::endl;
}


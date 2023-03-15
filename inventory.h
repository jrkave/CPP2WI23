#include <vector>
#include <string>
#include "json.hpp"
#include "customer.h"
#include "sqlite3.h"

#ifndef INVENTORY_H_
#define INVENTORY_H_
using namespace std;

class Inventory {
public:
    // Public variables
    vector<Book> inventory;

    // Constructor
    Inventory() {
        inventory = createInventory();
    }

    // Inventory functions
    vector<Book> createInventory();
    int findIndex(string title);
    Book createBook();
    void addToInventory(const vector<Book> &booksToAdd, sqlite3 *db);
    void writeUserListToCSV(const vector<Book> &booksToAdd);
    void removeFromInventory(string title);
    void retrieveBookInformation(string title);
    Book retrieveBook(string title);
    void printInventory();

    // Database Operations
    void writeInventoryToDatabase(const vector<Book> &inventory);
    int removeData(sqlite3 *db, const char *title);
    int addData(sqlite3 *db, const char *isbn, const char *title, const char *author, const char *year, const char *publisher, const char *genre, const char *description, double msrp, int quantity);

private:
    string jsonFile = "book.json";
    string userListFile = "csvaddbooks.csv";

    // Validation Functions
    bool validISBNYear(string userTitle, int len1, int len2);
    bool validMSRP(string MSRP);
    bool validQuantity(string quant);
};

#endif /* INVENTORY_H_ */
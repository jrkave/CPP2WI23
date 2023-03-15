#include "book.h"
#include "sqlite3.h"
#include <fstream>
#include <vector>

class Customer {
public:
    // Variables
    vector<Book> purchases;
    string name, email;

    // Constructors
    Customer() {
        name = "Name";
        email = "example@email.com";
        purchases = {};
    }
    Customer(string userName, string userEmail) {
        name = userName;
        email = userEmail;
        purchases = {};
    }

    // Customer functions
    void setName(string userName);
    void setEmail(string userEmail);
    string getName();
    string getEmail();
    void addPurchase(Book &book, int amount);
    double displayReceipt();
    void writePurchasesToCSV(string fileName);
    bool hasValidCoupon(string coupon);
    void resetPurchases();

    // Database functions
    int addShopper(sqlite3 *db, const char *name, const char *email);
    int addShopperTotal(sqlite3 *db, const char *name, double total);

private:
    // Variables
    string purchaseFile = "csvpurchases.csv";
    string couponFile = "csvcoupon.csv";
    ofstream file;

    // Private functions
    static bool compareByValue (const Book &a, const Book &b);
    void sortPurchases();
};
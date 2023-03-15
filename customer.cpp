#include "customer.h"
#include "math.h"
#include <string>
#include <iostream>
using namespace std;

    void Customer::setName(string userName) {
        name = userName;
    }  

    void Customer::setEmail(string userEmail) {
        email = userEmail;
    }

    string Customer::getName() {
        return name;
    }

    string Customer::getEmail() {
        return email;
    }

    void Customer::resetPurchases() {
        purchases.clear();
    }

    void Customer::addPurchase(Book &book, int amount) {
        if ((book.quantity - amount) < 0) {
            std::cout << "Not enough books in inventory to purchase that amount." << std::endl;
        }
        else if (book.isbn == "isbn") {
            return;
        }
        else {
            book.quantity = amount;
            purchases.push_back(book);   
        }
    }

    bool Customer::compareByValue(const Book &a, const Book &b) {
        return a.msrp < b.msrp;
    }

    void Customer::sortPurchases() {
        sort(purchases.begin(), purchases.end(), compareByValue);
    }

    void Customer::writePurchasesToCSV(string fileName) {
        // Sorts purchases vector
        sortPurchases();

        // Open file
        file.open(fileName, ios_base::app);
        for (int i = 0; i < purchases.size(); i++) {
            if (purchases[i].isbn == "isbn") {
                continue;
            }
            file << purchases[i].isbn + ",";
            file << purchases[i].title + ",";
            file << purchases[i].author + ",";
            double msrp = round(purchases[i].msrp * 100.0) / 100.0;
            file << to_string(msrp) + ",";
            file << to_string(purchases[i].quantity) + "," << endl;
            }
        std::cout << "Entry successfully written to CSV file." << std::endl;
    }

    double Customer::displayReceipt() {
        double tax = 0.0;
        double total = 0.0;

        std::cout << std::endl;
        std::cout << "=== SALES RECEIPT ===" << std::endl;
        std::cout << "Customer: " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << std::endl;

        for (int i = 0; i < purchases.size(); i++) {
            tax += round((purchases[i].msrp * purchases[i].quantity * 0.06) * 100.0) / 100.0;
            total += (purchases[i].msrp * purchases[i].quantity);

            std::cout << "Title: " << purchases[i].title << std::endl;
            std::cout << "MSRP: $" << purchases[i].msrp << std::endl;
            std::cout << "Quantity purchased: " << purchases[i].quantity << std::endl;
            //std::cout << "Tax: $" << tax << std::endl;
            std::cout << "Total: $" << (purchases[i].quantity * purchases[i].msrp) << std::endl;
            std::cout << std::endl;
        }
        std::cout << "Total Before Tax: $" << total << std::endl;
        std::cout << "Tax: $" << tax << std::endl;
        std::cout << "Total After Tax: $" << (total + tax) << std::endl;
        return (total + tax);
    }

    // Add shopper to database
    int Customer::addShopper(sqlite3 *db, const char *name, const char *email) {
        // Use placeholders in the query string
        string query = "INSERT INTO SHOPPERS (NAME, EMAIL) VALUES (?, ?)";
        sqlite3_stmt *stmt;

        // Prepare the statement
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
        }

        // Bind the parameters with the correct data types
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "Shopper was not able to be added. " << std::endl;
            return -1;
        }
        std::cout << "Shopper successfully added to database. " << std::endl;

        // Finalize the statement
        sqlite3_finalize(stmt);
        return 0;
    }

    // Add shopper total to db
    int Customer::addShopperTotal(sqlite3 *db, const char *name, double total) {
    // Prepare the SQL statement with parameter placeholders
        const char* sql = "UPDATE SHOPPERS SET TOTAL = ? WHERE NAME = ?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return rc;
        }

        // Bind the total and name parameters to the statement
        rc = sqlite3_bind_double(stmt, 1, total);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
        rc = sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }

        // Execute the statement and finalize it
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
        sqlite3_finalize(stmt);

        // Return the number of rows affected (should be 1)
        return sqlite3_changes(db);
    }

    // Coupon codes
    bool Customer::hasValidCoupon(string coupon) {
        coupon += ",";
        string line;
        ifstream file(couponFile);
        // Exception handling
        if (!file.is_open()) {
            cerr << "Failed to open file: " << couponFile << endl;
        }
        // Search for coupon code
        while (getline(file, line)) {
            if (line == coupon) {
                return true;
            }
        }
        file.close();
        return false;
    }


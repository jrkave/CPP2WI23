#include <iostream>
#include <regex>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include "inventory.h"
using json = nlohmann::json;

    // Create vector of Book objects from JSON data
    vector<Book> Inventory::createInventory() {
        Book book;

        // Open file
        ifstream json_file(jsonFile);
        if (json_file.is_open()) {
            std::cout << "Starting system..." << std::endl;
        }
        else {
            std::cout << "Error opening the file." << std::endl;
        }

        // Make JSON object and fill it with file data
        json data;
        json_file >> data;

        srand(time(NULL));
        for(int i = 1; i < 3001; i++) {
            try {
                const auto &bookInfo = data[i];
                    book.isbn = bookInfo["isbn"];
                    book.title = bookInfo["title"];
                    book.author = bookInfo["author"];
                    book.year = bookInfo["year"];
                    book.publisher = bookInfo["publisher"];
                    book.genre = "NULL";
                    book.description = "NULL";
                    int randQuantity = rand() % (56 - 2 + 1) + 2;
                    book.quantity = randQuantity;
                    double randMSRP = (103.97 - 34.99) * ((double)rand() / (double)RAND_MAX) + 34.99;
                    randMSRP = round(randMSRP * 100.0) / 100.0;
                    book.msrp = randMSRP;
                    inventory.push_back(book);
            } catch (const exception& e) {
                std::cout << "Error in parsing file." << std::endl;
                std::cout << e.what() << std::endl;
            }
        }
        return inventory;
    }

    // Finds index of book in inventory
    int Inventory::findIndex(string title) {
        for (int i = 0; i < inventory.size(); i++) {
            if (inventory[i].title == title) {
                return i;
            }
        }
        return -1;
    }

    // Removes book from inventory
    void Inventory::removeFromInventory(string title) {
        int index = findIndex(title);
        if (index == -1) {
            std::cout << "Book not found." << std::endl;
        }
        else {
            if (index == inventory.size() - 1) {
                inventory.pop_back();
                std::cout << "Book removed." << std::endl;
            }
            else {
                inventory.erase(inventory.begin() + index);
                std::cout << "Book removed." << std::endl;
            }
        }
    }

    // Creates book for user list
    Book Inventory::createBook() {
        Book book;
        string isbn, title, author, year, publisher, genre, description, msrp, quantity;

        // Obtain user input
        std::cout << "ISBN to add: " << std::endl;
        cin.ignore();
        std::getline(cin, isbn);
        while (validISBNYear(isbn, 13, 10) == false) {
            std::getline(cin, isbn);
        }
        std::cout << "Title to add: " << std::endl;
        std::getline(cin, title);
        std::cout << "Author to add: " << std::endl;
        std::getline(cin, author);
        std::cout << "Year to add: " << std::endl;
        std::getline(cin, year);
        while (validISBNYear(year, 4, 4) == false) {
            getline(cin, year);
        }
        std::cout << "Publisher to add: " << std::endl;
        std::getline(cin, publisher);
        std::cout << "Genre to add: " << std::endl;
        std::getline(cin, genre);
        std::cout << "Description to add: " << std::endl;
        std::getline(cin, description);
        std::cout << "MSRP to add: " << std::endl;
        std::getline(cin, msrp);
        while (validMSRP(msrp) == false) {
            std::getline(cin, msrp);
        }
        std::cout << "Quantity to add: " << std::endl;
        std::getline(cin, quantity);
        while (validQuantity(quantity) == false) {
            std::getline(cin, quantity);
        }

        // Put data into Book object
        book.isbn = isbn;
        book.title = title;
        book.author = author;
        book.year = year;
        book.publisher = publisher;
        book.genre = genre;
        book.description = description;
        book.msrp = stod(msrp);
        book.quantity = stoi(quantity);

        return book;
    }

    // Adds user list of books to inventory
    void Inventory::addToInventory(const vector<Book> &booksToAdd, sqlite3 *db) {
        for (int i = 0; i < booksToAdd.size(); i++) {
            // add to vector<Book> inventory
            inventory.push_back(booksToAdd[i]);
            // add to database
            addData(db, booksToAdd[i].isbn.c_str(), booksToAdd[i].title.c_str(), booksToAdd[i].author.c_str(), booksToAdd[i].year.c_str(), booksToAdd[i].publisher.c_str(), booksToAdd[i].genre.c_str(), booksToAdd[i].description.c_str(), booksToAdd[i].msrp, booksToAdd[i].quantity);
        }
    }

    // Writes user list to CSV file
    void Inventory::writeUserListToCSV(const vector<Book> &booksToAdd) {
        ofstream file;
        file.open(userListFile, ios_base::app);
        for (int i = 0; i < booksToAdd.size(); i++) {
            file << booksToAdd[i].isbn + ",";
            file << booksToAdd[i].title + ",";
            file << booksToAdd[i].author + ",";
            file << booksToAdd[i].year + ",";
            file << booksToAdd[i].publisher + "," << endl;
        }
        std::cout << "Entry successfully written to CSV file." << std::endl;
    }

    // Retrieves book from inventory
    void Inventory::retrieveBookInformation(string title) {
        int index = findIndex(title);
        if (index == -1) {
            std::cout << "Book not found." << std::endl;
        }
        else {
            std::cout << "ISBN: " << inventory[index].isbn << std::endl;
            std::cout << "Title: " << inventory[index].title << std::endl;
            std::cout << "Author: " << inventory[index].author << std::endl;
            std::cout << "Year: " << inventory[index].year << std::endl;
            std::cout << "Publisher: " << inventory[index].publisher << std::endl;
            std::cout << "Genre: " << inventory[index].genre << std::endl;
            std::cout << "Description: " << inventory[index].description << std::endl;
            std::cout << "MSRP: " << inventory[index].msrp << std::endl;
            std::cout << "Quantity: " << inventory[index].quantity << std::endl;
        }
    }

    // Returns book from inventory
    Book Inventory::retrieveBook(string title) {
        int index = findIndex(title);
        if (index == -1) {
            std::cout << "Book not found." << std::endl;
            Book book;
            return book;
        }
        else {
            return inventory[index];
        }
    }

    // Prints inventory up to 10
    void Inventory::printInventory() {
        for (int i = 0; i < 10; i++) {
            std::cout << "Book " << (i + 1) << ": " << std::endl;
            std::cout << inventory[i].isbn << std::endl;
            std::cout << inventory[i].title << std::endl;
            std::cout << inventory[i].author << std::endl;
            std::cout << std::endl;
        }
    }
    
    /* VALIDATION */

    // Validate ISBN or Year
    bool Inventory::validISBNYear(string userTitle, int len1, int len2) {
        if ((userTitle.length() != len1) && (userTitle.length() != len2)) {
            std::cout << "Incorrect argument length. Please try again. " << std::endl;
            return false;
        }
        else {
            for (int i = 0; i < userTitle.length(); i++) {
                if (isdigit(userTitle[i]) == false) {
                    std::cout << "Number expected, but other character found. Please try again. " << std::endl; 
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    // Validate MSRP
    bool Inventory::validMSRP(string MSRP) {
        // Formats
        regex integer_expr1("[0-9][0-9]\\.[0-9][0-9]"); // 10.00
        regex integer_expr2("[0-9][0-9][0-9]\\.[0-9][0-9]"); // 100.00
        regex integer_expr3("[0-9]\\.[0-9][0-9]"); // 1.00
        // Check if valid using regex_match()
        if (regex_match(MSRP, integer_expr1) || regex_match(MSRP, integer_expr2) || regex_match(MSRP, integer_expr3)) {
            return true;
        }
        else {
            std::cout << "MSRPs must be written in the correct format, such as: 1.00, 10.00, or 100.00." << std::endl;
            return false;
        }
    }

    // Validate quantity
    bool Inventory::validQuantity(string quant) {
        for (int i = 0; i < quant.length(); i++) {
            if (isdigit(quant[i]) == false) {
                std::cout << "Integer expected. Please try again. " << std::endl;
                return false;
            }
        }
        return true;
    }

    /* DATABASE OPERATIONS */
    
    // Initialize database
    void Inventory::writeInventoryToDatabase(const vector<Book> &inventory) {
        // Open database connection
        sqlite3* db;
        int rc = sqlite3_open("books.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }
        
        // Prepare SQL statement to insert a row into the books table
        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO BOOKS (ISBN, TITLE, AUTHOR, PUBLISHER, YEAR, GENRE, DESCRIPTION, MSRP, QUANTITY) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }
        
        // Bind parameter values to the SQL statement and execute it for the first 300 rows of inventory
        int count = 0;
        for (const auto& book : inventory) {
            if (count == 3000) break;
            sqlite3_bind_text(stmt, 1, book.isbn.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, book.title.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, book.author.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, book.publisher.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, book.year.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, book.genre.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 7, book.description.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 8, book.msrp);
            sqlite3_bind_int(stmt, 9, book.quantity);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "Error inserting row: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }
            sqlite3_reset(stmt);
            ++count;
        }
        
        // Finalize statement and close database connection
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    // Remove data from "books.db"
    int Inventory::removeData(sqlite3 *db, const char *title) {
        int rc;
        sqlite3_stmt *stmt;
        const char *sql;
        
        sql = "DELETE FROM BOOKS WHERE TITLE = ?"; // writes sql statement to be executed
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        rc = sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
            return rc;
        }
        sqlite3_finalize(stmt);
        fprintf(stdout, "Data removed successfully from databse.\n");
        return SQLITE_OK;
    }

    // Add data to "books.db"
    int Inventory::addData(sqlite3 *db, const char *isbn, const char *title, const char *author, const char *year, const char *publisher, const char *genre, const char *description, double msrp, int quantity) {
        // Use placeholders in the query string
        string query = "INSERT INTO BOOKS (ISBN, TITLE, AUTHOR, YEAR, PUBLISHER, GENRE, DESCRIPTION, MSRP, QUANTITY) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        sqlite3_stmt *stmt;

        // Prepare the statement
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
        }

        // Bind the parameters with the correct data types
        sqlite3_bind_text(stmt, 1, isbn, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, title, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, author, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, year, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, publisher, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, genre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 7, description, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 8, msrp);
        sqlite3_bind_int(stmt, 9, quantity);

        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "Data was not able to be added. " << std::endl;
            return -1;
        }
        std::cout << "Data successfully added to database. " << std::endl;

        // Finalize the statement
        sqlite3_finalize(stmt);
        return 0;
    }
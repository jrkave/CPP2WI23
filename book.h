#include <string>
#ifndef BOOK_H_
#define BOOK_H_
using namespace std;

class Book {
public:
    string isbn;
    string title;
    string author;
    string year;
    string publisher;
    string genre;
    string description;
    double msrp;
    int quantity;

    Book() {
        isbn = "isbn";
        title = "title";
        author = "author";
        year = "year";
        publisher = "publisher";
        genre = "genre";
        description = "description";
        msrp = 0.00;
        quantity = 0;
    }
};

#endif /* BOOK_H_ */
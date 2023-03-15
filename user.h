#include <string>
#ifndef USER_H_
#define USER_H_
using namespace std;

class User {
public:
    // Variables
    string username;
    string password;

    // Constructors
    User() {
        username = "username";
        password = "password";
    }
    User(string user, string pass) {
        username = user;
        password = pass;
    }
};

#endif /* USER_H_ */
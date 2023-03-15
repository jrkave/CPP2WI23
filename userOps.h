#include "user.h"
#include <vector>

class UserOps {
public:
    // Variables
    vector<User> users;

    // Constructor
    UserOps() {
        users = readUsers();
    }

    // UserOps functions
    User parseUser(string line);
    vector<User> readUsers();
    void writeUsers();
    void createNewAccount(string username, string password);
    bool verifyReturningUser(string username, string password);
    void changeUserPassword(string username, string password, string newPassword);
    void printUsers();

private:
    string csvFile = "csvlogin.csv";
    ofstream file;
};
#ifndef USER_H
#define USER_H

#include <mysql.h>
#include <string>

class User {
public:
    static void LoginAndRegister();
    static bool GetChoice();
    static bool LoginUser();
    static bool RegisterUser();
private:
    static MYSQL* conn;
    static bool ConnectToDatabase();
    static bool CheckIfUserExisted(const std::string& username);
    static bool CheckPassword(const std::string& username, const std::string& password);
};

#endif

#include "user.h"
#include <iostream>
#include <mysql.h>

MYSQL* User::conn = nullptr;

void User::LoginAndRegister() 
{
    if (User::GetChoice())  {
        User::LoginUser();
    }
    else {
        User::RegisterUser();
    } 
}
bool User::GetChoice()
{
    std::string Choice;
    while (true)
    {
        std::cout << "请选择“登录”还是“注册”" << std::endl;
        std::cin >> Choice;
        if (Choice == "登录") {
            return TRUE;
        }
        else if (Choice == "注册") {
            return FALSE;
        }
        else {
            std::cout << "请勿输入其他内容" << std::endl;
        }
    }
}
bool User::LoginUser()
{
    if (!ConnectToDatabase()) {
        return FALSE;
    }
    std::string username, password, email;

    while (true)
    {
        std::cout << "请输入用户名" << std::endl;
        std::cin >> username;
        if (CheckIfUserExisted(username)) {
            std::cout << "请输入密码" << std::endl;
            std::cin >> password;
            if (CheckPassword(username, password)) {
                std::cout << "登陆成功" << std::endl;
                return TRUE;
            }
            else {
                std::cout << "密码错误" << std::endl;
            }
        }
        else {
            std::cout << "用户名不存在" << std::endl;
        }
    }  
}
bool User::RegisterUser()
{
    if (!ConnectToDatabase()) {
        return FALSE;
    }
    std::string username, password, email;
    while (true)
    {
        std::cout << "请输入用户名" << std::endl;
        std::cin >> username;
        if (CheckIfUserExisted(username)) {
            std::cout << "用户名已存在" << std::endl;
        }
        else {
            std::cout << "请输入密码" << std::endl;
            std::cin >> password;
            std::cout << "请输入邮箱" << std::endl;
            std::cin >> email;
            std::string query = "INSERT INTO usertable(username,password,email) VALUES('" + username + "','" + password + "','" + email + "')";
            if (mysql_query(conn, query.c_str())) {
                std::cerr << "INSERT query() failed\n";
                return FALSE;
            }
            return TRUE;
        }
    }
}
bool User::ConnectToDatabase()
{
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed\n";
        return FALSE;
    }
    conn = mysql_real_connect(conn, "localhost", "root", "123456", "Online_Shopping_db", 3306, nullptr, 0);
    if (conn == nullptr) {
        std::cerr << "mysql_real_connect() failed\n";
        return FALSE;
    }
    return TRUE;
}
bool User::CheckIfUserExisted(const std::string& username)
{
    std::string query = "SELECT * FROM usertable WHERE username = '" + username + "'";
    if (mysql_query(conn, query.c_str()))
    {
        std::cerr << "SELECT query() failed\n";
        return FALSE;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr)
    {
        std::cerr << "mysql_store_result() failed\n";
        return FALSE;
    }
    return mysql_num_rows(res);
}
bool User::CheckPassword(const std::string& username, const std::string& password)
{
    std::string query = "SELECT password FROM usertable WHERE username='" + username + "'";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "SELECT query failed\n";
        return FALSE;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    std::string storedPassword = row[0];
    return storedPassword == password;
}
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>

class DatabaseManager{
    private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    bool isConnected;

    public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect();
    void disconnect();
    bool getStatus() const;

    bool fetchMachineCategories();
};
#endif
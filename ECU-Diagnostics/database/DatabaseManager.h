#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>
#include <QObject>
#include <QVariantMap>
#include <QString>

class DatabaseManager : public QObject {
    Q_OBJECT

private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    bool isConnected;

public:
    DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connect();
    void disconnect();
    bool getStatus() const;

    bool fetchMachineCategories();

    Q_INVOKABLE QVariantMap scanAndDetectECU();
};

#endif 
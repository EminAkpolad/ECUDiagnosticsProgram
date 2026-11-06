#include "DatabaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <iostream>

using namespace std;

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent), isConnected(false)
{
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

bool DatabaseManager::connect()
{
    if (isConnected && QSqlDatabase::contains("ECU_Connection"))
        return true;

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "ECU_Connection");
    
   QString connString = "DRIVER={SQL Server};"
                         "SERVER=localhost;"
                         "DATABASE=ECUDiagnosticsDB;"
                         "Trusted_Connection=yes;";
                         
    db.setDatabaseName(connString);

    if (db.open())
    {
        cout << "Success connect gaddar (Qt QSqlDatabase)" << endl;
        isConnected = true;
        return true;
    }
    else
    {
        cerr << "Error database not connected. Error: " << db.lastError().text().toStdString() << endl;
        isConnected = false;
        return false;
    }
}

void DatabaseManager::disconnect()
{
    if (isConnected)
    {
        {
            QSqlDatabase db = QSqlDatabase::database("ECU_Connection");
            if (db.isOpen())
                db.close();
        }
        QSqlDatabase::removeDatabase("ECU_Connection");
        isConnected = false;
        cout << "Database disconnected success" << endl;
    }
}

bool DatabaseManager::getStatus() const
{
    return isConnected;
}

QVariantMap DatabaseManager::scanAndDetectECU()
{
    QVariantMap vehicleData;

    if (!isConnected)
    {
        connect();
    }

    if (!isConnected)
    {
        vehicleData["status"] = "Error";
        vehicleData["ecuModel"] = "Error";
        vehicleData["vehicleInfo"] = "Error";
        vehicleData["faultCode"] = "Error";
        vehicleData["description"] = "Error";
        return vehicleData;
    }

    QSqlDatabase db = QSqlDatabase::database("ECU_Connection");
    QSqlQuery query(db);
    
    query.prepare("SELECT ModelName, VehicleInfo, FaultCode, Description FROM ECU_MasterTable WHERE EcuSignature = ?");
    query.addBindValue("ECU_SIGNATURE_99");

    if (query.exec() && query.next())
    {
        vehicleData["status"] = "Success";
        vehicleData["ecuModel"] = query.value(0).toString();
        vehicleData["vehicleInfo"] = query.value(1).toString();
        vehicleData["faultCode"] = query.value(2).toString();
        vehicleData["description"] = query.value(3).toString();
    }
    else
    {
        if (query.lastError().isValid()) {
            cerr << "SQL Hata: " << query.lastError().text().toStdString() << endl;
            vehicleData["status"] = "Error";
            vehicleData["ecuModel"] = "SQL Çalıştırma Hatası";
            vehicleData["faultCode"] = "ERR_SQL_EXEC";
            vehicleData["description"] = query.lastError().text();
        } else {
            vehicleData["status"] = "NotFound";
            vehicleData["ecuModel"] = "Tanımsız ECU";
            vehicleData["vehicleInfo"] = "Bilinmeyen Araç/Makine";
            vehicleData["faultCode"] = "ERR_ECU_UNKNOWN";
            vehicleData["description"] = "Veritabanında kayıt bulunamadı.";
        }
    }

    return vehicleData;
}

bool DatabaseManager::fetchMachineCategories()
{
    if (!isConnected)
    {
        cerr << "Error Sql command could not be started" << endl;
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("ECU_Connection");
    QSqlQuery query(db);

    if (query.exec("SELECT CategoryName, Description FROM MachineCategories"))
    {
        cout << "\n--- VERITABANINDAN MAKINELER ÇEKİLİYOR ---" << endl;
        while (query.next())
        {
            QString catName = query.value(0).toString();
            QString catDesc = query.value(1).toString();
            wcout << L"-> Kategori: " << catName.toStdWString() << L" | Açıklama: " << catDesc.toStdWString() << endl;
        }
        cout << "--------------------------------------------------------------------" << endl;
        return true;
    }
    else
    {
        cerr << "ERROR: " << query.lastError().text().toStdString() << endl;
        return false;
    }
}
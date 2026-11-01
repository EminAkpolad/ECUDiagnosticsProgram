#include "DatabaseManager.h"
#include <iostream>
using namespace std;


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

    QString ecuSignature = "ECU_SIGNATURE_99";

    SQLHSTMT hStmt = SQL_NULL_HSTMT;
    SQLRETURN retcode;

    QString queryStr = "SELECT ModelName, VehicleInfo, FaultCode, Description FROM ECU_MasterTable WHERE EcuSignature = ?";

    SQLWCHAR *sqlQuery = reinterpret_cast<SQLWCHAR*>(const_cast<ushort *>(queryStr.utf16()));

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
    {
        vehicleData["status"] = "Error";
        vehicleData["ecuModel"] = "Sorgu Hatası";
        vehicleData["faultCode"] = "ERR_SQL_PREPARE";
        vehicleData["description"] = "SQL sorgusu hazırlanamadı, tablo/sütun adını kontrol edin.";
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return vehicleData;
    }

    SQLLEN cbSignature = SQL_NTS;
    retcode = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
                               ecuSignature.length(), 0,
                               (SQLPOINTER)ecuSignature.utf16(), 0, &cbSignature);

    retcode = SQLExecute(hStmt);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        SQLWCHAR dbModel[100] = {0};
        SQLWCHAR dbVehicle[100] = {0};
        SQLWCHAR dbFault[50] = {0};
        SQLWCHAR dbDesc[255] = {0};

        SQLLEN cbModel = 0, cbVehicle = 0, cbFault = 0, cbDesc = 0;

        if (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_WCHAR, dbModel, sizeof(dbModel), &cbModel);
            SQLGetData(hStmt, 2, SQL_C_WCHAR, dbVehicle, sizeof(dbVehicle), &cbVehicle);
            SQLGetData(hStmt, 3, SQL_C_WCHAR, dbFault, sizeof(dbFault), &cbFault);
            SQLGetData(hStmt, 4, SQL_C_WCHAR, dbDesc, sizeof(dbDesc), &cbDesc);

            vehicleData["status"] = "Success";
            vehicleData["ecuModel"] = QString::fromWCharArray(dbModel);
            vehicleData["vehicleInfo"] = QString::fromWCharArray(dbVehicle);
            vehicleData["faultCode"] = QString::fromWCharArray(dbFault);
            vehicleData["description"] = QString::fromWCharArray(dbDesc);
        }
        else
        {
            vehicleData["status"] = "NotFound";
            vehicleData["ecuModel"] = "Tanımsız ECU";
            vehicleData["vehicleInfo"] = "Bilinmeyen Araç/Makine";
            vehicleData["faultCode"] = "ERR_ECU_UNKNOWN";
            vehicleData["description"] = "Bağlı olan ECU imzasız veya veritabanında kayıtlı değil (" + ecuSignature + ")";
        }
    }
    else
    {
        vehicleData["status"] = "Error";
        vehicleData["ecuModel"] = "SQL Çalıştırma Hatası";
        vehicleData["faultCode"] = "ERR_SQL_EXEC";
        vehicleData["description"] = "Veritabanı sorgusu yürütülemedi.";
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return vehicleData;
}


DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent), hEnv(NULL), hDbc(NULL), isConnected(false)
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_SUCCESS)
    {
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    }
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
    if (hEnv != NULL)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
}

bool DatabaseManager::connect()
{
    if (isConnected)
        return true;

    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
    {
        cerr << "Hata ODBC not create" << endl;
        return false;
    }

    SQLWCHAR connectionString[] = L"DRIVER={ODBC Driver 17 for SQL SERVER};"
                                  L"SERVER=localhost\\SQLEXPRESS;"
                                  L"DATABASE=ECUDiagnosticsDB;"
                                  L"Trusted_Connection=yes;";

    SQLRETURN retcode = SQLDriverConnectW(
        hDbc, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        cout << "Success connect gaddar" << endl;
        isConnected = true;
        return true;
    }
    else
    {
        cerr << "Error database not connected. Error Code:" << retcode << endl;
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        hDbc = NULL;
        isConnected = false;
        return false;
    }
}

void DatabaseManager::disconnect()
{
    if (isConnected)
    {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        hDbc = NULL;
        isConnected = false;
        cout << "Database disconnected success" << endl;
    }
}

bool DatabaseManager::getStatus() const
{
    return isConnected;
}

bool DatabaseManager::fetchMachineCategories()
{
    if (!isConnected)
    {
        cerr << "Error Sql command could not be started" << endl;
        return false;
    }

    SQLHSTMT hStmt = NULL;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        cerr << "ERROR failed to allocate statement handle" << endl;
        return false;
    }

    SQLWCHAR *query = (SQLWCHAR *)L"SELECT CategoryName,Description FROM MachineCategories";
    SQLRETURN ret = SQLExecDirectW(hStmt, query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLWCHAR catName[100];
        SQLWCHAR catDesc[200];
        SQLLEN indName, indDesc;

        cout << "\n--- VERITABANINDAN MAKINELER ÇEKİLİYOR ---" << endl;

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_WCHAR, catName, sizeof(catName), &indName);
            SQLGetData(hStmt, 2, SQL_C_WCHAR, catDesc, sizeof(catDesc), &indDesc);

            wcout << L"-> Kategori: " << catName << L" | Açıklama: " << catDesc << endl;
        }
        cout << "--------------------------------------------------------------------" << endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return true;
}
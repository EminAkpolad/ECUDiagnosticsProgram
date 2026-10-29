#include "DatabaseManager.h"
#include <iostream>
using namespace std;

DatabaseManager::DatabaseManager() : hEnv(NULL),hDbc(NULL),isConnected(false){
    if(SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE, &hEnv)==SQL_SUCCESS){
        SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,0);
    }
}

DatabaseManager::~DatabaseManager(){
    disconnect();
    if(hEnv!=NULL){
        SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
    }
}

bool DatabaseManager::connect(){
    if(isConnected) return true;

    if(SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc)!=SQL_SUCCESS){
        cerr<<"Hata ODBC not create"<<endl;
        return false;
    }

    SQLWCHAR connectionString[]=L"DRIVER={ODBC Driver 17 for SQL SERVER};"
    L"SERVER=localhost;"
    L"DATABASE=ECUDiagnosticsDB;"
    L"Trusted_Connection=yes;";

    SQLRETURN retcode=SQLDriverConnectW(
        hDbc, NULL,connectionString,SQL_NTS,NULL,0,NULL,SQL_DRIVER_NOPROMPT
    );

    if(retcode==SQL_SUCCESS || retcode==SQL_SUCCESS_WITH_INFO){
        cout<<"Success connect gaddar"<<endl;
        isConnected=true;
        return true;
    }
    else {
        cerr<<"Error database not connected. Error Code:"<<retcode<<endl;
        SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
        hDbc=NULL;
        isConnected=false;
        return false;
    }
}

void DatabaseManager::disconnect(){
    if(isConnected){
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
        hDbc=NULL;
        isConnected=false;
        cout<<"Database disconnected success"<<endl;
    }
}

bool DatabaseManager::getStatus() const{
    return isConnected;
}

bool DatabaseManager::fetchMachineCategories(){
    if(!isConnected){
        cerr<<"Error Sql command could not be started"<<endl;
        return false;
    }

    SQLHSTMT hStmt=NULL;
    if(SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt)!=SQL_SUCCESS){
        cerr<<"ERROR failed to allocate statement handle"<<endl;
        return false;
    }

    SQLWCHAR* query=(SQLWCHAR*)L"SELECT CategoryName,Description FROM MachineCategories";
    SQLRETURN ret=SQLExecDirectW(hStmt,query,SQL_NTS);

    if(ret==SQL_SUCCESS || ret==SQL_SUCCESS_WITH_INFO){
        SQLWCHAR catName[100];
        SQLWCHAR catDesc[200];
        SQLLEN indName,indDesc;

        cout<< "\n--- VERITABANINDAN MAKINELER ÇEKİLİYOR ---"<<endl;

        while(SQLFetch(hStmt)==SQL_SUCCESS){
            SQLGetData(hStmt,1,SQL_C_WCHAR,catName,sizeof(catName),&indName);
            SQLGetData(hStmt,2,SQL_C_WCHAR,catDesc,sizeof(catDesc),&indDesc);

            wcout << L"-> Kategori: " << catName << L" | Açıklama: " << catDesc <<endl;
        }
        cout<<"--------------------------------------------------------------------"<<endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
    return true;
}
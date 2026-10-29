#include "db_connect.h"
#include<iostream>
using namespace std;

bool connectToSqlExpress(){
    SQLHENV hEnv=NULL;
    SQLHDBC hDbc=NULL;
    SQLRETURN retCode;

    SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv);
    SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,0);

    SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);

    SQLWCHAR connectionString[]=L"DRIVER={ODBC DRIVER 17 FOR SQL Server};"
    L"SERVER=localhost;"
    L"DATABASE=ECUDiagnosticsDB;"
    L"Trusted_Connection=yes;";
    cout<<"SQL express Connection.."<<endl;

    retCode=SQLDriverConnectW(hDbc,NULL,connectionString,SQL_NTS,NULL,0,NULL,SQL_DRIVER_NOPROMPT);

    if(retCode==SQL_SUCCESS || retCode==SQL_SUCCESS_WITH_INFO){
        cout<<"Connection success"<<endl;

        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
        return true;
    }
    else{
        cout<<"Connection error"<<endl;
        SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
        return false;
    }
}
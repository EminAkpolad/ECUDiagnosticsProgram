#include<iostream>
#include "database/db_connect.h"
using namespace std;

int main(){
    cout<<"ECU BACKEND START"<<endl;

    if(connectToSqlExpress()){
        cout<<"işlemelre devam edilebilir...."<<endl;
    }
    else{
        cout<<"Veritabani baglantisi olmadan backend baslatilamadi"<<endl;
    }
    return 0;
}
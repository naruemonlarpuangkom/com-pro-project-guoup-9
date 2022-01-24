#include <iostream> 
#include<stdlib.h>
#include "sqlite3.h"

using namespace std;

sqlite3* db; 

void f();
int main() 
{ 
    // Pointer to SQLite connection
    //sqlite3* db; 
    sqlite3_stmt * stmt;
    
    // Save the connection result
    int exit = 0;
    exit = sqlite3_open("database.db", &db); 
  
    // Test if there was an error
    if (exit) { 
        
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
        
    } else {

        cout << "Opened Database Successfully!" << endl; 
        sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement
        sqlite3_step( stmt );//executing the statement
        char * str = (char *) sqlite3_column_text( stmt, 1 );///reading the 1st column of the result
        //cout << str << endl;
    }
    
    f();

    // Close the connection
    sqlite3_close(db);
    return (0); 
} 

void f(){

    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement

    const unsigned char* text;
    bool done = false;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
                text = sqlite3_column_text(stmt, 1);
                cout << text << endl;
                break;

            case SQLITE_DONE:
                done = true;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return;
        }
    }

    sqlite3_finalize(stmt);

}

//gcc sqlite3.c -c   ทำแบบนี้มันจะไม่ได้ exe แต่จะได้เป็น object file ที่ลิงค์เข้ากับโค้ดให้โปรแกรมเราไปเรียกใช้ตอน compile ครับ
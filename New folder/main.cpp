#include <iostream>
#include <stdlib.h> 
#include "sqlite3.h"

using namespace std;

// Pointer to SQLite connection 
sqlite3* db; 

//prototype
void allmenu();
void showmenu();
void addmenu();
void deletemenu();
int callback(void *NotUsed, int argc, char **argv, char **azColName);

int main() 
{ 
    sqlite3_stmt * stmt;
    
    // Save the connection result
    int exit = 0;
    exit = sqlite3_open("database.db", &db); 
  
    // Test if there was an error
    if (exit) { 
        
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
        
    } else {

        cout << "Opened Database Successfully!" << endl; 
        //sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement  
        //int x=sqlite3_step( stmt );//executing the statement
        //cout << x;
        //char * str = (char *) sqlite3_column_text( stmt, 1 );///reading the 1st column of the result
        //cout << str << endl;
    }
    //showmenu(1);
    allmenu();
    //addmenu();
    deletemenu();

    // Close the connection
    sqlite3_close(db);
    return (0); 
} 

void allmenu(){

    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement

    const unsigned char* text;
    bool done = false;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
            for(int i=0; i<3;i++){
                text = sqlite3_column_text(stmt, i);
                cout << text <<" ";
            }
                break;
            

            case SQLITE_DONE:
                done = true;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return;
        }
        cout << endl;
    }

    sqlite3_finalize(stmt);

}

void showmenu(){
    sqlite3_stmt * stmt;
  
    int id;
    cin >> id;
    id -= 1;

    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement
    const unsigned char* text;
    
    for(int j=0;j<=id;j++){
        sqlite3_step(stmt);
        if(j==id){
            for(int i=0;i<3;i++){
                text = sqlite3_column_text(stmt,i);
                cout << text << " ";
            }
            break;
        }
    }
    sqlite3_finalize(stmt);
}

void addmenu(){
    sqlite3_stmt * stmt;
    
    string name,price;
    cin >> name >> price;

    string sqlstatement = "INSERT INTO menu (food_name, price) VALUES ('" + name + "','" + price + "');";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
    
}

void deletemenu(){
    int id;
    cin >> id;
    id-=1;
    sqlite3_stmt * stmt;
    string sqlstatement = "DELETE FROM menu WHERE food_id = 'id';";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
}

int callback(void *NotUsed, int argc, char **argv, char **azColName){

    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    for(int i = 0; i < argc; i++) {
        
        // Show column name, value, and newline
        cout << azColName[i] << ": " << argv[i] << endl;
    
    }

    // Insert a newline
    cout << endl;

    // Return successful
    return 0;
}
//gcc sqlite3.c -c   ทำแบบนี้มันจะไม่ได้ exe แต่จะได้เป็น object file ที่ลิงค์เข้ากับโค้ดให้โปรแกรมเราไปเรียกใช้ตอน compile ครับ
//how to run (safe file first) ; g++ main.cpp sqlite3.o ; ./a.exe
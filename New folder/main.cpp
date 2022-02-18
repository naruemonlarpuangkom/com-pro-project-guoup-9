#include <iostream>
#include <stdlib.h> 
#include<iomanip>
#include<string>
#include<vector>
#include <ctime>
#include "sqlite3.h"

using namespace std;

// Pointer to SQLite connection 
sqlite3* db; 

struct menubook
{
    int id ;
    string name;
    int price;

};

struct  table_order
{
    int food_id;
    int order;
    
};

struct daily_income
{
    string date;
    int income;
};

char table[9] = {'1','2','3','4','5','6','7','8','9'};
vector<table_order> table1,table2,table3,table4,table5,table6,table7,table8,table9;
vector<daily_income> daily;
vector<menubook> book;

//prototype
void allmenu();     void showmenu(int);    void addmenu();     void deletemenu();      
void edit_menu();
int callback(void *NotUsed, int argc, char **argv, char **azColName);

void number_table_order(int); void loop_order(vector<table_order> &table);

void start();   void select_table();
void select_checkbill(); void delete_old_daily();

void push_menubook(vector<menubook> &book);                     
void printmenubook(vector<menubook> &book);
void vec_addorder(vector<table_order> &note , int id, int num);
void add_table_order(vector<table_order> &thetable);
bool vec_cancelorder(vector<table_order> &note ,int id,int num);
void cancel_table_order(vector<table_order> &thetable);
int find_id(vector<menubook> book ,int id);  
int find_id_in_order (vector<table_order> thetable ,int id);                   
int find_name(vector<menubook> book ,string name);
void add_daily_income(vector<daily_income> &daily,int income);
void show_bill(vector<table_order> &note ,vector<menubook> book);
int check_bill(vector<table_order> &note ,vector<menubook> book);
void show_table_order(vector<table_order> thetable,vector<menubook> book);
vector<table_order>* which_table(int number);
void update_menubook(vector<menubook> &book);
void table_bill(vector<daily_income> &daily);
void print_daily_income(vector<daily_income> daily);
void push_daily_income(vector<daily_income> &daily);
void pull_daily_income(vector<daily_income> &daily);
string datenow();

int main() 
{ 
    sqlite3_stmt * stmt;    // Save the connection result
    int exit = 0;
    exit = sqlite3_open("database.db", &db); 
  
    // Test if there was an error
    if (exit) { 
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
    } else {
        cout << "Opened Database Successfully!" << endl; 
    }
    
    pull_daily_income(daily);
    push_menubook(book);
    cout<<"printmenubook\n";  
    printmenubook(book);
    start();


    push_daily_income(daily);
    daily.clear();
    sqlite3_close(db);// Close the connection
    return (0); 
} 

void start(){
    int select;
    bool done = true;
    
    while(done){
        cout <<setw(57) << ">_< >_< >_< >_< >_< >_< >_< >_< >_< >_< >_< >_<" <<endl;
        cout <<setw(12)<<"|"<<setw(26) <<"--START--"<<setw(19) << "|" <<endl;
        cout <<setw(12)<<"|"<<"  1.Select table ( press 1 )" <<setw(17)  <<"|" <<endl;
        cout <<setw(12)<<"|"<<"  2.Add menu for selected table ( press 2 )"<< setw(2) <<"|" <<endl;
        cout <<setw(12)<<"|"<<"  3.Check bill ( press 3 )"<<setw(19) <<"|" <<endl ;
        cout <<setw(12)<<"|"<<"  4.Edit menu ( press 4 )" <<setw(20) <<"|" <<endl;
        cout <<setw(12)<<"|"<<"  5.Check daily balance ( press 5 )"<<setw(10) <<"|" <<endl;
        cout <<setw(12)<<"|"<<"  6.Exit ( press 6 )"<<setw(25)  << "|" << endl;
        cout <<setw(57)<< ">_< >_< >_< >_< >_< >_< >_< >_< >_< >_< >_< >_<" <<endl;
        cout <<setw(16)<<"\t" <<"  What would you choose ? : ";
        cin >> select;
        


        switch (select)
        {
            case 1:
                select_table();
                break;
            case 2:
                number_table_order(0);
                break;
            case 3:
                table_bill(daily);
                break;
            case 4:
                cout<<"4.Edit menu"<<endl;
                edit_menu();
                break;
            case 5:
                print_daily_income(daily);
                break; 
            case 6:
                done = false;
                cout<<setw(63)<<"^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^\n\n";
                cout<<setw(40)<< "Ending Program"<<endl;
                cout<<setw(63)<<"^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^ ^.^\n\n";
                break;
            default:
                cout<<setw(63)<<"' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^\n\n";
                cout<<setw(38)<< "Error!!\n";
                cout<<setw(39)<<"Select again"<<endl;
                cout<<setw(63)<<"' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^\n\n";
                break;
        }
        
    }
    
}

void select_table(){
    
    int sl_table,count=0;
    cout<<setw(63)<<"._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._.\n\n";
    for(int j=0 ; j<9;j++){
        if(j%3 == 0){
            cout << setw(28) << table[j];
        }else{
            cout<<setw(5) << table[j];
        }
        if((j+1)%3 == 0 && j != 0){
            if((j+1) == 9){
                cout << "\n";
            }else{
                cout << "\n\n";
            }
        }
        if(table[j]=='-')
        {
            count++;
        }
    }
    cout << endl;
    cout<<setw(63)<<"._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._.\n\n";
    if(count==9)
    {
        cout<<setw(63)<<"-_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_-\n\n";
        cout<<setw(45)<<"Sorry all table are used." <<"\n";
        cout<<setw(43)<<"Please wait it empty." << endl;
        cout<<setw(63)<<"-_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_-\n\n";
        return;
    }
    while(true)
    {
        cout<<setw(55)<< "input table want to select ( exit input 0 ): ";
        cin >> sl_table;
        cout << "\n";
        if(sl_table == 0){
            return; // out of select table
        }
        if (sl_table<1||sl_table>9)
        {
            cout<<setw(63)<<"=_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_=\n";
            cout<<setw(43)<<"Don't have this table\n";
            cout<<setw(63)<<"=_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_= =_=\n";
            continue;
        }
        
        if(table[sl_table-1]!='-')
        {
           break; 
        }
        else 
        {
            cout<<setw(63)<<"T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T\n";
            cout<<setw(60)<<"Sorry this table is used. Please select table again." << endl;
            cout<<setw(63)<<"T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T  T-T\n";
            continue;
        }
    }
    
    table[sl_table-1] = '-';

    for(int j=0 ; j<9;j++){
        if(j%3 == 0){
            cout << setw(28) << table[j];
        }else{
            cout<<setw(5) << table[j];
        }
        if((j+1)%3 == 0 && j != 0){
            if((j+1) == 9){
                cout << "\n";
            }else{
                cout << "\n\n";
            }
        }
    }
    number_table_order(sl_table);
}

void select_checkbill(){
    int sl_table;
    cout << "input num : ";
    cin >> sl_table;

    table[sl_table-1] = '-';
    for(int j=0 ; j<9;j++){
        cout << table[j] << " ";
    }
    cout <<endl;
}

void loop_order(vector<table_order> &thetable){
    int num;
    while (true)
    {
        cout<<setw(63)<<">< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< ><\n\n";
        cout<<setw(44)<< "Select these choice\n";
        cout<<setw(42)<< "[1] Add Order \n";
        cout<<setw(45)<<"[2] Cancel Order \n";
        cout<<setw(36)<<"[3] Exit "<<endl;
        cout<<setw(44)<<"Input Your choice : ";
        cin >> num;
        cout<<setw(63)<<">< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< >< ><\n\n";
        switch (num)
        {
            case 1 :
                cout<<"[1] Add Order Process\n"<<endl; //เอาอยู่บ่?
                add_table_order(thetable);
                break;

            case 2 :
                cout<<"[2] Cancel Order Process\n"<<endl;
                cancel_table_order(thetable);
                break;

            case 3 :
                cout<<"[3] Exit Order Food Process\n"<<endl;
                return ;
                break;
    
            default:
                cout<<setw(44)<< "Error Select again!!"<<endl;
                //cout<<setw(63)<<"---------------------------------------------------------\n";

                break;
        }
    }
    return ;
}

void show_table_order(vector<table_order> thetable,vector<menubook> book)
{
    cout<<setw(64)<<"O_o O_o O_o O_o O_o O_o O_o O_o O_o O_o O_o O_o O_o O_o O_o\n\n";
    cout <<setw(21)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Quantity"<<endl;
    for (unsigned int i = 0; i < thetable.size(); i++)
    {
        cout <<setw(21)<<right<<thetable[i].food_id;
        int k= find_id(book,thetable[i].food_id);
        cout <<"\t"<<setw(15)<<left<<book[k].name; 
        cout <<setw(10)<<right<<thetable[i].order<<endl;
    }
    cout << endl;
    //cout<<setw(63)<<"---------------------------------------------------------\n";
}

void number_table_order(int tb_num=0){
    int sl_table,count=0;
    if(tb_num==0)
    {
        cout<<setw(63)<<"---------------------------------------------------------\n\n";
        for(int j=0 ; j<9;j++){
            if(j%3 == 0){
                cout << setw(28) << table[j];
            }else{
                cout<<setw(5) << table[j];
            }
            if((j+1)%3 == 0 && j != 0){
                if((j+1) == 9){
                cout << "\n";
                }else{
                cout << "\n\n";
                }
            }
            if(table[j]!='-')
            {
            count++;
            }

        }
        cout << endl;
        if(count==9)
        {
            cout<<setw(63)<<"T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T\n";
            cout<<setw(63)<<"---------------------------------------------------------\n";
            cout<<setw(45)<<"Sorry all table are used." <<"\n"; // maybe [please select table first]
            cout<<setw(43)<<"Please wait it empty." << endl;
            cout<<setw(63)<<"---------------------------------------------------------\n";
            cout<<setw(63)<<"T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T\n";
            return;
        }
        while(true)
        {
            cout<<setw(63)<<"---------------------------------------------------------\n";
            cout <<setw(50)<< "input table want to Order Menu : "; 
            cin >> sl_table;
            if (sl_table<1||sl_table>9)
            {
                cout<<setw(63)<<"---------------------------------------------------------\n";
                cout<<setw(43)<<"Don't have this table\n";
                //cout<<setw(63)<<"---------------------------------------------------------\n";
                continue;
            }
        
            if(table[sl_table-1]=='-')
            {
               break; 
            }
            else 
            {
                cout<<setw(63)<<"---------------------------------------------------------\n";
                cout<<setw(60)<<"Sorry this table is empty. Please select table again." << endl;
                //cout<<setw(63)<<"---------------------------------------------------------\n";
                continue;
            }
        }
    }
    else
    {
        sl_table=tb_num;
    }
    vector<table_order>* vec_point;
    vec_point=which_table(sl_table);
    if(vec_point==NULL)
    {
        cout<<setw(63)<<"---------------------------------------------------------\n";
        cout<<"Error table" << endl;//how to access
        cout<<setw(63)<<"---------------------------------------------------------\n";

        return;
    }
    loop_order(*vec_point);
    return;
}

vector<table_order>* which_table(int number)
{
    switch (number)
    {
    case 1:
        return &table1;
        break;
    case 2:
        return &table2;
        break;
    case 3:
        return &table3;
        break;
    case 4:
        return &table4;
        break;
    case 5:
        return &table5;
        break;
    case 6:
        return &table6;
        break;
    case 7:
        return &table7;
        break;
    case 8:
        return &table8;
        break;
    case 9:
        return &table9;
        break;
    default:
        return NULL;
        break;
    }     
}

void allmenu(){

    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement

    const unsigned char* text;
    bool done = false;

    cout<<"~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~" << "\n";
    cout <<setw(10)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Price"<<endl;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
            for(int i=0; i<3;i++){
                text = sqlite3_column_text(stmt, i);
                if(i==1)
                {
                    cout <<"\t"<<setw(15)<<left; 
                } 
                else cout <<setw(10)<<right;
                cout <<text ;
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
    cout<<"~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~ ~-~\n\n";

    sqlite3_finalize(stmt);

}

void showmenu(int id){
    sqlite3_stmt * stmt;

    string str_id = to_string(id);
    string text2 = "SELECT * FROM menu WHERE food_id = " + str_id + " ;" ;

    sqlite3_prepare( db, text2.c_str(), -1, &stmt, NULL );//preparing the statement
    const unsigned char* text;
    sqlite3_step(stmt);
            for(int i=0;i<3;i++){
                text = sqlite3_column_text(stmt,i);
                cout << text << " ";
            }
    sqlite3_finalize(stmt);
}

void addmenu(){
    sqlite3_stmt * stmt;
    
    string name,price;
    cout << "Input food name : ";
    cin >> name ;
    cout << "Input food price : ";
    cin >> price ;

    string sqlstatement = "INSERT INTO menu (food_name, price) VALUES ('" + name + "','" + price + "');";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
    update_menubook(book);
    
}

void deletemenu(){
    bool no_empty= false;
    for (int i = 0; i < 9; i++)
    {
        if(table[i]=='-')
        {
            no_empty=true;
            break ;
        }
    }
    if(no_empty)
    {
       for (int i = 0; i < 9; i++)
        {
            cout<<table[i]<<" ";
        }
        cout<<"\nThese are used table. Wait all table are empty "<<endl; 
        return ;
    }
    string id;
    cout << "Input food ID : ";
    cin >> id;
    sqlite3_stmt * stmt;
    string sqlstatement = "DELETE FROM menu WHERE food_id = ('" + id + "');";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
    update_menubook(book);
}

void edit_menu(){
    int select;
    bool done=true;
    while(done){
        cout<<setw(63)<<"' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ `\n";
        cout<<setw(40)<< "what you want to edit\n";
        cout<<setw(41)<<"1.Add menu ( press 1 )\n"; //Add menu in database
        cout<<setw(44)<<"2.Delete menu ( press 2 )\n"; // Delete menu in database
        cout<<setw(42)<<"3.Show menu ( press 3 )\n";  //Show menu in book
        cout<<setw(40)<<"4.Go back ( press 4 )\n";
        cout<<setw(38)<<"Input your choice : ";
        cin >> select;

        switch (select)
            {
                case 1:
                    addmenu();
                    break;
                case 2:
                    deletemenu();
                    break;
                case 3:
                    printmenubook(book);
                    break;
                case 4:
                    done = false;
                    break;
                default:
                    cout<<"---------------------------------------------------------\n";
                    cout<< "Error Select again"<<endl;
                    cout<<"---------------------------------------------------------\n";
                    break;
            }
    }


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

void checkbill(){
    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement
}

void add_table_order(vector<table_order> &thetable)
{
    allmenu();
    int id,num,want;
    while(true){
        cout<<setw(63)<<"' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ ' _ ^ \n\n";
        cout<<setw(60)<< "Input order menu ID to add (If want to exit input 0) : "; // can use 03,04,0..
        cin >> id;
        if(id == 0) break;
        int x=find_id(book,id);
        //cout<<x<<endl;
        if(x==-1)
        {
            cout<<setw(63)<<"-_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_- -_-\n\n";
            cout<<setw(49)<<"Don't have this ID in Menubook\n";
            //cout<<setw(63)<<"---------------------------------------------------------\n";
            continue;
        }
        cout <<setw(47)<< "Input quantity order to add : ";
        cin >> num;
        if(num>0)
        {
            vec_addorder(thetable,id,num);
            show_table_order(thetable,book);
        }
        else 
        {
            cout<<setw(63)<<"---------------------------------------------------------\n";
            cout<<setw(48) << "Input quantity order Error!! \n";
        }       
    }
    return ;

}

void cancel_table_order(vector<table_order> &thetable )
{
    int id,num,quantity;
    if(thetable.size()==0)
    {
        cout<<setw(63)<<"---------------------------------------------------------\n";
        cout<<setw(42)<<"The Order is empty"<<endl;
       //cout<<setw(63)<<"---------------------------------------------------------\n";
        return;
    }
    show_table_order(thetable,book);
    while(true){
        bool done=true;
        cout<<setw(63)<<"._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._. ._.\n\n";
        cout<<setw(62)<< "Input order menu ID to cancel(If want to exit input 0) : ";
        cin >> id;
        if(id == 0) 
        {
        break;
        }
        int x = find_id_in_order (thetable,id);
        //cout<<x<<endl;
        if(x==-1)
        {
            cout<<setw(63)<<"' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ `\n";
            cout<<setw(49)<<"Don't have this ID in Order\n";
            //cout<<setw(63)<<"---------------------------------------------------------\n";
            continue;
        }
        else if(thetable[x].order==0)
        {
            cout<<setw(63)<<"' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ `\n";
            cout<<setw(47)<<"Don't have this ID in Order\n";
            //cout<<setw(63)<<"---------------------------------------------------------\n";
            continue;
        }

        quantity = thetable[x].order;
        cout<<setw(45) << "Input quantity order to cancel "<<"[ 1 ~ "<<quantity<<" ] : ";
        cin >> num;
        if(num<=quantity&&quantity!=0&&num>0)
        {
            done=vec_cancelorder(thetable,id,num);//>>how to access;
            show_table_order(thetable,book);
        }
        else 
        {
            cout<<setw(63)<<"---------------------------------------------------------\n";
            cout<<setw(49)<< "Input quantity order Error \n";
            
        }
        if(done==false)
        {
            cout<<setw(63)<<"' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ `\n\n";
            cout<<setw(47)<< "Cancelorder order Error!! \n";//how to access
            cout<<setw(63)<<"' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ ` ' ~ `\n\n";
        } 

    }
    return ;
}

bool vec_cancelorder(vector<table_order> &note ,int id,int num)
{
    bool having=true;

    for(unsigned int i=0 ;i<note.size();i++)
    {
        if(note[i].food_id==id)
        {
            note[i].order-=num;
            if(note[i].order==0)
            {
               note.erase(note.begin()+i); 
            }
            return true;
        }
    }
    return false;

}

void vec_addorder(vector<table_order> &note , int id, int num)
{
    
    bool having=true;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        if(note[i].food_id==id)
        {
            having=false;
            note[i].order+=num;
            return;
        }
    }
    if(having)
    {
        table_order temp={id,num};
        note.push_back(temp);
    }
}

int find_id_in_order (vector<table_order> thetable ,int id)
{
    for(unsigned int i=0 ;i<thetable.size();i++)
    {
        if(thetable[i].food_id==id)
        {
            return i;    
        }
    }
    return -1;

}

int find_id(vector<menubook> book ,int id)
{
    for(unsigned int i=0 ;i<book.size();i++)
    {
        if(book[i].id==id)
        {
            return i;    
        }
    }
    return -1;

}

int find_name(vector<menubook> book ,string name)
{
    for(unsigned int i=0 ;i<book.size();i++)
    {
        if(book[i].name==name)
        {
            return i;    
        }
    }
    return -1;
}

string datenow()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string year=to_string(1900 + ltm->tm_year);
    string month=to_string(1 + ltm->tm_mon);
    string day=to_string(ltm->tm_mday);
    string date=year+"-"+month+"-"+day;
    return date;

}

void add_daily_income(vector<daily_income> &daily,int income)
{   
     string date = datenow();
    for(unsigned int i=0 ;i<daily.size();i++)
    {
        if(daily[i].date==date)
        {
           daily[i].income+=income;
           return ;
        }
    }
        daily_income temp={date,income};
        daily.push_back(temp);
}

void show_bill(vector<table_order> &note ,vector<menubook> book)
{
    double sum=0;

    cout<<setw(65)<<"$ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $\n";
    cout<<setw(65)<<"|---------------------------------------------------------|\n";
    cout<<setw(6)<<"|"<<setw(58)<<"|"<<endl;
    cout <<setw(6)<<"|"<<setw(4)<<right<<" Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Price";
    cout <<setw(10)<<right<<"Quantity"<<setw(10)<<right<<"Total"<<setw(3)<<"|"<<endl;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        int j=find_id(book,note[i].food_id);
        cout <<setw(6)<<"|"<<setw(4)<<right<<book[j].id<<"\t"<<setw(15)<<left <<book[j].name<<setw(10)<<right<<book[j].price;
        cout <<setw(10)<<right<<note[i].order<<setw(9)<<right<<note[i].order*book[j].price<<setw(4)<<"|"<<endl;
        sum+=note[i].order*book[j].price;
    }
    cout<<setw(65)<<"|---------------------------------------------------------|\n";
    cout <<setw(6)<<"|"<<setw(4)<<right<<" "<<"\t"<<setw(15)<<left <<"SUM"<<setw(29)<<right<<sum <<setw(4)<<"|"<<endl;
    cout<<setw(65)<<"|---------------------------------------------------------|\n";
    cout<<setw(65)<<"$ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $\n";
    return;
}

int check_bill (vector<table_order> &note ,vector<menubook> book)
{
    double sum=0;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        int j=find_id(book,note[i].food_id);
        sum+=note[i].order*book[j].price;
    }
    return sum;
}

void table_bill(vector<daily_income> &daily)
{
    int number ,count=0 ;
    vector<table_order> *vec_point; 
    vec_point = which_table(number);
    cout<<setw(63)<<"-----------------------------------------------------------------\n";
    cout<<setw(64)<<"u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u\n\n";
    for(int j=0 ; j<9;j++){
        if(j%3 == 0){
            cout << setw(28) << table[j];
        }else{
            cout<<setw(5) << table[j];
        }
        if((j+1)%3 == 0 && j != 0){
            if((j+1) == 9){
                cout << "\n";
            }else{
                cout << "\n\n";
            }
        }
        if(table[j]!='-')
        {
            count++;
        }
    }
    cout << endl;
    cout<<setw(63)<<"u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u u_u\n";
    cout<<setw(63)<<"------------------------------------------------------------------\n";
    if(count==9)
    {
        cout<<setw(43)<<"All table are empty." << endl;
        cout<<setw(63)<<"---------------------------------------------------------\n";
        return;
    }
    while(true)
    {
        cout<<setw(63)<< "input table want to check bill (If want to exit input 0): ";
        cin >> number ;
        //cout<<setw(64)<<"---------------------------------------------------------\n\n";
        if(number==0)
        {  
            return;
        }
        else if(table[number-1]!='-')
        {
            cout<<setw(64)<<"T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T\n\n";
            cout<<setw(61)<<"Sorry this table are empty. Please select table again.\n\n" << endl;
            cout<<setw(64)<<"T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T T_T\n\n"; 
        }
        else 
        {
            break;
        }
    }
    vec_point = which_table(number);
    show_bill(*vec_point,book);
    int money=check_bill(*vec_point,book);
    add_daily_income(daily,money);
    table[number-1]=(number+48);
    (*vec_point).clear();


}

void print_daily_income(vector<daily_income> daily)
{
    cout<<"-------------------------------------------------------------------\n";
    cout<<"^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_-\n";
    cout <<setw(25)<<right<<"Date"<<"\t"<<setw(20)<<right<<"income"<<endl;
    for(unsigned int i=0;i<daily.size();i++)
    {
        cout<<setw(25)<<right<<daily[i].date<<"\t"<<setw(20)<<right<<daily[i].income<<endl;
    }
    cout<<"^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- ^_- \n";
    cout<<"-------------------------------------------------------------------\n";
    return ;

}

void push_daily_income(vector<daily_income> &daily)
{
    delete_old_daily();

    sqlite3_stmt * stmt;
    //menu --------> daily income
    sqlite3_prepare( db, "SELECT * FROM dailybill;", -1, &stmt, NULL );
    string date , income;
    int int_income;
    for(unsigned int i=0;i<daily.size();i++){
        date = daily[i].date;
        income = to_string(daily[i].income);
        string sqlstatement = "INSERT INTO dailybill (date,income) VALUES ('" + date + "','" + income + "');";
        sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
        sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
    }
    
}

void pull_daily_income(vector<daily_income> &daily)
{
    sqlite3_stmt * stmt;
    //menu --------> daily income
    sqlite3_prepare( db, "SELECT * FROM dailybill;", -1, &stmt, NULL );
    const unsigned char* text;
    daily_income tempdaily;
    int num;
    bool done = false;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
                for(int i=0; i<2;i++){
                    if(i==0)
                    {
                        basic_string<unsigned char> temp = sqlite3_column_text(stmt, i);
                        string text( temp.begin(), temp.end() );
                        tempdaily.date=text;
                        daily.push_back(tempdaily);
                    }
                    else if(i==1)
                    {
                        num= sqlite3_column_int(stmt,i);
                        daily[daily.size()-1].income =num;
                    }
                }
                break;
            
            case SQLITE_DONE:
                done = true;
                cout<<"Push Daily Income Successfully!"<<endl;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return;
        }
        
    }

    sqlite3_finalize(stmt);

}

void all_daily_income(){

    sqlite3_stmt * stmt;
    //menu --------> daily income
    sqlite3_prepare( db, "SELECT * FROM dailybill;", -1, &stmt, NULL );//preparing the statement

    const unsigned char* text;
    bool done = false;
    cout<<"---------------------------------------------------------\n";
    cout <<setw(15)<<right<<"Date"<<"\t"<<setw(10)<<right<<"income"<<endl;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
            for(int i=0; i<2;i++){
                text = sqlite3_column_text(stmt, i);
                if(i==1)
                {
                    cout <<"\t"<<setw(10)<<right; 
                } 
                else cout<<setw(15)<<right;
                cout <<text ;
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
    cout<<"---------------------------------------------------------\n";

    sqlite3_finalize(stmt);

}
void update_menubook(vector<menubook> &book)
{
    
    book.clear();
    push_menubook(book);
    cout<<"Menubook is update"<<endl;
    printmenubook(book);
    return;
}

void push_menubook(vector<menubook> &book){

    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );
    const unsigned char* text;
    menubook tempbook;
    int num;
    bool done = false;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
            for(int i=0; i<3;i++){
                if(i==0)
                {
                    tempbook.id = sqlite3_column_int(stmt,i);
                    book.push_back(tempbook);
                }
                else if(i==1)
                {
                    basic_string<unsigned char> temp = sqlite3_column_text(stmt, i);
                    string text( temp.begin(), temp.end() );
                    book[book.size()-1].name=text;
                }
                else if(i==2)
                {
                    num =sqlite3_column_int(stmt,i);
                    book[book.size()-1].price=num;

                }
            }
                break;
            
            case SQLITE_DONE:
                done = true;
                cout<<"Push Menubook Successfully!"<<endl;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return;
        }
        
    }

    sqlite3_finalize(stmt);

}

void printmenubook(vector<menubook> &book)
{
    cout<<"O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O\n";
    cout <<setw(20)<<right<<">>>>Food ID"<<"\t"<<setw(20)<<left <<">>Food Name"<<setw(10)<<right<<">>>>Price"<<endl;
    for (unsigned int i = 0; i < book.size(); i++)
    {
        cout <<setw(20)<<right<<book[i].id;
        cout <<"\t"<<setw(20)<<left<<book[i].name; 
        cout <<setw(10)<<right<<book[i].price<<endl;

    }
    cout<<"O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O O.O\n";
}

void delete_old_daily(){
    
    sqlite3_stmt * stmt;
    string sqlstatement = "DELETE FROM dailybill;";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);

}
//gcc sqlite3.c -c   ทำแบบนี้มันจะไม่ได้ exe แต่จะได้เป็น object file ที่ลิงค์เข้ากับโค้ดให้โปรแกรมเราไปเรียกใช้ตอน compile ครับ
//how to run (safe file first) ; g++ main.cpp sqlite3.o ; ./a.exe
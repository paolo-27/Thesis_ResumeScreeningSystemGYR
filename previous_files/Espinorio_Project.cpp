/*
    This program is a simple debt management system that allows users to register, log in, add debts, view debts, and save their records.
    It uses file handling to store user accounts and debt records.
    The program also allows sorting of debts by date or amount.
    Note: The system use system("clear") to clear the console screen, this is for linux users, if you are using windows, please change it to system("cls")
*/

using namespace std;
#include<iostream>
#include<fstream>
#include<vector> // this will be used to store the debt records and the user accounts before writing to the file and storing after reading the file
#include<string>
#include<sstream>
#include<cstring>
#include<algorithm> //this will be used for sorting the debt records
#include<iomanip> //this will be used to format the output and to have proper alignment 

//struct for storing the borrower's information
struct debt_info{

    string lender; // this variable is used to store the lender's username and is used to track debts for each current user(lender)
    string name; // borrower's name
    double amount;  // amount borrowed
    string contact_num; // borrower's contact number
    string borrow_date; // date when the debt was borrowed
    string due_date; // date when the debt is due to be paid
};

//struct for storing the user account information
struct debter_account{

    string username;
    string password;
};

//=================================
// class for debt management system functions
//=================================
class debt_system_function{

private:
    // helper function to split a string by a delimiter and return a vector of strings
    vector<string> csvline(const string& line, const char delimeter = ','){
        vector<string> result;
        istringstream ss(line);
        string item;
        while(getline(ss,item,delimeter)){
            result.push_back(item);
        }
        return result;
    }
    // function to clear the console screen
    void clearscreen(){
        system("clear"); // for linux users, if you are using windows, please change it to system("cls")
    }

public:
    // function to register a new account
    void register_account(vector<debter_account>&debter){
        clearscreen(); 
        cout<<string(29, '-');cout<<"Register Account";cout<<string(29, '-')<<endl;
        debter_account d;
        cout<<"Enter Username:";
        cin>>ws;getline(cin,d.username);

        // check if the username already exists
        for(size_t i=0;i<debter.size();i++){
            if(debter[i].username == d.username){
                cout<<"Username already exists\n";
                return;
            }
        }
        // if the username does not exist, proceed to enter password
        cout<<"Enter password:";
        cin>>ws;getline(cin, d.password);
        cout<<"Registration Successfully\n";
        debter.push_back(d); // add the new account to the vector
        
    }

    void display_menu1(){
        // display the main menu after logging in for the debt management system
        clearscreen();
        cout<<string(29, '-');cout<<"DEBT MANAGEMENT SYSTEM";cout<<string(29, '-')<<endl;
        cout<<"[1] Add Debt\n[2] View Debt\n[3] Save added and Deleted Debt\n[4] Exit\n ";
    }

    void add_debt(vector<debt_info>&debt,const string &current_user, const string& filename){ // this function uses reference to the debt vector and the current user to add a new debt record
        // function to add a new debt record
        clearscreen();
        debt_info df;
        df.lender = current_user; // set the lender's username to the current user
        double init_debt;
        double init_interest;
        cout<<string(29, '-');cout<<"Borrower's Information";cout<<string(29, '-')<<endl;
        cout<<"Enter Fullname:";
        cin>>ws;getline(cin,df.name);
            for(size_t i = 0; i<debt.size(); i++){
                if(debt[i].lender == current_user){ // check if the debt record belongs to the current user
                    if(debt[i].name == df.name){    // check if the borrower already exists
                        cout<<"Please Make "<<df.name<<" Pay Before Making New Debt Record\n"; // if the borrower already exists, prompt the user to make the borrower pay before adding a new debt record
                        return;
                    }       
                } 
            }
        cout<<"Enter Contact Number: ";
        cin>>ws;getline(cin,df.contact_num);
        cout<<"Enter Amount: ";
        cin>>init_debt;
        while(init_debt<=0){ // check if the amount is less than 0
            cout<<"Please Enter Greater than 0\n";
            cout<<"Enter Amount: ";
            cin>>init_debt;
        }
        cin.ignore();
        do{
            cout<<"Enter Interest Rate(%):";
            cin>>init_interest;
            cin.ignore();
                if(init_interest>=0){
                    double interest = init_interest/100;
                        if(interest > 0){
                        df.amount = init_debt + (init_debt * interest); // calculate the total amount to be paid including interest, user can also enter 0% interest rate
                        }
                        else{df.amount = init_debt;}
                }
                else{
                    cout<<"Please Enter Equal or Greater than 0\n";
                }
        }while(init_interest<0);
        cout<<"Enter Borrow date: ";
        cin>>ws;getline(cin,df.borrow_date);
        cout<<"Enter Due Date: ";
        cin>>ws;getline(cin,df.due_date);
        cout<<"Debt Added Successfully!\n";
        cout<<"Expect "<<df.name<<" to pay "<<df.amount<<" before "<<df.due_date<<endl;
        debt.push_back(df); // add the new debt record to the vector
        save_debt(debt, filename, current_user); // save the debt records to the file
    }
    // function to view the debt records of the current user
    void view_debt(vector<debt_info>&debt,const string& current_user){
        clearscreen();
        cout<<string(29, '-');cout<<"Borrower's List";cout<<string(36, '-')<<endl;
        cout<<string(80, '-')<<endl;
        cout<<left<<setw(25)<<"NAME"<<setw(20)<<"CONTACT NUMBER"<<setw(20)<<"AMOUNT BORROWED"<<setw(15)<<"DUE DATE"<<endl;
        cout<<string(80, '-')<<endl;
            for( size_t i = 0; i<debt.size(); i++){
                if(debt[i].lender == current_user){ // check if the debt record belongs to the current user
                    cout<<left<<setw(25)<<debt[i].name<<setw(20)<<debt[i].contact_num<<setw(20)<<debt[i].amount<<setw(15)<<debt[i].due_date<<endl<<endl<<endl;
                }
            }

    }
    // function to save the debt records to a file in csv format(with message)
    void save_debt(vector<debt_info>&debt, const string& filename, const string& current_user){
        
        ofstream file(filename);
        for(size_t i = 0; i<debt.size(); i++){
            if(debt[i].lender == current_user){ // check if the debt record belongs to the current user and write the file in csv format
                file<<debt[i].name<<","<<debt[i].contact_num<<","<<debt[i].amount<<","<<debt[i].borrow_date<<","<<debt[i].due_date<<"\n";
            }
        }
        file.close();
        cout<<"Debt Saved Successfully!\n";

    }
    // function to save the debt records to a file in csv format after removing a debt record (without message)
    void save_debt_remove(vector<debt_info>&debt, const string& filename, const string& current_user){
        
        ofstream file(filename);
        for(size_t i = 0; i<debt.size(); i++){
            if(debt[i].lender == current_user){ // check if the debt record belongs to the current user and write the file in csv format
                file<<debt[i].name<<","<<debt[i].contact_num<<","<<debt[i].amount<<","<<debt[i].borrow_date<<","<<debt[i].due_date<<"\n";
            }
        }
        file.close();
    }
    // function to save the user accounts to a file in csv format
    void save_account(vector<debter_account>&debter, const string filename){
        ofstream file(filename);
        for( const auto&d : debter){
            file<<d.username<<","<<d.password<<"\n"; // save the username and password in csv format
        }
        file.close();
        cout<<"Account Registered Saved!\n";
    }



    void load_debt_file(vector<debt_info>&debt,const string& filename,const string& current_user){

        debt.erase(remove_if(debt.begin(), debt.end(), [&](const debt_info& d){ // remove debts to ensure that only the debts of the current user are loaded, also to avoid duplicate debts
            return d.lender == current_user;}), debt.end());

        ifstream file(filename); // open the file to read the debt records
        string line;
        while(getline(file,line)){
            vector<string> part = csvline(line); // split the line by comma
                if (part.size() == 5){
                    string name = part[0]; // get the borrower's name
                    string contact_num = part[1]; // get the borrower's contact number
                    double amount = stod(part[2]);  // get the amount and convert the it from string to double
                    string borrow_date = part[3];   // get the borrow date
                    string due_date = part[4];  // get the due date
                    debt.push_back({current_user,name, amount, contact_num, borrow_date, due_date}); // add the debt record to the vector
                }
        }
        file.close();
    }

    void load_account(vector<debter_account> &debter, const string&filename){
        ifstream file(filename); // open the file to read the user accounts
        string line;
        while(getline(file,line)){
            vector<string> part = csvline(line);
            if (part.size() == 2 ){
                string username = part[0]; // get the username
                string password = part[1]; // get the password
                debter.push_back({username,password}); // add the account to the vector
            }
        }
        file.close();
    }

    // function to view the debt records and provide options to sort or remove debts
    void view_debt_menus(vector<debt_info>& debt, const string& current_user, const string& filename){
        int chs;
        view_debt(debt,current_user);
            do{
                cout<<"[1] Sort by Date\n[2] Sort by Amount(highest to lowest)\n[3] Sort by Amount(lowest to highest)\n[4] Remove Debt\n[5] Exit\n";
                cout<<"Enter Choice: ";
                cin>>chs;
                cin.ignore();
                    switch(chs){
                        case 1:
                            clearscreen();
                            debt_sort_date(debt);
                            view_debt(debt,current_user);
                            load_debt_file(debt, filename, current_user); // reload the debt file to ensure that it will be back to unsorted debt records
                            break;
                        case 2:
                            clearscreen();
                            debt_sort_amount_highest(debt);
                            view_debt(debt,current_user);
                            load_debt_file(debt, filename, current_user); 
                            break;
                        case 3:
                            clearscreen();
                            debt_sort_amount_lowest(debt);
                            view_debt(debt,current_user);
                            load_debt_file(debt, filename, current_user); 
                            break;

                        case 4:
                            clearscreen();
                            remove_debt(debt,current_user);
                            view_debt(debt,current_user);
                            save_debt_remove(debt, filename, current_user); // save function specific to save the debt records after removing a debt to avoid display redundant message
                            break;

                        case 5:
                            break;
                    }
               
            }while(chs!=5);
    }
    // function to remove a debt record by the borrower's name
    void remove_debt(vector<debt_info>& debt, const string& current_user){
        string name;
        cout<<"enter name of the borrower: ";
        cin>>ws;getline(cin,name);
            for(size_t i = 0; i<debt.size(); i++){
                if(debt[i].lender == current_user){
                    debt.erase(remove_if(debt.begin(), debt.end(), [&](const debt_info& d){ // remove the debt record if the borrower's name matches the input name
                    return d.name == name;}), debt.end());
                }
            }
        
    }

    // lambda function to sort the debt records by due date in earliest to latest
    void debt_sort_date(vector<debt_info>& debt){
        sort(debt.begin() , debt.end() , [] (const debt_info& d, const debt_info& e){ return d.due_date < e.due_date ; }); // specify the sorting criteria (earlier to latest) using a lambda function 
    }
    // function to sort the debt records by amount from highest to lowest
    void debt_sort_amount_highest(vector<debt_info>& debt){
        sort(debt.begin(),debt.end() , [] (const debt_info& d, const debt_info& e){return d.amount > e.amount;}); //specify the sorting criteria (highest to lowest) using a lambda function
    }
    // function to sort the debt records by amount from lowest to highest
    void debt_sort_amount_lowest(vector<debt_info>& debt){
        sort(debt.begin(), debt.end() , [] (const debt_info& d, const debt_info& e){return d.amount < e.amount;});  // specify the sorting criteria (lowest to highest) using a lambda function
    }

    void log_in(vector<debter_account>&debter, vector<debt_info>&debt ){
        debter_account d;
        int chs;
        cout<<"enter username:";
        cin>>ws;getline(cin,d.username);
        cout<<"enter password:";
        cin>>ws;getline(cin,d.password);
        bool found = false; // variable to check if the username and password match
            for(size_t i=0; i<debter.size(); i++){
                if(debter[i].username == d.username && debter[i].password == d.password){ // check if the username and password match
                    string current_user = d.username; // set the current user to the logged in user
                    string filename = current_user + ".txt"; // create a file for the current user to store their debt records
                    load_debt_file(debt,filename,current_user); // automatically load the debt records of the current user from the file
                    cout<<"Log in Successfully!\n";
                    found = true; // set found to true if the username and password match
                    do{
                        display_menu1();
                        cout<<"Enter Choice:";
                        cin>>chs;
                        cin.ignore();
                            switch(chs){
                                case 1: 
                                    add_debt(debt, current_user,filename );
                                    break;

                                case 2:
                                    view_debt_menus(debt,current_user,filename);
                                    break;
                                
                                case 3:
                                    save_debt(debt,filename, current_user); //manually save the debt records with message
                                    break;

                                case 4:
                                    save_debt_remove(debt,filename, current_user); //automatically save the debt records without message before logging out 
                                    cout<<"Logging out...\n";
                                    return;
                                    
                            }

                            if(chs!=4){
                                cout<<"Press Enter to continue...\n";
                                cin.ignore();
                            }
                    }while(chs!=4);
                }
            
            }   
        if(!found){cout<<"Username or Password is Incorrect!\n";} // if the username and password do not match, prompt the user that the username or password is incorrect
    }
};

int main(){
    vector<debt_info> debt; // vector to store the debt records
    vector<debter_account> debter;
    string account_file = "accounts.txt"; // file to store the user accounts
    debt_system_function go; // object of the debt_system_function class to access its functions
    go.load_account(debter, account_file); // automatically load the user accounts from the file when the program starts
    int chs;
        do{
            system("clear"); // for linux users, if you are using windows, please change it to system("cls")
            cout<<string(29, '-');cout<<"Debt Management System";cout<<string(29, '-')<<endl;
            cout<<"[1] Register\n[2] Log in\n[3] Exit\n";
            cout<<"Enter Choice:";
            cin>>chs;
            cin.ignore();
                switch(chs){
                    case 1:
                        go.register_account(debter);
                        break;
                    
                    case 2:
                        go.log_in(debter, debt);
                        break;
                    
                    case 3:
                        go.save_account(debter, account_file); // automatically save the user accounts before exiting the program
                        cout<<"Exiting program....\n";
                        break;
                }
                if(chs!=3){
                    cout<<"Press Enter to continue.....\n";
                    cin.ignore();
                }
        }while(chs!=3);


    return 0;
}
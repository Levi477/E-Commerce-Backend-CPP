#include <__utility/pair.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "./headers/seller.hpp"
#include "./headers/buyer.hpp"
#include "./headers/product.hpp"

using namespace std;

// helper functions
void sort_by_price(vector<product::Product>& products){
    sort(products.begin(),products.end(),[](product::Product& a,product::Product& b){
      return a.get_price() > b.get_price(); 
  });
  for(auto & product : products){
      product.get_product_details();
  }
}

void sort_by_name(vector<product::Product>& products){
    sort(products.begin(),products.end(),[](product::Product& a,product::Product& b){
      return a.get_name() < b.get_name();
  });
  for(auto & product : products){
      product.get_product_details();
  }
}

// seller typedefs
using seller_class = seller::Seller ;
using seller_addr_class = seller::Address ;
//buyer  typedefs
using buyer_class = buyer::Buyer ;
using buyer_addr_class = buyer::Address;

// all global declaration
unordered_map<string,buyer::Buyer> buyer_map;
unordered_map<string,seller::Seller> seller_map;
unordered_map<string,json> product_map;

int main() {
    char choice = ' ';
    char sec_choice = ' ';
    while(choice != 'q'){

    // Main Choice
    cout << "-----------------------------" << endl;
    cout << "1 . Buyer " << endl;
    cout << "2 . Seller " << endl;
    cout << "Enter your choice : " << endl;
    cin >> choice;
    buyer::load_json_data_to_map(buyer_map);
    seller::load_json_data_to_map(seller_map,product_map);
    if(choice == '1'){
    // If user is Buyer
    cout << "-----------BUYER------------------" << endl;
    cout << "1 . Register" << endl;
    cout << "2 . Delete User " << endl;
    cout << "3 . View Cart " << endl;
    cout << "4 . View Favourites " << endl;
    cout << "5 . View User info " << endl;
    cout << "6 . Add product to cart  " << endl;
    cout << "7 . Add product to favourites" << endl;
    cout << "Enter your choice : " << endl;
    cin >> sec_choice;  

    string username;
    product::Product product;
    string temp = " ";
    unordered_map<string,vector<product::Product> > all_seller_products;
    int item_number ; 
    string seller_name ;

    switch (sec_choice) {

      case '1':
        buyer::register_buyer(buyer_map);
        buyer::save_data(buyer_map);
        break;
      case '2':
        cout << "Enter your username : ";
        cin.ignore();
        getline(cin,username);
        if(buyer_map.count(username) == 0){
            cout << "Username doesn't exist!!";
            break;
          }
        buyer::delete_buyer(buyer_map,username);
        buyer::save_data(buyer_map);
        break;

      case '3':
        cout << "Enter username : ";
        cin.ignore();
        getline(cin,username);
        if(buyer_map.count(username) == 0){
            cout << "Username doesn't exist!!";
            break;
          }
        sort_by_name(buyer_map[username].get_cart());
        break;

      case '4':
        cout << "Enter your username : ";
        cin.ignore();
        getline(cin,username);
          if(buyer_map.count(username) == 0){
            cout << "Username doesn't exist!!";
            break;
          }
        sort_by_name(buyer_map[username].get_favourite());
        break;

      case '5':
        cout << "Enter username : ";
        cin.ignore();
          getline(cin,username);
          if(buyer_map.count(username) == 0){
            cout << "Username doesn't exist!!";
            break;
          }
        buyer_map[username].print_buyer_info();
        break;

      case '6':

        seller::load_all_seller_products(all_seller_products);
          for (auto &[name,product_] : all_seller_products ){
              cout << "---------------------------------" << endl;
              cout << name << endl;
              int count = 1;
              for(auto &_product_ : product_){
                cout << count;
                _product_.get_product_details();
                count++;
            }
          }

          cout << "Enter your username : ";
          cin.ignore();
          getline(cin,username);
          if(buyer_map.count(username) == 0){
            cout << "Username doesn't exist!!";
            break;
          }
          cout << "Enter seller name : ";
          cin.ignore();
          getline(cin,seller_name);
          cout << "Enter item number to add to cart : ";
          cin >> item_number;
          buyer::load_json_data_to_map(buyer_map);
          buyer_map[username].add_to_cart(all_seller_products[seller_name][item_number-1]);
          buyer::save_data(buyer_map);
          cout << "Item added succesfully!" << endl;

        break;

      case '7':

        seller::load_all_seller_products(all_seller_products);
          for (auto &[name,product_] : all_seller_products ){
              cout << "---------------------------------" << endl;
              cout << name << endl;
              int count = 1;
              for(auto &_product_ : product_){
                cout << count;
                _product_.get_product_details();
                count++;
            }
          }
          cout << "Enter your username : ";
          cin.ignore();
          getline(cin,username);
          if(buyer_map.count(username) == 0){
            cout << "Username doesn't exist!!";
            break;
          }
          cout << "Enter seller name : ";
          cin.ignore();
          getline(cin,seller_name);
          cout << "Enter item number to add to favourites : ";
          cin >> item_number;
          buyer::load_json_data_to_map(buyer_map);
          buyer_map[username].add_to_favourites(all_seller_products[seller_name][item_number-1]);
          buyer::save_data(buyer_map);
          cout << "Item added succesfully!" << endl;

        break;

      default: 
        cout << "Enter valid choice!!!" ;
        break;
    }
    }
    else if(choice == '2'){
    // If user is Seller
    cout << "-----------SELLER------------------" << endl;
    cout << "1 . Register" << endl;
    cout << "2 . Delete User " << endl;
    cout << "3 . View user info " << endl;
    cout << "4 . Add product " << endl;

    cout << "Enter your choice : " << endl;
    cin >> sec_choice;
    string username; 

    switch (sec_choice) {
      case '1':
          seller::register_user(seller_map);
          seller::save_data(seller_map,product_map);
          break;
      case '2':
          cout << "Enter your username : ";
          cin.ignore();
          getline(cin,username);
          if(seller_map.count(username) == 0){
            cout << "Username doesn't exist";
            break;
          }
          seller::delete_user(seller_map,username);
          break;
      case '3':
          cout << "Enter your username : ";
          cin.ignore();
          getline(cin,username);
          if(seller_map.count(username) == 0){
            cout << "Username doesn't exist!" << endl;
            break;
          }
          seller_map[username].print_seller_info(product_map);
          break;
      case '4':
          cout << "Enter your username : ";
          cin.ignore();
          getline(cin,username);
          if(seller_map.count(username) == 0){
            cout << "Username doesn't exist";
            break;
          }
          seller::add_product(username,product_map);
          seller::save_data(seller_map,product_map);
        break;

      case 'q':
        break;

      default:
          cout << "Enter valid choice!!!" << endl;
          break;
    }

    }
    else if(choice == 'q'){
    // quit the programme 
    break;
    }
    else{
    // Invalid choice entered
    cout << "Please enter valid choice!!!" << endl;
    }
  }
    

    return 0;
}

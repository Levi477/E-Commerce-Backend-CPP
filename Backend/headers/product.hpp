#ifndef  PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include "json.hpp"


typedef nlohmann::json json;
using namespace std;

namespace product{

class Product {

private:
    string name;
    string price;
    string stock;
    string category;

public:
    Product() {}

    Product(string name, string price, string stock, string category) 
        : name(name), price(price), stock(stock), category(category) {}

    // Setters
    void set_name(string name) { this->name = name; }
    void set_price(string price) { this->price = price; }
    void set_stock(string stock) { this->stock = stock; }
    void set_category(string category) { this->category = category; }

    // Getters
    string get_name() const { return this->name; }
    double get_price() const { return stoi(this->price); }
    string get_stock() const { return this->stock; }
    string get_category() const { return this->category; }

    // Convert Product to JSON
    json to_json() const {
        return {
            {"name", name},
            {"price", price},
            {"stock", stock},
            {"category", category}
        };
    }

    void get_product_details(){
      cout << "product : " << endl;
      cout << "name : " << name << endl;
      cout << "price : " << price << endl;
      cout << "stock : " << stock << endl;
      cout << "category : " << category << endl;
  }

};

}

#endif // ! PRODUCT_H

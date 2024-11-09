#ifndef SELLER_H
#define SELLER_H

#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"
#include <fstream>
#include <unordered_map>
#include "product.hpp"

using namespace std;
typedef nlohmann::json json;

namespace seller {

class Address {
private:
    string shop_name;
    string street;
    string city;
    string state;
    string zip;

public:
    Address() {}

    Address(string street, string city, string state, string zip)
        : street(street), city(city), state(state), zip(zip) {}

    // Setters
    void set_street(string street) { this->street = street; }
    void set_shop_name(string shop) { this->shop_name = shop; }
    void set_city(string city) { this->city = city; }
    void set_state(string state) { this->state = state; }
    void set_zip(string zip) { this->zip = zip; }

    // Getters
    string get_shop_name() const { return shop_name; }
    string get_street() const { return street; }
    string get_city() const { return city; }
    string get_state() const { return state; }
    string get_zip() const { return zip; }
    string get_address() const {
        return shop_name + ", " + street + ", " + city + ", " + state + ", " + zip + ".";
    }
};

class Seller {
private:
    string name;
    string email;
    string phone;
    Address address;

public:
    Seller() {}

    Seller(string name, string email, string phone, Address addr)
        : name(name), email(email), phone(phone), address(addr) {}

    void print_seller_info(unordered_map<string,json > &product_data) const {
        string username = this->name;
        cout << "name : " << name << endl;
        cout << "email : " << email << endl;
        cout << "phone : " << phone << endl;
        cout << "address : " << address.get_address() << endl;   
    std::cout << "Products:" << std::endl;
    for (const auto &product_entry : product_data) {
        const std::string &product_name = product_entry.first;
        const json &product_details = product_entry.second;

 if (product_data.find(username) == product_data.end()) {
        std::cout << "No products found for this seller." << std::endl;
        return;
    }

    // Print products by category
    std::cout << "Products:" << std::endl;
    for (const auto &category_entry : product_data.at(username).items()) {
        const std::string &category = category_entry.key(); // Get the category name
        const json &products = category_entry.value();      // Get the list of products

        std::cout << "  Category: " << category << std::endl;
        
        // Loop through each product in the category
        for (const auto &product : products) {
            std::cout << "    Product Name: " << product["name"] << std::endl;
            std::cout << "    Price: " << product["price"] << std::endl;
            std::cout << "    Stock: " << product["stock"] << std::endl;
            std::cout << std::endl;
        }
      }
    }
  }

    // Setters
    void set_name(string name) { this->name = name; }
    void set_email(string email) { this->email = email; }
    void set_phone(string phone) { this->phone = phone; }
    void set_address(Address addr) { this->address = addr; }

    // Getters
    string get_name() const { return name; }
    string get_email() const { return email; }
    string get_phone() const { return phone; }
    Address get_address() const { return address; }
};

inline void load_all_seller_products(unordered_map<string, vector<product::Product> > &seller_products) {
    ifstream infile("/Users/levi/Desktop/E-Commerce-Backend-CPP/JSON/seller.json");
    if (!infile.is_open()) {
        cerr << "Error opening seller.json file." << endl;
        return;
    }

    json j;
    try {
        infile >> j;
    } catch (...) {
        cerr << "Error reading JSON data." << endl;
        return;
    }
    infile.close();

    // Iterate through each seller in the JSON
    for (auto &[seller_name, seller_data] : j.items()) {
        vector<product::Product> products;

        // Check if "products" field exists
        if (seller_data.contains("products")) {
            for (auto &[category, product_list] : seller_data["products"].items()) {
                for (const auto &product_json : product_list) {
                    // Create Product object and assign category
                    product::Product product(
                        product_json.value("name", ""),
                        product_json.value("price", ""),
                        product_json.value("stock", ""),
                        category  // Use category from the JSON key
                    );
                    products.push_back(product);
                }
            }
        }
        // Add seller's products to the map
        seller_products[seller_name] = products;
    }
}

inline void register_user(unordered_map<string,Seller> &seller_map){

    Address address;
    Seller seller;

    string temp;
    string name;
    cin.ignore();

    cout << "Enter your name : " ;
    getline(cin,name);
    seller.set_name(name);

    cout << "Enter your email : ";
    getline(cin,temp);
    seller.set_email(temp);

    cout << "Enter your phone number : ";
    getline(cin,temp);
    seller.set_phone(temp);

    cout << "Address : " << endl;
    cout << "Enter shop name : ";
    getline(cin,temp);
    address.set_shop_name(temp);

    cout << "Enter street address : ";
    getline(cin,temp);
    address.set_street(temp);

    cout << "Enter city name : ";
    getline(cin,temp);
    address.set_city(temp);

    cout << "Enter state name : ";
    getline(cin,temp);
    address.set_state(temp);

    cout << "Enter zipcode : ";
    getline(cin,temp);
    address.set_zip(temp);

    // add address object to the seller class
    seller.set_address(address);

    // add object to the map
    seller_map[name] = seller;

}

inline void delete_user(unordered_map<string, Seller>& seller_map, const string& name) {
    seller_map.erase(name);
}

inline void add_product(const string& username, unordered_map<string, json>& product_data) {
    product::Product product;
    string temp, category;


    cout << "Enter product name: ";
    cin.ignore();
    getline(cin,temp);
    product.set_name(temp);

    cout << "Enter product price: ";
    getline(cin,temp);
    product.set_price(temp);

    cout << "Enter product stock: ";
    getline(cin,temp);
    product.set_stock(temp);

    cout << "Enter product category: ";
    getline(cin,temp);
    product.set_category(temp);
    
    // Check if the category exists for the given user in product_data
    if (product_data[username].contains(category)) {
        // Append product to the existing category
        product_data[username][category].push_back(product.to_json());
    } else {
        // Create a new category and add the product
        product_data[username][category] = json::array({product.to_json()});
    }
}

inline void load_json_data_to_map(unordered_map<string, Seller>& seller_map, unordered_map<string, json>& product_data) {
    json j;
    ifstream infile("/Users/levi/Desktop/E-Commerce-Backend-CPP/JSON/seller.json");
    if (infile.is_open()) {
        try {
            infile >> j;
        } catch (...) {
            cerr << "Error reading existing JSON data" << endl;
        }
        infile.close();
    } else {
        cerr << "Error opening seller.json file for loading data to map." << endl;
        return;
    }

    for (auto& [name, seller_json] : j.items()) {
        try {
            json addr_json = seller_json["address"];
            Address address(
                addr_json["street"].get<string>(),
                addr_json["city"].get<string>(),
                addr_json["state"].get<string>(),
                addr_json["zip"].get<string>()
            );
            address.set_shop_name(addr_json["shop_name"].get<string>());

            Seller seller(
                seller_json["name"].get<string>(),
                seller_json["email"].get<string>(),
                seller_json["phone"].get<string>(),
                address
            );

            seller_map[name] = seller;

            if (seller_json.contains("products")) {
                product_data[name] = seller_json["products"];
            }
        } catch (...) {
            cerr << "Error parsing seller data in loading function for " << name << endl;
        }
    }
}

inline void save_data(const unordered_map<string, Seller>& seller_map, const unordered_map<string, json>& product_data) {
    json j;
    for (const auto& [name, seller] : seller_map) {
        json address_json = {
            {"shop_name", seller.get_address().get_shop_name()},
            {"street", seller.get_address().get_street()},
            {"city", seller.get_address().get_city()},
            {"state", seller.get_address().get_state()},
            {"zip", seller.get_address().get_zip()}
        };

        json seller_json = {
            {"name", seller.get_name()},
            {"email", seller.get_email()},
            {"phone", seller.get_phone()},
            {"address", address_json}
        };

        if (product_data.find(name) != product_data.end()) {
            seller_json["products"] = product_data.at(name);
        }

        j[name] = seller_json;
    }

    ofstream file("/Users/levi/Desktop/E-Commerce-Backend-CPP/JSON/seller.json");
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    } else {
        cout << "Error saving data in seller.json file" << endl;
    }
}

} // namespace seller

#endif // SELLER_H

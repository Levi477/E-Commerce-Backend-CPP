#ifndef BUYER_H
#define BUYER_H

#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"
#include <fstream>
#include <unordered_map>
#include "product.hpp"
using namespace std;
typedef nlohmann::json json;

namespace buyer {


class Address {

private:
    string home_name;
    string street;
    string city;
    string state;
    string zip;

public:
    Address() {}

    Address(string street, string city, string state, string zip) {
        this->street = street;
        this->city = city;
        this->state = state;
        this->zip = zip;
    }

    // setters
    void set_street(string street) { this->street = street; }
    void set_home_name(string home) { this->home_name = home; }
    void set_city(string city) { this->city = city; }
    void set_state(string state) { this->state = state; }
    void set_zip(string zip) { this->zip = zip; }

    // getters
    string get_home_name() const { return home_name; }
    string get_street() const { return street; }
    string get_city() const { return city; }
    string get_state() const { return state; }
    string get_zip() const { return zip; }
    string get_address() const {
        return home_name + ", " + street + ", " + city + ", " + state + ", " + zip + ".";
    }
    json to_json(){
      return {
        {"home" , home_name},
        {"street" , street},
        {"city" , city},
        {"state" , state},
        {"zip" , zip},
    };
  }
};

class Buyer {

private:
    string name;
    string email;
    string phone;
    vector<product::Product> cart;
    vector<product::Product> favourites;
public:
    Address address;
    Buyer() {}

    Buyer(string name, string email, string phone, Address addr) {
        this->name = name;
        this->email = email;
        this->phone = phone;
        this->address = addr;
    }

    void print_buyer_info() {
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Address: " << address.get_address() << endl;
    }

    // setters
    void set_name(string name) { this->name = name; }
    void set_email(string email) { this->email = email; }
    void set_phone(string phone) { this->phone = phone; }
    void set_addr(Address addr) { this->address = addr; }

    // getters
    string get_name() const { return name; }
    string get_email() const { return email; }
    string get_phone() const { return phone; }
    Address get_address() const { return address; }
    vector<product::Product>& get_cart()  { return cart; }
    vector<product::Product>& get_favourite()  { return favourites; }

    void add_to_cart(const product::Product& product) {
        cart.push_back(product);
    }

    void add_to_favourites(const product::Product& product) {
        favourites.push_back(product);
    }
};

inline void register_buyer(unordered_map<string, Buyer> &buyer_map) {
    Address address;
    Buyer buyer;

    string temp;
    string name;
    cout << "Enter your name: ";
    cin >> name;
    buyer.set_name(name);

    cout << "Enter your email: ";
    cin >> temp;
    buyer.set_email(temp);

    cout << "Enter your phone number: ";
    cin >> temp;
    buyer.set_phone(temp);

    cout << "Address: " << endl; cout << "Enter home name: ";
    cin >> temp;
    address.set_home_name(temp);

    cout << "Enter street address: ";
    cin >> temp;
    address.set_street(temp);

    cout << "Enter city name: ";
    cin >> temp;
    address.set_city(temp);

    cout << "Enter state name: ";
    cin >> temp;
    address.set_state(temp);

    cout << "Enter zipcode: ";
    cin >> temp;
    address.set_zip(temp);

    // add address object to the buyer class
    buyer.set_addr(address);

    // add object to the map
    buyer_map[name] = buyer;
}

inline void delete_buyer(unordered_map<string, Buyer> &buyer_map, const string &name) {
    buyer_map.erase(name);
}

// Load function for buyer data including cart and favourites
inline void load_json_data_to_map(unordered_map<string, Buyer> &buyer_map) {
    json j;
    ifstream infile("/Users/levi/Desktop/E-Commerce-Backend-CPP/JSON/buyer.json");
    if (infile.is_open()) {
        try {
            infile >> j;
        } catch (...) {
            cerr << "Error reading existing JSON data" << endl;
        }
        infile.close();
    } else {
        cerr << "Error opening buyer.json file for loading data to map." << endl;
        return;
    }

    for (auto &[name, buyer_json] : j.items()) {
        try {
            Buyer buyer;
            buyer.set_name(name);
            buyer.set_email(buyer_json["email"].get<string>());
            buyer.set_phone(buyer_json["phone"].get<string>());

            // Parse cart
            for (const auto& product_json : buyer_json["cart"]) {
                product::Product product(
                    product_json["name"].get<string>(),
                    product_json["price"].get<string>(),
                    product_json["stock"].get<string>(),
                    product_json["category"].get<string>()
                );
                buyer.add_to_cart(product);
            }

            // Parse favourites
            for (const auto& product_json : buyer_json["favourite"]) {
                product::Product product(
                    product_json["name"].get<string>(),
                    product_json["price"].get<string>(),
                    product_json["stock"].get<string>(),
                    product_json["category"].get<string>()
                );
                buyer.add_to_favourites(product);
            }

            buyer_map[name] = buyer;
        } catch (...) {
            cerr << "Error parsing buyer data for " << name << endl;
        }
    }
}

// Save function for buyer data including cart and favourites
inline void save_data(unordered_map<string, Buyer> &buyer_map) {
    json j;

    for ( auto &[name, buyer] : buyer_map) {
        json buyer_json;
        buyer_json["email"] = buyer.get_email();
        buyer_json["phone"] = buyer.get_phone();
        buyer_json["address"] = buyer.get_address().to_json();
        // Save cart products
        for ( auto& product : buyer.get_cart()) {
            buyer_json["cart"].push_back(product.to_json());
        }

        // Save favourite products
        for ( auto& product : buyer.get_favourite()) {
            buyer_json["favourite"].push_back(product.to_json());
        }

        j[name] = buyer_json;
    }

    ofstream file("/Users/levi/Desktop/E-Commerce-Backend-CPP/JSON/buyer.json");
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    } else {
        cout << "Error saving data to buyer.json file" << endl;
    }
}
}

#endif

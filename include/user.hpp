#pragma once
#include <iostream>
#include <string>
#include <limits>

struct User {
    std::string name;
    int age;
    int id;

    User() : name(""), age(0), id(0) {}
    User(std::string name, int age) : name(std::move(name)), age(age), id(0) {}

    bool operator==(const User& other) const {
        return name == other.name && age == other.age && id == other.id;
    }

    bool operator!=(const User& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        return os << "User(name=\"" << user.name << "\", age: " << user.age << ", id: " << user.id << ")";
    }

    friend std::istream& operator>>(std::istream& is, User& user) {
        std::cout << "Enter name: ";
        std::getline(is >> std::ws, user.name);
        std::cout << "Enter age (0-150): ";
        while (!(is >> user.age) || user.age < 0 || user.age > 150) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter age between 0 and 150: ";
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter ID (positive number): ";
        while (!(is >> user.id) || user.id < 0) {
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter positive ID: ";
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return is;
    }
};

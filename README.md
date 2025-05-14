# 📊 C++ CRM System

A console-based **Customer Relationship Management (CRM)** system built in **C++**, designed to help businesses manage customer interactions, store data, and simulate real-world software development processes. Developed as part of a team-based project with an emphasis on Object-Oriented Programming (OOP), modularity, and code quality.

---

## 🚀 Features

- 🎯 Object-Oriented Design (Classes, Inheritance, Polymorphism)
- ➕ Operator Overloading (`<<`, `>>`)
- 📦 Templates and Standard Template Library (STL)
- ⚠️ Exception Handling for invalid input
- 💾 File Handling for persistent data
- 👤 Role-Based Access:
  - Admin
  - Agent/Employee
- 🧰 Core Functionalities:
  - Add, remove, search, and modify client information
  - Display client records
  - Record and manage client interactions

---

## 🧪 Extra Feature: Advanced Search & Sorting

Utilizes advanced STL algorithms to enhance data handling, offering optimized searching and sorting for client information.

---

## 🛠️ How to Compile & Run

### Requirements

- C++17-compatible compiler (`g++`, `clang++`, etc.)
- Command line or terminal access

### Compile

```bash
g++ -std=c++17 -o crm_system main.cpp Client.cpp CRMSystem.cpp

#include <string>
#include <typeinfo>

// #define NDEBUG
#include <assert.h>

#include "../namespaces.h"

using namespace std;
using namespace clients;

#ifndef DATA_H
#define DATA_H

// --- Functions
int isCharOnArray(int character, int array[], int n);
void addClients(Clients *clients);
void removeClient(Clients *clients);
void viewClients(Clients *clients, bool fields[], int sortBy[]);
void filterClients(Clients *clients, string **params, int sortBy[]);
void fields(); // As a Parameter or as a Subcommand
void sortByParameters();
void howToUseViewClients();
void howToUseFilterClients();
void getBalance(Clients *clients);
void depositMoney(Clients *clients);
void cashoutMoney(Clients *clients);
void sendMoney(Clients *clients);
void changeStatus(Clients *clients);

// --- Templates

// Template to Return a String that's Surrounded by Brackets
template <typename T>
string addBrackets(T message)
{
  string addedBrackets;

  if (typeid(T) == typeid(const int)) // Checks if it's a Char
    addedBrackets += message;         // Works fine if the  Message is a Char
  else
    addedBrackets = message; // Message is of type String

  assert(addedBrackets.length() > 0); // Check if the Character Could be Appended to the String
  addedBrackets.insert(0, 1, '[');

  return addedBrackets.insert(addedBrackets.length(), 1, ']');
}

#endif
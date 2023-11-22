#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include "../namespaces.h"
#include "../clients/clientsOp.h"
#include "../clients/transactionsOp.h"
#include "../data/dataOp.h"
#include "../datatables/output.h"
#include "../terminal/ansiEsc.h"
#include "../terminal/input.h"

using namespace std;
using namespace files;
using namespace commands;
using namespace terminal;

// --- Extern Variables Declaration
extern bool validFieldsToFilter[];
extern int fieldCmdsChar[];
extern char *fieldCmdsStr[], *accountStr[];

// --- Function Prototypes
int isCharOnArray(int character, int array[], int n);
void addClients(Clients *clients);
void viewClients(Clients *clients, bool fields[], int sortBy[]);
void filterClients(Clients *clients, string **params, int sortBy[]);
void validParameters(int nCharTitle);
void fields();
void sortByParameters();
void printExamples(string cmds[], string explanations[], int n);
void howToUseViewClients();
void howToUseFilterClients();
void depositMoney(Clients *clients);
void cashoutMoney(Clients *clients);
void sendMoney(Clients *clients);
void changeStatus(Clients *clients);
bool clientActionConfirm(clientActions action);
int getSortByStr(int sortBy[], string sortByStr[], int n);

// --- Functions

// Function to check if the Integer that Represents a Character is on Char Array
int isCharOnArray(int character, int array[], int n)
{ // Linear Search
  for (int i = 0; i < n; i++)
    if (array[i] == character)
      return i;
  return -1;
}

// Function to Add Clients to clients.csv
void addClients(Clients *clients)
{
  while (true)
  {
    cout << clear;
    printTitle("Add Client", applyBgColor, applyFgColor, false);

    cout << '\n';
    addClientToFile(clients);

    if (!booleanQuestion("Do you want to Add more Clients?"))
      break;
  }
}

// Function to View Clients Stored in clients.csv
void viewClients(Clients *clients, bool fields[], int sortBy[])
{
  int m = fieldEnd - 1, n = sortByEnd / 2;
  string fieldsStr[m], sortByStr[n], applied;

  if (fields[fieldAll])
    for (int i = 0; i < m; i++)
      fields[i] = true;

  for (int i = 0; i < m; i++)
  {
    applied = fields[i] ? "[Y] " : "[N] ";
    fieldsStr[i] = applied.append(fieldCmdsStr[i]); // Data to Print in the Field Parameters Row
  }

  n = getSortByStr(sortBy, sortByStr, n); // Get Sort By Array Length

  cout << clear;
  printTitle("Clients Fields", applyBgColor, applyFgColor, false);
  printArray(fieldsStr, m, "Fields");

  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  sortClients(clients, sortBy, sortByEnd); // Sort Clients
  printClients(clients, fields);           // Print Clients

  pressEnterToCont("Press ENTER to Continue", false);

  clientsMergeSort(clients, sortByIdA); // Return Clients Array to Initial Order
}

// Function to Filter Clients
void filterClients(Clients *clients, string **params, int sortBy[])
{
  int l = fieldEnd - 1, m = maxParamPerSubCmd, n = sortByEnd / 2, nClientsFiltered;
  bool fields[l];
  string sortByStr[n];

  fill(fields, fields + l, true); // Client Fields to Print (All)
  n = getSortByStr(sortBy, sortByStr, n);

  cout << clear;
  printTitle("Client Fields Parameters", applyBgColor, applyFgColor, false);
  print2DArray(params, l, m, fieldCmdsStr);
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  printArray(sortByStr, n, "Sort By");

  pressEnterToCont("Press ENTER to Continue", false);

  filterClients(clients, params, fields, sortBy); // Filter Clients
  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print the Two Types of Parameters (Used in Filter Clients Command)
void validParameters(int nCharTitle)
{
  string temp = "\"sentence to search\"";

  cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets("word") << "Word to Search\n"
       << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(temp) << "Sentence to Search\n\n";
}

// Function to Print Field as a Parameter and as a Command
void fields()
{
  string temp;

  cout << clear; // Clear Terminal
  printTitle("Field as a Parameter (for View Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
    cout << tab1 << setw(nCharTitle) << setfill(' ') << addBrackets(fieldCmdsChar[i]) << fieldCmdsStr[i] << '\n';
  cout << '\n';

  printTitle("Field as a Command (for Filter Clients)", applyBgColor, applyFgColor, false);
  for (int i = 0; i < fieldEnd - 1; i++)
    if (validFieldsToFilter[i])
    {
      temp = addBrackets(fieldCmdsChar[i]).append(" [param...]");
      cout << tab1 << setw(nCharTitle) << setfill(' ') << temp << "Parameters for Client's " << fieldCmdsStr[i] << '\n';
    }
  cout << '\n';

  printTitle("Valid Field Parameters (for Filter Clients)", applyBgColor, applyFgColor, false);
  validParameters(nCharTitle);

  pressEnterToCont("Press ENTER to Continue", false);
}

// Function to Print Possible Parameters for Sort By Subcommand
void sortByParameters()
{
  string ascending, descending;

  cout << clear; // Clear Terminal
  printTitle("Sort By Parameters", applyBgColor, applyFgColor, false);
  for (int i = 0; i < sortByEnd / 2; i++)
  {
    ascending = addBrackets(fieldCmdsChar[i]).append(" Ascending");
    descending = addBrackets(toupper(fieldCmdsChar[i])).append(" Descending");

    cout << tab1 << setw(nCharTitle) << setfill(' ') << fieldCmdsStr[i]
         << setw(nCharTitle) << setfill(' ') << ascending
         << setw(nCharTitle) << setfill(' ') << descending << '\n';
  }
  cout << '\n';

  pressEnterToCont("Press ENTER to Continue", false);
}

// Print Usage Examples of the View Clients Command
void howToUseViewClients()
{
  const int nCmds = 3; // Number of Code Examples
  string cmds[nCmds] = {"v -f . -s i",
                        "v -f i n -s I", "v -f i t -s S"};

  string explanations[nCmds] = {"View All Client Fields. Sort them by Id in Ascending Order",
                                "View Id and Client Name Field. Sort them by Id in Descending Order", "View Id and Account Type Fields. Sort them by Suspended Status in Descending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Print Usage Examples of the Filter Clients Command
void howToUseFilterClients()
{
  const int nCmds = 4; // Number of Code Examples
  string cmds[nCmds] = {"f -f --n Ramon Ronald -s i",
                        "f -f --n \"Ramon Alvarez\" \"Ronald Lopez\" -s I",
                        "f -f --n Ronald --i 123456789 -s a",
                        "f -f --n Ramon --a 100200300 -s N"};

  string explanations[nCmds] = {
      "Search for Clients Named as Ramon or Ronald. Sort them by Id in Ascending Order",
      "Search for Clients Named as Ramon Alvarez or Ronald Lopez. Sort them by Id in Descending Order",
      "Search for Clients Named as Ronald and the Client with Id 123456789. Sort them by Account Number",
      "Search for Clients Named as Ramon and the Client with Account Number 100200300. Sort them by Name in Descending Order"};

  printExamples(cmds, explanations, nCmds);

  cout << '\n';
  pressEnterToCont("Press ENTER to Continue", false);
}

// Function for Clients to Deposit Money in their Accounts
void depositMoney(Clients *clients)
{
  bool suspended;
  int id, index;
  float amount;
  string message;
  clientStatus clientStatus;

  cout << clear; // Clear Terminal
  printTitle("Deposit Money", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    id = getClientId("Client ID");
    clientStatus = checkClient(clients, id, fieldId, &index); // Check if the Clients Exists

    if (clientStatus != clientFound)
      checkClientStatus(clientStatus);

    if (clientStatus == clientNotFound)
      break;
    else if (clientStatus == clientSuspended)
    { // The Client Cannot Deposit Any Money while his Account is Suspended
      suspended = true;
      break;
    }

    cout << '\n';
    printClientInfo((*clients).getClient(index), true); // Print Client Info
    cout << '\n';
    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  if (clientStatus != clientNotFound && !suspended)
  { // Check if the Client isn't Suspended
    amount = getFloat("Enter the Amount to Deposit", minDeposit, maxDeposit);
    if (clientActionConfirm(clientDeposit)) // Asks the Client for Confirmation
    {
      message = "You Have Successfully Deposited $";
      message.append(toStringWithPrecision(amount, precision));

      storeBalance(clientDeposit, id, (*clients).getClient(index).account, amount);
      pressEnterToCont(message, false);
    }
  }
}

// Function for Clients to Cashout Money from their Accounts
void cashoutMoney(Clients *clients)
{
  bool suspended;
  int id, index;
  float amount;
  string message;
  clientStatus clientStatus;

  cout << clear; // Clear Terminal
  printTitle("Cash Out Money", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    id = getClientId("Client ID");
    clientStatus = checkClient(clients, id, fieldId, &index); // Check if the Clients Exists

    if (clientStatus != clientFound)
      checkClientStatus(clientStatus);

    if (clientStatus == clientNotFound)
      break;
    else if (clientStatus == clientSuspended)
    { // The Client Cannot Deposit Any Money while his Account is Suspended
      suspended = true;
      break;
    }

    cout << '\n';
    printClientInfo((*clients).getClient(index), true); // Print Client Info
    cout << '\n';
    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  if (clientStatus != clientNotFound && !suspended)
  { // Check if the Client isn't Suspended
    amount = getFloat("Enter the Amount to Cash Out", minDeposit, maxDeposit);
    if (clientActionConfirm(clientCashout)) // Asks the Client for Confirmation
    {
      message = "You Have Successfully Cashed Out $";
      message.append(toStringWithPrecision(amount, precision));

      storeBalance(clientCashout, id, (*clients).getClient(index).account, amount);
      pressEnterToCont(message, false);
    }
  }
}

// Function for Clients to Transfer Money from their Balance to other Client Accounts
void sendMoney(Clients *clients)
{
  bool suspended;
  int idFrom, idTo, indexFrom, indexTo;
  float amount;
  string message;
  clientStatus clientStatus;

  cout << clear; // Clear Terminal
  printTitle("Send Money", applyBgColor, applyFgColor, false);
  cout << '\n';

  while (true)
  {
    idFrom = getClientId("Client ID");
    clientStatus = checkClient(clients, idFrom, fieldId, &indexFrom); // Check if the Clients Exists

    if (clientStatus != clientFound)
      checkClientStatus(clientStatus);

    if (clientStatus == clientNotFound)
      break;
    else if (clientStatus == clientSuspended)
    { // The Client Cannot Deposit Any Money while his Account is Suspended
      suspended = true;
      break;
    }

    cout << '\n';
    printClientInfo((*clients).getClient(indexFrom), true); // Print Client Info
    cout << '\n';
    if (booleanQuestion("Is this your Client Account?"))
      break;
  }

  if (clientStatus != clientNotFound && !suspended)
  { // Check if the Client isn't Suspended
    while (true)
    {
      idTo = getClientId("Send to Client Id");
      clientStatus = checkClient(clients, idTo, fieldId, &indexTo); // Check if the Clients Exists

      if (idFrom == idTo)
      { // Client Cannot Send to Himself
        pressEnterToCont("Error: You Cannot Send to Yourself", false);
        continue;
      }

      cout << '\n';
      printClientInfo((*clients).getClient(indexTo), true); // Print Client Info
      cout << '\n';
      if (booleanQuestion("Is this the Account you Want to Send the Money to?"))
        break;
    }

    amount = getFloat("Enter the Amount to Send", minDeposit, maxDeposit);
    if (clientActionConfirm(clientSend)) // Asks the Client for Confirmation
    {
      message = "You Have Successfully Sent $";
      message.append(toStringWithPrecision(amount, precision));

      storeTransactions(idFrom, (*clients).getClient(indexFrom).account, amount, idTo);
      storeBalance(clientSend, idFrom, (*clients).getClient(indexFrom).account, amount);
      pressEnterToCont(message, false);
    }
  }
}

// Function to Change the Status of a Client
void changeStatus(Clients *clients)
{
  bool change;
  string temp;
  clientStatus clientStatus;
  int id, index;
  string message;

  cout << clear;
  printTitle("Change Account Status", applyBgColor, applyFgColor, false); // Examples of the Usage of the Search Command
  cout << '\n';

  id = getClientId("Client ID to Change Status");
  clientStatus = checkClient(clients, id, fieldId, &index); // Check if the Clients Exists

  if (clientStatus == clientNotFound)
    checkClientStatus(clientStatus);
  else if (clientStatus != errorStatus)
  {
    if (clientStatus == clientSuspended) // Ask wether to Suspend or Active Account
      change = booleanQuestion("Do you want to Activate the Client?");
    else
      change = booleanQuestion("Do you want to Suspend the Client?");
    cout << '\n';

    if (!change)
      message = "Client Found: Nothing to Change";
    else
    {
      message = "Client Found: Changed Status";
      (*clients).changeClientStatus(index);

      ofstream outfile(clientsFilename);

      Client client;
      string suspended;

      for (int i = 0; i < (*clients).getNumberClients(); i++) // Write to File
      {
        client = (*clients).getClient(i);

        suspended = (client.suspended) ? "true" : "false"; // Get Account Status

        outfile << client.id << sep << client.name << sep
                << setw(maxAccountDigits) << setfill('0') << right << fixed << setprecision(0) << client.account << left
                << sep << accountStr[client.type] << sep << suspended << '\n';
      }

      outfile.close(); // Close file
    }
    printClientInfo((*clients).getClient(index), true);
    pressEnterToCont(message, false);
  }
}

// Function to Print Message Confirmation when a Client Tries to make any Financial Transaction
bool clientActionConfirm(clientActions action)
{
  string message;

  switch (action)
  {
  case clientDeposit:
    message = "Do you Want to Deposit this Amount?";
    break;
  case clientCashout:
    message = "Do you Want to Cashout this Amount?";
    break;
  case clientSend:
    message = "Do you Want to Send this Amount?";
    break;
  }
  return booleanQuestion(message);
}

// Function to Get a String Array from a Int Array of the Sort By Commands that will be Applied to the Clients
int getSortByStr(int sortBy[], string sortByStr[], int n)
{
  bool nullParam;
  int charIndex, nParams = 0;
  string order;

  for (int j = 0; j < n; j++)
  {
    charIndex = sortBy[j];
    nullParam = false;

    if (charIndex == -1)
      nullParam = true;
    else if (charIndex % 2 == 0)
      order = "[A] ";
    else
      order = "[D] ";

    if (!nullParam)
    {
      sortByStr[nParams] = order.append(fieldCmdsStr[charIndex / 2]); // Data to Print in the Sort By Parameters Row
      nParams++;
    }
  }
  return nParams;
}
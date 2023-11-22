#include "../namespaces.h"

using namespace clients;

#ifndef OUTPUT_H
#define OUTPUT_H

// --- Functions
void printClientInfo(Client client, bool censoreInfo);
void printArray(string *params, int m, string paramTitle);
void print2DArray(string **params, int m, int n, char **paramsTitle);
void printClients(Clients *clients, bool *fields);

#endif
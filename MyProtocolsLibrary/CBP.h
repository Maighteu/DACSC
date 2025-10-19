#ifndef CBP_H
#define CBP_H
#include "sqlQuerry.h"
#include <list>
#include <string>
#include <pthread.h> 


using namespace std;

// structure utile
// typedef struct{
// int id;
// char specialite[20];
// char doctor[40];
// char date[12];
// char hour [6];
// }consultation;

// typedef struct{
// int id;
// char nom[40];
// }docteur;

// typedef struct{
// 	int id;
// 	char nom[40];
// }specialite;

//'Machine à état'
bool CBP(char* requete, char* reponse, const int socket);

// Gére LOGIN nvx user ou non et verifie accés unique ([thread] [-] [socket]
// -> [thread] [userId] [socket], si userId pas déja présent)
int CBP_LOGIN(char* buffer, char *saveptr);

//utilise l'ip pour retirer de la liste de connexion
bool CBP_LOGOUT(const int id);

void CBP_GET_SPECIALTIES(char* reponse);
void CBP_GET_DOCTORS(char* reponse);
void CBP_SEARCH_CONSULTATIONS(char* requete, char* reponse);

bool CBP_BOOK_CONSULTATIONS(char* requete);

void CBP_CLOSE(const int socket);

int  estPresent(const int socket); 
void ajout(const int socket); 

#endif
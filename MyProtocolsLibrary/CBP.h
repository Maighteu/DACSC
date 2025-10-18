#ifndef CBP_H
#define CBP_H
#include "sqlQuerry.h"
using namespace std;

// structure utile
typedef struct{
int id,
char specialite[20],
char doctor[40],
char date[12],
char hour [6]
}consultation;

typedef struct{
int id,
int nom[40]
}docteur;

typedef struct{
	int id,
	char nom
}specialite;

//'Machine à état'
bool CBP(char* requete, char* reponse, const int socket);

// Gére LOGIN nvx user ou non et verifie accés unique ([thread] [-] [socket]
// -> [thread] [userId] [socket], si userId pas déja présent)
bool CBP_LOGIN(char* buffer);

//utilise l'ip pour retirer de la liste de connexion
bool CBP_LOGOUT(const int* id);

std::list<specialite> CBP_GET_SPECIALTIES();
std::list<docteur> CBP_GET_DOCTORS();
std::list<consultation> CBP_SEARCH_CONSULTATIONS();

bool CBP_BOOK_CONSULTATIONS(const int idConsultation, const int idPatient, const char* raison);

void CBP_CLOSE();

int  estPresent(int socket); 
void ajoute(int socket); 
void retire(int socket); 

#endif
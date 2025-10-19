#ifndef SQL_QUERRY_H
#define SQL_QUERRY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <mysql.h>


MYSQL * ConnexionBD();

int AjouterPatient(const char *nom, const char *prenom);
int AuthentifierPatient(const char *nom, const char *prenom,const int idPatient);

//int AjouterConsulation(const int idSpecialite, const int idMedecin, const string date, const string heure);
int ReserverConsulation(const char* idConsultation, const char* idPatient, const char *raison);

void rechercheConsultation(char *reponse,const char *Specialite, const char *Medecin, const char *dateDebut, const char *dateFin );

void obtenirMedecin(char *reponse);
void obtenirSpecialite(char *reponse);
#endif

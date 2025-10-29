#include "CBP.h"


int loggedClient[20];
int nbClient = 0;
int id;
pthread_mutex_t mutexClients = PTHREAD_MUTEX_INITIALIZER; //macro qui evite le init

bool CBP(char* requete, char* reponse, const int socket)
{

	char *saveptr;
	char brequete[250];
	strcpy(brequete,requete);
	char *buffer = strtok_r(brequete,"#",&saveptr);



	// strtok va "effacer ce qui etait present precedemmnt"
	//LOGIN
	//////////////////////////////////////////////////////
	if (id = (strcmp(buffer,"LOGIN") == 0))
	{
		if (CBP_LOGIN(requete) >=0)
		{
			ajout(socket);
			sprintf(reponse, "LOGIN#ok#%d#", id);
			return true;
		}

		else
		{
			sprintf(reponse, "LOGIN#ko#");
			return false;
		}
	}

	//LOGOUT
	/////////////////////////////
	else if (strcmp(buffer,"LOGOUT") == 0)
	{

		if (CBP_LOGOUT(socket))
		{
			sprintf(reponse, "LOGOUT#ok#");
			return true;
		}

		else
		{
			sprintf(reponse, "LOGOUT#ko#");
			return false;
		}
	}
	//Specialite
	/////////////////////////////
	else if (strcmp(buffer,"GET_SPECIALTIES") == 0)
	{
		CBP_GET_SPECIALTIES(reponse);
		return true;
	}

//doctor
	//////////////////////
	else if (strcmp(buffer,"GET_DOCTORS") == 0)
	{
		CBP_GET_DOCTORS(reponse);
		return true;
	}
//consultations
	//////////////////////////////////////
	else if (strcmp(buffer,"SEARCH_CONSULTATIONS") == 0)
	{
		CBP_SEARCH_CONSULTATIONS(requete, reponse);
		return true;
	}
//rdv
	////////////////////////
	else if (strcmp(requete,"BOOK_CONSULTATIONS") == 0)
	{
		if (CBP_BOOK_CONSULTATIONS(requete) == true)
		{
			strcpy(reponse,"BOOK_CONSULTATIONS#ok#");
		}
		else
		{
			strcpy(reponse,"BOOK_CONSULTATIONS#ok#");

		}
		return true;
	}
	else return false;
}
	
 

int CBP_LOGIN(char* requete)
{
		char* saveptr;
		char prenom[20], nom[20];
		int id;
		//on recup sans le LOGIN donc juste new#...
		char* buffer = strtok_r(requete,"#",&saveptr);
			printf("dans buffer post tok: %s \n", buffer);
			buffer = strtok_r(NULL,"#",&saveptr);
						printf("dans buffer post second tok: %s \n", buffer);


		if (strcmp(buffer,"NEW") == 0)
		{
			printf("new started \n");
			strcpy(nom, strtok_r(NULL,"#",&saveptr));
						printf("nom copy \n");

			strcpy(prenom, strtok_r(NULL,"#",&saveptr));
									printf("prenom copy \n");


			if((id=AjouterPatient(nom,prenom)) >= 0)
			{	
				return id;
			}
			else
			{
				return -1;
			}
		}
		else if (strcmp(buffer,"OLD") == 0)
		{

			printf("old started \n");

			strcpy(nom, strtok_r(NULL,"#",&saveptr));
			strcpy(prenom, strtok_r(NULL,"#",&saveptr));
			id = atoi(strtok_r(NULL,"#",&saveptr));

			if (AuthentifierPatient(nom, prenom, id) >=0)
			{
				return id;
			}
			else
			{
				return -2;
			}
		}
		printf("nothing started \n");

		return -3;
}


bool CBP_LOGOUT(const int socket)
{
	int pos = estPresent(socket);
	if (pos >=0)
	{
	pthread_mutex_lock(&mutexClients);
	for (int i=pos; i<nbClient-1; i++)
	{
		loggedClient[i] = loggedClient[i+1];
	}
	nbClient--;

	pthread_mutex_unlock(&mutexClients);
	return true;
	}
	pthread_mutex_unlock(&mutexClients);
	return false;

}

//Crée une liste temporaire
void CBP_GET_SPECIALTIES(char* reponse)
{
	char* saveptr;
	char buffer[500];
	obtenirSpecialite(buffer);
	strcpy(reponse,buffer);
}

void CBP_GET_DOCTORS(char* reponse)
{
	char* saveptr;
	char buffer[500];
	obtenirMedecin(buffer);
	//retour de la bd
	strcpy(reponse,buffer);

}

void CBP_SEARCH_CONSULTATIONS(char*requete,char* reponse)
{
	char specialite[20], medecin[30], dateDebut[12], dateFin[12];
	char buffer[500];
	char *saveptr;

// on recoit une requete du genre SEARCH_CONSULTATIONS#specialite#medecin#dateDebut#dateFin
	//ON la dans le champ reponse, il va donc servir "en ecriture et en lecture"

	char* ptr = strtok_r(requete,"#", &saveptr);
		strcpy(specialite, ptr);
		ptr = strtok_r(NULL,"#",&saveptr);
		strcpy(medecin, ptr);
		ptr = strtok_r(NULL,"#",&saveptr);
		strcpy(dateDebut, ptr);
		ptr = strtok_r(NULL,"#",&saveptr);
		strcpy(dateFin, ptr);
		rechercheConsultation(buffer, specialite, medecin, dateDebut, dateFin );
		strcpy(reponse,buffer);
	
}

bool CBP_BOOK_CONSULTATIONS(char* buffer)
{
	char idConsultation[5];
	char idPatient[5];
	char raison[250];
	char *saveptr;

	char* ptr = strtok_r(buffer,"#", &saveptr);
		strcpy(idConsultation, ptr);
		ptr = strtok_r(NULL,"#",&saveptr);
		strcpy(idPatient, ptr);
		ptr = strtok_r(NULL,"#",&saveptr);
		strcpy(raison, ptr);
		if (ReserverConsulation(idConsultation, idPatient, raison)==1)
		{
			return true;
		}
		else return false;
}

void CBP_CLOSE(const int socket)
{
	printf("close");
}

int  estPresent(const int socket)
{
	pthread_mutex_lock(&mutexClients);
	for (int i=0; i<nbClient; i++)
	{
		if (loggedClient[i] == socket)
		{
			pthread_mutex_unlock(&mutexClients);

			return i;
		}
	}
	pthread_mutex_unlock(&mutexClients);

	return -1;
}

void ajout(const int socket)
{
	pthread_mutex_lock(&mutexClients);
	loggedClient[nbClient] = socket;
	nbClient ++;
	pthread_mutex_unlock(&mutexClients);
}
#include "CBP.h"

bool CBP(char* requete, char* reponse, const int socket)
{
	char *buffer = strtok(requete,"#");
	//LOGIN
	//////////////////////////////////////////////////////
	if (strcmp(buffer,LOGIN) == 0)
	{
		if (CBP_LOGIN( buffer) >=0)
		{
			sprintf(reponse, "LOGIN#ok#");
			return true;
		}

		else
		{
			sprintf(reponse, "LOGIN#ko#");
			return false;
		}
	}
}
	
 

int CBP_LOGIN(char* buffer)
{


		char prenom[20], nom[20];
		int id;
		buffer = strtok(NULL,"#");

		if (strcmp(buffer,"NEW") == 0)
		{
			strcpy(nom, strtok(NULL,"#"));
			strcpy(prenom, strtok(NULL,"#"));
			if((id=AjouterPatient(nom,prenom)) > 0)
			{	
				return id;
			}
			else
			{
				return -2;
			}
		}
		else if (strcmp(buffer,"OLD") == 0)
		{
			strcpy(nom, strtok(NULL,"#"));
			strcpy(prenom, strtok(NULL,"#"));
			id = stoi(strtok(NULL,"#"));

			if (AuthentifierPatient(nom, prenom, id) >=0)
			{
				return id;
			}
			else
			{
				return -1;
			}
		}
}

bool CBP_LOGOUT(const int* id)
{

}

std::list<specialite> CBP_GET_SPECIALTIES()
{

}

std::list<docteur> CBP_GET_DOCTORS()
{

}

std::list<consultation> CBP_SEARCH_CONSULTATIONS()
{

}

bool CBP_BOOK_CONSULTATIONS(const int idConsultation, const int idPatient, const char* raison)
{

}

void CBP_CLOSE()
{

}

int  estPresent(int socket); 
void ajoute(int socket); 
void retire(int socket); 

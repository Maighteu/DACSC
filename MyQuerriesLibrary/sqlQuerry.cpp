#include "sqlQuerry.h"

MYSQL * ConnexionBD()
{
	MYSQL *connexion = mysql_init(NULL);
  if (mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, 0, 0) == NULL)
  {
    fprintf(stderr,"(ACCESBD %d) (ERROR)acces bd echoue\n",getpid());
    exit(1);  
  }
    fprintf(stderr,"(ACCESBD %d) (SUCCESS)acces bd Reussi\n",getpid());
    return connexion;
}

int AjouterPatient(const char *nom, const char *prenom)
{
	MYSQL * connexion = ConnexionBD();
	char requete[255];
	sprintf(requete,"INSERT INTO patients (last_name,first_name) VALUES ('%s','%s')", nom, prenom);
	
	if (mysql_query(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	mysql_close(connexion);
    	return -1;
	}

	int i = mysql_insert_id(connexion); // recup id pour l'aficher dans id déroulant

	mysql_close(connexion);
	return i;
}

int AuthentifierPatient(const char *nom, const char *prenom, int idPatient)
{
	MYSQL * connexion = ConnexionBD();
	char requete[255];
	sprintf(requete,"SELECT * FROM patients WHERE last_name LIKE '%s' AND first_name LIKE '%s' AND id = '%d'", nom, prenom, idPatient);
	
	if (mysql_query(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	return -1;
	}
	MYSQL_RES* resultat = mysql_store_result(connexion);
    if (resultat == NULL)
    {
        fprintf(stderr,"(ACCESBD) Erreur de récupération du résultat : %s\n",mysql_error(connexion));
        return -1;
    }

	int i = mysql_num_rows(resultat);
	mysql_free_result(resultat);
	mysql_close(connexion);
	return i;
}

void rechercheConsultation(char *reponse, const char *specialite, const char *medecin, const char *dateDebut, const char *dateFin )
{
	char *nomMedecin, *prenomMedecin;

    char temp[40];
    strcpy(temp, medecin);

    char *token = strtok(temp, " ");

    nomMedecin = strdup(token);
    token = strtok(NULL, " ");
    prenomMedecin = strdup(token);


	MYSQL * connexion = ConnexionBD();
	char requete[255];
	// utiliser '%' en sql pour afficher tout ce qui a 0,1,tout caractere
	sprintf(requete,
	"SELECT  id, specialties.name,CONCAT(doctors.first_name, ' ',doctors.last_name), date, hour "
	"FROM consultations "
	"INNER JOIN  doctors on doctors.id = consultations.doctor_id "
	"INNER JOIN  specialties on specialties.id = doctors.specialty_id "
	"WHERE specialties.name LIKE '%s' "
	"AND doctors.last_name LIKE '%s' "
	"AND doctors.first_name LIKE '%s' "
	"AND patient_id IS NULL "
	"AND date BETWEEN '%s' AND '%s' "
	"ORDER BY date, hour;",specialite, nomMedecin, prenomMedecin, dateDebut, dateFin);
	
	if (mysql_query(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	mysql_close(connexion);
    	return;
	}

	MYSQL_RES* resultat = mysql_store_result(connexion);
	if (resultat == NULL)
	{
	    fprintf(stderr,"(ACCESBD) Erreur de récupération du résultat : %s\n",mysql_error(connexion));
	    mysql_free_result(resultat);
    	mysql_close(connexion);
	    return;
	}
	
	MYSQL_ROW ligne;

	reponse[0] = '\0';
	strcpy(reponse,"GET_CONSULTATIONS#ok#");
    while((ligne = mysql_fetch_row(resultat)))
    {
        char tempLigne[256];
        sprintf(tempLigne, "%d#%s#%s#%s#%s#", atoi(ligne[0]), ligne[1], ligne[2], ligne[3], ligne[4]);
        strcat(reponse, tempLigne);
    }

	mysql_free_result(resultat);
    mysql_close(connexion);
	return;

}

int ReserverConsulation( const char* idConsultation, const char* idPatient, const char *raison)
{
	MYSQL * connexion = ConnexionBD();
	char requete[255];
	sprintf(requete,
	"UPDATE consultations "
	"SET patient_id = '%s', reason = '%s' "
	"WHERE id = '%s' "
	"AND patient_id IS NULL",idPatient, raison ,idConsultation);
	if (mysql_query(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	mysql_close(connexion);
    	return -1;
	}

	my_ulonglong nbLignes = mysql_affected_rows(connexion);

	if (nbLignes == 0) 
	{
	    mysql_close(connexion);
		return 0; // cas ou qqu a prit le rdv avant nous
	}
    mysql_close(connexion);
	return 1;
}


void obtenirMedecin(char *reponse)
{
	MYSQL * connexion = ConnexionBD();
	char requete[255];
	sprintf(requete,"SELECT id, CONCAT(first_name,' ',last_name) FROM doctors ORDER BY first_name, last_name;");
	reponse[0] = '\0';
	    MYSQL_RES* resultat = mysql_store_result(connexion);
    if (resultat == NULL)
    {
    	strcpy(reponse,"GET_DOCTORS#ko#");

        fprintf(stderr,"(ACCESBD) Erreur de récupération du résultat : %s\n",mysql_error(connexion));
        return ;
    }
	strcpy(reponse,"GET_DOCTORS#ok#");
	MYSQL_ROW ligne;

    while((ligne = mysql_fetch_row(resultat)))
    {
        char tempLigne[256];
        sprintf(tempLigne, "%d#%s#", ligne[0], ligne[1]);
        strcat(reponse, tempLigne);
    }
	mysql_free_result(resultat);
    mysql_close(connexion);
	return;
}

void obtenirSpecialite(char *reponse)
{
	MYSQL * connexion = ConnexionBD();
	char requete[255];
	sprintf(requete,"SELECT id, name FROM specialties ORDER BY name;");
	reponse[0] = '\0';
	MYSQL_RES* resultat = mysql_store_result(connexion);
    if (resultat == NULL)
    {
    	strcpy(reponse,"GET_SPECIALTIES#ko#");
        fprintf(stderr,"(ACCESBD) Erreur de récupération du résultat : %s\n",mysql_error(connexion));
        return;
    }
	strcpy(reponse,"GET_SPECIALTIES#ok#");
	MYSQL_ROW ligne;
    while((ligne = mysql_fetch_row(resultat)))
    {
        char tempLigne[256];
        sprintf(tempLigne, "%d#%s#", ligne[0], ligne[1]);
        strcat(reponse, tempLigne);
    }
	mysql_free_result(resultat);
    mysql_close(connexion);
	return;
}
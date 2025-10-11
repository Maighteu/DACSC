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
	
	if (mysql_querry(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	return -1;
	}

	int i = mysql_insert_id(connexion)
	mysql_free_result(resultat);
	mysql_close(connexion);
	return i;
}

int AuthentifierPatient(const char *nom, const char *prenom,prenom int idPatient)
{
	MYSQL * connexion = ConnexionBD();
	char requete[255];
	sprintf(requete,"SELECT * FROM patients WHERE last_name LIKE '%s' AND first_name LIKE '%s' AND id = '%d'", nom, prenom, idPatient);
	
	if (mysql_querry(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	return -1;
	}

	int i = mysql_num_rows(resultat)
	mysql_free_result(resultat);
	mysql_close(connexion);
	return i;
}

//TODO dateDEBUT/dateFIN a mettre sous forme utilisable -> Regarder toutes dates entres ces deux dates (conditions)
void rechercheConsultation(const char *specialite, const char *medecin, const char *dateDebut, const char *dateFin )
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
	"SELECT * 
	FROM consultations
	INNER JOIN  doctors on doctors.id = consultations.doctor_id
	INNER JOIN  specialties on specialties.id = doctors.specialty_id
	WHERE specialties.name LIKE '%s' 
	AND doctors.last_name LIKE '%s' 
	AND doctors.first_name LIKE '%s' 
	AND patient_id IS NULL
	AND 
	AND
	 "),specialite, nomMedecin, prenomMedecin, dateDebut, dateFin;
	
	if (mysql_querry(connexion, requete))
	{
    	fprintf(stderr,"(ERROR)envoi requete bd echoue\n");
    	return -1;
	}

}

int ReserverConsulation(const int idConsultation, const int idPatient, const char *raison)
{

}


void obtenirMedecin(char *reponse)
void obtenirSpecialite(char *reponse)
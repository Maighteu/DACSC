#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>
#include <string.h>

typedef struct {
  int  id;
  char name[30];
} SPECIALTY;

typedef struct {
  int  id;
  int  specialty_id;
  char last_name[30];
  char first_name[30];
} DOCTOR;

typedef struct {
  int  id;
  char last_name[30];
  char first_name[30];
  char birth_date[20];
} PATIENT;

typedef struct {
  int  id;
  int  doctor_id;
  int  patient_id;
  char date[20];
  char hour[10];
  char reason[100];
} CONSULTATION;

SPECIALTY specialties[] = {
  {-1, "Cardiologie"},
  {-1, "Dermatologie"},
  {-1, "Neurologie"},
  {-1, "Ophtalmologie"}
};
int nbSpecialties = 4;

DOCTOR doctors[] = {
  {-1, 1, "Dupont", "Alice"},
  {-1, 2, "Lemoine", "Bernard"},
  {-1, 3, "Martin", "Claire"},
  {-1, 2, "Maboul", "Paul"},
  {-1, 1, "Coptere", "Elie"},
  {-1, 3, "Merad", "Gad"}
};
int nbDoctors = 6;

PATIENT patients[] = {
  {-1, "mv", "158", "1980-05-12"},
  {-1, "Petit", "Sophie", "1992-11-30"}
};
int nbPatients = 2;

CONSULTATION consultations[] = {
  {-1, 3,  1, "2025-10-01", "09:00", "Check-up"},
  {-1, 1,  2, "2025-10-02", "14:30", "Premier rendez-vous"},
  {-1, 2,  1, "2025-10-03", "11:15", "Douleurs persistantes"},
  {-1, 4, -1, "2025-10-04", "9:00", ""},
  {-1, 4, -1, "2025-10-04", "9:30", ""},
  {-1, 4, -1, "2025-10-04", "10:00", ""},
  {-1, 4, -1, "2025-10-04", "10:30", ""},
  {-1, 6, -1, "2025-10-07", "13:00", ""},
  {-1, 6, -1, "2025-10-07", "13:30", ""},
  {-1, 6, -1, "2025-10-07", "14:00", ""},
  {-1, 6, -1, "2025-10-07", "14:30", ""},
};
int nbConsultations = 11;

void finish_with_error(MYSQL *con) {
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main() {
  MYSQL* connexion = mysql_init(NULL);
  if (!mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0)) {
    finish_with_error(connexion);
  }

  mysql_query(connexion, "DROP TABLE IF EXISTS consultations;");
  mysql_query(connexion, "DROP TABLE IF EXISTS patients;");
  mysql_query(connexion, "DROP TABLE IF EXISTS doctors;");
  mysql_query(connexion, "DROP TABLE IF EXISTS specialties;");

  if (mysql_query(connexion, "CREATE TABLE specialties (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(30));"))
    finish_with_error(connexion);

  if (mysql_query(connexion, "CREATE TABLE doctors ("
                             "id INT AUTO_INCREMENT PRIMARY KEY, "
                             "specialty_id INT, "
                             "last_name VARCHAR(30), "
                             "first_name VARCHAR(30), "
                             "FOREIGN KEY (specialty_id) REFERENCES specialties(id));"))
    finish_with_error(connexion);

  if (mysql_query(connexion, "CREATE TABLE patients ("
                             "id INT AUTO_INCREMENT PRIMARY KEY, "
                             "last_name VARCHAR(30), "
                             "first_name VARCHAR(30), "
                             "birth_date DATE);"))
    finish_with_error(connexion);

  if (mysql_query(connexion, "CREATE TABLE consultations ("
                             "id INT AUTO_INCREMENT PRIMARY KEY, "
                             "doctor_id INT NOT NULL, "
                             "patient_id INT NULL, "
                             "date DATE, "
                             "hour VARCHAR(10), "
                             "reason VARCHAR(100), "
                             "FOREIGN KEY (doctor_id) REFERENCES doctors(id), "
                             "FOREIGN KEY (patient_id) REFERENCES patients(id));"))
    finish_with_error(connexion);

  char request[512];
  for (int i = 0; i < nbSpecialties; i++) {
    sprintf(request, "INSERT INTO specialties VALUES (NULL, '%s');", specialties[i].name);
    if (mysql_query(connexion, request)) finish_with_error(connexion);
  }

  for (int i = 0; i < nbDoctors; i++) {
    sprintf(request, "INSERT INTO doctors VALUES (NULL, %d, '%s', '%s');",
            doctors[i].specialty_id, doctors[i].last_name, doctors[i].first_name);
    if (mysql_query(connexion, request)) finish_with_error(connexion);
  }

  for (int i = 0; i < nbPatients; i++) {
    sprintf(request, "INSERT INTO patients VALUES (NULL, '%s', '%s', '%s');",
            patients[i].last_name, patients[i].first_name, patients[i].birth_date);
    if (mysql_query(connexion, request)) finish_with_error(connexion);
  }

  for (int i = 0; i < nbConsultations; i++) {
    if (consultations[i].patient_id == -1) {
      sprintf(request, "INSERT INTO consultations (doctor_id, patient_id, date, hour, reason) "
                       "VALUES (%d, NULL, '%s', '%s', '%s');",
              consultations[i].doctor_id, consultations[i].date, consultations[i].hour, consultations[i].reason);
    } else {
      sprintf(request, "INSERT INTO consultations (doctor_id, patient_id, date, hour, reason) "
                       "VALUES (%d, %d, '%s', '%s', '%s');",
              consultations[i].doctor_id, consultations[i].patient_id, consultations[i].date,
              consultations[i].hour, consultations[i].reason);
    }
    if (mysql_query(connexion, request)) finish_with_error(connexion);
  }

  mysql_close(connexion);
  return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <mysql.h>

// void finish_with_error(MYSQL *con) {
//     fprintf(stderr, "%s\n", mysql_error(con));
//     mysql_close(con);
//     exit(1);
// }

// int main() {
//     MYSQL *connexion = mysql_init(NULL);
//     if (!connexion) {
//         fprintf(stderr, "Erreur d'initialisation MySQL\n");
//         exit(1);
//     }

//     if (!mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0)) {
//         finish_with_error(connexion);
//     }

//     printf("✅ Connecté à la base de données.\n\n");

//     MYSQL_RES *result;
//     MYSQL_ROW row;

//     // ---- TABLE specialties ----
//     if (mysql_query(connexion, "SELECT id, name FROM specialties;")) {
//         finish_with_error(connexion);
//     }

//     result = mysql_store_result(connexion);
//     if (!result) finish_with_error(connexion);

//     printf("📘 TABLE : specialties\n");
//     printf("---------------------------------\n");
//     printf("%-5s | %-20s\n", "ID", "Nom");
//     printf("---------------------------------\n");

//     while ((row = mysql_fetch_row(result))) {
//         printf("%-5s | %-20s\n", row[0], row[1]);
//     }
//     mysql_free_result(result);

//     printf("\n");

//     // ---- TABLE doctors ----
//     if (mysql_query(connexion, "SELECT id, specialty_id, last_name, first_name FROM doctors;")) {
//         finish_with_error(connexion);
//     }

//     result = mysql_store_result(connexion);
//     if (!result) finish_with_error(connexion);

//     printf("🩺 TABLE : doctors\n");
//     printf("------------------------------------------------------\n");
//     printf("%-5s | %-12s | %-15s | %-15s\n", "ID", "Spécialité", "Nom", "Prénom");
//     printf("------------------------------------------------------\n");

//     while ((row = mysql_fetch_row(result))) {
//         printf("%-5s | %-12s | %-15s | %-15s\n", row[0], row[1], row[2], row[3]);
//     }
//     mysql_free_result(result);

//     printf("\n✅ Affichage terminé.\n");

//     mysql_close(connexion);
//     return 0;
// }

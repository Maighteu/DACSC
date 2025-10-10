#include "TCP.h"

int ServerSocket(int port)
{
	int sockServListen;
	struct addrinfo hints, *results;
	char cPort[6];
	snprintf(cPort, sizeof(cPort), "%d", port);

	//Création de la socket
	/////////////////////////////////////////////////
	if ((sockServListen= socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("Creation socket echouée, socket()");
		exit(1);
	}
	printf("Creation socket réussie, socket()\n");

	//Construction adresse
	///////////////////////////////////////////////
	memset (&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST; 

	if (getaddrinfo(NULL,cPort,&hints,&results) != 0) 
	{
		perror("Erreur de resolution de nom, getaddrinfo()");
		exit(1);
	}
	printf("Resolution de nom réussie, socket()\n");

	//Binding Socket
	//////////////////////////////////////////////////////////////
	if (bind(sockServListen, results->ai_addr, results->ai_addrlen) < 0)
	{
		perror("Erreur de binding, bind()"); 
		exit(1);

	}
	printf("Binding socket réussie, bind()\n");
	freeaddrinfo(results);

	//Listen on  socket
	/////////////////////////////////////////////////
	if (listen(sockServListen, SOMAXCONN) == -1) //SOMAXCONN originaire de <sys/socket.h> 
	{
		perror("Erreur de listen, listen()"); 
		exit(1);
	}
	printf("Listen on socket réussi, listen()\n");

	return sockServListen;


}
int Accept(int sEcoute,char *ipClient)
{
  struct sockaddr_in adrClient; 
  socklen_t adrClientLen = sizeof(struct sockaddr_in); // struct taille binaire
  int sService;
  // accept on socket serv
  /////////////////////////////////////
  if ((sService = accept(sEcoute, (struct sockaddr*)&adrClient, & adrClientLen)) == -1) //(struct sockaddr*) permet de cast sockaddr_in en sockaddr pour le compilateur
  									//Ici on ajoute l'ip client attendu par l'accept dans adrClient
  {
  		perror("Erreur d'accept serv, accept()"); 
  		exit(2);
  }
  	printf("accept on socket serv réussi, accept()\n");

  	//Translate ip to text
  	/////////////////////////////////////////
  	if (ipClient != NULL)
  	{
  		inet_ntop(AF_INET, &adrClient.sin_addr, ipClient, INET_ADDRSTRLEN);// ici on change l'ip binaire en chaine de caractere à ajouter
  		//INET_ADDRSTRLEN car INET size est définie
  		printf("Client connecte, Adresse IP txt visible %s \n", ipClient);
  	}
  	else printf("Client connecte, Adresse IP txt non visible\n");
  	return sService;
 }

int ClientSocket(char* ipServeur,int portServeur)
{
	int sockCliListen;
	struct addrinfo hints, *results;
	char cPort[6];
	snprintf(cPort, sizeof(cPort), "%d", portServeur);
	//create socket
	///////////////////////////////////
	if ((sockCliListen = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("Creation socket echouée, socket()");
		exit(3);
	}
	printf("Creation socket réussie, socket()\n");

	//Construction adresse
	///////////////////////////////////////////////
	memset (&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST; 

	if (getaddrinfo(NULL,cPort,&hints,&results) != 0) 
	{
		perror("Erreur de resolution de nom, getaddrinfo()");
		exit(3);
	}
	printf("resolution de nom réussie, socket()\n");

	//Connect to server
	//////////////////////////////////
	if (connect(sockCliListen,results->ai_addr, results->ai_addrlen)==-1)
	{
		perror("Erreur de connect, connect()");
		exit(3);
	}
	printf("Connect réussi, connect()\n");
	return sockCliListen;
}

int Send(int sSocket,char* data,int taille)
{
	size_t data_lenght = taille + strlen(END_MARKER);
	char* data_send = new char[data_lenght];
	memcpy(data_send,data,taille);
	memcpy(data_send+taille,END_MARKER, strlen(END_MARKER));

	//ssize pour signer en cas de negatif
	ssize_t data_sent = send(sSocket, data_send, data_lenght, 0);
	delete[] data_send;
    
    if (data_sent != data_lenght) return -1;
        
    return data_sent - (ssize_t) strlen(END_MARKER);

}

int Receive(int sSocket,char* data) 
{
	bool boucle = true;
	ssize_t data_received_lenght;
	int i = 0;
	char reception[3]; // pour recup /r/r/n/0

	while (boucle)
	{
		if((data_received_lenght = recv(sSocket,reception,1,0))==-1)
			return -1;
		else if (data_received_lenght == 0)
			return 0; //deco client

		if (reception[0] == END_MARKER[0])
		{
			if((data_received_lenght = recv(sSocket,&reception[1],1,0))==-1)
				return -1;
			else if (data_received_lenght == 0)
				return 0;

			if (reception[1] == END_MARKER[1])
			{
				if((data_received_lenght = recv(sSocket,&reception[2],1,0))==-1)
					return -1;
				else if (data_received_lenght == 0)
					return 0;
				if (reception[2] == END_MARKER[2])
				{
					boucle = false;
				}
				else
				{
					data[i++] = reception[0];
					data[i++] = reception[1];
					data[i++] = reception[2];
				}
			}
			else
			{
				data[i++] = reception[0];
				data[i++] = reception[1];
			}
		}
		else data[i++] = reception[0];
	}

	data[i] = '\0';
	fprintf(stderr, "Reception = %s\n", data);
	return i;

}
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void RaiseError(char *msg){
	perror(msg);
	exit(1);
}

#define BUFSIZE 256
char recvBuffer[BUFSIZE];
char sendBuffer[BUFSIZE];

int main( int argc, char *argv[]){
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if(argc < 2){
		fprintf(stderr, "ERROR: NO PORT PROVIDED.\n");
		exit(1);
	}
	
	// Socket file descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sockfd < 0 )
		RaiseError("socket() failed.");
	
	// Construct address struct
	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(portno);
	
	// Bind
	if( bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		RaiseError("bind() failed.");

	// Listen
	listen(sockfd, 5);
	
	// Accept connection
	printf("Waiting for connection ...\n");
	clilen = sizeof(cli_addr);	
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if( newsockfd < 0 )
			RaiseError("accept() failed.");
	
		// Read from client
		memset((char*)recvBuffer, 0, sizeof(recvBuffer));
		if( read(newsockfd, recvBuffer, sizeof(recvBuffer)-1) < 0)
			RaiseError("read() failed.");
		//printf("%s", buffer);
		int curPoint = atoi(recvBuffer);
		int highPoint = 0;
		
		FILE *fptr = fopen("log.data", "r+");
		fscanf(fptr, "%d", &highPoint);
		
		if( highPoint > curPoint ){
			printf("high = %d, cur = %d\n", highPoint, curPoint);
			fprintf(fptr, "%d\n", curPoint);
			printf("updated.\n");

			memset((char*)sendBuffer, 0, sizeof(sendBuffer));
			sprintf(sendBuffer, "%d", 1);
			printf("%s\n", sendBuffer);

			// send to client
			if( write(newsockfd, "1", 1) < 0 )
				RaiseError("write() failed.");

		}else{
			printf("high = %d, cur = %d\n", highPoint, curPoint);


			memset((char*)sendBuffer, 0, sizeof(sendBuffer));
			sprintf(sendBuffer, "%d", 0);	
			printf("%s\n", sendBuffer);
			// send to client
			if( write(newsockfd, "0", 1) < 0 )
				RaiseError("write() failed.");


		}

		fclose(fptr);
	}	
	// Close
	close(sockfd);
	close(newsockfd);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFSIZE 512

void udp_mess (struct sockaddr_in server_address) {
    char buf[BUFSIZE];
    /* Vytvoreni soketu */ 
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		perror("ERROR: socket");
		exit(EXIT_FAILURE);
	}
    while (1){
        /* nacteni zpravy od uzivatele */
        bzero(buf, BUFSIZE);
        printf("Please enter msg: ");
        fgets(buf + 2, BUFSIZE - 2, stdin);
        if (strcmp((buf), EOF))
        buf[0] = 0x00;
        buf[1] = strlen(buf + 2)

        /* odeslani zpravy na server */
        serverlen = sizeof(server_address);
        bytestx = sendto(client_socket, buf, strlen(buf + 2) + 2, 0, (struct sockaddr *) &server_address, serverlen);
        if (bytestx < 0) 
        perror("ERROR: sendto");
        
        /* prijeti odpovedi a jeji vypsani */
        bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
        if (bytesrx < 0) 
        perror("ERROR: recvfrom");
        buf[5] = 0;
        printf("Echo from server: %s", buf+3);
    }
    return 0;
}

int main (int argc, const char * argv[]) {
	int client_socket, port_number, bytestx, bytesrx;
    socklen_t serverlen;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
     
    /* 1. test vstupnich parametru: */
    server_hostname = argv[2];
    port_number = atoi(argv[4]);
    
    /* 2. ziskani adresy serveru pomoci DNS */
    
    if ((server = gethostbyname(server_hostname)) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
        exit(EXIT_FAILURE);
    }
    
    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
   
    /* tiskne informace o vzdalenem soketu */ 
    printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
    
	
    if (!(strcmp(argv[6], "tcp"))){
        //tcp_mess();
    }
    else if (!(strcmp(argv[6], "udp"))){
        udp_mess(server_address);
    }
    
}
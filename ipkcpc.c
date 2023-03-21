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
#define OPCODE_INDEX 0
#define BAF_DATA_OFSET_Q 2
#define BAF_DATA_OFSET_R 3
#define RESPONSE_DATA_LEN buf[2]

int client_socket;

/*Funkce pro UDP*////////////////////////////////////////////////////////
void udp_mess (struct sockaddr_in server_address) {
    char buf[BUFSIZE];
    int bytestx, bytesrx;
    socklen_t serverlen;
    int opc;
    int stat;
    /* Vytvoreni a overeni socketu */ //////////////////////////////////
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		perror("ERROR: socket");
		exit(EXIT_FAILURE);
	}
    ///////////////////////////////////////////////////////////////
    while (1){
        bzero(buf, BUFSIZE);
        /*Nic k nacteni*///////////////////////////////////////
        if (!fgets(buf + BAF_DATA_OFSET_Q, BUFSIZE - BAF_DATA_OFSET_Q, stdin)){
            break;
        }
        ////////////////////////////////////////////////////////
        /*Nastaveni hlavicky socketu*///////////////////////////
        buf[OPCODE_INDEX] = 0x00;
        buf[1] = strlen(buf + BAF_DATA_OFSET_Q);
        ////////////////////////////////////////////////////////
        /* odeslani zpravy*/
        serverlen = sizeof(server_address);
        bytestx = sendto(client_socket, buf, strlen(buf + BAF_DATA_OFSET_Q) + BAF_DATA_OFSET_Q, 0, (struct sockaddr *) &server_address, serverlen);
        if (bytestx < 0) {
            perror("ERROR: sendto");
        }
        ///////////////////////////////////////////////////////
        /* prijeti odpovedi a validace*//////////////////////
        bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
        /*chyba prijeti*///////////////////////////////////
        if (bytesrx < 0) {
            perror("ERROR: recvfrom");
        }
        /////////////////////////////////////////////////
        buf[RESPONSE_DATA_LEN + BAF_DATA_OFSET_R] = 0;
        opc = (int)buf[OPCODE_INDEX];
        /*Chyba nacteni odpovedi*///////////////////////////
        if (opc != 1){
            fprintf(stderr, "Response error\n");
            return;
        }
        /////////////////////////////////////////
        stat = (int)buf[1];
        /*Vypis odpovedi nebo erroru pri chybe*/////////
        if (stat == 1){
            printf("ERR:%s\n", (buf + BAF_DATA_OFSET_R));
        }
        else if (stat == 0){
            printf("OK:%s\n", (buf + BAF_DATA_OFSET_R));
        }
        /////////////////////////////////////////////////
    }
}

/*Funkce pro TCP*////////////////////////////////////////////////////////
void tcp_mess (struct sockaddr_in server_address) {
    char buf[BUFSIZE];
    int bytestx, bytesrx;
    int closed = 0;
    socklen_t serverlen;
    /* Vytvoreni a overeni soketu *///////////////////////////////////////
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
    /////////////////////////////////////////////////////////////////////////
    /*Vytvoreni spojeni*/////////////////////////////////////////////////////
    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
        {
            perror("ERROR: connect");
            exit(EXIT_FAILURE);        
        }
    //////////////////////////////////////////////////////////////////////////
    while (1) {         
        /*Vynulovani bufferu*///////////////////////////////////////
        bzero(buf, BUFSIZE);
        /*Chyba nacteni a ukonceni*///////////////////////////////////////////
        if (fgets(buf, BUFSIZE, stdin) == NULL){
            if(closed == 1){
                bytestx = send(client_socket, "BYE\n", strlen("BYE\n"), 0);
                if (bytestx < 0) {
                    perror("ERROR in sendto");
                }
                bytesrx = recv(client_socket, buf, BUFSIZE, 0);
                if (bytesrx < 0) {
                    perror("ERROR in recvfrom");
                }
                break;
            }
            return;
        }
        ////////////////////////////////////////////////////////
        /*Nastaveni priznaku pro vypsani "BYE" a ukonceni*//////
        if(strcmp((buf + BAF_DATA_OFSET_Q), "BYE\n")){
            closed = 1;
        }
        //////////////////////////////////////////////////////
        /* odeslani zpravy*////////////////////////
        bytestx = send(client_socket, buf, strlen(buf), 0);
        if (bytestx < 0) {
            perror("ERROR in sendto");
        }
        //////////////////////////////////////////////////////
        /* prijeti odpovedi*///////////////////////////////////
        bzero(buf, BUFSIZE);
        bytesrx = recv(client_socket, buf, BUFSIZE, 0);
        if (bytesrx < 0) {
            perror("ERROR in recvfrom");
        }
        //////////////////////////////////////////////////////
        /*Vypsani odpovedi*//////////////////////////////////
        printf("%s", buf);
        ///////////////////////////////////////////////////////
        /*Ukonceni pri "BYE"*/////////////////////////////////
        if (!(strcmp(buf, "BYE\n"))){
            break;
        }
    }
    /*Ukonceni spojeni*///////////////////////////////////////////
    close(client_socket);
}

int main (int argc, const char * argv[]) {
	int port_number;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
     
    /*Nastaveni parametru*///////////////////////////////////////////
    server_hostname = argv[2];
    port_number = atoi(argv[4]);
    //////////////////////////////////////////////////////
    /*Ziskani adresy serveru pomoci DNS*///////////////////
    if ((server = gethostbyname(server_hostname)) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*Nalezeni IP adresy serveru a inicializace struktury server_address*/
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*Urceni TCP/UDP a volani prislusne funkce*/
    if (!(strcmp(argv[6], "tcp"))){
        tcp_mess(server_address);
    }
    else if (!(strcmp(argv[6], "udp"))){
        udp_mess(server_address);
    }
    else{
        printf("ERR:Chyba parametru -m\n");
        return -1;
    }
    
}
//chat client
/******************************************************************
 * Kierin Doherty (dohertki)
 * CS372 Project 1- chatclient.c
 * 30 Oct 2016
 * 
 * Description: Program is a chat client that connects to the chat
 *              server (chatserve.py)

 * Input: The program takes the following arguments:
 *           /.chatclient   [server-hostname] [port#]
 *
 ******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h> //gethostname()
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h> 

#define NAMELEN 20
#define  MSGLEN 1024


//Set the user name
void setName(char *name);  
 //Get chat message from user
int getMessage(char *msg, char *name, int sockfd_m);
 //Read messages from socket connection
void readMessage(int sockfd_r);   
/*Set the port number and server from command line arguments*/
int setAddress(int argc_a, char **argv_a, int *port_a, char *myserver_a);
/*Send handshake message to the chat server*/
int handshakeMsg(int sockfd_h, char *name_h);

int main(int argc, char *argv[]){

    int sockfd;     //Pointer to socket
    int port;       //Port number
    int loop = 1;
    struct addrinfo hints, *res; //Struct used by getaddrinfo to hold result 
    char *mycomputer;
    char *myserver;
	char name[NAMELEN];
	char message[MSGLEN];
    myserver = malloc(sizeof(char) * 64);
    mycomputer = malloc(sizeof(char) * 64);
    ssize_t  w, r;
 
    loop = setAddress(argc, argv, &port, myserver);

    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;//IPv4.....  
    hints.ai_socktype = SOCK_STREAM;//TPC socket type

    gethostname(mycomputer, 50); //returns that name of my computer
    getaddrinfo(myserver, argv[2], &hints, &res);

    printf("Local computer: %s \n", mycomputer);
    
/*  source  
http://stackoverflow.com/questions/1276294/getting-ipv4-address-from-a-sockaddr-structure
  */  
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ipAddress[INET_ADDRSTRLEN];    inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);

    printf("The IP address is: %s\n", ipAddress);
    
    



/*  Setting up client*/    
    //Step 1. Create a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
       perror("Error opening socket");
  
    //Step2. Connect.
    if(connect(sockfd,res->ai_addr, res->ai_addrlen) < 0){
        perror("Error connecting");
        loop = 0;
    }else
        printf("Connection made\n");
    
    /*Set the user's chat room name*/  
    if(loop){
        setName(name);
        loop = handshakeMsg(sockfd, name);
    }
    
    
    while(loop){

       loop = getMessage(message, name, sockfd);
        
       readMessage(sockfd);    
      
  
  }


    return 0;


}



/******************************************************************************
 * Function: setName()
 *
 * Use: Function prompts user for a name to be used as chat alias.
 *
 * Input: char *name: value hold the name of the user
 *       
 *
 * Output: Function is void, but modifies the argument (*name).
 *
 *****************************************************************************/

void setName(char *name){
    

    char *flag = "> ";
	
    printf("Enter name: ");
	fgets(name,NAMELEN, stdin);
	fflush;
    printf("Login name: %s\n", name);
    if(name[strlen(name)-1] == '\n')
        name[strlen(name) -1] = '\0';
    strncat(name,flag, 12);    
    
    return;
}

/*******************************************************************
 * Function: getMessage() 
 *
 * Use: Function gets a message from the user and sends it to the
 *      chat server.
 *
 * Input: char *msg:  function can take a string and append to it
 *                    for chat message.
 *        char *name: User name, is used as a handle in message.
 *        int sockfd_m: pointer to the socket in which to send
 *                      message
 * Output: Return type is void. message string can be changed 
 *
 ******************************************************************/

int getMessage(char *msg, char *name, int sockfd_m){

    int w;
    int quit_flag = 1;
    char input[1024]; 
    char test[] = "\\quit";
    strcpy(msg,"");    
    
    printf(":");
    bzero(input,1024); 
    fgets(input, 500, stdin);
    input[strlen(input)-1] = '\0';
//    printf("%s\n",input);
    if(strcmp(input, "\\quit") == 0){
//    if(strncmp(input, test, 5 ) == 0 ){
        strncat(msg,"User <", 500);
        strncat(msg, name, 500);
        strncat(msg, "has left chat!\n", 500);
        quit_flag = 0;
    }else{
        strncat(msg,name,500);
        strncat(msg,input,512);
    }


    w= write(sockfd_m, msg ,strlen(msg)); //Write cipher text to server
    if (w < 0) 
        perror("ERROR writing to socket");

    return quit_flag;

}



/******************************************************************************
 * Function: readMessage()
 *
 * Use: Function reads incomming messages from socket connection and prints 
 *      them to the display.
 *
 * Input: int sockfd_r: Pointer to a socket.
 *       
 * Output: The function returns void, incoming messages are sent display(stdout)
 *
 *****************************************************************************/
void readMessage(int sockfd_r){
    int n;
    char small_buffer[1024];

    bzero(small_buffer,1024);  //clear buffer.
    n = read(sockfd_r,small_buffer,1024); //read message back from server.
    if (n < 0) 
        perror("ERROR reading from socket");//        error("Error connecting");
  
    printf("%s", small_buffer); 
    
    return;

}

/******************************************************************************
 * Function: setAddress
 *
 * Use: This function uses values from the command line arguments and puts them
 *      into value for the server host name and its port number
 *
 * Input: int argc_a: int argc, the number of arguments passed to the command
                      line
 *        char **argv_a: char** argv, argument checked for the server hostname
 *        int *port_a:  the port number assinged by user
 *        char *myserver_a: the server hostname entered by user
 *       
 * Output: The function return an int, 1 if ok, 0 if not.
 *
 *****************************************************************************/

int setAddress(int argc_a, char **argv_a, int *port_a, char *myserver_a){
    int flag =1;
    printf("args %d\n", argc_a);
    if(argc_a == 3){
        *port_a = atoi(argv_a[2]);
        strcpy(myserver_a, argv_a[1]);
    
        printf("Chat server: %s\n", myserver_a);

        printf("port set to: %d\n", *port_a);
    }else{
        printf("Clientchat usage:  ./clientchat [server-hostname] [server-port#]\n");
        flag = 0;
    }
    
    return flag;

}

/******************************************************************************
 * Function: handshakeMsg()
 *
 * Use: Function sends and receives a inial message form the server.
 *
 * Input: int sockfd_h: The pointer to a socket
 *        char *name_h: The users chat room alias
 *
 * Output: Function returns an int, 1 for ok, 0 for not ok. 
 *
 *****************************************************************************/

int handshakeMsg(int sockfd_h, char *name_h){

    char tiny_buffer[1024];
    int n;
    int flag = 1;
    char content[256]= "Connection from <";
    strncat(content, name_h, 256);
    
    n= write(sockfd_h ,content, strlen(content)); //Write message to server
    if (n < 0){
        perror("ERROR writing to socket");
        flag = 0;
    }
        
    bzero(tiny_buffer,1024);  //clear buffer.
    n = read(sockfd_h, tiny_buffer,1024); //read message back from server.
    if (n < 0){
        perror("ERROR reading from socket");//        error("Error connecting");
        flag = 0; 
    }
    printf("%s\n",tiny_buffer);
    return flag;
}
/******************************************************************************
 * Function: 
 *
 * Use: 
 *
 * Input: 
 *       
 *
 * Output: 
 *
 *****************************************************************************/

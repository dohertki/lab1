//chat client
/******************************************************************
 * Kierin Doherty (dohertki)
 * CS372 Project 1- chatclient.c
 * 30 Oct 2016
 * 
 * Description: Program is a chat client that connects to the chat
 *              server (chatserve.py)

 * Input: The program takes the following arguments:
 *           /.chatclient.c   [server-hostname] [port#]
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

void setme(){
    return;
}

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
//  getaddrinfo(mycomputer, argv[2], &hints, &res);
    getaddrinfo(myserver, argv[2], &hints, &res);

    printf("Local computer: %s \n", mycomputer);
    
/*  source  
http://stackoverflow.com/questions/1276294/getting-ipv4-address-from-a-sockaddr-structure
  */  
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ipAddress[INET_ADDRSTRLEN];    inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);

    printf("The IP address is: %s\n", ipAddress);
    
    

/*Set up the struct members*/


/*  Setting up client
    1. Create a socket with socket()
    2. Connect to the server 'Host A' with connect() 
*/    

    sockfd = socket(AF_INET, SOCK_STREAM, 0);//Step 1. Create a socket.
    if(sockfd < 0)
       perror("Error opening socket");
  
      
    /*Format:
        int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen),
    */
    if(connect(sockfd,res->ai_addr, res->ai_addrlen) < 0)//Step2. Connect.
       perror("Error connecting");
    printf("Connection made\n");
    
    setName(name);
    
    int n;
    char content[255]= "Have a nice day!";

    n= write(sockfd,content ,strlen(content)); //Write cipher text to server
    if (n < 0) 
        perror("ERROR writing to socket");
        
    char tiny_buffer[1024];
    bzero(tiny_buffer,256);  //clear buffer.
    n = read(sockfd,tiny_buffer,255); //read message back from server.
    if (n < 0) 
        perror("ERROR reading from socket");//        error("Error connecting");
  
  

    bzero(tiny_buffer,256);  //clear buffer.
    
    while(loop){

       loop = getMessage(message, name, sockfd);
        
       readMessage(sockfd);    
      
       
    
  
  
  
  }


    return 0;


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
    char input[500]; 
    strcpy(msg,"");    
    
//    printf("%s",name);
	
    fgets(input, 500, stdin);
	if(strncmp(input, "\\quit", 5 ) == 0 ){
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

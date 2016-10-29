//chat client


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
#define PORT "8888" //FIXME 
#define MAXDATASIZE 100 //FIXME

#define NAMELEN 20
#define  MSGLEN 513

void setme(){
    return;
}
/*
void setsocket(int n, char **arg){

    if(argc > 1){
        n = atoi(argv[2]);
        printf("Port set to: %d\n", n);
    }else{
        n = 8888;
        printf("port set to default: %d\n", n);
    }
   return;
}

*/

void setName(char *name);
void getMessage(char *msg, char *name);

int main(int argc, char *argv[]){

    int sockfd;     //Pointer to socket
    int port;       //Port number
//    struct sockaddr_in serv_addr;
    struct addrinfo hints, *res; //Struct used by getaddrinfo to hold result 
    char *mycomputer;
    char *myserver;
	char name[NAMELEN];
	char message[MSGLEN];
    myserver = malloc(sizeof(char) * 64);
    mycomputer = malloc(sizeof(char) * 64);
    ssize_t  w, r;
 
    port = atoi(argv[2]);

  
    
  //  setsocket(port, argv);
    strcpy(myserver, argv[1]);
    printf("Chat server: %s\n", myserver);
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;//IPv4.....   beej was UNSPEC
    hints.ai_socktype = SOCK_STREAM;//TPC socket type

    gethostname(mycomputer, 50); //returns that name of my computer
//  getaddrinfo(mycomputer, argv[2], &hints, &res);
    getaddrinfo(myserver, argv[2], &hints, &res);

    printf("Local computer: %s \n", mycomputer);
    
//    printf("This is my local IP: %s \n", res->ai_addr->sa_data);
/*  source  
http://stackoverflow.com/questions/1276294/getting-ipv4-address-from-a-sockaddr-structure
  */  
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);

    printf("The IP address is: %s\n", ipAddress);
/************************************************************************************/    
    
    printf("port set to: %d\n", port);
    

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
    char small_buffer[256];
    char content[255]= "Have a nice day!";

    n= write(sockfd,content ,strlen(content)); //Write cipher text to server
    if (n < 0) 
        perror("ERROR writing to socket");
        
    bzero(small_buffer,256);  //clear buffer.
    n = read(sockfd,small_buffer,255); //read message back from server.
    if (n < 0) 
        perror("ERROR reading from socket");//        error("Error connecting");
  
    printf("%s", small_buffer);  
  

    setName(name);

    while(1){
    printf("%s ",name);
    getMessage(message, name);
    printf("The message out of function: %s", message);

    
    
    w= write(sockfd, message ,strlen(message)); //Write cipher text to server
    if (w < 0) 
        perror("ERROR writing to socket");
      
    strcpy(message,"");    
    
    r= read(sockfd, message, strlen(message));

    if(strlen(message) > 1)
        printf("%s",message);
  
  
  
  }




    return 0;


}




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





void getMessage(char *msg, char *name){

    char input[500]; 
    strcpy(msg,"");    
	fgets(input, 500, stdin);
	strncat(msg,name,500);
	strncat(msg,input,512);
	printf("\n%s", msg);

    return;
}






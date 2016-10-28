//chat client


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //gethostname()
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h> 
#define PORT "8888" //FIXME 
#define MAXDATASIZE 100 //FIXME


/*
setsocket(int &n, int count, char *arg){

    if(argc > 1){
        n = atoi(argv[1]);
        printf("port set to: %d\n", n);
    }else{
        n = 8888;
        printf("port set to default: %d\n", n);
    }
   return 0;
}
*/


int main(int argc, char *argv[]){

    int sockfd;     //Pointer to socket
    int port;       //Port number
    struct sockaddr_in serv_addr;
    struct addrinfo hints, *res; //Struct used by getaddrinfo to hold result

    char *mycomputer;


 




    if(argc > 1){
         port = atoi(argv[1]);
        printf("port set to: %d\n", port);
    }else{
        port = 8888;
        printf("port set to default: %d\n", port);
    }



    mycomputer = malloc(sizeof(char) * 50);
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;//IPv4.....   beej was UNSPEC
    hints.ai_socktype = SOCK_STREAM;//TPC socket type

    gethostname(mycomputer, 50); //returns that name of my computer
    getaddrinfo(mycomputer, argv[1], &hints, &res);

    printf("This is my local computer: %s \n", mycomputer);
    
    printf("This is my local IP: %s \n", res->ai_addr->sa_data);
/*  source  
http://stackoverflow.com/questions/1276294/getting-ipv4-address-from-a-sockaddr-structure
  */  
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);

    printf("The IP address is: %s\n", ipAddress);
/************************************************************************************/    
    
    

/*Set up the struct members*/


/*  Setting up client
    1. Create a socket with socket()
    2. Connect to the server 'Host A' with connect() 
*/    

    sockfd = socket(AF_INET, SOCK_STREAM, 0);//Step 1. Create a socket.
    if(sockfd < 0)
        error("Error opening socket");
  
  
    /*Format:
        int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen),
    */
    if(connect(sockfd,res->ai_addr, res->ai_addrlen) < 0)//Step2. Connect.
        error("Error connecting");

    
    int n;
    char small_buffer[256];
    char content[255]= "Have a nice day!";

    n= write(sockfd,content ,strlen(content)); //Write cipher text to server
    if (n < 0) 
        error("ERROR writing to socket");
        
    bzero(small_buffer,256);  //clear buffer.
    n = read(sockfd,small_buffer,255); //read message back from server.
    if (n < 0) 
        error("ERROR reading from socket");//        error("Error connecting");
  
    printf("%s", small_buffer);  
  


    return 0;


}








//  printf("inet_ntoa(in_addr)sin = %s\n",inet_ntoa((struct in_addr)addr->sin_addr));

    //Set Address family 
  //  serv_addr.sin_family = AF_INET;
    //Set IP address
    // void bcopy(const void *src, void *dest, size_t n);
//    bcopy((char *)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    //Set IP port        
//    serv_addr.sin_port = 8888;
//    if(connect(sockfd,(struct sockaddr * ) &serv_addr, sizeof(serv_addr) < 0)//Step2. Connect.

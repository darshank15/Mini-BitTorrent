#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <sys/socket.h> 
#include <netinet/in.h> 

using namespace std;

//#define PORT 7000 

int main(int argc, char *argv[])
{

	if (argc != 5)
    {
        cout << "Invalid Argument !!!" << endl;

    }
    else{

        trackersocket1.setsocketdata(string(argv[1]));
        trackersocket2.setsocketdata(string(argv[2]));
        string seederfile=string(argv[3]);
        string logfile=string(argv[4])

    	int server_fd, new_socket, valread; 
	    struct sockaddr_in address; 
	    int opt = 1; 
	    int addrlen = sizeof(address); 

	    // Creating socket file descriptor 
	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	    { 
	        perror("socket failed"); 
	        exit(EXIT_FAILURE); 
	    } 
	       
	    // Forcefully attaching socket to the port 8080 
	    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
	                                                  &opt, sizeof(opt))) 
	    { 
	        perror("setsockopt"); 
	        exit(EXIT_FAILURE); 
	    } 
	    address.sin_family = AF_INET; 
	    address.sin_addr.s_addr = inet_addr(trackersocket1.ip)
	    address.sin_port = htons(trackersocket1.port); 
	       
	    // Forcefully attaching socket to the port 8080 
	    if (bind(server_fd, (struct sockaddr *)&address,  
	                                 sizeof(address))<0) 
	    { 
	        perror("bind failed"); 
	        exit(EXIT_FAILURE); 
	    } 
	    if (listen(server_fd, 3) < 0) 
	    { 
	        perror("listen"); 
	        exit(EXIT_FAILURE); 
	    } 
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
	                       (socklen_t*)&addrlen))<0) 
	    { 
	        perror("accept"); 
	        exit(EXIT_FAILURE); 
	    } 
	    while(1)
	    {
	       	char buffer[1024] = {0}; 
	        valread = read( new_socket , buffer, 1024); 
	        printf("Server get Data from Client : %s\n",buffer );
	    
	        string msg;
	        cout<<"\nEnter msg to send : ";
	        cin>>msg;
	        char *serverreply = new char[msg.length() + 1];
			strcpy(serverreply, msg.c_str());
			//cout<<"serverreply : "<<serverreply<<endl;
	        send(new_socket , serverreply , strlen(serverreply) , 0 ); 
	        
	        printf("Reply message sent from server\n"); 
	     }
    }

}
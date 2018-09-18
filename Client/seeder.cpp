#include "clientheader.h"
#include "socket.cpp"

void *seederservice(void *socket_desc)
{
	int new_socket = *(int*)socket_desc;
	char buffer[1024] = {0}; 
    read( new_socket , buffer, 1024); 
    printf("Seeder gets Data from Clients : %s\n",buffer );
    string short_hash=string(buffer);
    string file_path=filehashmap[short_hash];
    char *serverreply = new char[file_path.length() + 1];
    strcpy(serverreply, file_path.c_str());
    send(new_socket , serverreply , strlen(serverreply) , 0 ); 
    printf("Reply message sent from seeder\n"); 
    close(new_socket);
	return socket_desc;
}

void *seederserverservice(void *socket_desc)
{
    string cli_socket = *(string*)socket_desc;
    socketclass cserversocket;
    pthread_t thread_id;
    cserversocket.setsocketdata(cli_socket);

        int server_fd, new_socket; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address); 

        // Creating socket file descriptor 
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
            perror("socket failed in seeder"); 
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
        address.sin_addr.s_addr = inet_addr(cserversocket.ip);
        address.sin_port = htons(cserversocket.port); 
           
        // Forcefully attaching socket to the port 8080 
        if (bind(server_fd, (struct sockaddr *)&address,  
                                     sizeof(address))<0) 
        { 
            perror("bind failed in seeder"); 
            exit(EXIT_FAILURE); 
        } 
        if (listen(server_fd, 10) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        }

        while(1)
        {
                if((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
                { 
                    perror("Error in accept connection in seeder"); 
                    exit(EXIT_FAILURE); 
                }
                cout<<"******Connection accepted in seeder *******"<<endl;
                if( pthread_create( &thread_id , NULL ,  seederservice , (void*)&new_socket) < 0)
                {
                    perror("\ncould not create thread in seeder\n");
                }
                if (new_socket < 0)
                {
                    perror("accept failed in seeder");
                }
        }

        return socket_desc;
}
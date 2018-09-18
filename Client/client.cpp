#include "clientheader.h"
#include "socket.cpp"

// #define PORT 7000 


int main(int argc, char const *argv[]) 
{ 
    socketclass clientsocket;
    socketclass trackersocket1;
    socketclass trackersocket2;
    if (argc != 5)
    {
        cout << "Invalid Argument !!!" << endl;

    }
    else
    {
        string clientsocketstr=string(argv[1]);
        string trackersocket1str=string(argv[2]);
        string trackersocket2str=string(argv[3]);
        clientsocket.setsocketdata(clientsocketstr);
        trackersocket1.setsocketdata(trackersocket1str);
        trackersocket2.setsocketdata(trackersocket2str);
        string logfile=string(argv[4]);
        
        cout<<"Client socket : "<<clientsocket.ip<<" : "<<clientsocket.port<<endl;
        cout<<"Tracker 1 socket: "<<trackersocket1.ip<<" : "<<trackersocket1.port<<endl;
        cout<<"Tracker 2 socket: "<<trackersocket2.ip<<" : "<<trackersocket2.port<<endl;

        pthread_t cserverid;
        if( pthread_create(&cserverid , NULL ,  seederserverservice , (void*)&clientsocketstr) < 0)
        {
            perror("\ncould not create thread\n");
        }
                 

        //struct sockaddr_in address; 
        int sock = 0; 
        struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        { 
            printf("\n Socket creation error \n"); 
            return -1; 
        } 
       
        memset(&serv_addr, '0', sizeof(serv_addr)); 
       
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons(trackersocket1.port);
        //serv_addr.sin_addr.s_addr = inet_addr(trackersocket1.ip);
           
        //Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, trackersocket1.ip, &serv_addr.sin_addr)<=0)  
        { 
            printf("\nClient File  : Invalid address/ Address not supported \n"); 
            return -1; 
        } 
       
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        { 
            printf("\nConnection Failed \n"); 
            return -1; 
        }
        cout<<"******Connection stablished successfully !!!"<<endl;

        while(1)
        {

            string strcmd;
            char *mtorrentfilepath;
            cout<<"Enter the command : "<<endl;
            getline(cin >> ws, strcmd);

            // cout<<"Command from cient : "<<strcmd;

            vector <string> tokens; 
            stringstream check1(strcmd); 
            string intermediate; 
              
            // Tokenizing w.r.t. space ' ' 
            while(getline(check1, intermediate, ' ')) 
            { 
                tokens.push_back(intermediate); 
            }

            //int errorflag=0; 
            string complexdata;
            
            if(tokens[0]=="share")
            {
                complexdata=executeshareclient(tokens,clientsocketstr,trackersocket1str,trackersocket1str);
                if(complexdata=="-1")
                    continue;
            }
            else if(tokens[0]=="get")
            {
                cout<<"GET command exe in client side"<<endl;
                complexdata=executegetclient(tokens);
                if(complexdata=="-1")
                    continue;
            }
            else if(tokens[0]=="remove")
            {
                mtorrentfilepath = new char[tokens[1].length() + 1];
                strcpy(mtorrentfilepath, tokens[1].c_str());
                cout<<"REMOVE command exe in client side"<<endl;
                complexdata=executeremoveclient(tokens,clientsocketstr);
                if(complexdata=="-1")
                    continue;
            }

                // while(1)
                // {

                    // string msg;
                    // cout<<"\nEnter msg to send : ";
                    // cin>>msg;
                    char *clientreply = new char[complexdata.length() + 1];
                    strcpy(clientreply, complexdata.c_str());
                    //cout<<"clientreply : "<<clientreply<<endl;
                    send(sock , clientreply , strlen(clientreply) , 0 ); 
                    
                    printf("client msg message sent\n"); 
                    
                    char buffer[1024] = {0}; 
                    read( sock , buffer, 1024); 
                    printf("client got reply from server: %s\n",buffer );


                    string responce=string(buffer);
                    if(responce=="Record removed successfully !!!")
                    {
                         if(remove(mtorrentfilepath) != 0 )
                            perror( "\nError deleting mtorrent file\n");
                    }
                // }

        }
    }
        
    return 0; 
} 
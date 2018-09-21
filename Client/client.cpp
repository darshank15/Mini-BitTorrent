#include "clientheader.h"
#include "socket.cpp"

#define CSIZE 512*1024

const char *logpath;
vector<string> listdownload;
vector<pair<string,string>> clientfilepath;

int dofiletransfering(string replydata,string destpath)
{
    writelog("dofiletransfering called : "+replydata);
    stringstream check1(replydata); 
    string intermediate;
    while(getline(check1, intermediate, '@')) 
    { 
        stringstream check2(intermediate);
        vector <string> clientsocketvc;
        string subintermediate;
        while(getline(check2, subintermediate, '#')) 
        {
            clientsocketvc.push_back(subintermediate);
        }
        clientfilepath.push_back({clientsocketvc[0],clientsocketvc[1]});
    }
    writelog("******Avialable seeders for downloading file*******");
    for(unsigned int i=0;i<clientfilepath.size();i++)
    {
        writelog(clientfilepath[i].first +"--->"+clientfilepath[i].second);
    }
    writelog("****************************************************");
    socketclass csocket;
    csocket.setsocketdata(clientfilepath[0].first);
    string filepath=clientfilepath[0].second;
        int sock = 0; 
        struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        { 
            printf("\n Socket creation error \n"); 
            return -1; 
        } 
       
        memset(&serv_addr, '0', sizeof(serv_addr)); 
       
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons(csocket.port);
           
        //Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, csocket.ip, &serv_addr.sin_addr)<=0)  
        { 
            printf("\nClient File  : Invalid address/ Address not supported \n"); 
            return -1; 
        } 
       
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        { 
            printf("\nConnection Failed in Client\n"); 
            return -1; 
        }

        writelog("******Connection  established for file transffered !!!");

        char *d_path = new char[destpath.length() + 1];
        strcpy(d_path, destpath.c_str());

        ofstream myfile(d_path, ofstream::binary);

        char *clientreply = new char[filepath.length() + 1];
        strcpy(clientreply, filepath.c_str());
        send(sock , clientreply , strlen(clientreply) , 0 );
        int n;
        do{

            char *buffer = new char[CSIZE];
            n=read( sock , buffer, CSIZE);
            myfile.write(buffer,n);

        }while(n>0);
       
        myfile.close();

        return 1;
}

int main(int argc, char const *argv[]) 
{ 

    socketclass clientsocket;
    socketclass trackersocket1;
    socketclass trackersocket2;
    if (argc != 5)
    {
        cout << "Invalid Argument in client!!!" << endl;

    }
    else
    {
        string clientsocketstr=string(argv[1]);
        string trackersocket1str=string(argv[2]);
        string trackersocket2str=string(argv[3]);
        clientsocket.setsocketdata(clientsocketstr);
        trackersocket1.setsocketdata(trackersocket1str);
        trackersocket2.setsocketdata(trackersocket2str);
        logpath=argv[4];
        ofstream myfile(logpath,std::ios_base::out); 
        myfile.close();
        writelog("********new client started *********");
        writelog("Client socket : "+clientsocketstr);
        writelog("Tracker 1 socket : "+trackersocket1str);
        writelog("Tracker 2 socket : "+trackersocket2str);
        // cout<<"Tracker 1 socket: "<<trackersocket1.ip<<" : "<<trackersocket1.port<<endl;
        // cout<<"Tracker 2 socket: "<<trackersocket2.ip<<" : "<<trackersocket2.port<<endl;

        pthread_t cserverid;
        if( pthread_create(&cserverid , NULL ,  seederserverservice , (void*)&clientsocketstr) < 0)
        {
            perror("\ncould not create thread in client side\n");
        }
                 

        //struct sockaddr_in address; 
        int sock = 0; 
        struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        { 
            printf("\n Socket creation error in client side\n"); 
            return -1; 
        } 
       
        memset(&serv_addr, '0', sizeof(serv_addr)); 
       
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons(trackersocket1.port);
           
        //Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, trackersocket1.ip, &serv_addr.sin_addr)<=0)  
        { 
            printf("\nClient File  : Invalid address/ Address not supported \n"); 
            return -1; 
        } 
       
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        { 
            printf("\nConnection Failed in client side\n"); 
            return -1; 
        }
        writelog("******Connection stablished successfully with tracker!!!");

        while(1)
        {

            int getflag=0;
            char *mtorrentfilepath;
            string strcmd,destpath,getcmdmtorrentpath;
           
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
                if(tokens.size()!=3)
                {
                    cout<<"INVALID_ARGUMENTS --- SHARE Command"<<endl;
                    continue;
                }
                writelog("SHARE command exe in client side");
                complexdata=executeshareclient(tokens,clientsocketstr,trackersocket1str,trackersocket2str);
                if(complexdata=="-1")
                    continue;
                else
                    cout<<"SUCCESSFULLY SHARED"<<endl;
            }
            else if(tokens[0]=="get")
            {
                if(tokens.size()!=3)
                {
                    cout<<"INVALID_ARGUMENTS --- GET Command"<<endl;
                    continue;
                }
                writelog("GET command exe in client side");
                complexdata=executegetclient(tokens);
                destpath=tokens[2];
                getcmdmtorrentpath=tokens[1];
                if(complexdata=="-1")
                    continue;
                else{
                    getflag=1;
                    //cout<<"SUCCESSFULLY GET"<<endl;
                }
            }
            else if(tokens[0]=="remove")
            {
                if(tokens.size()!=2)
                {
                    cout<<"INVALID_ARGUMENTS --- REMOVE Command"<<endl;
                    continue;
                }
                mtorrentfilepath = new char[tokens[1].length() + 1];
                strcpy(mtorrentfilepath, tokens[1].c_str());
                writelog("REMOVE command exe in client side");
                complexdata=executeremoveclient(tokens,clientsocketstr);
                if(complexdata=="-1")
                    continue;        
            }
            else if(tokens[0]=="show_downloads")
            {
                if(listdownload.size()<=0)
                {
                    cout<<"NO DOWNLOADS TILL NOW"<<endl;
                }
                else
                {
                     cout<<"********* DOWNLOADS **********"<<endl;
                    for(unsigned int i=0;i<listdownload.size();i++)
                    {
                        cout<<listdownload[i]<<endl;
                    }
                } 
                continue;
            }
            else{
                cout<<"INVALID COMMAND"<<endl;
                continue;
            }

            char *clientreply = new char[complexdata.length() + 1];
            strcpy(clientreply, complexdata.c_str());
            //cout<<"clientreply : "<<clientreply<<endl;
            send(sock , clientreply , strlen(clientreply) , 0 ); 
            
            writelog("client msg message sent to Tracker"); 
            
            char buffer[1024] = {0}; 
            read( sock , buffer, 1024); 
            writelog("client("+clientsocketstr+")got reply from tracker ===> "+string(buffer));
            cout<<"Server Reply ===> "<<string(buffer)<<endl;

            string responce=string(buffer);

            if(getflag==1)
            {
                int ans=dofiletransfering(responce,destpath);
                if(ans==1)
                {
                    cout<<"FILE SUCCESSFULLY DOWNLOADED"<<endl;
                    listdownload.push_back(destpath);
                    vector<string> temptokens;
                    temptokens.push_back("share");
                    temptokens.push_back(destpath);
                    temptokens.push_back(getcmdmtorrentpath);
                    complexdata=executeshareclient(temptokens,clientsocketstr,trackersocket1str,trackersocket2str);
                    if(complexdata!="-1")
                    {
                        char *clt = new char[complexdata.length() + 1];
                        strcpy(clt, complexdata.c_str());
                        send(sock , clt , strlen(clt) , 0 );
                        char buff[1024] = {0}; 
                        read( sock , buff, 1024);
                    }
                }
                else
                {
                    cout<<"ERROR IN DOWNLOADING FILE"<<endl;
                }

            }

            getflag=0;

            //When Server Send Response for remove command
            if(responce=="Record removed successfully !!!")
            {
                if(remove(mtorrentfilepath) != 0 )
                    perror( "\nError deleting mtorrent file\n");
                else
                     cout<<"SUCCESSFULLY REMOVED"<<endl;
            }


        }
    }
        
    return 0; 
} 
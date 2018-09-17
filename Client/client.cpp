#include "clientheader.h"
#include "socket.cpp"

// #define PORT 7000 
string executeshareclient(vector <string> tokens,string clntsckstr,string trcksck1str,string trcksck2str)
{
    string cmd=tokens[0];
    string fpath=tokens[1];
    string mtpath=tokens[2];
    cout<<"Command  : "<<cmd<<endl;
    cout<<"File path : "<<fpath<<endl;
    cout<<"Mtorrent path : "<<mtpath<<endl;
    char *fp,*tp;
    cout<<"SHARE command exe in client side"<<endl;
    fp = new char[fpath.length() + 1];
    strcpy(fp, fpath.c_str());

    tp = new char[mtpath.length() + 1];
    strcpy(tp, mtpath.c_str());
    string filehash=createTorrentFile(fp,tp,trcksck1str,trcksck2str);
    if(filehash=="-1")
    {
        cout<<"\nError Encounter for creating hash of file : "<<fpath<<endl;
        return "-1";
    }
    cout<<"Long Hash : "<<filehash<<endl; //need to do hash of hash
    char *longhash = new char[filehash.length() + 1];
    strcpy(longhash, filehash.c_str());
    string shorthash=calHashofchunk(longhash,filehash.length(),0);
    //cout<<"ShortHash : "<<shorthash<<endl;
    
    string ans=cmd+"#"+shorthash+"#"+clntsckstr+"#"+fpath;
    cout<<"Complex Data need to send to tracker :"<<ans<<endl;
    return ans;
}

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


        //struct sockaddr_in address; 
        int sock = 0, valread; 
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
                    valread = read( sock , buffer, 1024); 
                    printf("client got reply from server: %s\n",buffer ); 
                // }

        }
    }
        
    return 0; 
} 
#include "trackerheader.h"
#include "../Client/socket.cpp"
//#define PORT 7000 

class trackerdata
{
    public:

        string csocket;
        string cfpath;
        string shash;

        trackerdata()
        {
            csocket="";
            cfpath="";
            shash="";
        }

        trackerdata(string hash,string ipport,string path)
        {
        	shash=hash;
        	csocket=ipport;
        	cfpath=path;
        }
};

map<string,vector<trackerdata>> trackertable;

int main(int argc, char *argv[])
{
    socketclass trackersocket1;
    socketclass trackersocket2;
	if (argc != 5)
    {
        cout << "Invalid Argument !!!" << endl;

    }
    else
    {

        trackersocket1.setsocketdata(string(argv[1]));
        trackersocket2.setsocketdata(string(argv[2]));
        string seederfile=string(argv[3]);
        string logfile=string(argv[4]);

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
	    address.sin_addr.s_addr = inet_addr(trackersocket1.ip);
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
	    	 
	    	string bufdata=string(buffer);
	    	vector <string> tokens1;
            stringstream check2(bufdata); 
            string intermediate1;   
            // Tokenizing w.r.t. space '#' 
            while(getline(check2, intermediate1, '#'))
            { 
                tokens1.push_back(intermediate1); 
            } 
            
            trackerdata td(tokens1[0],tokens1[1],tokens1[2]);
            // string shorthash=tokens1[0];
            // string cleintsocket=tokens1[1];
            // string cleintfilepath=tokens1[2];
            //cout<<shorthash<<"::"<<cleintsocket<<"::"<<cleintfilepath<<endl;
            
        	cout<<td.shash<<"::"<<td.csocket<<"::"<<td.cfpath<<endl;
	        
            trackertable[td.shash].push_back(td);

            vector<trackerdata> vectortd=trackertable[td.shash];
            cout<<vectortd[0].shash<<"++"<<vectortd[0].csocket<<"++"<<vectortd[0].cfpath<<"++"<<endl;

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
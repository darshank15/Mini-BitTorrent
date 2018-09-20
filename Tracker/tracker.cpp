#include "trackerheader.h"
#include "../Client/socket.cpp"
#include "trackerdata.cpp"

using namespace std;
const char* trackerlogpath;
//#define PORT 7000 

char *seederfilep;
map<string,vector<trackerdata>> trackertable;

void writelog(string str)
{
    ofstream myfile(trackerlogpath, std::ios_base::app | std::ios_base::out);  
    myfile<<str<<endl;
    myfile.close();
}

int readseederlist(char *fpath)
{
    ifstream fp(fpath, ifstream::binary);
    if(!fp) 
    {
        cout << "Cannot open seedlist file !!!"<<endl;
        return 0;
    }
    
    string linecontent;

    writelog("Contest of seedlist file : ");
    while (getline(fp, linecontent))
    {   
        string data=string(linecontent);
        vector <string> tokens1;
        stringstream check2(data); 
        string intermediate1;
        while(getline(check2, intermediate1, ' '))
        { 
            tokens1.push_back(intermediate1); 
        }
        trackerdata td1(tokens1[0],tokens1[1],tokens1[2]); 
        trackertable[td1.shash].push_back(td1);
    }
    return 1;
}

int writeseederlist(char *fpath, string data)
{
    ofstream myfile(fpath, std::ios_base::app | std::ios_base::out);  
    myfile<<data<<endl;
    myfile.close();
    return 1;
}

void updateseederlist(char *seederlistfp)
{
    ofstream filep(seederlistfp,std::ios_base::out);  

    for(auto it : trackertable)
    {
        string hs=it.first;
        vector<trackerdata> temptd=it.second;
        for(unsigned int j=0;j<temptd.size();j++)
        {
            string seederlistdata=temptd[j].shash+" "+temptd[j].csocket+" "+temptd[j].cfpath;
            //cout<<seederlistdata<<endl;
            filep<<seederlistdata<<endl;
        }
    }
    filep.close();
}

void printeverything()
{
    writelog("****************seederlist Data**************");
	for(auto it : trackertable)
	{
		string hs=it.first;
		writelog("Data for Hash value : "+hs);
		vector<trackerdata> temptd=it.second;
    	for(unsigned int j=0;j<temptd.size();j++)
    	{
    		writelog(temptd[j].csocket + " -----> " + temptd[j].cfpath);
    	}
	}
    writelog("**********************************************");
}

string executeget(vector <string> tokens1)
{
        string ans;
        string shash=tokens1[1];
        if(trackertable.find(shash) != trackertable.end())
        {
            unsigned int i;
            vector<trackerdata> temptd =trackertable[shash];
            for(i=0;i<temptd.size()-1;i++)
            {
                ans += temptd[i].csocket+"#";
            }
            //cout<<"--->"<<ans<<endl;
            ans += temptd[i].csocket;

            //cout<<"--->"<<ans<<endl;
        }
        else
        {
            ans="No Client found for this File !!!";
        }

        return ans;
}

string executeremove(vector <string> tokens1,string data,char *seederlistfp)
{
        string ans;
        int flag=0;
        int cflag=0;
        string shash=tokens1[1];
        //cout<<"---------->shash : "<<shash<<endl;
        string clsocket=tokens1[2];
        //cout<<"---------->csocket : "<<clsocket<<endl;
        if(trackertable.find(shash) != trackertable.end())
        {
            vector<trackerdata>::iterator it;
            vector<trackerdata> temptd =trackertable[shash];
            int sizeofvector=temptd.size();
            for(it=temptd.begin();it!=temptd.end();it++)
            {
                if((*it).csocket==clsocket)
                {
                    flag=1;
                    // cout<<"---------->hit"<<endl;
                    if(sizeofvector==1)
                    {
                        // cout<<"---------->single entry erase"<<endl;
                        trackertable.erase(shash);
                        break;
                    }
                    else{

                        cflag=1;
                        break;
                    }
                }
            }
            if(cflag==1)
            {
                 temptd.erase(it);
                 trackertable[shash]=temptd; 
            }      
        }
    
        if(flag==1)
        {
            updateseederlist(seederlistfp);
            return "Record removed successfully !!!";
        }
        else{
            return "Record doesn't found in database !!!";
        }
}

string executeshare(vector <string> tokens1,string data,char *seederlistfp)
{
	string ans;
    string seederlistdata=tokens1[1]+" "+tokens1[2]+" "+tokens1[3];
	trackerdata td(tokens1[1],tokens1[2],tokens1[3]);         
	// cout<<td.shash<<"::"<<td.csocket<<"::"<<td.cfpath<<endl;
    if(trackertable.find(td.shash) == trackertable.end())
    {
    	ans="Data Recorded successfully in server for "+data;
    	trackertable[td.shash].push_back(td);
        writeseederlist(seederlistfp,seederlistdata);
    }
    else if(trackertable.find(td.shash) != trackertable.end())
    {
    	vector<trackerdata> temptd=trackertable[td.shash];
    	int flag=0;
    	for(unsigned int j=0;j<temptd.size();j++)
    	{
    		if(temptd[j].csocket == td.csocket)
    		{
    			flag=1;
    			break;
    		}
    	}
    	if(flag)
    	{
    		ans="Server already has these data :  "+data;
    			
    	}
    	else{
    		ans="Data Recorded successfully in server for "+data;
    		trackertable[td.shash].push_back(td);
            writeseederlist(seederlistfp,seederlistdata);
    	}
    }

    return ans;
}


void *serverservice(void *socket_desc)
{
    int new_socket = *(int*)socket_desc;
    while(1)
    {
        char buffer[1024] = {0}; 
        read( new_socket , buffer, 1024); 
        writelog("Tracker get Data from Client : "+string(buffer));
        string clientreplymsg;
         
        string data=string(buffer);
        vector <string> tokens1;
        stringstream check2(data); 
        string intermediate1;   
        // Tokenizing w.r.t. space '#' 
        while(getline(check2, intermediate1, '#'))
        { 
            tokens1.push_back(intermediate1); 
        } 

        if(tokens1[0]=="share")
        {
            writelog("Tracker executing for SHARE command !!!");
            clientreplymsg=executeshare(tokens1,data,seederfilep);
        }
        else if(tokens1[0]=="get")
        {
            writelog("Tracker executing for GET command !!!");
            clientreplymsg=executeget(tokens1);
        }
        else if(tokens1[0]=="remove")
        {
            writelog("Tracker executing for REMOVE command !!!");
            clientreplymsg=executeremove(tokens1,data,seederfilep);
        }

       
        printeverything();
        
        //cout<<"serverreply : "<<string(serverreply)<<endl;
        char *serverreply = new char[clientreplymsg.length() + 1];
        strcpy(serverreply, clientreplymsg.c_str());
        //cout<<"serverreply : "<<serverreply<<endl;
        send(new_socket , serverreply , strlen(serverreply) , 0 ); 
        
        writelog("Reply message sent from Tracker to client"); 

     }
}


int main(int argc, char *argv[])
{
    socketclass trackersocket1;
    socketclass trackersocket2;
    pthread_t thread_id;
	if (argc != 5)
    {
        cout << "Invalid Argument !!!" << endl;

    }
    else
    {
        readseederlist(argv[3]);
        writelog("----------Initially Data in seederfile ------------");
        printeverything();
        trackersocket1.setsocketdata(string(argv[1]));
        trackersocket2.setsocketdata(string(argv[2]));
        seederfilep=argv[3];
        trackerlogpath=argv[4];
        ofstream myfile(trackerlogpath,std::ios_base::out);  
        myfile.close();

    	int server_fd, new_socket; 
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
	    if (listen(server_fd, 10) < 0) 
	    { 
	        perror("listen"); 
	        exit(EXIT_FAILURE); 
	    } 


        while(1)
        {
                if((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
                { 
                    perror("Error in accept connection"); 
                    exit(EXIT_FAILURE); 
                }

                writelog("******Connection accepted in tracker side*******");
                if( pthread_create( &thread_id , NULL ,  serverservice , (void*)&new_socket) < 0)
                {
                    perror("\ncould not create thread\n");
                }
                 
                //Now join the thread , so that we dont terminate before the thread
                //pthread_join( thread_id , NULL);
                // cout<<"New Client created assigned"<<endl;
                 
                if (new_socket < 0)
                {
                    perror("accept failed");
                    //return 1;
                }
        }

    }

}
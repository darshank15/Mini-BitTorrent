#include<bits/stdc++.h>
using namespace std;
class socketclass
{
    public:

        char *ip;
        int port;

        socketclass()
        {
            // ip="";
            port=0;
        }

        // socketclass(string ip1, int port1)
        // {
        //     ip = ip1;
        //     port = port1;
        // }

        void setsocketdata(string sc)
        {
            vector <string> tokens; 
  
            stringstream check1(sc); 
              
            string intermediate; 
              
            // Tokenizing w.r.t. space ' ' 
            while(getline(check1, intermediate, ':')) 
            { 
                tokens.push_back(intermediate); 
            } 
            
            string strip=tokens[0];
            ip = new char[strip.length() + 1];
            strcpy(ip, strip.c_str());
            port=stoi(tokens[1]);
        }
        
};

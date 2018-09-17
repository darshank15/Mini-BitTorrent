class socket
{
	
	string ip;
	string port;

	public:

		socket()
		{
			ip="";
			port="";
		}
		socket(string sc)
		{
			vector <string> tokens; 
  
		    stringstream check1(sc); 
		      
		    string intermediate; 
		      
		    // Tokenizing w.r.t. space ' ' 
		    while(getline(check1, intermediate, ':')) 
		    { 
		        tokens.push_back(intermediate); 
		    } 
		      
		    ip=tokens[0];
		    port=tokens[1];
		}

		socket(string ip1, string port1)
		{
			ip = ip1;
			port = port1;
		}
}
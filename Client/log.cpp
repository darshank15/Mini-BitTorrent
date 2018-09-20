#include "clientheader.h"

void writelog(string str)
{
	ofstream myfile(logpath, std::ios_base::app | std::ios_base::out);  
    myfile<<str<<endl;
    myfile.close();
}
#include "trackerheader.h"

class trackerdata
{
    public:

        string shash;
        string csocket;
        string cfpath;
       
        trackerdata()
        {
            shash="";
            csocket="";
            cfpath="";
        }

        trackerdata(string hash,string ipport,string path)
        {
        	shash=hash;
        	csocket=ipport;
        	cfpath=path;
        }
};
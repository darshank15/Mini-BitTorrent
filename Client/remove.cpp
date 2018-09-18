#include "clientheader.h"

string executeremoveclient(vector <string> tokens,string clntsckstr)
{
    string cmd=tokens[0];
    string mtpath=tokens[1];
    //cout<<"mpath :"<<mtpath<<endl;
    char *tp = new char[mtpath.length() + 1];
    strcpy(tp, mtpath.c_str());
    ifstream fileptr(mtpath, ifstream::binary);
    if (!fileptr)
    {
        cout << "Mtorrent not exist : " << string(mtpath) << endl;
        return "-1";
    }
    int count=4;
    string linecontent;
    while ((count>0) && getline(fileptr, linecontent))
    {   
       count--;
    }
    getline(fileptr, linecontent);
    string mtorrenthash=linecontent;
    //cout<<"mtorrent contains hash is : "<<mtorrenthash<<endl;
    fileptr.close();

    char *longhash = new char[mtorrenthash.length() + 1];
    strcpy(longhash, mtorrenthash.c_str());
    string shorthash=calHashofchunk(longhash,mtorrenthash.length(),0);

    string ans=cmd+"#"+shorthash+"#"+clntsckstr;
    cout<<"Complex Data need to send to tracker from Remove cmd :"<<ans<<endl;
    return ans;

}
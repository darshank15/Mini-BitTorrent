#include "clientheader.h"

map<string,string> filehashmap;

string executeshareclient(vector <string> tokens,string clntsckstr,string trcksck1str,string trcksck2str)
{
    string cmd=tokens[0];
    string fpath=tokens[1];
    string mtpath=tokens[2];
    //cout<<"Command  : "<<cmd<<endl;
    //cout<<"File path : "<<fpath<<endl;
    //cout<<"Mtorrent path : "<<mtpath<<endl;
    char *fp,*tp;
    fp = new char[fpath.length() + 1];
    strcpy(fp, fpath.c_str());

    tp = new char[mtpath.length() + 1];
    strcpy(tp, mtpath.c_str());
    string filehash=createTorrentFile(fp,tp,trcksck1str,trcksck2str);
    if(filehash=="-1")
    {
        cout<<"\nError Encounter for creating hash of file in sharing: "<<fpath<<endl;
        return "-1";
    }
    cout<<"Long Hash : "<<filehash<<endl; //need to do hash of hash
    char *longhash = new char[filehash.length() + 1];
    strcpy(longhash, filehash.c_str());
    string shorthash=calHashofchunk(longhash,filehash.length(),0);
    
    string ans=cmd+"#"+shorthash+"#"+clntsckstr+"#"+fpath;
    cout<<"(SHARE cmd)Complex Data need to send to tracker :"<<ans<<endl;

    filehashmap.emplace(shorthash,fpath);
    return ans;
}
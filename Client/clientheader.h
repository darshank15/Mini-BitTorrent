#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/sha.h>

using namespace std;

string calHashofchunk(char *schunk,int length1,int shorthashflag);
string getFileHash(char *fpath);
string createTorrentFile(char *fpath, char *mtpath,string tcksocket1,string tcksocket2);
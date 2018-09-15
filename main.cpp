#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define CSIZE 150*1024;
using namespace std;

char *filepath, *mtorrentpath;

string calHashofchunk(char *schunk,int length1)
{

    unsigned char hash[SHA_DIGEST_LENGTH];
    char buf[SHA_DIGEST_LENGTH*2];
    SHA1((unsigned char *)schunk, length1, hash);

    //printf("\n*****hash ********");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf((char*)&(buf[i*2]), "%02x", hash[i]);

    //cout<<"hash : "<<buf<<endl;

    string ans;
    for(int i=0;i<20;i++)
    {
    	ans += buf[i];
    }
    return ans;
}

string getFileHash(char *fpath)
{
	string fileHash;
    ifstream file1(fpath, ifstream::binary);

    /* basic sanity check */
    if (!file1)
    {
        cout << "Can't Open file1  : " << string(fpath) << endl;
        return "-1";
    }

    struct stat fstatus;
    stat(fpath, &fstatus);

	// Logic for deviding file1 into chunks
    long int total_size = fstatus.st_size;
    long int chunk_size = CSIZE;

    int total_chunks = total_size / chunk_size;
    int last_chunk_size = total_size % chunk_size;

    if (last_chunk_size != 0) // if file1 is not exactly divisible by chunks size 
    {
        ++total_chunks; // add last chunk to count
    }
    else //when file1 is completely divisible by chunk size 
    {
        last_chunk_size = chunk_size;
    }

    // loop to getting each chunk
    for (int chunk = 0; chunk < total_chunks; ++chunk)
    {
    	int cur_cnk_size;
    	if(chunk == total_chunks - 1)
    		cur_cnk_size = last_chunk_size;
    	else
    		cur_cnk_size = chunk_size;  

        char *chunk_data = new char[cur_cnk_size];
        file1.read(chunk_data,   /* address of buffer start */
                  cur_cnk_size); /* this many bytes is to be read */

        string sh1out=calHashofchunk(chunk_data,cur_cnk_size);
        fileHash = fileHash + sh1out;
    }

    return fileHash;
}

int createTorrentFile(char *fpath, char *mtpath)
{
    string tcksocket1 = "10.42.0.394:4444";
    string tcksocket2 = "10.42.0.666:5555";
    struct stat sb;
    if (stat(fpath, &sb) == -1)
    {
        perror("lstat");
    }
    ofstream myfile;
    myfile.open(string(mtpath));
    myfile << tcksocket1 << endl;
    myfile << tcksocket2 << endl;
    myfile << string(fpath) << endl;
    myfile << sb.st_size << endl;
    string flhash=getFileHash(fpath);
    myfile << flhash << endl;
    myfile.close();
    return 1;
}

int main(int argc, char *argv[])
{

    if (argc == 3)
    {
        filepath = argv[1];
        mtorrentpath = argv[2];
        createTorrentFile(filepath, mtorrentpath);
    }
    else
    {
        cout << "Invalid Argument !!!" << endl;
    }

    return 0;
}

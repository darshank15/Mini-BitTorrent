#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define BUFSIZE 512
using namespace std;

char *filepath, *mtorrentpath;

string calHashofchunk(string schunk)
{

    char *data = new char[schunk.length() + 1];
    strcpy(data, schunk.c_str());

    size_t length = strlen(data);

    unsigned char hash[SHA_DIGEST_LENGTH];
    char buf[SHA_DIGEST_LENGTH*2];
    SHA1((unsigned char *)data, length, hash);

    //printf("\n*****hash ********");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf((char*)&(buf[i*2]), "%02x", hash[i]);

    cout<<"hash : "<<buf<<endl;

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
    ifstream file(fpath, ifstream::binary);

    /* basic sanity check */
    if (!file)
    {
        cout << "Can't Open File  : " << string(fpath) << endl;
        return "-1";
    }

    struct stat fstatus;
    stat(fpath, &fstatus);

	// Logic for deviding file into chunks
    long int total_size = fstatus.st_size;
    long int chunk_size = 3;

    int total_chunks = total_size / chunk_size;
    int last_chunk_size = total_size % chunk_size;

    if (last_chunk_size != 0) // if file is not exactly divisible by chunks size 
    {
        ++total_chunks; // add last chunk to count
    }
    else //when file is completely divisible by chunk size 
    {
        last_chunk_size = chunk_size;
    }

    // loop to getting each chunk
    for (size_t chunk = 0; chunk < total_chunks; ++chunk)
    {
    	size_t cur_cnk_size;
    	if(chunk == total_chunks - 1)
    		cur_cnk_size = last_chunk_size;
    	else
    		cur_cnk_size = chunk_size;  

        vector<char> chunk_data(cur_cnk_size);
        file.read(&chunk_data[0],   /* address of buffer start */
                  cur_cnk_size); /* this many bytes is to be read */


        //cout << "chunk #" << chunk << endl;
        string schunk;
        for (const auto c : chunk_data) /* I like my C++11 extensions */
        {
            schunk += c;
        }
        cout <<"Chunks : "<<schunk << endl;
        string sh1out=calHashofchunk(schunk);
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

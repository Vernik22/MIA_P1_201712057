#ifndef RMDISK_H
#define RMDISK_H
#include <vector>
#include "string"

using namespace std;
class rmdisk
{
    public:
        rmdisk();
        virtual ~rmdisk();
        void ejecutarComandoRmdisk(rmdisk *disco);
        vector<string> split(string str, char pattern);
        void setPath(string s){
            path= s;
        }
        string getPath(){
            return path;
        }
    private:
        string path;
};

#endif // RMDISK_H

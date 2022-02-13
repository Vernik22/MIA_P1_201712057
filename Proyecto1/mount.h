#ifndef MOUNT_H
#define MOUNT_H
#include <string>
#include "estructuras.h"
#include <vector>

using namespace std;
class mount
{
    public:
        mount();
        virtual ~mount();
        void ejecutarComandoMount(mount *part, mount []);
        string generarId(mount *part,mount []);

        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setName(string s){
            name=s;
        }
        string getName(){
            return name;
        }
        void setId(string s){
            idMount=s;
        }
        string getId(){
            return idMount;
        }

    private:
        string path;
        string name;
        string idMount;

};

#endif // MOUNT_H

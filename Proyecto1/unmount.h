#ifndef UNMOUNT_H
#define UNMOUNT_H
#include <vector>
#include "mount.h"

using namespace std;

class unmount
{
    public:
        unmount();
        virtual ~unmount();
        void ejecutarComandoUnmount(unmount *part , vector<mount>& v);

        void setId(string s){
            idMount = s;
        }
        string getId(){
            return idMount;
        }

    private:
        string idMount;
};

#endif // UNMOUNT_H

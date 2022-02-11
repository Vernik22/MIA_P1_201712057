#ifndef MKFS_H
#define MKFS_H


class mkfs
{
    public:
        mkfs();
        virtual ~mkfs();
        void ejecutarComandoMkfs(mkfs *fs, mount []);

        void setId(string s){
            idMount = s;
        }
        string getId(){
            return idMount;
        }
        void setType(string s){
            type = s;
        }
        string getType(){
            return type;
        }
        void setFs(string s){
            fsT = s;
        }
        string getFs(){
            return fsT;
        }

    private:
        string idMount;
        string type;
        string fsT;
};

#endif // MKFS_H

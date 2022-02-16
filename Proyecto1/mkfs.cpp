#include "mkfs.h"
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <math.h>
#include <time.h>

mkfs::mkfs()
{
    //ctor
}

mkfs::~mkfs()
{
    //dtor
}

void mkfs::ejecutarComandoMkfs(mkfs *fs, mount paMoun[])
{
    cout<<"\n************Ejecutar MKFS************\n"<<endl;

    if(existeIdMount(fs->getId(),paMoun))
    {

        MBR mbrTemp;
        FILE *arch;
        string pathD = "";
        string nombreParticion = "";
        for(int i =0; i<100; i++)
        {
            if(fs->getId() == paMoun[i].getId())
            {
                pathD = paMoun[i].getPath();
                nombreParticion = paMoun[i].getName();
                break;

            }

        }
        arch = fopen(pathD.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);
            fclose(arch);
            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathD,nombreParticion,tamParticion, iniPart);

            SupB superBloque;
            Inodo inodoTemp;
            BCarpeta bCarpetaRaiz;
            Content contenidoUsuarios; //nombre e inodo del BCarpeta
            //BApun apuntadorCarpeta;
            BArchivo bloque; //contenido del archivo users.txt
            Bitacora bitacora;


            int numero_bloques_por_inodo = 12 + 16 + (16*16) + (16*16*16);
            int capacidad_carpeta = numero_bloques_por_inodo * 4 - 2;
            int capacidad_archivo = numero_bloques_por_inodo * 64;

            //tamaño_particion = sizeof(SupB)+ n +n*sizeof(Bitacora) + 3*n+n*sizeof(Inodo) + 3*n*sizeof(BArchivo) numero_estructuras =floor(n);
            int n ;
            if(fs->getFs()=="3fs")
            {
                //Ext3
                //n = (tamParticion - sizeof(SupB))/(1 + 3 + sizeof(Inodo) + 3*sizeof(BArchivo)+sizeof(Bitacora));
                //floor(n);
                n = (tamParticion - sizeof(SupB)) / (4 + sizeof(Inodo) + sizeof(Bitacora) + 3 * 64); //voy a usar esta
            }
            else
            {
                //Ext2
                n = (tamParticion - sizeof(SupB))/(1 + 3 + sizeof(Inodo) + 3*sizeof(BArchivo));
                floor(n);
                //n = (tamParticion - sizeof(SupB)) / (4 + sizeof(Inodo) + 3 * 64); //voy a usar esta
            }

            //No estructuras
            int cantidadAVD = n;
            int cantidadDD = n; //cantidadDD  cantidad de carpetas
            int cantidadInodos = n;
            int cantidadBloques = cantidadInodos * 3;
            int Bitacoras = n;


            //Bitmaps
            //int inicioBitmapAVD = iniPart + sizeof(SupB);
            int inicioAVD = inicioBitmapAVD + cantidadAVD;
            int inicioBitmapDD = inicioAVD + (sizeof(AVD)*cantidadAVD);
            int inicioDD = inicioBitmapDD + cantidadDD;
            int inicioBitmapInodo = iniPart + sizeof(SupB);
            int inicioInodo = inicioBitmapBloque + cantidadBloques;
            int inicioBitmapBloque = inicioBitmapInodo + cantidadInodos;
            int inicioBloque = inicioInodo + cantidadInodos;
            if(fs->getFs()=="3fs")
            {
                int inicioBitacora = iniPart + sizeof(SupB);
                int inicioCopiaSB = inicioBitacora + (sizeof(Bitacora)*Bitacoras);
                inicioBitmapInodo = inicioBitacora + sizeof(SupB);
                inicioInodo = inicioBitmapBloque + cantidadBloques;
                inicioBitmapBloque = inicioBitmapInodo + cantidadInodos;
                inicioBloque = inicioInodo + cantidadInodos;
            }


            cout<< "-----------------------------"<<endl;
            cout<<"pesoSB "<< sizeof(SupB) <<" pesoAvd "<<sizeof(AVD)<< " pesoDD "<<sizeof(BCarpeta)<< " pesoInodos "<<sizeof(Inodo)<< " pesoBloque "<<sizeof(BArchivo)<<" pesoBitacora "<<sizeof(Bitacora)<<endl;
            cout<< "-----------------------------"<<endl;
            cout<<"CantidadAVD "<<cantidadAVD<<" CantidadDD "<<cantidadDD<<" CantidadInodos "<<cantidadInodos<<" CantidadBloques "<<cantidadBloques<<" CantidadBitacoras "<<Bitacoras<<endl;

            cout<<n<<endl;

            //inicializando SuperBloque
            if(fs->getFs()=="3fs")
            {
                //ext3
                superBloque.s_filesystem_type = 3;
            }
            else  //ext2
            {
                superBloque.s_filesystem_type = 2;
            }
            superBloque.s_inodes_count = cantidadInodos;
            superBloque.s_blocks_count = cantidadBloques;
            superBloque.s_free_blocks_count = cantidadBloques;
            superBloque.s_free_inodes_count = cantidadInodos;
            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            string fecha = asctime(timeinfo);
            _mTime fcreacion;
            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
            superBloque.s_mtime = fcreacion;
            superBloque.s_umtime = fcreacion;
            superBloque.s_mnt_count = 1;
            superBloque.s_magic = 0xEF53;
            superBloque.s_inode_size = sizeof(Inodo);
            superBloque.s_block_size = sizeof(BArchivo);
            superBloque.s_first_ino = inicioInodo;
            superBloque.s_first_blo = inicioBloque;
            superBloque.s_bm_inode_start = inicioBitmapInodo;
            superBloque.s_bm_block_start = inicioBitmapBloque;
            //ahora escribir el superbloque en la particion
            arch = fopen(pathD.c_str(),"rb+");
            fseek(arch,iniPart,SEEK_SET);
            fwrite(&superBloque,sizeof(SupB),1,arch);
            int8_t llenar = 0;
            if(fs->getFs()=="3fs") //ext3
            {
                fseek(arch,inicioBitacora,SEEK_SET);
                bitacora.Size = -1;
                //lleno de bitacoras vacias el espacio de la bitacora
                for(int i=0; i<Bitacoras; i++)
                {
                    fwrite(&bitacora,sizeof(Bitacora),1,arch);
                }

            }
            //lleno de ceros el espacio del bitmap de inodos
            fseek(arch,inicioBitmapInodo,SEEK_SET);
            for(int i = 0; i < cantidadInodos; i++)
            {
                fwrite(&llenar,sizeof(llenar),1,arch);
            }
            //lleno de ceros el espacio del bitmap de bloques
            fseek(arch, inicioBitmapBloque,SEEK_SET);
            for(int i=0; i< cantidadBloques; i++)
            {
                fwrite(&llenar, sizeof(llenar),1,arch);
            }
            //escribir tabla de inodos
            fseek(arch,inicioInodo,SEEK_SET);
            inodoTemp.i_block = -1;
            inodoTemp.i_atime = fcreacion;
            inodoTemp.i_ctime = fcreacion;
            inodoTemp.i_mtime = fcreacion;
            for(int i=0; i<cantidadInodos; i++)
            {
                fwrite(&inodoTemp,sizeof(Inodo),1,arch);
            }
            //escribir los bloques
            fseek(arch, inicioBloque, SEEK_SET);
            for(int i=0; i<cantidadBloques; i++)
            {
                fwrite(&bloque,sizeof(BArchivo),1,arch);
            }


            fclose(arch);

            //creo el directorio raiz
            crearRaiz();
            cout<<"NO. Estructuras: " << n<<endl;
            cout<<"Particion a formatear: "<<nombreParticion<<endl;
            cout<<"Tamaño Particion: "<<tamParticion<<endl;
            cout<<"Fecha: "<<mbrTemp.mbr_fecha_creacion<<endl;

        }
        else
        {
            cout<<"Error, no se puede acceder al disco"<<endl;
        }


        fclose(arch);

    }
    else
    {
        cout<<"ERROR: El ID de la particion no existe, o no esta montada"<<endl;
    }

}

void mkfs::crearRaiz(){

}

bool mkfs::existeIdMount(string idB,mount paMoun[])
{
    for(int i =0; i<100; i++)
    {
        if(idB == paMoun[i].getId())
        {
            return true;

        }

    }
    return false;
}

void mkfs::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
{
    Particion parts[4];
    parts[0] = mbrTemp.mbr_partition_1;
    parts[1] = mbrTemp.mbr_partition_2;
    parts[2] = mbrTemp.mbr_partition_3;
    parts[3] = mbrTemp.mbr_partition_4;
    bool noEncontrada = true;

    if(mbrTemp.mbr_partition_1.part_name == nombrePart|| mbrTemp.mbr_partition_2.part_name == nombrePart|| mbrTemp.mbr_partition_3.part_name == nombrePart|| mbrTemp.mbr_partition_4.part_name == nombrePart)
    {
        for(int i = 0; i<4; i++)
        {
            if(parts[i].part_name == nombrePart)
            {
                tamPart = parts[i].part_size;
                iniPart = parts[i].part_start;
                noEncontrada = false;
                break;
            }
        }
    }
    if(noEncontrada)
    {
        EBR ebrExTemp;
        FILE *arch;
        arch = fopen(pathD.c_str(), "rb+");
        for(int i=0; i<4; i++) // encuentra la particion extendida para revisar dentro de sus particiones logicas
        {
            if(parts[i].part_type == 'E')
            {
                fseek(arch,parts[i].part_start,SEEK_SET);
                fread(&ebrExTemp, sizeof(EBR),1,arch); //partExTemp guarda el EBR de la particion E
                break;
            }
        }
        while(ebrExTemp.part_next != -1) //revisa hasta la ultima particion logica o hasta que la encuentre
        {
            if(ebrExTemp.part_name == nombrePart)
            {
                tamPart = ebrExTemp.part_size;
                iniPart = ebrExTemp.part_start;
                break;
            }
            fseek(arch, ebrExTemp.part_next, SEEK_SET);
            fread(&ebrExTemp, sizeof(EBR), 1, arch);
        }

        fclose(arch);
    }
}

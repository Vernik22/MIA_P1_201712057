#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <string>

using namespace std;
class estructuras
{
public:
    estructuras();
    virtual ~estructuras();

protected:

private:
};
//Estructuras para los discos y comandos
//structs para comandos entrantes
typedef struct
{
    string Nombre;
    string valor;
} Propiedad; //Para las propiedades que vienen en comandos

typedef struct{
    string Nombre;
    Propiedad propiedades[15];
}Comando; //las propiedades que traen los comandos

//estructura de tiempo
typedef struct{
    char mbr_fecha_creacion[20];
}_mTime;


//Estructuras para particiones y discos
typedef struct{
    char part_status;
    char part_type; //si es primaria o extendida P o E
    char part_fit;  //ajuste de la particion: B, F, W :: Best, First, Worst
    int part_start;
    int part_size; //tamaño de la particion en bytes
    char part_name[16];
}Particion;

//struct para el mbr
typedef struct{
    int mbr_tamano;
    _mTime mbr_fecha_creacion;
    int mbr_dsk_signature; //numero random, identifica de forma unica a cada disco
    char dsk_fit; //Tipo de ajuste del disco para colocar las particiones, B,F,W ::Best, First, Worst
    Particion mbr_partition_1;
    Particion mbr_partition_2;
    Particion mbr_partition_3;
    Particion mbr_partition_4;
}MBR;

//struct para el ebr
typedef struct{
    char part_status;
    char part_fit; //Tipo de ajuste de la particion, B,F,W ::Best, First, Worst
    int part_start;
    int part_size;
    int part_next; //Byte en el que esta el proximo EBR. -1 si no hay siguiente
    char part_name[16];
}EBR;

//estructura para el super bloque
typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    _mTime s_mtime; //time
    _mTime s_umtime; //time
    int s_mnt_count;
    int s_umnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino; //Primer inodo libre
    int s_first_blo; //Primer bloque libre
    int s_bm_inode_start; //guardara el inicio del bitmap de inodos
    int s_bm_block_start; //guarda el inicio del bitmap de bloques
    int s_inode_start;//guarda donde empiezan los inodos
    int s_block_start;//guarda donde empiezan los bloques
    /*
    int s_inode_start;
    int s_blockAp_start;
    int s_blockAr_start;
    int s_blockC_start;
    */
}SupB;

//struct para el inodo
typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    _mTime i_atime; //ultima vez que se leyo sin modificar
    _mTime i_ctime; //fecha en la que se creo el inodo
    _mTime i_mtime; //ultima vez que se modifico
    int i_block[15]; //cantidad de bloques que hay; apunta hacia el bloque apuntador que tiene los 16 apuntadores
    char i_type; //indica si es carpeta o archivo :: 1=Archivo  0=Carpeta
    //int i_array_bloques[4];
    int i_perm;
}Inodo;

typedef struct{ //arregloDD
    char b_name[12];
    int b_inodo; //apuntador hacie un inodo asociado al archivo/carpeta
}Content;

//struct para el bloque de carpeta [DD]
typedef struct{
    Content b_content[4]; //array con el contenido de la carpeta
}BCarpeta;

//struct para el bloque de archivos [Bloque]
typedef struct{
    char b_content[64];
}BArchivo;

//struct para el bloque de APUNTADORES
typedef struct{
    Content b_apuntadores[4]; //array con los apuntadores a bloques(archivo o carpeta)
}BApun;

typedef struct{
    char Log_tipo_operacion[19];
    char Log_tipo;
    char Log_nombre[35];
    char Log_Contenido[25];
    char Log_fecha[19];
    int Size;
}Bitacora;

typedef struct{
    string nombreUsuario;
    string idPartMontada;
}User;

#endif // ESTRUCTURAS_H

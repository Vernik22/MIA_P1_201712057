#include "manejador.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stdio.h>

//comandos
#include "mkdisk.h"
#include "rmdisk.h"
#include "mkfs.h"
#include "fdisk.h"
#include "login.h"
#include "mkgrp.h"
#include "mkusr.h"
#include "estructuras.h"
#include "reporte.h"
#include "unmount.h"
#include "chmod.h"
#include "mkfile.h"
#include "mkdir.h"
#include "commv.h"

manejador::manejador()
{
    //ctor
    setHayInicioSesion(false);
}

manejador::~manejador()
{
    //dtor
}

void manejador::leerTexto(string data, mount DisksMount[])
{
    vector<Comando> listaComandos;
    vector<string> lineasComando = split(data, '\n');
    Comando c;
    int sizeVec1 = lineasComando.size();
    for(int i = 0; i<sizeVec1 ; i++)
    {
        string esComentario = lineasComando[i];
        if (esComentario[0] != '#')
        {
            string comando = lineasComando[i];
            // lo separo por espacios por ejemplo: mkdisk -path size
            vector<string> propiedades = split(comando, ' ');
            string nombreComando = propiedades[0];
            //lo convierto a minusculas
            std::for_each(nombreComando.begin(), nombreComando.end(), [](char &c)
            {
                c = ::tolower(c);
            });
            c.Nombre = nombreComando;
            Propiedad propiedadesTemp[15];
            for(int f = 0; f < 15; f++)
            {
                propiedadesTemp[f].Nombre = "|";

            }
            int sizeVecCom = propiedades.size();
            for(int j = 1; j<sizeVecCom; j ++)
            {
                if(propiedades[j]=="" || propiedades[j]== " "||propiedades[j]== "#")
                {
                    continue;
                }
                else
                {
                    if(strstr(propiedades[j].c_str(), "=") != NULL)
                    {

                        if(strstr(propiedades[j].c_str(), "#") != NULL)
                        {
                            vector<string> quitComen = split(propiedades[j], '#');
                            vector<string> valor_propiedad_Comando = split(quitComen[0], '=');
                            propiedadesTemp[j - 1].Nombre = valor_propiedad_Comando[0];
                            propiedadesTemp[j - 1].valor = valor_propiedad_Comando[1];
                        }
                        else if(strstr(propiedades[j].c_str(), "\"") != NULL)
                        {
                            vector<string> valor_propiedad_Comando = split(propiedades[j], '=');
                            propiedadesTemp[j - 1].Nombre = valor_propiedad_Comando[0];
                            propiedadesTemp[j - 1].valor = valor_propiedad_Comando[1];
                            for(int f = j+1; f<sizeVecCom; f ++)
                            {
                                if(strstr(propiedades[f].c_str(), "\"") != NULL){
                                     propiedadesTemp[j - 1].valor += " "+ propiedades[f];
                                    break;

                                }else{
                                    propiedadesTemp[j - 1].valor += " "+ propiedades[f];

                                }
                            }

                        }

                        else
                        {
                            vector<string> valor_propiedad_Comando = split(propiedades[j], '=');
                            propiedadesTemp[j - 1].Nombre = valor_propiedad_Comando[0];
                            propiedadesTemp[j - 1].valor = valor_propiedad_Comando[1];

                        }
                    }else if(propiedades[j]=="-r" || propiedades[j]=="-R")
                        {
                            propiedadesTemp[j - 1].Nombre = propiedades[j];

                        }
                        else if(propiedades[j]=="-p" || propiedades[j]=="-P")
                        {
                            propiedadesTemp[j - 1].Nombre = propiedades[j];

                        }

                }
            }
            for (int f = 0; f < 15; f++)
            {
                if(propiedadesTemp[f].Nombre != "|")
                {
                    c.propiedades[f] = propiedadesTemp[f];

                }
            }
            //agregando el comando a la lista de comandos
            listaComandos.push_back(c);

        }
        else
        {
            cout<<"Es comentario"<<endl;
        }
    }
    listaComandosValidos(listaComandos, DisksMount);

}

void manejador::listaComandosValidos(vector<Comando> &listaComandos, mount DisksMount[])
{
    //verificar si ya hay un asesion iniciada en el comando LOGIN y guardar el nombre del usuario y el id de la particion
    int lis = listaComandos.size();
    if(lis != 0)
    {
        for(int i =0; i < lis ; i++)
        {
            Comando comandoTemp;
            comandoTemp = listaComandos[i];
            string nombreComando = comandoTemp.Nombre;

            //leer todos los comandos
            if (nombreComando == "mkdisk")
            {
                bool parametrosValidos = true;
                bool flagFit = true;  //bandera para este parametro que es opcional
                bool flagUnit = true; //bandera para este parametro que es opcional
                bool flagSize = true; //bandera para este parametro que es obligatorio
                bool flagPath = true; //bandera para este parametro que es obligatorio

                mkdisk *jdisco = new mkdisk();
                for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-size")
                    {
                        int s = stoi(prop.valor);
                        flagSize = false;
                        if(s>0)
                        {
                            jdisco->setSize(s);

                        }
                        else
                        {
                            cout<<"ERROR: tamano erroneo, es negativo o cero"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }
                    }
                    else if (nombreProp == "-fit")
                    {
                        string var1 = prop.valor;
                        std::for_each(var1.begin(), var1.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });
                        flagFit = false;
                        if(var1 =="bf" || var1 =="ff"||var1 =="wf")
                        {
                            jdisco->setFit(var1);

                        }
                        else
                        {
                            cout<<"ERROR: fit erroneo, no soportado"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }

                    }
                    else if (nombreProp == "-unit")
                    {
                        flagUnit = false;
                        if(prop.valor== "m"||prop.valor== "M"||prop.valor== "k"||prop.valor== "K")
                        {
                            jdisco->setUnit(prop.valor);

                        }
                        else
                        {
                            cout<<"ERROR: unidad no soportada"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }
                    }
                    else if (nombreProp == "-path")
                    {
                        if(strstr(prop.valor.c_str(), "\"")!=NULL)
                        {
                            vector<string> conc = split(prop.valor, '"');
                            jdisco->setSPath(conc[1]);
                            cout<<conc[1]<<endl;
                        }
                        else
                        {
                            jdisco->setSPath(prop.valor);
                        }

                        flagPath = false;
                    }

                }
                if (flagFit) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    jdisco->setFit("FF");
                }
                if (flagUnit) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    jdisco->setFit("M");
                }
                if(flagSize == false && flagPath == false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    jdisco->ejecutarComandoMkdisk(jdisco);
                }
            }
            else if (nombreComando == "rmdisk")
            {
                bool parametrosValidos = true;
                bool flagPath = true; //bandera para este parametro que es obligatorio
                rmdisk *discoaRem = new rmdisk();

                for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-path")
                    {
                        if(strstr(prop.valor.c_str(), "\"")!=NULL)
                        {
                            vector<string> conc = split(prop.valor, '"');
                            discoaRem->setPath(conc[1]);
                            cout<<conc[1]<<endl;
                        }
                        else
                        {
                            discoaRem->setPath(prop.valor);
                        }

                        flagPath = false;
                    }

                }
                if(flagPath == false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    discoaRem->ejecutarComandoRmdisk(discoaRem);
                }

            }
            else if (nombreComando == "fdisk")
            {
                bool parametrosValidos = true;
                bool flagFit = true;  //bandera para este parametro que es opcional
                bool flagUnit = true; //bandera para este parametro que es opcional
                bool flagType = true;
                bool flagAdd = true;
                bool flagDel = true;


                bool flagName = true;
                bool flagSize = true; //bandera para este parametro que es obligatorio
                bool flagPath = true; //bandera para este parametro que es obligatorio
                fdisk *formDisk = new fdisk();

                for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-size")
                    {
                        int s = stoi(prop.valor);
                        flagSize = false;
                        if(s>0)
                        {
                            formDisk->setSize(s);

                        }
                        else
                        {
                            cout<<"ERROR: tamano erroneo, es negativo o cero"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }
                    }
                    else if (nombreProp == "-unit")
                    {
                        flagUnit = false;
                        if(prop.valor== "m"||prop.valor== "M"||prop.valor== "k"||prop.valor== "K"||prop.valor== "b"||prop.valor== "B")
                        {
                            formDisk->setUnit(prop.valor);

                        }
                        else
                        {
                            cout<<"ERROR: unidad no soportada"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }
                    }
                    else if (nombreProp == "-path")
                    {
                        if(strstr(prop.valor.c_str(), "\"")!=NULL)
                        {
                            vector<string> conc = split(prop.valor, '"');
                            formDisk->setPath(conc[1]);
                            cout<<conc[1]<<endl;
                        }
                        else
                        {
                            formDisk->setPath(prop.valor);
                        }

                        flagPath = false;
                    }
                    else if (nombreProp == "-type")
                    {
                        flagUnit = false;
                        if(prop.valor== "p"||prop.valor== "P"||prop.valor== "e"||prop.valor== "E"||prop.valor== "l"||prop.valor== "L")
                        {
                            formDisk->setType(prop.valor);

                        }
                        else
                        {
                            cout<<"ERROR: tipo no soportada"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }
                    }
                    else if (nombreProp == "-fit")
                    {
                        string var1 = prop.valor;
                        std::for_each(var1.begin(), var1.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });
                        flagFit = false;
                        if(var1 =="bf" || var1 =="ff"||var1 =="wf")
                        {
                            formDisk->setFit(var1);

                        }
                        else
                        {
                            cout<<"ERROR: fit erroneo, no soportado"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }

                    }
                    else if (nombreProp == "-delete")
                    {
                        string var1 = prop.valor;
                        std::for_each(var1.begin(), var1.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });
                        flagDel = false;
                        if(var1 =="fast" || var1 =="full")
                        {
                            formDisk->setDel(var1);

                        }
                        else
                        {
                            cout<<"ERROR: tipo del erroneo, no soportado"<<endl;
                            parametrosValidos = true;
                            flagSize = true;
                            break;
                        }

                    }
                    else if (nombreProp == "-name")
                    {
                        flagName = false;

                        formDisk->setName(prop.valor);

                    }
                    else if (nombreProp == "-add")
                    {
                        flagAdd = false;
                        int s = stoi(prop.valor);
                        formDisk->setAdd(s);

                    }

                }

                if (flagFit) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    formDisk->setFit("WF");
                }
                if (flagUnit) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    formDisk->setUnit("K");
                }
                if (flagType) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    formDisk->setType("P");
                }

                if( flagPath == false && flagName ==false && flagSize== false)
                {
                    parametrosValidos = false;

                }
                else if(flagPath == false && flagName ==false && flagDel == false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    if(flagAdd == true && flagDel == true)
                    {
                        formDisk->ejecutarComandoFdisk(formDisk);

                    }
                    else if(flagDel == false && flagAdd == true)
                    {
                        formDisk->eliminarParticion(formDisk);
                    }
                    else if(flagDel == true && flagAdd ==false)
                    {

                    }
                    else
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                }


            }
            else if (nombreComando == "mount")
            {
                bool parametrosValidos = true;

                bool flagName = true;
                bool flagPath = true; //bandera para este parametro que es obligatorio
                mount *diskMo = new mount();

                 for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-path")
                    {
                        if(strstr(prop.valor.c_str(), "\"")!=NULL)
                        {
                            vector<string> conc = split(prop.valor, '"');
                            diskMo->setPath(conc[1]);
                            cout<<conc[1]<<endl;
                        }
                        else
                        {
                            diskMo->setPath(prop.valor);
                        }

                        flagPath = false;
                    }
                    else if (nombreProp == "-name")
                    {
                        flagName = false;

                        diskMo->setName(prop.valor);

                    }


                }
                if( flagPath == false && flagName ==false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    diskMo->ejecutarComandoMount(diskMo,DisksMount);
                }


            }
            else if (nombreComando == "unmount")
            {
                bool parametrosValidos = true;
                bool flagId = true;
                unmount *unM = new unmount();

                for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-id")
                    {
                        flagId = false;

                        unM->setId(prop.valor);
                        break;

                    }
                }

                if( flagId == false )
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    unM->ejecutarComandoUnmount(unM,DisksMount);
                }



            }
            else if (nombreComando == "mkfs")
            {
                bool parametrosValidos = true;
                bool flagId = true;

                bool flagType = true;
                bool flagFs = true;
                mkfs *fileSis = new mkfs();

                for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-id")
                    {
                        flagId = false;

                        fileSis->setId(prop.valor);

                    }
                    else if (nombreProp == "-type")
                    {
                         string var1 = prop.valor;
                        std::for_each(var1.begin(), var1.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });
                        flagType = false;
                        if(var1 =="fast" || var1 =="full")
                        {
                            fileSis->setType(var1);

                        }
                        else
                        {
                            cout<<"ERROR: tipo del erroneo, no soportado"<<endl;
                            parametrosValidos = true;
                            flagId = true;
                            break;
                        }
                    }
                    else if (nombreProp == "-fs")
                    {
                         string var1 = prop.valor;
                        std::for_each(var1.begin(), var1.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });
                        flagFs = false;
                        if(var1 =="2fs" || var1 =="3fs")
                        {
                            fileSis->setFs(var1);

                        }
                        else
                        {
                            cout<<"ERROR: tipo de fs erroneo, no soportado"<<endl;
                            parametrosValidos = true;
                            flagId = true;
                            break;
                        }
                    }
                }

                if (flagType) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    fileSis->setType("full");
                }
                if (flagFs) // si el parametro no venia entra aqui y se pone el parametro por defecto
                {
                    fileSis->setFs("2fs");
                }

                if( flagId == false )
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    fileSis->ejecutarComandoMkfs(fileSis,DisksMount);
                }
            }
            else if (nombreComando == "login")
            {
                bool parametrosValidos = true;
                bool flagId = true;
                bool flagUsu = true;
                bool flagPass = true;

                login *log = new login();

                for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-id")
                    {
                        flagId = false;

                        log->setIdMount(prop.valor);

                    }
                    else if (nombreProp == "-usuario")
                    {
                        flagUsu= false;

                        log->setUsuario(prop.valor);
                    }
                    else if (nombreProp == "-password")
                    {
                        flagPass = false;

                        log->setPassword(prop.valor);
                    }

                }

                if( flagId == false && flagUsu ==false && flagPass == false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    if(!getHayInicioSesion()){
                        bool ver = log->ejecutarComandoLogin(log,DisksMount);
                        if(ver){
                            setHayInicioSesion(true);
                            User usuario;
                            usuario.idPartMontada = log->getIdMount();
                            usuario.nombreUsuario = log->getUsuario();
                            setUsuarioAct(usuario);

                        }
                    }else{
                        cout<<"ERROR: ya hay una sesion iniciada\n"<<endl;
                    }
                }


            }
            else if (nombreComando == "logout")
            {

               if(getHayInicioSesion()){
                    cout<<"\n************Ejecutar LOGOUT************\n"<<endl;


                            setHayInicioSesion(false);
                            User usuario;
                        usuario = getUsuarioAct();
                    cout<<"Se cerro la sesion de: "<<usuario.nombreUsuario<<endl;
                            usuario.idPartMontada = " ";
                            usuario.nombreUsuario = " ";
                            setUsuarioAct(usuario);


                    }else{
                        cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                    }

            }
            else if (nombreComando == "mkgrp")
            {

               if(getHayInicioSesion()){
                     bool parametrosValidos = true;

                    bool flagUsu = true;


                    mkgrp *grup = new mkgrp();

                    for (int f = 0; f < 15; f++)
                    {
                        Propiedad prop=comandoTemp.propiedades[f];
                        string nombreProp = prop.Nombre;
                        std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });


                        if (nombreProp == "-name")
                        {
                            flagUsu= false;

                            grup->setName(prop.valor);
                            break;
                        }


                    }

                    if( flagUsu ==false )
                    {
                        parametrosValidos = false;

                    }

                    if (parametrosValidos)
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                    else
                    {
                        grup->setDatosUsu(getUsuarioAct());
                        grup->ejecutarMkgrp(grup, DisksMount);

                    }



                }else{
                    cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                }

            }
            else if (nombreComando == "rmgrp")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "mkusr")
            {

                if(getHayInicioSesion()){
                     bool parametrosValidos = true;

                    bool flagUsu = true;
                    bool flagPass = true;
                    bool flagGrp = true;


                    mkusr *grup = new mkusr();

                    for (int f = 0; f < 15; f++)
                    {
                        Propiedad prop=comandoTemp.propiedades[f];
                        string nombreProp = prop.Nombre;
                        std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });


                        if (nombreProp == "-usuario")
                        {
                            flagUsu= false;

                            grup->setUsuario(prop.valor);

                        }
                        else if (nombreProp == "-pwd")
                        {
                            flagPass= false;

                            grup->setPassword(prop.valor);

                        }
                        else if (nombreProp == "-grp")
                        {
                            flagGrp= false;

                            grup->setGrupo(prop.valor);

                        }


                    }

                    if( flagUsu ==false && flagPass ==false && flagGrp ==false )
                    {
                        parametrosValidos = false;

                    }

                    if (parametrosValidos)
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                    else
                    {
                        grup->setDatosUsu(getUsuarioAct());
                        grup->ejecutarMkusr(grup, DisksMount);

                    }



                }else{
                    cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                }

            }
            else if (nombreComando == "rmusr")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "chmod")
            {
                if(getHayInicioSesion()){
                     bool parametrosValidos = true;

                    bool flagPath = true;
                    bool flagUgo = true;

                    bool flagR = true;


                    chmod *permisos = new chmod();

                    for (int f = 0; f < 15; f++)
                    {
                        Propiedad prop=comandoTemp.propiedades[f];
                        string nombreProp = prop.Nombre;
                        std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });


                        if (nombreProp == "-path")
                        {
                            flagPath= false;

                            permisos->setPath(prop.valor);

                        }
                        else if (nombreProp == "-ugo")
                        {
                            flagUgo= false;

                            int s = stoi(prop.valor);
                            if(s>=0 && s<778)
                            {
                                permisos->setUgo(s);

                            }
                            else
                            {
                                cout<<"ERROR: UGO erroneo, esta fuera del rango permitido"<<endl;
                                parametrosValidos = true;
                                flagUgo= true;
                                break;
                            }

                        }
                        else if (nombreProp == "-r")
                        {
                            flagR= false;

                            permisos->setR(true);

                        }


                    }

                    if( flagPath ==false && flagUgo ==false )
                    {
                        parametrosValidos = false;

                    }

                    if (parametrosValidos)
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                    else
                    {
                        permisos->setDatosUsu(getUsuarioAct());
                        permisos->ejecutarComandoChmod(permisos, DisksMount);

                    }



                }else{
                    cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                }

            }
            else if (nombreComando == "mkfile")
            {
                    if(getHayInicioSesion()){
                     bool parametrosValidos = true;

                    bool flagPath = true;

                    bool flagSize = true;
                    bool flagR = true;
                    bool flagCont = true;

                    mkfile *mfile = new mkfile();

                    for (int f = 0; f < 15; f++)
                    {
                        Propiedad prop=comandoTemp.propiedades[f];
                        string nombreProp = prop.Nombre;
                        std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });


                        if (nombreProp == "-path")
                        {
                            if(strstr(prop.valor.c_str(), "\"")!=NULL)
                            {
                                vector<string> conc = split(prop.valor, '"');
                                mfile->setPath(conc[1]);
                                cout<<conc[1]<<endl;
                            }
                            else
                            {
                                mfile->setPath(prop.valor);
                            }

                            flagPath = false;

                        }
                        else if (nombreProp == "-size")
                        {
                            flagSize= false;

                            int s = stoi(prop.valor);
                            if(s>0)
                            {
                                mfile->setSize(s);

                            }
                            else
                            {
                                cout<<"ERROR: size erroneo, es negativo"<<endl;
                                parametrosValidos = true;
                                flagPath= true;
                                break;
                            }

                        }
                        else if (nombreProp == "-r")
                        {
                            flagR= false;

                            mfile->setR(true);

                        }
                        else if (nombreProp == "-cont")
                        {
                            if(strstr(prop.valor.c_str(), "\"")!=NULL)
                            {
                                vector<string> conc = split(prop.valor, '"');
                                mfile->setCont(conc[1]);
                                mfile->setBoolCont(true);
                                cout<<conc[1]<<endl;
                            }
                            else
                            {
                                mfile->setCont(prop.valor);
                                mfile->setBoolCont(true);
                            }

                            flagCont = false;

                        }


                    }
                    if( flagSize  )
                    {
                        mfile->setSize(0);

                    }
                    if( flagCont  )
                    {
                        mfile->setBoolCont(false);

                    }
                    if(flagR){
                        mfile->setR(false);
                    }


                    if( flagPath ==false )
                    {
                        parametrosValidos = false;

                    }

                    if (parametrosValidos)
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                    else
                    {
                        mfile->setDatosUsu(getUsuarioAct());
                        mfile->ejecutarComandoMkfile(mfile, DisksMount);

                    }



                }else{
                    cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                }

            }
            else if (nombreComando == "cat")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "remove")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "edit")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "rename")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "mkdir")
            {

                if(getHayInicioSesion()){
                     bool parametrosValidos = true;

                    bool flagPath = true;

                    bool flagP = true;

                    mkdir *mDir = new mkdir();

                    for (int f = 0; f < 15; f++)
                    {
                        Propiedad prop=comandoTemp.propiedades[f];
                        string nombreProp = prop.Nombre;
                        std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });


                        if (nombreProp == "-path")
                        {
                            if(strstr(prop.valor.c_str(), "\"")!=NULL)
                            {
                                vector<string> conc = split(prop.valor, '"');
                                mDir->setPath(conc[1]);
                                cout<<conc[1]<<endl;
                            }
                            else
                            {
                                mDir->setPath(prop.valor);
                            }

                            flagPath = false;

                        }
                        else if (nombreProp == "-p" )
                        {
                            flagP= false;

                            mDir->setP(true);

                        }


                    }

                    if( flagP  )
                    {
                        mDir->setP(false);

                    }


                    if( flagPath ==false )
                    {
                        parametrosValidos = false;

                    }

                    if (parametrosValidos)
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                    else
                    {
                        mDir->setDatosUsu(getUsuarioAct());
                        mDir->ejecutarComandoMkDir(mDir, DisksMount);

                    }



                }else{
                    cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                }

            }
            else if (nombreComando == "copy")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "move")
            {

                if(getHayInicioSesion()){
                     bool parametrosValidos = true;

                    bool flagPath = true;

                    bool flagDestino = true;

                    commv  *cMove = new commv();

                    for (int f = 0; f < 15; f++)
                    {
                        Propiedad prop=comandoTemp.propiedades[f];
                        string nombreProp = prop.Nombre;
                        std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });


                        if (nombreProp == "-path")
                        {
                            if(strstr(prop.valor.c_str(), "\"")!=NULL)
                            {
                                vector<string> conc = split(prop.valor, '"');
                                cMove->setPath(conc[1]);
                                cout<<conc[1]<<endl;
                            }
                            else
                            {
                                cMove->setPath(prop.valor);
                            }

                            flagPath = false;

                        }
                        else if (nombreProp == "-destino" )
                        {
                            if(strstr(prop.valor.c_str(), "\"")!=NULL)
                            {
                                vector<string> conc = split(prop.valor, '"');
                                cMove->setDestino(conc[1]);
                                cout<<conc[1]<<endl;
                            }
                            else
                            {
                                cMove->setDestino(prop.valor);
                            }

                            flagDestino = false;

                        }


                    }



                    if( flagPath ==false && flagDestino ==false )
                    {
                        parametrosValidos = false;

                    }

                    if (parametrosValidos)
                    {
                        cout<<"--- Parametros Invalidos ---\n"<<endl;
                    }
                    else
                    {
                        cMove->setDatosUsu(getUsuarioAct());
                        cMove->ejecutarComandoMove(cMove, DisksMount);

                    }



                }else{
                    cout<<"ERROR: No hay una sesion iniciada\n"<<endl;
                }

            }
            else if (nombreComando == "find")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "chown")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "chgrp")
            {

                printf("Pause Presione una tecla para continuar... \n");

            }
            else if (nombreComando == "pause")
            {

                printf("Pause Presione una tecla para continuar... \n");
                cin.get();
            }
            else if (nombreComando == "exec")
            {

                bool parametrosValidos = true;
                bool flagPath = true; //bandera para este parametro que es obligatorio
                string pathEx = "";


                 for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-path")
                    {
                        if(strstr(prop.valor.c_str(), "\"")!=NULL)
                        {
                            vector<string> conc = split(prop.valor, '"');
                            pathEx=conc[1];
                            cout<<conc[1]<<endl;
                        }
                        else
                        {
                            pathEx=prop.valor;
                        }

                        flagPath = false;
                        break;
                    }


                }
                if( flagPath == false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    comandoExec(pathEx,DisksMount);
                }

            }
            else if (nombreComando == "rep")
            {

                bool parametrosValidos = true;
                bool flagPath = true; //bandera para este parametro que es obligatorio
                bool flagName = true;
                bool flagId = true;

                bool flagRuta = true;
                string tipoRep = "";

                reporte *rep = new reporte();

                 for (int f = 0; f < 15; f++)
                {
                    Propiedad prop=comandoTemp.propiedades[f];
                    string nombreProp = prop.Nombre;
                    std::for_each(nombreProp.begin(), nombreProp.end(), [](char &c)
                    {
                        c = ::tolower(c);
                    });

                    if (nombreProp == "-path")
                    {
                        if(strstr(prop.valor.c_str(), "\"")!=NULL)
                        {
                            vector<string> conc = split(prop.valor, '"');
                            rep->setPath(conc[1]);
                            cout<<conc[1]<<endl;
                        }
                        else
                        {
                            rep->setPath(prop.valor);
                        }

                        flagPath = false;

                    }else if(nombreProp == "-name"){

                        tipoRep = prop.valor;
                        std::for_each(tipoRep.begin(), tipoRep.end(), [](char &c)
                        {
                            c = ::tolower(c);
                        });

                        if(tipoRep =="mbr" || tipoRep =="disk"|| tipoRep =="inode"|| tipoRep =="journaling"|| tipoRep =="block"|| tipoRep =="bm_inode"|| tipoRep =="bm_block"|| tipoRep =="tree"|| tipoRep =="sb"|| tipoRep =="file"|| tipoRep =="ls")
                        {
                            flagName=false;
                            rep->setName(prop.valor);

                        }
                        else
                        {
                            cout<<"ERROR: nombre del erroneo, no soportado"<<endl;
                            parametrosValidos = true;
                            flagName = true;
                            break;
                        }


                    }
                    else if(nombreProp == "-id"){
                            rep->setId(prop.valor);
                        flagId = false;


                    }else if(nombreProp == "-ruta"){
                        rep->setRuta(prop.valor);
                        flagRuta = false;

                    }


                }


                if( flagPath == false && flagName == false && flagId == false)
                {
                    parametrosValidos = false;

                }

                if (parametrosValidos)
                {
                    cout<<"--- Parametros Invalidos ---\n"<<endl;
                }
                else
                {
                    if(tipoRep == "mbr"){
                            rep->ejecutarRepMBR(rep,DisksMount);

                    }else if(tipoRep == "disk"){
                            rep->ejecutarRepDisk(rep,DisksMount);
                    }
                    else if(tipoRep == "inode"){
                            rep->ejecutarRepInodo(rep,DisksMount);
                    }
                    else if(tipoRep == "journaling"){
                            //rep->ejecutarRepMBR(rep,DisksMount);
                    }
                    else if(tipoRep == "block"){
                            rep->ejecutarRepBloque(rep,DisksMount);
                    }
                    else if(tipoRep == "bm_inode"){
                            rep->ejecutarRepBmInodo(rep,DisksMount);
                    }
                    else if(tipoRep == "bm_block"){
                            rep->ejecutarRepBmBloque(rep,DisksMount);
                    }
                    else if(tipoRep == "tree"){
                            rep->ejecutarRepTree(rep,DisksMount);
                    }
                    else if(tipoRep == "sb"){
                            rep->ejecutarRepSuperB(rep,DisksMount);
                    }
                    else if(tipoRep == "file"){
                            if(!flagRuta){
                                rep->ejecutarRepFile(rep,DisksMount);
                            }else{
                                cout << "Se necesita una Ruta para este reporte" << endl;
                            }
                    }
                    else if(tipoRep == "ls"){
                            if(!flagRuta){
                                rep->ejecutarRepLs(rep,DisksMount);
                            }else{
                                cout << "Se necesita una Ruta para este reporte" << endl;
                            }
                    }
                }

            }
            else
            {
                cout << "no se reconoce el comando" << endl;
            }

        }

    }


}

void manejador::comandoExec(string s, mount DisksMount[])
{
    cout<<"************Comando EXEC************\n"<<endl;
    ifstream f;
    string data;

    f.open(s, ios::in); //abrimos el archivo en modo lectura

    if (f.fail())
    {
        cout << "no se pudo abrir el archivo" << endl;
    }
    else
    {
        while (!f.eof()) //mientras no sea el final del archivo
        {
            getline(f, data);
            cout <<endl<<"Linea en el script: " <<data << endl<<endl;
            leerTexto(data, DisksMount);

        }
        f.close(); //cerramos el archivo

    }
}


vector<string> manejador::split(string str, char pattern)
{

    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}

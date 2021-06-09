#ifndef _FILE_NAMAGER_
#define _FILE_NAMAGER_

class manager;

#include "structure.h"
#include "command.h"

static std::string command;
using std::cout;
using std::endl;
using std::cin;
using std::string;

class manager {
public:
    manager(int);
    void start();
    void run(int user, int layer);
    void mkdir(int user, int layer, string name, int right);
    void touch(int user, int layer, string name, int right, string content);
private:
    _disk disk;
    int usernum;
    _user user[25];
};

#include <cstring>

inline void
manager::touch(int user, int layer, string name, int right, string contents)
{
    //修改父文件夹内容
    disk.df[layer].contents.size++;
    strcpy(disk.df[layer].contents.dir[disk.df[layer].contents.size].f_name, name.c_str());
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_type = false;
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_mode = right;
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_size = contents.length();
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_addr = disk.f_size + 1;
    //修改disk内容
    disk.f_size++;
    strcpy(disk.f[disk.f_size].f_direct.f_name, name.c_str());
    disk.f[disk.f_size].f_direct.f_type = false;
    disk.f[disk.f_size].f_direct.f_mode = right;
    disk.f[disk.f_size].f_direct.f_size = contents.length();
    disk.f[disk.f_size].f_direct.f_addr = disk.f_size;
    strcpy(disk.f[disk.f_size].f_contents, contents.c_str());
}

inline void
manager::mkdir(int user, int layer, string name, int right)
{
    //修改父文件夹内容
    disk.df[layer].contents.size++;
    strcpy(disk.df[layer].contents.dir[disk.df[layer].contents.size].f_name, name.c_str());
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_type = true;
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_mode = right;
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_size = 0;
    disk.df[layer].contents.dir[disk.df[layer].contents.size].f_addr = disk.df_size + 1;
    //修改disk内容
    disk.df_size++;
    strcpy(disk.df[disk.df_size].f_name,  name.c_str());
    disk.df[disk.df_size].contents.size = 0;
}

inline
manager::manager(int i)
{
    strcpy(user[0].user_name, "root");
    user[0].password = "000000";
    user[0].id = 0;
    usernum = 1;
    
    //初始化磁盘
    disk.df_size = 0;
    disk.f_size = 0;
    //添加/文件夹
    strcpy(disk.df[0].f_name, "/");
    disk.df[0].contents.size = 0;
    //默认文件夹/用户root
    mkdir(0, 0, "root", 0);

    /*strcpy(disk.df[0].contents.dir[0].f_name, "root");
    //默认文件夹root
    strcpy(disk.df[0].contents.dir[0].f_name, "root");
    disk.df[0].contents.dir[0].f_type = true;
    disk.df[0].contents.dir[0].f_mode = 0;
    disk.df[0].contents.dir[0].f_size = 1;
    disk.df[0].contents.dir[0].f_addr = 1;
    strcpy(disk.df[1].f_name, "root");
    disk.df_size = 1;*/
    //root下默认文件config,conf
    touch(0, 1, "config.conf", 1, "none");
}


#include <iostream>
#include <cstdio>

inline void
manager::start()
{
    while(true) { 
        cout<<"input user name: ";
        cin>>command;
        for(int i = 0; i < usernum; ++i) {
            if(!strcmp(command.c_str(), user[i].user_name)) {
                while(true) {
                    cout<<user[i].user_name<<"`s password: ";
                    cin>>command;
                    if(command == user[i].password) run(i, 0);
                    else cout<<"wrong"<<endl;
                }
            }
        }
        cout<<"no such user exist"<<std::endl;
    }
}

inline void
manager::run(int usr, int layer)
{
    while(true) {
        cout<<"["<<user[usr].user_name<<"@"
        <<"localhost"<<" "<<disk.df[layer].f_name<<"]# ";
        cin>>command;
        const char * str = command.c_str();
        switch(hash_(str)) {
            case "ls"_hash :
                //cout<<disk.df[layer].contents.size;
                for(int i = 1; i <= disk.df[layer].contents.size; ++i) {
                    cout<<disk.df[layer].contents.dir[i].f_name<<" ";
                }
                cout<<endl;
                break;
            case "cd"_hash :
                cin>>command;
                if(!strcmp("", command.c_str())) {
                    cout<<"bash: "<<command<<": command not found"<<endl;
                    goto __command_end;
                }
                else if(!strcmp("..", command.c_str())) {
                    if(layer == 0) goto __command_end;
                    else {
                        layer = 0;
                        goto __command_end;
                    }
                }
                else {
                    for(int i = 1; i <= disk.df[layer].contents.size; ++i) {
                        if(!strcmp(disk.df[layer].contents.dir[i].f_name, command.c_str())) {
                            if(disk.df[layer].contents.dir[i].f_type) {
                                layer = i;
                                goto __command_end;
                            }
                            else {
                                cout<<"bash: cd: "<<command<<": Not a directory"<<endl;
                                goto __command_end;
                            }
                        }
                    }
                    cout<<"bash: cd: "<<command<<": No such file or directory"<<endl;
                }
                break;
            case "mkdir"_hash :
                cin>>command;
                //检查是否有重复文件
                for(int i = 1; i <= disk.df[layer].contents.size; ++i) {
                    if(!strcmp(disk.df[layer].contents.dir[i].f_name, command.c_str())) {
                        cout<<"mkdir: cannot create directory ‘"<<command<<": File exists"<<endl;
                        goto __command_end;
                    }
                    else {
                        mkdir(usr, layer, command, 0);
                        goto __command_end;
                    }
                }
                break;
            case "touch"_hash :
                cin>>command;
                //检查是否有重复文件
                for(int i = 1; i <= disk.df[layer].contents.size; ++i) {
                    if(!strcmp(disk.df[layer].contents.dir[i].f_name, command.c_str())) {
                        cout<<"mkdir: cannot create file ‘"<<command<<": File exists"<<endl;
                        goto __command_end;
                    }
                    else {
                        string contents;
                        cout<<"输入文件内容"<<endl;
                        cin>>contents;
                        touch(usr, layer, command, 0, contents);
                        goto __command_end;
                    }
                }
                break;
            case "vim"_hash :
                for(int i = 1; i <= disk.df[layer].contents.size; ++i) {
                    if(!strcmp(disk.df[layer].contents.dir[i].f_name, command.c_str())) {

                    }
                }
                break;
            default :
                cout<<"bash: "<<command<<": command not found"<<endl;
        }
        __command_end:;
    }
}

#endif  //_FILE_NAMAGER_

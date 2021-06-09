#ifndef _STRUCTURE_
#define _STRUCTURE_

#define FILESIZE 1000

#include "dir_structure.h"

//normal file sturcture
typedef struct file {
    //file direct
    direct f_direct;
    //file contents
    char f_contents[FILESIZE];
} file;

//disk memory
typedef struct _disk {
    int df_size;
    int f_size;
    dir_file df[NADDR];
    file f[NADDR];
} _disk;

typedef struct _user {
    //name
    char user_name[25];
    //password
    std::string password;
    //id
    int id;
} _user;


#endif  //_STRUCTURE_

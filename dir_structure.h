#ifndef _DIR_STRUCTURE_
#define _DIR_STRUCTURE_

#define NAMESIZE 255
#define NADDR 1024
#define DIRNUM 1024

//dirct structure
typedef struct direct {
    //file name
    char f_name[NAMESIZE];
    //ture:dir, false:normal
    bool f_type;
    //access privilege, 0: all user, 1 :owner only
    unsigned int f_mode;
    //file size
    unsigned int f_size;
    //block number
    unsigned int f_addr;
} direct;

//dirct
typedef struct dir_contents {
    //dir array
    direct dir[DIRNUM];
    //size
    int size;
} dir_contents;

//dir file structure
typedef struct dir_file {
    //file name
    char f_name[NAMESIZE];
    //file contents
    dir_contents contents;
} dir_file;

#endif  //_DIR_STRUCTURE_

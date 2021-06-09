# 实验三：文件系统仿真 （2 学时）   
一．实验目的:   
在操作系统的文件管理中通过建立文件目录，以实现“按名存取”，  
即建立文件名与外存空间中的物理地址的对应关系。文件目录的结构  
有多种，现以二级文件目录为例，编写并调试简单的二级文件系统，  
深化对文件管理的认识。   
二. 实验内容   
##### 1.设计思想   
本文件系统采用两级目录，其中第一级对应于用户账号，第二级  
对应于用户帐号下的文件。另外，为了简便文件系统未考虑文件共享，文件系统安全以及管道文件与设备文件等特殊内容。该系统需要有以  
下文件处理命令：   
文件控制块（目录项）中至少有:文件名、物理地址、文件长度、保护
码（编码形式参考 linux 系统文件保护编码，详见课本或课件）。   
##### 2.主要数据结构参考   
a) 文件   
一般文件   
> ogin 用户登录，分：所有者、访问组 1、其他用户  
> dir 罗列目录，输出该目录下所有文件的文件名  
> create 创建文件  
> delete 删除文件  
> open 打开文件  
> close 关闭文件  
> read 读文件  
> write 写文件

以上对文件的操作除创建和删除外，均不需要对文件有任何实质性操作，但需要考虑用户权限
```cpp
Struct file   
{ char f_name[NAMESIZE]; /*文件名*/   
 char f_contents[FILESIZE]; /*文件内容*/   
}   
目录文件   
Struct dir_file   
{ char f_name[NAMESIZE]; /*文件名*/   
 struct dir_contents contents; /*文件内容*/   
}   
b) 目录   
Struct dir_contents   
{ strut direct dir[DIRNUM]; /*目录项数组*/   
 int size; /*目录项数量*/   
}   
c)目录项结构   
Struct direct   
{ char f_name[FILESIZE]; /*文件名*/   
 bool f_type; /*0:目录文件，1:一般文件/   
unsigned int f_mode; /*存取权限*/   
unsigned long f_size; /*文件大小*/   
unsigned int f_addr[NADDR]; /*物理块号*/   
} d）磁盘中的存储   
Struct disk   
{ struct dir_file df[NADDR];   
 struct file f[HADDR];   
}  
```

// #pragma once
// #include "type.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>

// #include <ext2fs/ext2_fs.h>

// #include <string.h>
// #include <libgen.h>
// #include <sys/stat.h>
// #include <time.h>

/************* cd_ls_pwd.c file **************/
char *t1 = "xwrxwrxwr ";
char *t2 = "--------";

int cd(char * pathname)
{
  printf("cd: under construction READ textbook!!!!\n");

  // READ Chapter 11.7.3 HOW TO chdir
  int ino = getino(pathname);
  if(ino == 0)
  {
    printf("NO WORK!\n");
    return -1;
  }
  MINODE *mip = iget(dev,ino);
  if ((mip->INODE.i_mode & 0xF000) == 0x4000) // if (S ISDIR())
  {
    iput(running->cwd);
    running->cwd = mip;
  }
  else
  {
    printf("NO WORK!\n");
    return -1;
  }

}

int ls_file(MINODE *mip, char *name)
{
  int i =0;
  char ftime[64];
  if ((mip->INODE.i_mode & 0xF000) == 0x8000) // if (S ISREG())
      printf("%c",'-');
  if ((mip->INODE.i_mode & 0xF000) == 0x4000) // if (S ISDIR())
      printf("%c",'d');
  if ((mip->INODE.i_mode & 0xF000) == 0xA000) // if (S ISLNK())
      printf("%c",'l');
  for (i=8; i >= 0; --i )
  {
      if (mip->INODE.i_mode & (1 << i)) // print r|w|x
          printf("%c", t1[i]);
      else
          printf("%c", t2[i]); // or print
  }
    printf("%4d",mip->INODE.i_links_count); // link count

    printf("%4d",mip->INODE.i_gid); // gid

    printf("%4d   ",mip->INODE.i_uid); // uid

    time_t time = mip->INODE.i_ctime;
    strcpy(ftime, ctime(&(time)));
    ftime[strlen(ftime)-1]=0;
    printf("%s   ", ftime);

    printf("%d   ",mip->INODE.i_size );


    printf("%s", name);

    // // if ((sp->st_mode & 0xF000)== 0xA000)
    // // {
    // // // use readlink() to read linkname
    // //     char linkname[256];
    // //     readlink(fname,linkname,256);
    // //     printf("-> %s", linkname); // print linked name
    // // }
    printf("\n");

}

int ls_dir(MINODE *mip)
{
  char buf[BLKSIZE], temp[256];
  DIR *dp;
  char *cp;

  get_block(dev, mip->INODE.i_block[0], buf);
  dp = (DIR *)buf;
  cp = buf;
  
  while (cp < buf + BLKSIZE){
     strncpy(temp, dp->name, dp->name_len);
     temp[dp->name_len] = 0;
	
    //  printf("[%s]\n", temp);
     ls_file(mip, temp);
     cp += dp->rec_len;
     dp = (DIR *)cp;
  }
  printf("\n");
}

int ls()
{
  ls_dir(running->cwd);
}

char* rpwd(MINODE *wd)
{
  if(wd == root)
  {
    return;
  }
  int myIno = -1;
  int parentIno = -1;
  parentIno = findino(wd,&myIno);
  MINODE* temp = iget(dev,parentIno);
  char myName[64];
  findmyname(temp,myIno,myName);
  rpwd(temp);
  printf("/%s", myName);
}

char *pwd(MINODE *wd)
{
  if (wd == root){
    printf("/\n");
    return;
  }
  rpwd(running->cwd);
  printf("\n");
  return;
  

  
}




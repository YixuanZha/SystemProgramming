// Display file's metadata

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"Usage: %s <FilePath>\n",argv[0]);
        return 1;
    }
    
    const char* FileName = argv[1];
    struct stat FileStat;

    if(lstat(FileName,&FileStat) == -1)
    {
        perror("lstat failed to get the stat of the file");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "File types:     ");
    if (S_ISREG(FileStat.st_mode))       printf("General file\n");
    else if (S_ISDIR(FileStat.st_mode))  printf("Directory\n");
    else if (S_ISLNK(FileStat.st_mode))  printf("Symbolic link\n");
    else if (S_ISCHR(FileStat.st_mode))  printf("Character device\n");
    else if (S_ISBLK(FileStat.st_mode))  printf("Block device\n");
    else if (S_ISFIFO(FileStat.st_mode)) printf("FIFO (Named tube)\n");
    else if (S_ISSOCK(FileStat.st_mode)) printf("Socket\n");
    else                                printf("Unknown type\n");

    fprintf(stdout,"File authority: %o\n",FileStat.st_mode & 0777);

    fprintf(stdout,"Inode:     %ld\n",(long)FileStat.st_ino);

    fprintf(stdout,"Blocks:    %ld\n",FileStat.st_blocks); // how many blocks(512 bytes) for PORSIX
    
    fprintf(stdout,"Block size:    %ld\n",FileStat.st_blksize);

    fprintf(stdout,"Numbers of hard link: %ld\n",FileStat.st_nlink);
    
    struct passwd *pw = getpwuid(FileStat.st_uid);
    struct group  *gr = getgrgid(FileStat.st_gid);
    fprintf(stdout, "Owner/Group-----%s/%s\n",pw->pw_name,gr->gr_name);

    fprintf(stdout,"Size of the file:   %lld bytes\n",(long long) FileStat.st_size);

    fprintf(stdout,"Last access time:   %s\n",ctime(&FileStat.st_atim.tv_sec));
    fprintf(stdout,"Last modify time:   %s\n",ctime(&FileStat.st_mtim.tv_sec));
    fprintf(stdout,"Last status change time:    %s\n",ctime(&FileStat.st_ctim.tv_sec));

    return 0;
}
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

int eq(char *, char *, off_t);
void perm(struct stat *, char *);

int main(int argc, char *argv[])
{
  const long long int SIZE = 10000;                                                 // Size of block to be checked 
  int p, p1, p2;
  off_t n = 0, l, size = 0;
  char *a = argv[1];
  char *b = argv[2];
  char *ch = argv[3];
  p = open(a, O_RDONLY);
  if(p < 0)
  {
    perror("Open error");
    exit(1);
  }
  p1 = open(b, O_RDONLY);
  if(p1 < 0)
  {
    perror("Open error");
    exit(1);
  }
  struct stat s1, s2, s3;
  fstat(p, &s1);                                                                    
  fstat(p1, &s2);
  stat(ch, &s3);                                                                    
  if (!S_ISDIR(s3.st_mode))                                                         // Checking the existence of directory
  {
    perror("Directory doesn't exist");
    exit(1);
  }
  else if ((s1.st_mode & F_OK))                                                     // Checking existence of new file
  {
    perror("New file doesn't exist");
    exit(1);
  }
  else if ((s2.st_mode & F_OK))                                                     // Checking existence of old file
  {
    perror("Old file doesn't exist");
    exit(1);
  }
  else 
  {
    l = lseek(p1, 0, SEEK_END);
    n = lseek(p1, -SIZE, SEEK_END);                                                 // Initializing l,n as limits for reading blocks
    size = l - n;
    char c[SIZE + 1], buf[SIZE + 1];                                                // Arrays for checking equality
    while (n >= 0)
    {
      read(p, c, size);
      c[size] = '\0';
      read(p1, buf, size);
      buf[size] = '\0';
      if (eq(c, buf, size) == 0)                                                    // Function to check equality
        break;
      l = n;
      n = lseek(p1, -(2 * SIZE), SEEK_CUR);
      if (n == -1 && l > 0)
        n = lseek(p1, 0, SEEK_SET);
    }
    write(1, "Directory is created: Yes\n", 26);
    if (n < 0)                                                                      // Occurs when you read both the files completely which means the file has been reversed
      write(1, "Whether file contents are reversed in new file: Yes\n", 52);
    else
      write(1, "Whether file contents are reversed in new file: No\n", 51);

    perm(&s1, " new file");
    perm(&s2, " old file");
    perm(&s3, " directory");                                                        // Functions for checking permissions
  }
}

int eq(char *ch, char *buf, off_t n)
{
  off_t i, j;
  for (i = 0, j = n - 1; i < n; i++, j--)
  {
    if (ch[i] != buf[j])                                                            // Checks if each character is equal or not
      return 0;
  }
  return 1;
}

void perm(struct stat *s, char *c) 
{
  write(1, "\nUser has read permissions on ", 29);
  write(1, c, strlen(c));
  if (s->st_mode & S_IRUSR)                                                        // Macros which are used to check permissions
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "User has write permissions on ", 29);
  write(1, c, strlen(c));
  if (s->st_mode & S_IWUSR)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "User has execute permissions on ", 31);
  write(1, c, strlen(c));
  if (s->st_mode & S_IXUSR)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "\nGroup has read permissions on ", 30);
  write(1, c, strlen(c));
  if (s->st_mode & S_IRGRP)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "Group has write permissions on ", 30);
  write(1, c, strlen(c));
  if (s->st_mode & S_IWGRP)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "Group has execute permissions on ", 32);
  write(1, c, strlen(c));
  if (s->st_mode & S_IXGRP)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "\nOthers have read permissions on ", 32);
  write(1, c, strlen(c));
  if (s->st_mode & S_IROTH)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "Others have write permissions on ", 32);
  write(1, c, strlen(c));
  if (s->st_mode & S_IWOTH)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
  write(1, "Others have execute permissions on ", 34);
  write(1, c, strlen(c));
  if (s->st_mode & S_IXOTH)
    write(1, ": Yes\n", 6);
  else
    write(1, ": No\n", 5);
}

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>

#define Size 409600
#define SZ 3

void printperc(long int);
void strng(long int);
void rev(char *, char *, off_t);
void dir(char *, char *);

int main(int argc, char *argv[]) 
{                              // Initializing the size to be transferred per every write
  off_t n = 1, l;
  int p, p1;
  char ch[Size + 1], buf[Size + 1], c[100];                         // Two arrays used for copying while the third is for getting the path of file
  char *d = argv[1];                                                // Contains filename
  struct stat f,s;
  p = open(d, O_RDONLY | O_CREAT, 0600);
  if(p == -1)
  {
    perror("Open Error");
    exit(1);
  }
  if(stat("Assignment",&s) == -1)
    mkdir("Assignment", 0700);
  getcwd(c, 100);
  dir(c, d);                                                        // Function for appending the drectory path to file
  p1 = open(c, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if(p1 == -1)
  {
    perror("Open Error");
    exit(1);
  }
  l = lseek(p, 0, SEEK_END);
  n = lseek(p, -Size, SEEK_END);                                    // Initializing two variables(l,n) as limits for reading data from file
  if(n == -1)
    n = lseek(p, 0, SEEK_SET);                                      // Incase the filesize is less than the size to be transferred
  fstat(p, &f);
  off_t size = f.st_size;                                           // Contains filesize
  long int fsize = 0, nsize = 0;
  nsize = 0;
  write(1, "\nInitialized Copying\n", 21);
  write(1, "\rCompleted: 0%", 14);
  while (n >= 0) 
  {
    read(p, ch, l - n);
    rev(ch, buf, l - n);                                            // Reverses the string that is read
    write(p1, buf, l - n);
    fsize = fsize + (l - n);                                        // Stores the file size that is read
    if (nsize < (fsize * 100 / size) && fsize != size)
    {
      nsize = (long int)(fsize * 100 / size);                                 // Used for printing percentage
      printperc(nsize);        
      if(nsize % 3 == 0)
        fsync(p1);                                       // Flushes the buffer to disk
    }
    l = n;
    n = lseek(p, -(2 * Size), SEEK_CUR);                            // For reading the next block
    if (n == -1 && l > 0)
      n = lseek(p, 0, SEEK_SET);
  }
  write(1, "\rCompleted: 100%\n", 17);
  write(1, "\nFile Copied\n", 13);

  if(close(p) < 0)
  {
    perror("Exit Error");
    exit(1);
  }
  if(close(p1) < 0)
  {
    perror("Exit Error");
    exit(1);
  }
}

void printperc(long int n) 
{
  write(1, "\rCompleted: ", 12);
  strng(n);                                                         // Prints number as a string
}

void strng(long int n) 
{
  char c[SZ], ch[SZ];                                               
  long int i, j;
  for (i = 0; n > 0; n = n / 10, i++)
    c[i] = (char)((n % 10) + '0');                                  // Converts unit's digit to char
  c[i] = '\0';
  n = i;
  for (i = n - 1, j = 0; i >= 0; i--, j++)
    ch[j] = c[i];                                                   // Reverses the numbers
  ch[j++] = '%';
  ch[j] = '\0';
  write(1, ch, j);                                                  // Writes the number to stdout
}

void rev(char *c, char *ch, off_t n) 
{
  long int i, j;
  for (i = 0, j = n - 1; j >= 0; i++, j--)
    ch[i] = c[j];
  ch[i] = '\0';
}

void dir(char *c, char *ch) 
{
  long int i, j, k = 0;
  for (i = 0; c[i] != '\0'; i++);                                   // Moving to end of string i.e path
  c[i++] = '/';
  char buf[11] = "Assignment";                          
  for (j = 0; buf[j] != '\0'; j++)
    c[i++] = buf[j];                                                // Appending Assignment to path
  c[i++] = '/';
  
  for (j = 0; ch[j] != '\0'; j++)
  {
    if(ch[j] == '/')
      k = j;                                                        // Used to cut the filename out of the path
  }
  for(j = k; ch[j] != '\0'; j++)
    c[i++] = ch[j];                                                 // Appending filename to path
  c[i] = '\0';
}

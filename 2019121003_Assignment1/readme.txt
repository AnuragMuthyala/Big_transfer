Q1. Reversing the contents of a file and storing it in a directory

Input : Filename as command-line argument
Output : File with the same fileame as input stored in a directory called 'Assignment'.

1.) We read the file from the end in blocks of fixed size and reverse it.
2.) Then we write the reversed block into the new file.
3.) We continue this until there are no more characters to read.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Q2. Checking whether the file is reversed or not and also the permissions of new,old files and directory

Input : Paths to files and directory as command-line arguments in the order-new file,old file,directory
Output : Prints the permissions and whether the file is revered or not

1.) We read the starting and ending blocks of fixed length of the two files respectively.
2.) Then we reverse one of them and compare them both.
3.) If they are same for all blocks,then file has been reversed.
4.) The macros S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH are used for permissions.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# MemFiles
Structs and functions for replecating File structures in memory for faster testing
The memcp.h file contains the structs Folder and File an da few functions for manipulation of them in memory such as:
void* ValidPath which return the pointer to the file or folder if it exists and nullptr if not.
void write which writes data in the form of a std::vector<char> and creates a new file at the spesified location if it doesn't exist.
void DeleteFile does what it says on the tin.
and a few functions that are used to implement the above.
The aim is to eventualy rewrite it in C from c++ and remove a lot of the dependancy from the standard libraries especially std::vector as this was the main reason for writting in c++ over C.

#pragma once
#include <3ds.h>
#include <stdio.h>
#include <sys/dirent.h>
#include <string>
#include <vector>

//stol...ehm borrowed from d0k3's CTRXplorer
typedef struct {
    const std::string path;
    const std::string name;
} FileInfo;

#include "fs.h"

inline bool ends_with(std::string const & value, std::string const & ending);
std::vector<FileInfo> fsGetDirectoryContents(const std::string directory);
void loadCode(const char * path, std::string *runCode);
unsigned long OpenFile(const char * path, u8** buf);
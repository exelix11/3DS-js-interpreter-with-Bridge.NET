#include "fs.h"

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::vector<FileInfo> fsGetDirectoryContents(const std::string directory) {
    std::vector<FileInfo> result;
    bool hasSlash = directory.size() != 0 && directory[directory.size() - 1] == '/';
    const std::string dirWithSlash = hasSlash ? directory : directory + "/";

    DIR* dir = opendir(dirWithSlash.c_str());
    if(dir == NULL) {
        return result;
    }

    while (true) {
        struct dirent* ent = readdir(dir);
        if(ent == NULL) {
            break;
        }
		std::string fileName = std::string(ent->d_name);
		if (ends_with(fileName,".js"))
       		result.push_back({dirWithSlash + fileName, fileName});
    }

    closedir(dir);
    return result;
}

void loadCode(const char * path, std::string *runCode)
{
	FILE * file;
	file = fopen(path , "r");
	fseek(file, 0L, SEEK_END);
	unsigned long FileLen = ftell(file);
	fseek(file, 0L, SEEK_SET);
	char * str = new char[FileLen];
	fread (str,1,FileLen,file);
	runCode->assign (str,FileLen);
	fclose(file);
	delete[] str;
}

unsigned long OpenFile(const char * path, u8** buf)
{
	FILE * file;
	file = fopen(path , "r");
	fseek(file, 0L, SEEK_END);
	unsigned long FileLen = ftell(file);
	fseek(file, 0L, SEEK_SET);
	(*buf) = new u8[FileLen];
	fread (*buf,1,FileLen,file);
    return FileLen;
}
#include "jsFS.h"
#include "../fs.h"

std::vector<DukBuffer*> LoadedFiles;
DukBuffer ReadAllBytes(std::string Path)
{
    DukBuffer * res = new DukBuffer();
	LoadedFiles.push_back(res);
    res->size = (size_t)OpenFile(Path.c_str(),&res->ptr); //Bad design ?
	return *res;
}

extern duk_context *mainCtx; //from main.cpp
void FsFreeFile(const DukBuffer& file)
{
	for (std::vector<DukBuffer*>::iterator it = LoadedFiles.begin() ; it != LoadedFiles.end(); ++it)
	{
        if ((*it)->ptr == file.ptr)
        {
            delete[] (u8*)(file.ptr);
    	    delete *it;
		    LoadedFiles.erase(it);
            return;
        }
	}
    
    delete[] (u8*)(file.ptr);
}

void FsFreeAll()
{
	for (std::vector<DukBuffer*>::iterator it = LoadedFiles.begin() ; it != LoadedFiles.end(); ++it)
	{        
        delete[] (u8*)((*it)->ptr);
        delete *it;
	}
	LoadedFiles.clear();
}

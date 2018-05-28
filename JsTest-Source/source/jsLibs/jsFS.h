#pragma once
#include <stdio.h>
#include <iostream>
#include <3ds.h>
#include <string.h>
#include <vector>
#include <dukglue/dukvalue.h>

DukBuffer ReadAllBytes(std::string Path);
void FsFreeFile(const DukBuffer& file);
void FsFreeAll();
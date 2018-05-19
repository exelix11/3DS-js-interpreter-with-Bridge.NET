#pragma once
#define PPCAT_NX(A, B) A ## B

#define GETLIBFN(CLASSNAME) PPCAT_NX(Get,CLASSNAME)
#define FREELIBFN(CLASSNAME) PPCAT_NX(Free,CLASSNAME)

#define LIBDEFINE(CLASSNAME) \
CLASSNAME* PPCAT_NX(CLASSNAME,_ptr) = nullptr; \
CLASSNAME* GETLIBFN(CLASSNAME)() \
{ \
    if (!PPCAT_NX(CLASSNAME,_ptr)) PPCAT_NX(CLASSNAME,_ptr) = new CLASSNAME(); \
    return PPCAT_NX(CLASSNAME,_ptr);  \
} \
extern duk_context *mainCtx; \
void FREELIBFN(CLASSNAME)() \
{ \
    if (!PPCAT_NX(CLASSNAME,_ptr)) return; \
    dukglue_invalidate_object(mainCtx, PPCAT_NX(CLASSNAME,_ptr));  \
    delete PPCAT_NX(CLASSNAME,_ptr); \
    PPCAT_NX(CLASSNAME,_ptr) = nullptr; \
} 

#define EXTERN_LIBDEFINE(CLASSNAME) \
extern CLASSNAME* GETLIBFN(CLASSNAME)(); \
extern void FREELIBFN(CLASSNAME)(); 

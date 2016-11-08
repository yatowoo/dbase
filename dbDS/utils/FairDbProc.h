#ifndef FAIRDBPROC_H
#define FAIRDBPROC_H

#include <string>
#include "FairRegistry.h"


class FairDbProcEnv;
class FairDbProcCmd;


class FairDbProc
{
public:
    FairDbProc();
    virtual ~FairDbProc();

    const char* GetName() const;
    virtual const FairRegistry& DefaultConfig() const;
    virtual FairRegistry& GetConfig();
    virtual void          Config(const FairRegistry& r);
    virtual void Cmd(const char* command);
    virtual void HandleCommand(FairDbProcCmd *command);
    virtual void Reset();
    virtual void Help();
    void              Init(const char* name);    

 private:
    std::string fName;  
    FairRegistry    fConfig; 
};

#endif // FAIRDBPROC_H


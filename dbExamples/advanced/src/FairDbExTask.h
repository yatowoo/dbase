#ifndef FAIRDBEXTASK_H
#define FAIRDBEXTASK_H

#include "FairTask.h"                   // for FairTask, InitStatus

#include "Rtypes.h"                     // for ClassDef

#include <stddef.h>                     // for NULL

class FairDbExPar4;

class FairDbExTask : public FairTask
{
  public:

    /** Default constructor **/
    FairDbExTask() : FairTask(), fTutParDef(NULL), fTutParAlt(NULL) {};

    /** Destructor **/
    virtual ~FairDbExTask() {};


    /** Virtual method Init **/
    virtual void SetParContainers();
    virtual InitStatus Init();
    virtual InitStatus ReInit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);
    /** Virtual method Finish **/
    virtual void Finish();

    FairDbExPar4* fTutParDef;
    FairDbExPar4* fTutParAlt;

    FairDbExTask(const FairDbExTask&);
    FairDbExTask& operator=(const FairDbExTask&);

    ClassDef(FairDbExTask,0);
};

#endif

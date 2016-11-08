#ifndef FAIRDBPROCPROXY_H
#define FAIRDBPROCPROXY_H

class FairDbProc;

class FairDbProcProxy 
{
public:
  FairDbProcProxy();
  virtual ~FairDbProcProxy();

  const char *GetName() const;
  const char *GetFunc() const;
  void        Help();

  virtual FairDbProc *CreateModule() = 0;

protected:
  const char *fName; //
  const char *fFunc; //
};

#endif

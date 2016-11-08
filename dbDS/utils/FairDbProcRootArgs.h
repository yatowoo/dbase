#ifndef FAIRDBPROCROOTARGS_H
#define FAIRDBPROCROOTARGS_H

class FairDbProcRootArgs
{
public:
  // FairDbProcRootArgs();
  FairDbProcRootArgs(int argc, char** argv);
  ~FairDbProcRootArgs();

  bool HaveBatchOpt() { return fHaveBatchOpt; }
  bool HaveQuitOpt()  { return fHaveQuitOpt;  }
  bool HaveInaccessibleFile() { return fHaveInaccessibleFile; }

  int RunTheApp();

private:
  void InterpreterConfig();
  void SignalConfig();
  void LoadIncludes();
  void LoadClasses();
  
private:
  bool fHaveBatchOpt;   
  bool fHaveQuitOpt;    
  bool fHaveMacroFiles; 
  bool fHaveInaccessibleFile;
};

#endif // FAIRDBPROCROOTARGS_H

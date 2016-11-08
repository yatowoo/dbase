#ifndef FAIRDBPROCARGS_H
#define FAIRDBPROCARGS_H

#include <vector>
#include <string>
#include <ctime>

class FairDbProcRootArgs;

class FairDbProcArgs
{
public:
  ~FairDbProcArgs();
  
  int           GetArgc()                    const;
  char* const*  GetArgv()                    const;
  const char*   GetArgv(int i)               const;
  //
  const char*   GetModuleHelpList()          const;
  const char*   GetMacroFile(unsigned int i) const;
  const char*   GetFileName(int i)           const;
  int           GetNfile()                   const;
  const char*   GetDefaultOutputFileName()   const;
  bool          HaveInaccessibleFile() const;
  // Run limits
  bool ContinueRun(int n)      const;
  bool CheckTimeLimit(int* t)  const;
  bool CheckRecordLimit(int n) const;  
  bool IsBatch() const;
  // Root appl.
  int RunRootApp();
  
  static FairDbProcArgs& Instance();
  static FairDbProcArgs& Instance(int argc, char** argv);
  

  struct Cleaner {
    void ClassIsUsed() { }; //
    ~Cleaner() {
      if (FairDbProcArgs::fInstance) {
	delete FairDbProcArgs::fInstance;
	FairDbProcArgs::fInstance = 0;
      }
    }
  };
  friend struct Cleaner;
  
 private:
  FairDbProcArgs();
  FairDbProcArgs(int argc, char** argv);

  void AddDataFile(const char *filename);
  void AddMacroFile(const char *filelist);
  void CreateInteractiveMom();
  void DeleteInteractiveMom();
  void ProcessCommandLine(int argc, char *const *argv);
  void SetTimeLimit(const char* timelimit);
  void SetSignalHandlers();
  
  //
  int                      fArgc;          
  char* const*             fArgv;          
  const char*              fModuleHelpList;
  std::vector<std::string> fDataFileList;  
  std::vector<std::string> fMacroFileList; 
  std::string              fDfltOutFile;   

  // Information about run limits
  time_t                   fTimeStart;     
  double                   fTimeLimit;     
  int                      fRecordLimit;   
  // Batch mode
  bool                     fIsBatch;       // Is this a batch job?


  FairDbProcRootArgs*       fRootEnv;
  
  static FairDbProcArgs* fInstance;
};

#endif // FAIRDBPROCARGS


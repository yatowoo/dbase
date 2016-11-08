#ifndef FAIRDBPROCCMD_H
#define FAIRDBPROCCMD_H


#include <vector>
#include <string>

class FairRegistry;

using namespace std;

//
// <DB>  General Processor command class
//       ... do not need to be Rootified
//

class FairDbProcCmd 
{
  friend ostream& operator<<(ostream& os, const FairDbProcCmd& jc);
public:
  FairDbProcCmd();
  FairDbProcCmd(const char *cmd);
  ~FairDbProcCmd();

  bool HaveCmd() const;
  bool HaveOpt() const;
  
  const char *PopCmd();
  const char *PopOpt();
  const char *PushCmd();
  const char *PushOpt();
  int         PopIntOpt();
  double      PopFloatOpt();

  static void SplitLine(const char *line, char sep, string& a, string& b);
  static void StringTok(std::vector<std::string>& ls,
			const std::string& str,
			const std::string& tok);
  static bool IsBool(const char* value);
  static bool IsInt(const char* s);
  static bool IsFloat(const char* s);
  static void StringToRegistry(FairRegistry& r, const char* s);
  static bool atob(const char* value);

private:
  // Methods
  void Parse(const char *cmd);

  // Data
  vector<string>           fCmdList;    
  vector<string>::iterator fItrCmdList; 
  vector<string>           fOptList;    
  vector<string>::iterator fItrOptList; 
};
#endif // FAIRDBPROCCMD_H

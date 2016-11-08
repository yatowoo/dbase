#ifndef FAIRDBCMDOPTS_H
#define FAIRDBCMDOPTS_H


#include <memory>
#include <map>
#include <list>
#include <string>
#include <string.h>

#include <TObject.h>

using namespace std;

class FairDbProcCmd;

class FairDbCmdOpts : public TObject {

public:
  FairDbCmdOpts(const string& validOpts = "",
		FairDbProcCmd* pcmd = 0,
                const list<string>* defaultOpts =0);

  virtual ~FairDbCmdOpts();
  //
  void  GetOptIntList(const string& opt, list<int>& nos) const;
  Int_t GetOptInt(const string& opt) const;
  string GetOptString(const string& opt) const;
  string GetOpts() const;
  Bool_t IsValid() const { return fIsValid; }
  Bool_t IsValid(const string& opt) const;
  UInt_t NumOpts() const { return fOptsMap.size(); }
  Bool_t TestOpt(const string& opt) const;
  // 
  Bool_t AddOpt(const string& opt, FairDbProcCmd& pcmd);
  Bool_t AddOpt(const string& opt, const string& val);
  Bool_t ProcessCmd(FairDbProcCmd& pcmd,
                    const list<string>* defaultOpts =0);
  
  // 
  Bool_t fIsValid;            
  string fValidOpts;     
  map<string,string> fOptsMap; 
                              
  ClassDef(FairDbCmdOpts,0)    // list of cmds options
};
#endif // FAIRDBCMDOPTS

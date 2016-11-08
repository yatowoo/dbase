#include "FairDbArgMacFile.h"

TString FairDbArgMacFile(const char* arg)
{
  int len = strlen(arg);

  // simple endings
  if ( strcmp(arg+len-2,".C")   == 0 ) return TString(arg);
  if ( strcmp(arg+len-2,".c")   == 0 ) return TString(arg);
  if ( strcmp(arg+len-3,".cc")  == 0 ) return TString(arg);
  if ( strcmp(arg+len-4,".cxx") == 0 ) return TString(arg);

  // ACLiC invocation
  if ( strcmp(arg+len-3,".C+")  == 0 ) return TString(arg,len-1);
  if ( strcmp(arg+len-4,".C++") == 0 ) return TString(arg,len-2);

  // macro with supplied arguments
  if ( strcmp(arg+len-1,")") == 0 ) {
    const char* argstart = strstr(arg,"(");
    len = argstart-arg;

    // normal file endings, before the arguments
    if ( strncmp(arg+len-2,".C",2)   == 0 ) return TString(arg,len);
    if ( strncmp(arg+len-2,".c",2)   == 0 ) return TString(arg,len);
    if ( strncmp(arg+len-3,".cc",3)  == 0 ) return TString(arg,len);
    if ( strncmp(arg+len-4,".cxx",4) == 0 ) return TString(arg,len);
    
    // ACLiC invocation before the arguments
    if ( strncmp(arg+len-3,".C+",3)  == 0 ) return TString(arg,len-1);
    if ( strncmp(arg+len-4,".C++",4) == 0 ) return TString(arg,len-2);

  }
  // Nothing correct found
  return "";
}

#include "FairDbUtilType.h"



std::string FairDbUtilType::demangle(const char* name) {
  // define some arbitrary value
  // to eliminate the compiler warning
  int status = -4; 
    
  std::unique_ptr<char, void(*)(void*)> res {
    abi::__cxa_demangle(name, NULL, NULL, &status),
      std::free
      };
    
  return (status==0) ? res.get() : "" ;
}

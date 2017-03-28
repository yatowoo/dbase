#ifndef CBMSTSWTQAWAPPLICATION_H
#define CBMSTSWTQAWAPPLICATION_H

#include "FairDbWtApplication.h"


namespace StsWt{

class CbmStsWtQaWApplication : public FairDbWtApplication
{  
 public:
  CbmStsWtQaWApplication(const WEnvironment& env);    
  virtual  ~CbmStsWtQaWApplication();
  
 protected:
  virtual void initialize();
  virtual void finalize();
  
};  

}

#endif

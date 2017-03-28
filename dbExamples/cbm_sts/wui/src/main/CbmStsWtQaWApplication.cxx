#include "CbmStsWtQaWApplication.h"
#include "CbmStsWtQaMainWidget.h"
#include "WBootstrapTheme"
#include <iostream>

namespace StsWt{
  
using namespace std;
using namespace Wt;


CbmStsWtQaWApplication::CbmStsWtQaWApplication(const WEnvironment& env)
  : FairDbWtApplication(env)
{
  Wt::log("debug") << "CbmStsWtQaWApplication::ctor()";
}

CbmStsWtQaWApplication::~CbmStsWtQaWApplication()
{
  Wt::log("debug") << "CbmStsWtQaWApplication::dtor()";
}

void CbmStsWtQaWApplication::initialize()
{
  Wt::log("debug") << "CbmStsWtQaWApplication::initialize()";
  WApplication::initialize();
}

void CbmStsWtQaWApplication::finalize()
{
  Wt::log("debug") << "CbmStsWtQaWApplication::finalize()";
  WApplication::finalize();
}

}

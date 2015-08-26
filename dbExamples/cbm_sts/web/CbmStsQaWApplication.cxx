#include "CbmStsQaWApplication.h"
#include "CbmStsQaWMainWidget.h"
#include "WBootstrapTheme"

#include <iostream>

using namespace std;

CbmStsQaWApplication::CbmStsQaWApplication(const WEnvironment& env)
  : FairDbWtApplication(env)
{
cout << "-I CbmStsQaWApplication ctor called ...  " << endl; 
}


void CbmStsQaWApplication::initialise()
{
 cout << "-I- CbmStsQaWApplication user initialisation" << endl;

  // Title of Application
    setTitle("Sts Qa Web Interface");
    setCssTheme("polished");  
    CbmStsQaWMainWidget* w_main = new CbmStsQaWMainWidget(root());
}

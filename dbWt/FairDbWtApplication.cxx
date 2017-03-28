#include <iostream>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>


#include "FairDbWtApplication.h"
#include "FairDbWtMainWidget.h"

using namespace std;

FairDbWtApplication::FairDbWtApplication(const WEnvironment& env)
  : WApplication(env)
{
  
  //  cout << "-I- FairDbWtApplication ctor " << endl;
  
}

FairDbWtApplication::~FairDbWtApplication()
{
  
  //  cout << "-I- ~FairDbWtApplication()" << endl;
  
}

/*
void FairDbWtApplication::initialise()
{
  // Title of Application
  cout << "-I- FairDbWtApplication user initialisation" << endl;
  setTitle("FairDB Wt");
  setCssTheme("polished");  
  FairDbWtMainWidget *w_main = new FairDbWtMainWidget(root());
}
*/

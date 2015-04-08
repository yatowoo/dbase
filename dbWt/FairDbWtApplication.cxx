#include <iostream>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WPushButton>


#include "FairDbWtApplication.h"
#include "FairDbWtMainWidget.h"


FairDbWtApplication::FairDbWtApplication(const WEnvironment& env)
  : WApplication(env)
{
  // Title of Application
  setTitle("FairDB Wt");
  setCssTheme("polished");  
  new FairDbWtMainWidget(root());

}



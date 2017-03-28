#include "FairDbWtMainWidget.h"
#include <iostream>

using namespace std;
using namespace Wt;

FairDbWtMainWidget::FairDbWtMainWidget(WContainerWidget *parent):
  WContainerWidget(parent)
  ,fNavigation(NULL)
  ,fContentstack(NULL)
  ,fMenu(NULL)
{
  Wt::log("debug") << "FairDbWtMainWidget::ctor()";
}


FairDbWtMainWidget::~FairDbWtMainWidget()
{
  Wt::log("debug") << "FairDbWtMainWidget()::dtor() ";
  if (fNavigation)
    {
      delete fNavigation;
      fNavigation=NULL;
    } 
}



WMenuItem *FairDbWtMainWidget::addToMenu(WMenu *menu,
                                         const WString& name,
                                         WContainerWidget *topic)
{
  
  WContainerWidget *result = new WContainerWidget();
  WContainerWidget *pane = new WContainerWidget();
  
  WVBoxLayout *vLayout = new WVBoxLayout(result);
  vLayout->setContentsMargins(0, 0, 0, 0);
  vLayout->addWidget(topic);
  vLayout->addWidget(pane, 1);
  
  WHBoxLayout *hLayout = new WHBoxLayout(pane);
  WMenuItem *item = new WMenuItem(name, result);
  item->addStyleClass("navigstyle");
  menu->addItem(item);
  
  return item;
  
}


void FairDbWtMainWidget::clearall()
{
  // remove all widgets
  clear();
  // reset the pointers
  fNavigation=NULL;
  fContentstack=NULL;
  fMenu=NULL;
}

#include "FairDbWtMainWidget.h"
#include "FairDbWtConnMySQLWidget.h"
#include "FairDbWtTreeView.h"
//#include "FileTreeTable.h"

#include <iostream>


using namespace std;
using namespace Wt;

FairDbWtMainWidget::FairDbWtMainWidget(WContainerWidget *parent):
  WContainerWidget(parent)
{
 // Create the basic widgets   
}


FairDbWtMainWidget::~FairDbWtMainWidget(){
}



WMenuItem *FairDbWtMainWidget::addToMenu(WMenu *menu,
                                        const WString& name,
                                        WContainerWidget *topic)
{
//menu->addItem("Home", new Wt::WText("There is no better place!"));
//menu->addItem("Layout", new Wt::WText("Layout contents"));


  WContainerWidget *result = new WContainerWidget();
  WContainerWidget *pane = new WContainerWidget();

  WVBoxLayout *vLayout = new WVBoxLayout(result);
  vLayout->setContentsMargins(0, 0, 0, 0);
  vLayout->addWidget(topic);
  vLayout->addWidget(pane, 1);

  WHBoxLayout *hLayout = new WHBoxLayout(pane);

  WMenuItem *item = new WMenuItem(name, result);
  menu->addItem(item);

  WStackedWidget *subStack = new WStackedWidget();
  subStack->addStyleClass("contents");
  subStack->setOverflow(WContainerWidget::OverflowAuto);
  
  WAnimation animation(WAnimation::Fade,
         WAnimation::Linear,
         100);
  subStack->setTransitionAnimation(animation, true);
  

  WMenu *subMenu = new WMenu(subStack);
  subMenu->addStyleClass("nav-pills nav-stacked submenu");
  subMenu->setWidth(200);

  hLayout->addWidget(subMenu);
  hLayout->addWidget(subStack,1);

  subMenu->setInternalPathEnabled();
  subMenu->setInternalBasePath("/" + item->pathComponent());

 
  return item;
}



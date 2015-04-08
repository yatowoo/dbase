#include <WAnchor>
#include <WText>
#include <WMenu>
#include <WStackedWidget>
#include <WVBoxLayout>
#include <WHBoxLayout>
#include <WNavigationBar>

#include <WApplication>
#include <Auth/AuthWidget>


#include "FairDbWtMainWidget.h"

#include "FairDbWtConnMySQLWidget.h"
#include "FairDbWtTreeView.h"
#include "FileTreeTable.h"

using namespace Wt;

FairDbWtMainWidget::FairDbWtMainWidget(WContainerWidget *parent):
  WContainerWidget(parent)
{
  fNavigation = new WNavigationBar();
  fNavigation->addStyleClass("main-nav");
  fNavigation->setTitle("FairDb Connections","");
  fNavigation->setResponsive(false);

  fContentstack = new WStackedWidget();

  WAnimation animation(WAnimation::Fade,
                       WAnimation::Linear,
                       200);
  fContentstack->setTransitionAnimation(animation, true);

  WMenu *menu = new WMenu(fContentstack, 0);
  menu->setInternalPathEnabled();
  menu->setInternalBasePath("/");

  
  addToMenu(menu, "MySQL@demac013", new FairDbWtTreeView());
  addToMenu(menu, "PgSQL@demac013", new FairDbWtConnMySQLWidget());

   //   ->setPathComponent("trees-tables");
     
  fNavigation->addMenu(menu);

  // Add it inside Layout
  WVBoxLayout *layout = new WVBoxLayout(this);
  layout->addWidget(fNavigation);
  layout->addWidget(fContentstack, 1);
  layout->setContentsMargins(0, 0, 0, 0);

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
  menu->addItem(item);

  WStackedWidget *subStack = new WStackedWidget();
  subStack->addStyleClass("contents");
  subStack->setOverflow(WContainerWidget::OverflowAuto);
  /*
  WAnimation animation(WAnimation::Fade,
         WAnimation::Linear,
         100);
  subStack->setTransitionAnimation(animation, true);
  */

  WMenu *subMenu = new WMenu(subStack);
  subMenu->addStyleClass("nav-pills nav-stacked submenu");
  subMenu->setWidth(200);

  hLayout->addWidget(subMenu);
  hLayout->addWidget(subStack,1);

  subMenu->setInternalPathEnabled();
  subMenu->setInternalBasePath("/" + item->pathComponent());

  // Should be here new inheritance
  //((FairDbWtConnMySQLWidget*) topic)->populateSubMenu(subMenu);

  return item;
}



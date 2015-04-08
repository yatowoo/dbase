#ifndef FAIRDBWTMAINWIDGET_H
#define FAIRDBWTMAINWIDGET_H

#include <WContainerWidget>

using namespace Wt;

class FairDbWtMainWidget : public WContainerWidget
{

public:
  FairDbWtMainWidget(WContainerWidget *parent = 0);
  
private:
 WNavigationBar *fNavigation;
 WStackedWidget *fContentstack;

 WMenuItem *addToMenu(WMenu *menu,
                      const WString& name,
                      WContainerWidget *topic);
  
};

#endif //FAIRDBWTMAINWIDGET_H

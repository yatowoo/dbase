#ifndef FAIRDBWTMAINWIDGET_H
#define FAIRDBWTMAINWIDGET_H

#include <WContainerWidget>
#include <WAnchor>
#include <WText>
#include <WMenu>
#include <WStackedWidget>
#include <WVBoxLayout>
#include <WHBoxLayout>
#include <WNavigationBar>
#include <WApplication>
#include <Auth/AuthWidget>



using namespace Wt;

class FairDbWtMainWidget : public WContainerWidget
{
public:
  FairDbWtMainWidget(WContainerWidget *parent = 0);
  virtual ~FairDbWtMainWidget();
   
  WNavigationBar* GetNavigationBar(){return fNavigation;}
  WStackedWidget *GetContentstack(){return fContentstack;}


  WMenuItem *addToMenu(WMenu *menu,
                       const WString& name,
                       WContainerWidget *topic);

  void clearall();

protected:
 WNavigationBar *fNavigation;
 WStackedWidget *fContentstack;
 WMenu * fMenu; 

};

#endif //FAIRDBWTMAINWIDGET_H

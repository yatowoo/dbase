#ifndef  FAIRDBWTCONNMYSQLWIDGET_H
#define  FAIRDBWTCONNMYSQLWIDGET_H

#include <WContainerWidget>
#include <WStandardItemModel>

#include <WMenu>
#include <WText>

using namespace Wt;

class FairDbWtConnMySQLWidget : public WContainerWidget
{
public:
  FairDbWtConnMySQLWidget();

  void populateSubMenu(Wt::WMenu *menu);

static WString reindent(const WString& text);

protected:
  static WText *addText(const WString& text,
         WContainerWidget *parent = 0);

private:
  WWidget *introduction();
  WText *fText; 
};

#endif //FAIRDBWTCONNMYSQLWIDGET_H 

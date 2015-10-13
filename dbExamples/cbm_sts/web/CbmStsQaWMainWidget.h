#ifndef CBMSTSQAWMAINWIDGET_H
#define CBMSTSQAWMAINWIDGET_H

#include "FairDbWtMainWidget.h"

#include "Session.h"

namespace Wt {
  class WStackedWidget;
  class WAnchor;
}

class Session;

class CbmStsQaWMainWidget : public FairDbWtMainWidget
{
public:
  CbmStsQaWMainWidget(WContainerWidget *parent = 0);
  virtual ~CbmStsQaWMainWidget();

  void showMainWidget();

private:
  Session  fSession;
  Wt::WAnchor *fMainAnchor;
  Wt::WStackedWidget *fMainStack;
  Auth::AuthWidget* authWidget;

  void onAuthEvent();
 
};

#endif //CBMSTSQAWMAINWIDGET_H


#ifndef CBMSTSWTQAMAINWIDGET_H
#define CBMSTSWTQAMAINWIDGET_H

#include "FairDbWtMainWidget.h"
#include <Session.h>


namespace Wt {
  class WStackedWidget;
  class WAnchor;
}

namespace Layout {
  
  class CbmStsWtQaMainWidget : public FairDbWtMainWidget
  {
  public:
    CbmStsWtQaMainWidget(WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaMainWidget();
    
    void showMainWidget();
    
  private:
    Session  fSession;
    Wt::WAnchor *fMainAnchor;
    Auth::AuthWidget* authWidget;
    
    void onAuthEvent();
    
  };
} 

#endif //CBMSTSWTQAMAINWIDGET_H


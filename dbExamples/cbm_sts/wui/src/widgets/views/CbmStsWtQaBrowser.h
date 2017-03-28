#ifndef CBMSTSWTQABROWSER_H
#define CBMSTSWTQABROWSER_H

#include <WGridLayout>
#include <WVBoxLayout>
#include <WToolBar>
#include <WContainerWidget>
#include <string>

#include "CbmStsWtQaBaseView.h"

using namespace Wt;
using namespace std;

namespace Views {
  
  class CbmStsWtQaFolderView;
  class CbmStsWtQaPresenterView;  
  
  class  CbmStsWtQaBrowser : public CbmStsWtQaBaseView
  {    
  public:
    CbmStsWtQaBrowser(WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaBrowser();
    
    void createLayout(WContainerWidget* parent = 0);
    void setEvents();
    
  protected:
    WGridLayout                     *fMainLayout;
    CbmStsWtQaFolderView            *fFolderView;
    CbmStsWtQaPresenterView         *fPresenterView;
    CbmStsWtQaSigStr                *fASignal;                
  };
  
}

#endif //CBMSTSWTQABROWSER_H

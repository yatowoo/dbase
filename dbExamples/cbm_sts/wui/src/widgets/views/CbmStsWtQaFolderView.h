#ifndef CBMSTSWTQAFOLDERVIEW_H
#define CBMSTSWTQAFOLDERVIEW_H

#include <string>

#include <WToolBar>
#include <WPopupMenu>
#include <WBreak>
#include <WMenuItem>
#include <WMessageBox>
#include <WPopupMenu>
#include <WPushButton>
#include <WText>
#include <WTemplate>
#include <WContainerWidget>
#include <WVBoxLayout>
#include <WHBoxLayout>

#include "CbmStsWtQaBaseView.h"
#include "CbmStsWtQaFolderTreeView.h"

using namespace Wt;
using namespace std;

namespace Views{
  
  class  CbmStsWtQaFolderView : public CbmStsWtQaBaseView
  {
    
  public:
    CbmStsWtQaFolderView(WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaFolderView();

    void createLayout(WContainerWidget* parent = 0);
    void createMenus(WContainerWidget* parent = 0);

    void         init();
    virtual void setSignal(CbmStsWtQaSigStr* sig, signal_t type);

  protected:
    Wt::WVBoxLayout   *fVbox;
    Wt::WPopupMenu    *fPopup;
    Wt::WPopupMenu    *fSubMenu;
    Wt::WMenu         *fLeftMenu;
    Wt::WText         *fStatus;
    Wt::WText         *fOut;
    
    CbmStsWtQaFolderTreeView        *fFolderTreeView; 
    
  };
  
}

#endif //CBMSTSWTQAFOLDERVIEW_H

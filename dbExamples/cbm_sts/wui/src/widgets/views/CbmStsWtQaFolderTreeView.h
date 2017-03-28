#ifndef CBMSTSWTQAFOLDERTREEVIEW_H
#define CBMSTSWTQAFOLDERTREEVIEW_H

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
#include <WHBoxLayout>
#include <WVBoxLayout>
#include <string>

#include "CbmStsWtQaBaseView.h"

using namespace Wt;
using namespace std;

namespace Layout
{
  class CbmStsWtQaTreeTable; 
}

namespace Views{
  
  class  CbmStsWtQaFolderTreeView : public CbmStsWtQaBaseView
  {    
  public:
    CbmStsWtQaFolderTreeView(WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaFolderTreeView();
    
    void createLayout(WContainerWidget* parent = 0);
    void initDataObjects();
    
    // Event handling
    void setEventSettings();
    void showPopup(const WMouseEvent& event);
    void selectedItem(WMenuItem* mItem);
    void showContent();
   
    
  protected:
    Wt::WVBoxLayout               *fVbox;
    WPopupMenu                    *fPopup;
    Layout::CbmStsWtQaTreeTable   *fTreeTable;
  };
  
}

#endif //CBMSTSWTQAFOLDERTREEVIEW_H

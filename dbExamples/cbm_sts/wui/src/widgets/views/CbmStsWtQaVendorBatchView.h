#ifndef CBMSTSWTQAVENDORBATCHVIEW_H
#define CBMSTSWTQAVENDORBATCHVIEW_H


#include <WToolBar>
#include <WPopupMenu>
#include <WBreak>
#include <WMenuItem>
#include <WMessageBox>
#include <WPopupMenu>
#include <WPushButton>
#include <WText>
#include <WContainerWidget>
#include <WTemplate>
#include <WTabWidget>
#include <WHBoxLayout>
#include <WVBoxLayout>

#include <WTableView>
#include <WPopupMenu>
#include <WMessageBox>
#include <WModelIndex>
#include <WAbstractTableModel>

#include <vector>
#include <string>

#include "CbmStsWtQaBaseView.h"
#include "CbmStsWtQaPresenterView.h"

using namespace Wt;
using namespace std;

namespace Views{
  
  class  CbmStsWtQaVendorBatchView : public CbmStsWtQaBaseView
  {
    
  public:
    CbmStsWtQaVendorBatchView(WContainerWidget *pView, const string& vendor, const string& batch_id, WContainerWidget *parent = 0);
    virtual ~CbmStsWtQaVendorBatchView();

    void init(); 
    void createLayout(const string& vendor, const string& batch_id, WContainerWidget *parent = 0);

    // Features Drawing  
    void displayIV(const WModelIndexSet& aSet);
    
    // Event notify
    void editFile(const WModelIndex& item, const WMouseEvent& event);
    void editFiled(const WModelIndex& item, WAbstractItemModel* model);

    // Menus
    void showPopup(const WModelIndex& item, const WMouseEvent& event);
    void popupAction();
    void dialogDone();
  
  protected:
    WPopupMenu    *fPopup;
    WMessageBox   *fPopupActionBox;
    WTableView    *fTableView;
    
    CbmStsWtQaPresenterView* fPView;
  };
  
}

#endif //CBMSTSWTQAVENDORBATCHIEW_H

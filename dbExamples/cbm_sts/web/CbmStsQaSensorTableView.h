#ifndef CBMSTSQASENSORTABLEVIEW
#define CBMSTSQASENSORTABLEVIEW

#include <WContainerWidget>
#include "CbmStsQaSensorDataModel.h"
#include "CbmStsQaWTreeView.h"

#include <WTableView>
#include <WPopupMenu>
#include <WMessageBox>
#include <WModelIndex>

#include <vector>


using namespace Wt;

class CbmStsQaSensorTableView : public WContainerWidget
{
public:
  CbmStsQaSensorTableView(WContainerWidget *parent, string pName);

// Event notify
  void editFile(const WModelIndex& item, const WMouseEvent& event); 
  void editFiled(const WModelIndex& item, WAbstractItemModel* model); 

// Menus
  void showPopup(const WModelIndex& item, const WMouseEvent& event); 
  void popupAction();
  void displayIV(const WModelIndexSet& aSet); 
  void dialogDone(); 

 
private:
WPopupMenu *fPopup;
WMessageBox *fPopupActionBox;
CbmStsQaWTreeView *fView;
WTableView* fTableView;

};


#endif //!CBMSTSQASENSORTABLEVIEW

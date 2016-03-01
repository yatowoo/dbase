#ifndef CBMSTSQASENSORRESULTSVIEW
#define CBMSTSQASENSORRESULTSVIEW

#include <WContainerWidget>
#include "CbmStsQaSensorResultsDataModel.h"
#include "CbmStsQaWTreeView.h"

#include <WTableView>
#include <WPopupMenu>
#include <WMessageBox>
#include <WModelIndex>

#include <vector>


using namespace Wt;

class CbmStsQaSensorResultsView : public WContainerWidget
{
public:
  CbmStsQaSensorResultsView(WContainerWidget *parent, string pName);
  CbmStsQaSensorResultsView(WContainerWidget *parent, string pName, vector<int>& sid);
  virtual ~CbmStsQaSensorResultsView();

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


#endif //!CBMSTSQASENSORRESULTSVIEW

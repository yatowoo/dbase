#ifndef CBMSTSQASENSORGEOVIEW
#define CBMSTSQASENSORGEOVIEW

#include <WContainerWidget>
#include "CbmStsQaSensorGeoDataModel.h"
#include "CbmStsQaWTreeView.h"

#include <WTableView>
#include <WPopupMenu>
#include <WMessageBox>
#include <WModelIndex>

#include <vector>


using namespace Wt;

class CbmStsQaSensorGeoView : public WContainerWidget
{
public:
  CbmStsQaSensorGeoView(WContainerWidget *parent, string pName);
  CbmStsQaSensorGeoView(WContainerWidget *parent, string pName, vector<int>& sid);
  virtual ~CbmStsQaSensorGeoView();

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


#endif //!CBMSTSQASENSORGEOVIEW

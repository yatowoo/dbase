#ifndef CBMSTSQASENSORTABLEVIEWNEW
#define CBMSTSQASENSORTABLEVIEWNEW

#include <WContainerWidget>
#include "CbmStsQaSensorDataModelNew.h"
#include "CbmStsQaWTreeView.h"

#include <WTableView>
#include <WPopupMenu>
#include <WMessageBox>
#include <WModelIndex>

#include <vector>


using namespace Wt;

class CbmStsQaSensorTableViewNew : public WContainerWidget
{
public:
  CbmStsQaSensorTableViewNew(WContainerWidget *parent, string pName);
  virtual ~CbmStsQaSensorTableViewNew();

  // Event notify
  void editFile(const WModelIndex& item, const WMouseEvent& event);
  void editFiled(const WModelIndex& item, WAbstractItemModel* model);

  // Menusg
  void showPopup(const WModelIndex& item, const WMouseEvent& event);
  void popupAction();
  void displayIV(const WModelIndexSet& aSet);
  void displayGeo(const WModelIndexSet& aSet);
  void displayOwner(const WModelIndexSet& aSet);
  void displayResults(const WModelIndexSet& aSet);
  void displayChannelMap(const WModelIndexSet& aSet);

  void dialogDone();


private:
  WPopupMenu *fPopup;
  WMessageBox *fPopupActionBox;
  CbmStsQaWTreeView *fView;
  WTableView* fTableView;
  Int_t fOption;
};


#endif //!CbmStsQaSensorTableViewNew

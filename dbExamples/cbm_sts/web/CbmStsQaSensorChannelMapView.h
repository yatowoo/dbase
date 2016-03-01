#ifndef CBMSTSQASENSORIVVIEW
#define CBMSTSQASENSORIVVIEW

#include <WContainerWidget>
#include "CbmStsQaSensorIvModel.h"
#include "CbmStsQaWTreeView.h"

#include <WPopupMenu>
#include <WMessageBox>



using namespace Wt;

class CbmStsQaSensorIvView : public WContainerWidget
{
public:
  CbmStsQaSensorIvView(WContainerWidget *parent, string pName);

// Event notify
  void editFile(const WModelIndex& item); 
  void editFiled(const WModelIndex& item, WAbstractItemModel* model); 

// Menus
  void showPopup(const WModelIndex& item, const WMouseEvent& event); 
  void popupAction();
  void displayIV(); 
  void dialogDone(); 

private:
WPopupMenu *fPopup;
WMessageBox *fPopupActionBox;
CbmStsQaWTreeView *fView;
};


#endif //!CBMSTSQASENSORIVVIEW

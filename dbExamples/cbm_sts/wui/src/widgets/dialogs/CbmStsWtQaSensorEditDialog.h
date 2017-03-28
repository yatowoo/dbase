#ifndef CBMSTSWTQASENSOREDITDIALOG
#define CBMSTSWTQASENSOREDITDIALOG

#include <WAbstractTableModel>
#include <WStandardItem>
#include <WString>
#include <WComboBox>
#include <WContainerWidget>
#include <WDatePicker>
#include <WDateValidator>
#include <WDialog>
#include <WEnvironment>
#include <WIntValidator>
#include <WItemDelegate>
#include <WLabel>
#include <WLineEdit>
#include <WMessageBox>
#include <WPushButton>
#include <WRegExpValidator>
#include <WGridLayout>
#include <WPopupMenu>
#include <WSortFilterProxyModel>
#include <WStandardItem>
#include <WStandardItemModel>
#include <WTableView>
#include <WTreeView>
#include <WText>
#include <WVBoxLayout>

#include <iostream>

using namespace Wt;
using namespace std;

class CbmStsWtQaSensorEditDialog : public WDialog
{

public:

  CbmStsWtQaSensorEditDialog(WAbstractItemModel* model, const WModelIndex& item);
 
private:
  WAbstractItemModel *model_;
  WModelIndex         item_;

  WLineEdit *nameEdit_, *sizeEdit_;
  WComboBox *typeEdit_;
  WDatePicker *createdPicker_, *modifiedPicker_;

  void handleFinish(DialogCode result);
 
};

#endif //!CBMSTSWTQASENSOREDITDIALOG


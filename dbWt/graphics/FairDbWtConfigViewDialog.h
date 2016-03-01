#ifndef FAIRDBWTCONFIGVIEWDIALOG_H
#define FAIRDBWTCONFIGVIEWDIALOG_H

#include <fstream>

#include <WComboBox>
#include <WContainerWidget>
#include <WDatePicker>
#include <WDateValidator>
#include <WDialog>
#include <WIntValidator>
#include <WItemDelegate>
#include <WLabel>
#include <WLineEdit>
#include <WMessageBox>
#include <WPushButton>
#include <WRegExpValidator>
#include <WGridLayout>
#include <WTabWidget>
#include <WPopupMenu>
#include <WSortFilterProxyModel>
#include <WStandardItem>
#include <WStandardItemModel>
#include <WTableView>
#include <WTreeView>
#include <WText>
#include <WVBoxLayout>



using namespace Wt;


class FairDbWtConfigViewDialog : public WDialog
{
public:
  FairDbWtConfigViewDialog(WTabWidget* aTab);

private:
  WLineEdit *fNameEdit;
  WLineEdit *fRowEdit, *fColEdit;
  WTabWidget* fTabWidget;

  void handleFinish(DialogCode result);


};







#endif //FAIRDBWTCONFIGVIEWDIALOG_H

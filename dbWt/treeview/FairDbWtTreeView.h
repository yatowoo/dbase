#ifndef FAIRDBWTTREEVIEW_H
#define FAIRDBWTTREEVIEW_H

#include <fstream>


#include <WTree>
#include <WTreeNode>
#include <WMenuItem>
#include <WTabWidget>
#include <WTextArea>
#include <Wt/WAbstractTableModel>
#include "FairDbWtFileModel.h"
#include "FairDbWtFileEditDialog.h"

#include <Wt/WStandardItem>
#include <Wt/Chart/WCartesianChart>
   
#include <Chart/WCartesianChart>
#include "CsvUtil.h"

#include <iostream>

using namespace std;
using namespace Wt;
using namespace Wt::Chart;
 
class FairDbWtParTreeTable;



class VirtualModel : public Wt::WAbstractTableModel
{
public:
  VirtualModel(int rows, int columns, Wt::WObject *parent = 0)
    : Wt::WAbstractTableModel(parent),
      rows_(rows),
      columns_(columns)
  { 
    for (int r=0;r<100;r++) irow[r]=r;
  }

  virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const
  {
    if (!parent.isValid())
      return rows_;
    else
      return 0;
  }

  virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const
  {
    if (!parent.isValid())
      return columns_;
    else
      return 0;
  }

  virtual boost::any data(const Wt::WModelIndex& index, int role = Wt::DisplayRole) const
  {
    switch (role) {
    case Wt::DisplayRole:
      if (index.column() == 0)
      return Wt::WString("Row {1}").arg(irow[index.row()]);
      else
      return Wt::WString("Item row {1}, col {2}").arg(irow[index.row()]).arg(index.column());
    default:
      return boost::any();
    }
  }

  virtual boost::any headerData(int section,
        Wt::Orientation orientation = Wt::Horizontal,
        int role = Wt::DisplayRole) const
  {
    if (orientation == Wt::Horizontal) {
      switch (role) {
      case Wt::DisplayRole:
      return Wt::WString("Column {1}").arg(section);
      default:
      return boost::any();
      }
    } else
      return boost::any();
  }

private:
  int rows_, columns_;
  int irow[100];

};






class FairDbWtTreeView : public WContainerWidget
{

public:
  FairDbWtTreeView(WContainerWidget *parent = 0);
  virtual ~FairDbWtTreeView();
  
  // Main Layout 
  virtual void createLayout(WContainerWidget *parent = 0);
  WGridLayout* getMainLayout(){ return fMainLayout;}

  // Title Definition
  WText* createTitle(const WString& title);

  // Set The Tree
  void setTreeTable(FairDbWtParTreeTable* aTable){fFileTreeTable=aTable;}

  // Widgets  
  virtual FairDbWtParTreeTable* TreeTableView();  
  WTableView * TableView();

  virtual void TreeTableChanged();
  virtual void InspectParameter(){;}
   
  // Events  
  virtual void showPopup_Lup(const WMouseEvent& event);
  virtual void showPopup_Rtop(const WMouseEvent& event);
  virtual void showPopup_Rdown(const WMouseEvent& event);
  virtual void showMeMouse(const WMouseEvent& event);
  virtual void popupAction_Rtop();
  virtual void popupAction_Rdown();
  virtual void popupAction_Lup(); 
  virtual void dialogDone_Rtop();
  virtual void dialogDone_Rdown();
 

  // Mouse Events Handling
  virtual Wt::WWidget *wMouseEvent();
  virtual void showClicked(const Wt::WMouseEvent &e);
  virtual void showDoubleClicked(const Wt::WMouseEvent &e);
  virtual void showMouseWentOut(const Wt::WMouseEvent &e);
  virtual void showMouseWentOver(const Wt::WMouseEvent &e);
  virtual void showMouseMoved(const Wt::WMouseEvent &e);
  virtual void showMouseWentUp(const Wt::WMouseEvent &e);
  virtual void showMouseWentDown(const Wt::WMouseEvent &e);
  virtual void showMouseWheel(const Wt::WMouseEvent &e);
  virtual void describe(const Wt::WMouseEvent &e);

  
  // Getters
  WTabWidget* getTabTop(){return fTabWidget_top;}
  WTabWidget* getTabDown(){return fTabWidget_down;}
  WTabWidget* getTabInfo(){return fTabWidget_info;}


  // Add to down tab
  virtual void show_Down(){;}


 
   // Test Functions
   WWidget *hist();  
   WWidget *rhist();
   WWidget *ghist();


protected:

  WGridLayout           *fMainLayout; 
  WStandardItemModel    *fFolderModel;
  WStandardItemModel    *fFileModel;
  WSortFilterProxyModel *fFileFilterModel;

  std::map<std::string, WString> fFolderNameMap;

  FairDbWtParTreeTable* fFileTreeTable;
  WTabWidget* fTabWidget_top;
  WTabWidget* fTabWidget_down;
  WTabWidget* fTabWidget_info;
  WContainerWidget* fWObject_Info;
  WContainerWidget* fConnections;

  WTreeView *fFolderView;
  WTableView *fFileView;
  WPopupMenu *fPopup;
  
  WMessageBox *fPopupActionBox;
  typedef std::set<WTreeNode *> WTreeNodeSet;

  Wt::WText *mouseEventType_;
  Wt::WText *mouseEventDescription_;

};

#endif //FAIRDBWTTREEVIEW_H

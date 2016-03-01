#ifndef FAIRDBWTROOTBROWSER
#define FAIRDBWTROOTBROWSER


#include "FairDbWtRootCanvas.h"

#include <WMenuItem>
#include <WTabWidget>
#include <WTextArea>
#include <WContainerWidget>
#include <WAbstractTableModel>
#include <WStandardItem>

#include "TFile.h"
#include "TKey.h"

#include <map>
#include <iostream>

using namespace std;
using namespace Wt;

#define            COL_SIZE        8
#define            ROW_SIZE        20

#define            B_WW          1000
#define            B_HH          1000

class FairDbWtFileTreeTable;
class FairDbWtConfigViewDialog;
class FairDbWtTFileNode;

class FairDbWtRootBrowser : public WContainerWidget
{
public:
  FairDbWtRootBrowser(WContainerWidget *root=nullptr, bool showTitle=true);
  virtual ~FairDbWtRootBrowser();

  // Main Layout Methids
  virtual void createLayout(WContainerWidget *parent = 0);
  WGridLayout* getMainLayout(){ return fMainLayout;}

  virtual void layoutSizeChanged(int w, int h){
   cout << "-I- FairDbWtBrowser size changed w : h "
        << w << " : " << h << endl;
   //if (fCanvas) fCanvas->layoutSizeChanged(w,h);
  }


  // File handling
  void inspectFile();
  void loadFileTree();
  void analyseTKey(FairDbWtTFileNode* cNode=nullptr);


  // Events
  virtual void showPopup_Lup(const WMouseEvent& event);
  virtual void showPopup_Ldown(const WMouseEvent& event);
  virtual void showPopup_Rtop(const WMouseEvent& event);
  virtual void showPopup_Rdown(const WMouseEvent& event);
  virtual void popupAction_Rtop();
  virtual void popupAction_Rdown();
  virtual void dialogDone_Rtop();
  virtual void dialogDone_Rdown();

  // File Browser Evt Handler
  virtual void treeTableChanged();

  // Drops
  virtual void dropEvent(WDropEvent event);

  // Algorithms methods
  void analyseSelectedNode(const string& name);

  // Display
  void display(TObject*  obj);
  void defineLayout(WContainerWidget* container, int col, int row);


  static map<string, TFile*> fTFileMap;


private:
  // Layout
  WGridLayout           *fMainLayout;
  WContainerWidget* fL_down, *fL_up;
  //
  FairDbWtRootCanvas* fCanvas;
  // File browser
  FairDbWtFileTreeTable* fFileTreeTable;
  // Tabs
  WTabWidget* fTabWidget_top;
  WTabWidget* fTabWidget_down;

  // LineEdit
  WLineEdit* fPathEdit;

  // popups actions
  WPopupMenu *fPopup;
  WMessageBox *fPopupActionBox;
  FairDbWtConfigViewDialog *fConfigDialog;

  typedef set<WTreeNode*> aWTreeNodeSet;
  WTreeNode* fCurrentNode;


};

#endif // FAIRDBWTROOTBROWSER_H

#include "CbmStsQaSensorIvView.h"
#include "CbmStsQaSensorIvModel.h"
#include "CbmStsQaSensorEditDialog.h"
#include "CbmStsQaWTreeView.h"

#include <Wt/WApplication>
#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <WMenuItem>
#include <WTabWidget>
#include "CsvUtil.h"

#include "FairDbWtParFactory.h"
#include "FairDbParFactory.h"
#include "FairDbWtParFactory.h"

using namespace Wt;


CbmStsQaSensorIvView::CbmStsQaSensorIvView(WContainerWidget *parent, string pName)
  : WContainerWidget(parent)
{

 fView = static_cast<CbmStsQaWTreeView*> (parent);

  
 FairDbWtParFactory::parInfo p_Info = FairDbWtParFactory::Instance().GetParInfo(pName);
 FairDbTableMetaData t_meta = p_Info.fTableInterface->GetMetaData();
  
 cout << "-I- CbmStsQaSensorTableView::  Table View name " << t_meta.TableName()
         << " nCols:  " << t_meta.NumCols() << endl; 
   
 int n_cols = t_meta.NumCols();

 Wt::WTableView *tableView = new Wt::WTableView(parent);

 CbmStsQaSensorIvModel *t_model =
         new CbmStsQaSensorIvModel(0,100, n_cols-3, tableView);

 // Fill the model with data
 t_model->fillrows();

 tableView->setModel(t_model);
 tableView->setColumnResizeEnabled(false);
 tableView->setColumnAlignment(0, Wt::AlignCenter);
 tableView->setHeaderAlignment(0, Wt::AlignCenter);

// treat first column as 'fixed' row headers
// <DB> check me : problem with event handling ?? 
//  date: 8.07.2015 
// Which container envelop ?
// tableView->setRowHeaderCount(1); 

 tableView->setSortingEnabled(true);
 tableView->setAlternatingRowColors(true);
 tableView->setRowHeight(28);
 tableView->setHeaderHeight(28);
 tableView->setSelectionMode(Wt::ExtendedSelection);

 tableView->setDragEnabled(true);
 tableView->sortByColumn(1, AscendingOrder);

 //tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
 tableView->resize(1000, 800);

 tableView->clicked().connect(this, &CbmStsQaSensorIvView::editFile);

 tableView->doubleClicked().connect(
           boost::bind(&CbmStsQaSensorIvView::editFiled, this, _1, t_model));
  
 // Events Notify
 tableView->mouseWentDown().connect(
    boost::bind(&CbmStsQaSensorIvView::showPopup, this, _1, _2));

 addWidget(tableView);

 // General Container definition
 decorationStyle().setBackgroundColor(Wt::white); 
 setPadding(10);
 setContentAlignment(AlignCenter);  
 setOverflow(OverflowAuto); 

}

void CbmStsQaSensorIvView::editFile(const WModelIndex& item)
{
 cout << "-I- CbmStsQaSensorIvView:: node :: item  row# " << item.row() <<  " column = " << item.column() << endl; 
 //if (item.column())
 //const WAbstractItemModel* aModel  = item.model();
 //boost::any aOut = aModel->data(item);
 //cout << "-I- CbmStsQaSensorTableView data : " << Wt::asString(aOut) << endl; 
 
 //fPopupActionBox = new WMessageBox("Tab selected","Action: " , NoIcon, Ok);
 //fPopupActionBox->show();
 
}

void CbmStsQaSensorIvView::editFiled(const WModelIndex& item, 
                                          WAbstractItemModel* model)
{
 cout << "-I- CbmStsQaSensorTableView:: doubleClicked :: item  row# " << item.row() 
      <<  " column = " << item.column() 
      << " model " << model 
      << endl; 
 const WAbstractItemModel* aModel  = item.model();
 boost::any aOut = aModel->data(item);
 cout << "-I--------------- data : " << Wt::asString(aOut) << endl; 

// create a dialog
 new CbmStsQaSensorEditDialog(model, item);

}


void CbmStsQaSensorIvView::showPopup(const WModelIndex& item, 
                                        const WMouseEvent& event ) 
{
    cout << "-I- CbmStsQaSensorTableView:: showpopup() :: item  row# "
       << item.row() 
      <<  " column = " << item.column() 
     << endl; 

    
    if (event.button() == WMouseEvent::RightButton) {
    // CHECK ME suboptimal
    //if (fPopup) {delete fPopup; fPopup=NULL;} 
    
      fPopup = new WPopupMenu();
      fPopup->addItem("icons/folder.gif", "Close");
      fPopup->addSeparator();
      
      WPopupMenu *subMenu = new WPopupMenu();
      subMenu->addItem("icons/hist.gif","Show IV");
      subMenu->addItem("Versions");
      fPopup->addMenu("Actions", subMenu);
    
      fPopup->aboutToHide().connect(this, &CbmStsQaSensorIvView::popupAction);
    

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event) 
  
}

void CbmStsQaSensorIvView::popupAction() 
{
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();
      fPopupActionBox = new WMessageBox("Sensor selected:","Action: " + text, NoIcon, Ok);
      if (text=="Close")  
          fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorIvView::dialogDone);
      else if (text=="Show IV")
          fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorIvView::displayIV); 

      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}


void CbmStsQaSensorIvView::displayIV(){
// Display the IV function for the 
// Selected Sensor.    

if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
}



// get the parent tree view
if (fView)
{
   // fView->Show_Down();
}

} 




void CbmStsQaSensorIvView::dialogDone() 
{
   // if (fTabWidget_top->currentIndex()>=0)
   //   {
   //    int index =  fTabWidget_top->currentIndex();
   //    fTabWidget_top->removeTab(fTabWidget_top->widget(index));
   //    }
    if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
    }
}








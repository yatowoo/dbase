#include "CbmStsQaSensorTableView.h"
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


CbmStsQaSensorTableView::CbmStsQaSensorTableView(WContainerWidget *parent, string pName)
  : WContainerWidget(parent)
{
// Set version of Data Model
fView = static_cast<CbmStsQaWTreeView*> (parent);

 FairDbWtParFactory::parInfo p_Info = FairDbWtParFactory::Instance().GetParInfo(pName);
 FairDbTableMetaData t_meta = p_Info.fTableInterface->GetMetaData();

 cout << "-I- CbmStsQaSensorTableView::  Table View name " << t_meta.TableName()
      << " nCols:  " << t_meta.NumCols() << endl;

int n_cols = t_meta.NumCols();
 fTableView = new Wt::WTableView(parent);
 // Try it out
 fTableView->decorationStyle().font().setSize(WFont::Small);

// Create the Data Model
CbmStsQaSensorDataModel *t_model =
                         new CbmStsQaSensorDataModel(100, n_cols-3, fTableView);
// Fill the model with data
t_model->fillrows();

fTableView->setModel(t_model);
fTableView->setColumnResizeEnabled(false);
fTableView->setColumnAlignment(0, Wt::AlignCenter);
fTableView->setHeaderAlignment(0, Wt::AlignCenter);

// treat first column as 'fixed' row headers
// <DB> check me : problem with event handling ??
//  date: 8.07.2015
// Which container envelop ?
// tableView->setRowHeaderCount(1);

fTableView->setSortingEnabled(true);
fTableView->setAlternatingRowColors(true);
fTableView->setRowHeight(28);
fTableView->setHeaderHeight(28);
fTableView->setSelectionMode(Wt::ExtendedSelection);

fTableView->setDragEnabled(true);
fTableView->sortByColumn(1, AscendingOrder);

//tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
fTableView->resize(1000, 800);

fTableView->clicked().connect(
  boost::bind(&CbmStsQaSensorTableView::editFile, this, _1, _2));
  //this, &CbmStsQaSensorTableView::editFile);

fTableView->doubleClicked().connect(
  boost::bind(&CbmStsQaSensorTableView::editFiled, this, _1, t_model));

// Events Notify
fTableView->mouseWentDown().connect(
    boost::bind(&CbmStsQaSensorTableView::showPopup, this, _1, _2));

addWidget(fTableView);

// General Container definition
decorationStyle().setBackgroundColor(Wt::white);
setPadding(10);
setContentAlignment(AlignCenter);
setOverflow(OverflowAuto);

}

CbmStsQaSensorTableView::~CbmStsQaSensorTableView()
{
 cout << "~CbmStsQaSensorTableView " << endl;
}



void CbmStsQaSensorTableView::editFile(const WModelIndex& item, const WMouseEvent& event)
{

  cout << "-I- CbmStsQaSensorTableView:: single clicked node :: item  row# "
       << item.row() <<  " column = " << item.column() << endl;


  if (fTableView){
   WString str("Selected Ids: ");

    WModelIndexSet indexSet_Tags = fTableView->selectedIndexes();
    for (WModelIndexSet::iterator index_iterator =
         indexSet_Tags.begin(); index_iterator != indexSet_Tags.end(); ++index_iterator)
    {
        WModelIndex index = *index_iterator;
        str += WString(std::to_string(index.row()));

      // Be careful that in the model to get data you should put
      // the proper Role.
       // boost::any aOut = aModel->data(item.row(),0,Wt::EditRole);

      // update the Selected SensorId
      //if (aOut.type() == typeid(int)){
      //   fSensorId.push_back(boost::any_cast<int>(aOut));
      // }

    }//!for
      cout << " ----> " << str << endl;
  }

 //cout << "-I- CbmStsQaSensorTableView: Selected Id : " << fSensorId  << endl;

}

void CbmStsQaSensorTableView::editFiled(const WModelIndex& item,
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


void CbmStsQaSensorTableView::showPopup(const WModelIndex& item,
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

      fPopup->aboutToHide().connect(this, &CbmStsQaSensorTableView::popupAction);


      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event)

}

void CbmStsQaSensorTableView::popupAction()
{

    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();


    WString str("Sensor Ids: ");
    if (fTableView){
      WModelIndexSet indexSet_Tags = fTableView->selectedIndexes();
     for (WModelIndexSet::iterator index_iterator =
          indexSet_Tags.begin(); index_iterator != indexSet_Tags.end(); ++index_iterator)
     {
        WModelIndex index = *index_iterator;
        const WAbstractItemModel* aModel  = index.model();


      // Be careful that in the model to get data you should put
      // the proper Role.
      // Here the sensor Id is taken from Column 0 and not from
      // the row() index
         boost::any aOut = aModel->data(index.row(),0,Wt::EditRole);

      // update the Selected SensorId
         int sensor_id = -1;
        if (aOut.type() == typeid(int)){
           sensor_id = boost::any_cast<int>(aOut);
         }
        str += WString(std::to_string(sensor_id) + " ");

     }//!for
      cout << " ----> " << str << endl;
   }


   fPopupActionBox = new WMessageBox(str,"Actions: " + text, NoIcon, Ok);
      if (text=="Close")
          fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorTableView::dialogDone);
      else if (text=="Show IV")
          fPopupActionBox->buttonClicked().connect(
            boost::bind(&CbmStsQaSensorTableView::displayIV, this, fTableView->selectedIndexes()));

      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}


void CbmStsQaSensorTableView::displayIV(const WModelIndexSet& aSet)
{
// Display the IV function for the
// Selected Sensor.

if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
}

// Selected Sensor Id list
vector<int> s_id;
s_id.clear();

if (fTableView){
      WModelIndexSet indexSet_Tags = fTableView->selectedIndexes();
     for (WModelIndexSet::iterator index_iterator =
          indexSet_Tags.begin(); index_iterator != indexSet_Tags.end(); ++index_iterator)
     {
        WModelIndex index = *index_iterator;
        const WAbstractItemModel* aModel  = index.model();
         boost::any aOut = aModel->data(index.row(),0,Wt::EditRole);
      // update the Selected SensorId
         int sensor_id = -1;
        if (aOut.type() == typeid(int)){
           sensor_id = boost::any_cast<int>(aOut);
           s_id.push_back(sensor_id);
         }
     }//!for
}//! tableView


// Show now the IV view for selected Sensor Ids
 if (fView)
   {
     for (int i=0; i < s_id.size(); i++)
     {

        fView->Show_Iv(s_id[i]);
      }
    }

}


void CbmStsQaSensorTableView::dialogDone()
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

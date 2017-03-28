#include "CbmStsQaSensorTableViewNew.h"
#include "CbmStsQaSensorEditDialog.h"
#include "CbmStsQaWTreeView.h"


#include <Wt/WApplication>
#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <WMenuItem>
#include <WTabWidget>
#include <WPanel>
#include "CsvUtil.h"

#include "FairDbWtParFactory.h"
#include "FairDbParFactory.h"
#include "FairDbWtParFactory.h"

using namespace Wt;


CbmStsQaSensorTableViewNew::CbmStsQaSensorTableViewNew(WContainerWidget *parent, string pName)
  : WContainerWidget(parent)
  ,fOption(0)
{
// Set version of Data Model
fView = static_cast<CbmStsQaWTreeView*> (parent);

FairDbWtParFactory::parInfo p_Info = FairDbWtParFactory::Instance().GetParInfo(pName);
FairDbTableMetaData t_meta = p_Info.fTableInterface->GetMetaData();

cout << "-I- CbmStsQaSensorTableViewNew::  Table View name " << t_meta.TableName()
     << " nCols:  " << t_meta.NumCols() << endl;

int n_cols = t_meta.NumCols();
fTableView = new Wt::WTableView(parent);
fTableView->decorationStyle().font().setSize(WFont::Small);

// Create the Data Model
CbmStsQaSensorDataModelNew *t_model =
                         new CbmStsQaSensorDataModelNew(100, n_cols-3, fTableView);
// Fill the model with data
t_model->fillrows();

fTableView->setModel(t_model);
fTableView->setColumnResizeEnabled(true);
fTableView->setColumnAlignment(0, Wt::AlignCenter);
fTableView->setHeaderAlignment(0, Wt::AlignCenter);

// treat first column as 'fixed' row headers
// <DB> check me : problem with event handling ??
//  date: 8.07.2015
// Which container envelop ?
// tableView->setRowHeaderCount(1);

// Sorting enabled
fTableView->setSortingEnabled(true);
// Alternative colors
fTableView->setAlternatingRowColors(true);

// Column size
//for( int i=0; i<n_cols;i++) fTableView->setColumnWidth(i,70);
// Row and Headers height
fTableView->setRowHeight(20);
fTableView->setHeaderHeight(20);
fTableView->setSelectionMode(Wt::ExtendedSelection);

fTableView->setDragEnabled(true);
fTableView->sortByColumn(1, AscendingOrder);

//tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);


fTableView->clicked().connect(
  boost::bind(&CbmStsQaSensorTableViewNew::editFile, this, _1, _2));
  //this, &CbmStsQaSensorTableViewNew::editFile);

fTableView->doubleClicked().connect(
  boost::bind(&CbmStsQaSensorTableViewNew::editFiled, this, _1, t_model));

// Events Notify
fTableView->mouseWentDown().connect(
    boost::bind(&CbmStsQaSensorTableViewNew::showPopup, this, _1, _2));

addWidget(fTableView);

// General Container definition
decorationStyle().setBackgroundColor(Wt::white);
setPadding(10);
setContentAlignment(AlignCenter);
//setOverflow(OverflowAuto);

// Modifying column width
 int max_col = n_cols-3;
 for(int i=0;i<max_col;i++) {
   fTableView->setColumnWidth(i,100);
 }
 fTableView->resize(55+8*100, 20*20);
}

CbmStsQaSensorTableViewNew::~CbmStsQaSensorTableViewNew()
{
 cout << "~CbmStsQaSensorTableViewNew " << endl;
}



void CbmStsQaSensorTableViewNew::editFile(const WModelIndex& item, const WMouseEvent& event)
{

 cout << "-I- CbmStsQaSensorTableViewNew:: single clicked node :: item  row# "
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

 //cout << "-I- CbmStsQaSensorTableViewNew: Selected Id : " << fSensorId  << endl;

}

void CbmStsQaSensorTableViewNew::editFiled(const WModelIndex& item,
                                          WAbstractItemModel* model)
{
 cout << "-I- CbmStsQaSensorTableViewNew:: doubleClicked :: item  row# " << item.row()
      <<  " column = " << item.column()
      << " model " << model
      << endl;
 const WAbstractItemModel* aModel  = item.model();
 boost::any aOut = aModel->data(item);
 cout << "-I--------------- data : " << Wt::asString(aOut) << endl;


// create a dialog
 new CbmStsQaSensorEditDialog(model, item);

}


void CbmStsQaSensorTableViewNew::showPopup(const WModelIndex& item,
                                        const WMouseEvent& event )
{
  cout << "-I- CbmStsQaSensorTableViewNew:: showpopup() :: item  row# "
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
    subMenu->addItem("icons/table.jpg","Geometry");
    subMenu->addItem("icons/table.jpg","Ownership");
    subMenu->addItem("icons/table.jpg","Results");
    subMenu->addItem("icons/hist.gif","I=f(V)");
    subMenu->addSeparator();

    WPopupMenu *subsubMenu = new WPopupMenu();
    subsubMenu->addItem("icons/hist.gif","Ac_cap_ok_pside");
    subsubMenu->addItem("icons/hist.gif","Ac_cap_ok_nside");
    subsubMenu->addItem("icons/hist.gif","Ac_cap_val_pside");
    subsubMenu->addItem("icons/hist.gif","Ac_cap_val_nside");
    subMenu->addMenu("ChannelMap", subsubMenu);

    fPopup->addMenu("Features", subMenu);

    fPopup->aboutToHide().connect(this, &CbmStsQaSensorTableViewNew::popupAction);

    if (fPopup->isHidden())
    fPopup->popup(event);
    else
    fPopup->hide();
  }//! if (event)
}

void CbmStsQaSensorTableViewNew::popupAction()
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

      cout << "-I- text: " << text << endl;
      // creat the actions
      fPopupActionBox = new WMessageBox(str,"Features: " + text, NoIcon, Ok);
      if (text=="Close")
      fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorTableViewNew::dialogDone);
      else if (text=="I=f(V)")
        fPopupActionBox->buttonClicked().connect(
                                                 boost::bind(&CbmStsQaSensorTableViewNew::displayIV, this, fTableView->selectedIndexes()));
        else if (text=="Geometry")
        fPopupActionBox->buttonClicked().connect(
          boost::bind(&CbmStsQaSensorTableViewNew::displayGeo, this, fTableView->selectedIndexes()));
          else if (text=="Ownership")
          fPopupActionBox->buttonClicked().connect(
            boost::bind(&CbmStsQaSensorTableViewNew::displayOwner, this, fTableView->selectedIndexes()));
            else if (text=="Results")
            fPopupActionBox->buttonClicked().connect(
              boost::bind(&CbmStsQaSensorTableViewNew::displayResults, this, fTableView->selectedIndexes()));
              else if (text=="Ac_cap_ok_pside")
              fPopupActionBox->buttonClicked().connect(
                boost::bind(&CbmStsQaSensorTableViewNew::displayChannelMap, this, fTableView->selectedIndexes()));
                // DB need to be implemented for the rest
                else if (text=="Ac_cap_ok_nside")
                fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorTableViewNew::dialogDone);
                else if (text=="Ac_cap_val_pside")
                fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorTableViewNew::dialogDone);
                else if (text=="Ac_cap_val_nside")
                fPopupActionBox->buttonClicked().connect(this, &CbmStsQaSensorTableViewNew::dialogDone);

                fPopupActionBox->show();

              } else {
                // if (fPopup result)
                fPopup->hide();
              }
}

void CbmStsQaSensorTableViewNew::dialogDone()
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


void CbmStsQaSensorTableViewNew::displayIV(const WModelIndexSet& aSet)
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




void CbmStsQaSensorTableViewNew::displayGeo(const WModelIndexSet& aSet)
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
        fView->Show_Geo(s_id);
    }
}


void CbmStsQaSensorTableViewNew::displayOwner(const WModelIndexSet& aSet)
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
        fView->Show_Owner(s_id);
    }
}


void CbmStsQaSensorTableViewNew::displayResults(const WModelIndexSet& aSet)
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
        fView->Show_Results(s_id);
    }
}


void CbmStsQaSensorTableViewNew::displayChannelMap(const WModelIndexSet& aSet)
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
        fView->Show_ChannelMap(s_id[i]);
      }
    }
}

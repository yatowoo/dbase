#include "CbmStsWtQaVendorBatchView.h"
#include "CbmStsWtQaLogger.h"
#include "CbmStsWtQaViewsUtils.h"
#include "CbmStsWtQaUtils.h"

#include "CbmStsDbQaSensorNewPar.h"
#include "CbmStsWtQaSensorDataModel.h"

#include "FairDbWtParFactory.h"
#include "FairDbParFactory.h"

using namespace std;
using namespace Models;

namespace Views{
  
  CbmStsWtQaVendorBatchView::CbmStsWtQaVendorBatchView(WContainerWidget *pView, const string& vendor, const string& batch_id,  WContainerWidget *parent)
    : CbmStsWtQaBaseView(parent)
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaVendorBatchView::ctor()"; 

    // Scrolling  Settings more detailed
    setOverflow(WContainerWidget::OverflowVisible/*Hidden*/, Horizontal);
    setOverflow(WContainerWidget::OverflowAuto, Vertical);
    // Disable events 
    setAttributeValue
      ("oncontextmenu",
       "event.cancelBubble = true; event.returnValue = false; return false;"); 
  
    // Set initial size
    resize(WLength(90,WLength::Percentage),WLength(90,WLength::Percentage));
    
    // Css style
    //setStyleClass("card-1");
    
    // Create the VendorBatchView Layout 
    createLayout(vendor, batch_id, parent);

    // Set the parent presenter
    fPView =  static_cast<CbmStsWtQaPresenterView*> (pView);
  }
  
  CbmStsWtQaVendorBatchView::~CbmStsWtQaVendorBatchView(){
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaVendorBatchView::dtor";
  }
  
  
  void
  CbmStsWtQaVendorBatchView::createLayout(const string& vendor, const string& batch_id, WContainerWidget *parent)
  {    
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaVendorBatchView::createLayout()";
    
    FairDbReader<CbmStsDbQaSensorNewPar> pReader;     
    FairDbTableMetaData t_meta = pReader.TableInterface().GetMetaData();
    
    QA_LOG(WVIEWS,INFO) << "CbmStsVendorBatchView::  Table View name " << t_meta.TableName()
                        << " nCols:  " << t_meta.NumCols();
    
    fTableView = new Wt::WTableView(parent);

    // Create the Data Model
    int n_cols = t_meta.NumCols();
    CbmStsWtQaSensorDataModel *t_model = new CbmStsWtQaSensorDataModel(150, n_cols-3, fTableView);

    // Filter out in Data Model 
    t_model->setSelection(vendor,batch_id);

    // Fill the model with selected data
    t_model->fillrows();

    // Set the model   
    fTableView->setModel(t_model);
    
    // Settings
    fTableView->setRowHeaderCount(1); // treat first column as 'fixed' row headers
    fTableView->setColumnResizeEnabled(false);
    fTableView->setColumnAlignment(0, Wt::AlignCenter);
    fTableView->setHeaderAlignment(0, Wt::AlignCenter);
    fTableView->setSortingEnabled(true);
    fTableView->setAlternatingRowColors(true);
    fTableView->setRowHeight(28);
    fTableView->setHeaderHeight(28);
    fTableView->setSelectionMode(Wt::ExtendedSelection);
    fTableView->setDragEnabled(true);
    fTableView->sortByColumn(1, AscendingOrder);
    //fTableView->resize(1000, 800);
    fTableView->decorationStyle().font().setSize(WFont::Small);

    fTableView->clicked().connect(
                                        boost::bind(&CbmStsWtQaVendorBatchView::editFile
                                                      , this
                                                      , _1
                                                      , _2)
                                        );

    fTableView->doubleClicked().connect(
                                        boost::bind(&CbmStsWtQaVendorBatchView::editFiled
                                                      , this
                                                      , _1
                                                      , t_model
                                                      )
                                        );

    // Events Notify
    fTableView->mouseWentDown().connect(
                                        boost::bind(&CbmStsWtQaVendorBatchView::showPopup
                                          , this
                                          , _1
                                          , _2
                                          )
                                        );

    // Add TableView Widget 
    addWidget(fTableView);
    
    // General Container definition
    decorationStyle().setBackgroundColor(Wt::white);

    setPadding(10);
    
    setContentAlignment( AlignCenter | AlignTop );
       
    // Resize TableView by modifying column width
    int max_col = n_cols-3;
    for(int i=0;i<max_col;i++) {
      fTableView->setColumnWidth(i,120);
    }
    fTableView->resize(55+10*120, 20*20);

  }
  
  void
  CbmStsWtQaVendorBatchView::init()
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaVendorBatchView::init()";
    
  }

  void CbmStsWtQaVendorBatchView::editFile(const WModelIndex& item, const WMouseEvent& event)
  {
    QA_LOG(WVIEWS,DEBUG) << " CbmStsQaVendorBatchView:: single clicked node :: item  row# "
                         << item.row() <<  " column = " << item.column();
  }
  
  void CbmStsWtQaVendorBatchView::editFiled(const WModelIndex& item,
                                            WAbstractItemModel* model)
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaVendorBatchView::editFiled()";
  }
  
  void CbmStsWtQaVendorBatchView::showPopup(const WModelIndex& item,
                                            const WMouseEvent& event )
  {
    QA_LOG(WVIEWS,DEBUG)  << "CbmStsWtQaVendorBatchView::showPopup()";
    QA_LOG(WVIEWS, INFO)  << "-I- CbmStsQaSensorTableViewNew:: showpopup() :: item  row# "
                          << item.row()
                          <<  " column = " << item.column();
    
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
      
      fPopup->aboutToHide().connect(this, &CbmStsWtQaVendorBatchView::popupAction);
      
      if (fPopup->isHidden())
        fPopup->popup(event);
      else
        fPopup->hide();
    }//! if (event)
        
  }

  void
  CbmStsWtQaVendorBatchView::popupAction()
  {
    
    if (fPopup->result())
      {
        WString text = fPopup->result()->text();
        fPopup->hide();
        
        WString str("Sensor UIds: ");
        if (fTableView)
          {
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
                
                // Update the Selected SensorId
                int sensor_id = -1;
                if (aOut.type() == typeid(int))
                  {
                    sensor_id = boost::any_cast<int>(aOut);
                  }
                str += WString(std::to_string(sensor_id) + " ");
                
              }//!for
            cout << " STR ----> " << str << endl;
            
          }//(!TableView)
        
        cout << "-I- Selected text: " << text << endl;
        // creat the actions
        fPopupActionBox = new WMessageBox(str,"Features: " + text, NoIcon, Ok);
        if (text=="Close")
          {
            fPopupActionBox->buttonClicked().connect(this, &CbmStsWtQaVendorBatchView::dialogDone);
          }
        else if (text=="I=f(V)")
          {
            fPopupActionBox->buttonClicked().connect(
                                                     boost::bind(&CbmStsWtQaVendorBatchView::displayIV,
                                                                 this,
                                                                 fTableView->selectedIndexes()));
          }
        
        
        // Show popup action
        fPopupActionBox->show();
        
      } else {
      fPopup->hide();
    }
  }
  
  void
  CbmStsWtQaVendorBatchView::dialogDone()
  {
    if (fPopupActionBox) {
      delete fPopupActionBox;
      fPopupActionBox = 0;
    }
  }
  

void CbmStsWtQaVendorBatchView::displayIV(const WModelIndexSet& aSet)
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
  
  if (fTableView)
    {
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
  if (fPView)
    {
      for (int i=0; i < s_id.size(); i++)
        {
          fPView->Show_Iv(s_id[i]);
        }
    }
}
  
  
}//!(namespace Views)

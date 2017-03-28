#include "CbmStsWtQaPresenterView.h"
#include "CbmStsWtQaVendorBatchView.h"
#include "CbmStsWtQaIvChartView.h"
#include "CbmStsWtQaLogger.h"
#include "CbmStsWtQaViewsUtils.h"
#include "CbmStsWtQaUtils.h"


namespace Views{
  
  CbmStsWtQaPresenterView::CbmStsWtQaPresenterView(WContainerWidget *parent)
    : CbmStsWtQaBaseView(parent)
    , fVbox(nullptr)
    , fTab_top(nullptr)
    , fTab_down(nullptr)
    , fFrame_top(nullptr)
    , fFrame_down(nullptr)
    , fPopup(nullptr)
    , fPopupActionBox(nullptr)    
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaPresenterView::ctor()"; 

    // Widget Settings
    setOverflow(OverflowAuto);
    
    // Disable events 
    setAttributeValue
      ("oncontextmenu",
       "event.cancelBubble = true; event.returnValue = false; return false;"); 
  
    // Set initial size
    resize(WLength(90, WLength::Percentage),WLength(90,WLength::Percentage));
    // Css style
    setStyleClass("card-0");
    
    // Create the PresenterView Layout 
    createLayout();   
  }
  
  CbmStsWtQaPresenterView::~CbmStsWtQaPresenterView(){
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaPresenterView::dtor";

    if (fVbox)
      {
        delete fVbox; fVbox = nullptr;
      }
    if (fFrame_top)
      {
        delete fFrame_top; fFrame_top = nullptr;
      }
    if (fFrame_down)
      {
        delete fFrame_down; fFrame_down = nullptr;
      }
    
  }
  
  void
  CbmStsWtQaPresenterView::createLayout(WContainerWidget* parent)
  {    
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaPresenterView::createLayout()"; 
    
    fVbox = new Wt::WVBoxLayout();

    // Create frome
    fFrame_top  = new WContainerWidget(); 
    fFrame_down = new WContainerWidget();
    
    fFrame_top->setOverflow(OverflowAuto);
    fFrame_down->setOverflow(OverflowAuto);

    fFrame_top->setStyleClass("card-1");
    fFrame_top->decorationStyle().setBackgroundColor(Wt::white);
    fFrame_down->setStyleClass("card-1");
    

    // Tabs top + down
    fTab_top = new WTabWidget();
    fTab_down = new WTabWidget();

    // Set style
    fTab_top->setStyleClass("Wt-tabs");
    fTab_top->setStyleClass("Wt-tabs");

    fFrame_top->addWidget(fTab_top);
    fFrame_down->addWidget(fTab_down);

    // Layout
    fVbox->addWidget(fFrame_top,  1);
    fVbox->addWidget(fFrame_down, 1);
    fVbox->setResizable(0);
    
    setLayout(fVbox);
  }

  void
  CbmStsWtQaPresenterView::init()
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaPresenterView::init()";


    
    // Connect signal (Browser emitted)
    getSignal(sig_browser)->get_str().connect(this, &CbmStsWtQaPresenterView::createTableView);
    
    // Tabs Event Settings 
    fFrame_top->mouseWentDown().connect(this, &CbmStsWtQaPresenterView::showPopupTop);    
    fFrame_down->mouseWentDown().connect(this, &CbmStsWtQaPresenterView::showPopupDown);    
    
  }

  void
  CbmStsWtQaPresenterView::createTableView(const std::string& batch_id)
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaPresenterView::createTableView() reveived batch_id: "<< batch_id;

    // Parse the received message 
    vector<string> res;
    res = Utils::parse_value<string> (batch_id, ":");

    // Get Signal Contents 
    string vendor   = res[0];
    string batch_nr = res[1];

    //DB <Closeable(true)> is still not working
    WMenuItem *tab = fTab_top->addTab(new CbmStsWtQaVendorBatchView(this,vendor,batch_nr), batch_id , WTabWidget::PreLoading);
  }

  void
  CbmStsWtQaPresenterView::showPopupTop(const WMouseEvent& event) 
  {
    if (event.button() == WMouseEvent::RightButton) {
      // CHECK ME suboptimal
      if (fPopup) {delete fPopup; fPopup=NULL;} 
    
      if (!fPopup) {
        fPopup = new WPopupMenu();
        fPopup->addItem("icons/folder.gif", "Close");
        fPopup->addSeparator();
      
        WPopupMenu *subMenu = new WPopupMenu();
        subMenu->addItem("Draw");
        subMenu->addItem("History");
        fPopup->addMenu("Actions", subMenu);
    
        fPopup->aboutToHide().connect(this, &CbmStsWtQaPresenterView::popupActionTop);
      }

      if (fPopup->isHidden())
        fPopup->popup(event);
      else
        fPopup->hide();
    }//! if (event) 
  }

  void
  CbmStsWtQaPresenterView::popupActionTop() 
  {
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();
         
      fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &CbmStsWtQaPresenterView::dialogDoneTop);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
  }

  void
  CbmStsWtQaPresenterView::dialogDoneTop() 
  {
    if (fTab_top->currentIndex()>=0)
      {
        int index =  fTab_top->currentIndex();
        fTab_top->removeTab(fTab_top->widget(index));
      }
    if (fPopupActionBox) {
      delete fPopupActionBox;
      fPopupActionBox = 0;
    }
  }

    void
  CbmStsWtQaPresenterView::showPopupDown(const WMouseEvent& event) 
  {
    if (event.button() == WMouseEvent::RightButton) {
      // CHECK ME suboptimal
      if (fPopup) {delete fPopup; fPopup=NULL;} 
    
      if (!fPopup) {
        fPopup = new WPopupMenu();
        fPopup->addItem("icons/folder.gif", "Close");
        fPopup->addSeparator();
      
        WPopupMenu *subMenu = new WPopupMenu();
        subMenu->addItem("Draw");
        subMenu->addItem("History");
        fPopup->addMenu("Actions", subMenu);
    
        fPopup->aboutToHide().connect(this, &CbmStsWtQaPresenterView::popupActionDown);
      }

      if (fPopup->isHidden())
        fPopup->popup(event);
      else
        fPopup->hide();
    }//! if (event) 
  }

  void
  CbmStsWtQaPresenterView::popupActionDown() 
  {
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();
         
      fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &CbmStsWtQaPresenterView::dialogDoneDown);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
  }

  void
  CbmStsWtQaPresenterView::dialogDoneDown() 
  {
    if (fTab_down->currentIndex()>=0)
      {
        int index =  fTab_down->currentIndex();
        fTab_down->removeTab(fTab_down->widget(index));
      }
    if (fPopupActionBox) {
      delete fPopupActionBox;
      fPopupActionBox = 0;
    }
  }
  
  void
  CbmStsWtQaPresenterView::Show_Iv(int sid){
    
    QA_LOG(WVIEWS,INFO)<< "-I- CbmStsQWtaWTreeView::Show_Iv()  sid:" << sid ;
    CbmStsWtQaIvChartView *iv_view = new CbmStsWtQaIvChartView(sid, fFrame_down);
    WMenuItem *item_down = fTab_down->addTab(iv_view,WString("I-V  sId: {1}").arg(sid), WTabWidget::PreLoading);
    int index = fTab_down->indexOf(iv_view);
    fTab_down->setCurrentIndex(index);
    //item_down->setCloseable(false);    
  }

  
  
}//!(namespace Views)

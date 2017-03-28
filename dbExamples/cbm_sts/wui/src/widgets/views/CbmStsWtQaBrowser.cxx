#include "CbmStsWtQaBrowser.h"
#include "CbmStsWtQaLogger.h"
#include "CbmStsWtQaFolderView.h"
#include "CbmStsWtQaPresenterView.h"


namespace Views{
  
  CbmStsWtQaBrowser::CbmStsWtQaBrowser(WContainerWidget *parent)
    : CbmStsWtQaBaseView(parent)
    ,fMainLayout(nullptr)
    ,fFolderView(nullptr)
    ,fPresenterView(nullptr)  
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaBrowser::ctor()"; 

    // Sizeable
    resize(1200, 1200);

    // Create the Browser Layout 
    createLayout();

    // Set propagated signals 
    setEvents();   
  }
  
  CbmStsWtQaBrowser::~CbmStsWtQaBrowser(){
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaBrowser::dtor";
    // delete signals
    if (fASignal)
      {
        delete fASignal;
        fASignal=nullptr;
      }
  }
  
  
  void
  CbmStsWtQaBrowser::createLayout(WContainerWidget* parent)
  {    
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaBrowser::CreateLayout()"; 
    
    // Main Grid Layout
    fMainLayout = new WGridLayout(parent);

    // Right Frame 
    WVBoxLayout* r_vbox = new WVBoxLayout();
    // Create the Folder View
    fFolderView = new CbmStsWtQaFolderView();
    // Add Folder to vbox
    r_vbox->addWidget(fFolderView);  

    // Left Frame 
    WVBoxLayout* l_vbox = new WVBoxLayout();
    // Create the Folder View
    fPresenterView = new CbmStsWtQaPresenterView();
    // Add Folder to vbox
    l_vbox->addWidget(fPresenterView);        

    // Finalize layout
    fMainLayout->addLayout(r_vbox, 0, 0);
    fMainLayout->addLayout(l_vbox, 0, 1);

    // Properties
    fMainLayout->setColumnResizable(0);

    // Coverage Folder 10% - Presenter 90 %
    fMainLayout->setColumnStretch(0, 1);
    fMainLayout->setColumnStretch(1, 9);
          
    setLayout(fMainLayout);
  }

  void
  CbmStsWtQaBrowser::setEvents()
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaBrowser::setEvents()";

    fASignal = new CbmStsWtQaSigStr();  
    // Set internal signals & Actions
    if(fPresenterView)
      {
        fPresenterView->setSignal(fASignal, sig_browser);
        fPresenterView->init();
      }
    if(fFolderView)
      {
        fFolderView->setSignal(fASignal, sig_browser);
        fFolderView->init();
      }
  }
}//! namespace (Views)

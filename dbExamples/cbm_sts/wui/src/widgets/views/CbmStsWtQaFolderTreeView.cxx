#include "CbmStsWtQaFolderTreeView.h"
#include "CbmStsWtQaLogger.h"
#include "CbmStsWtQaViewsUtils.h"
#include "CbmStsWtQaDataObjects.h"
#include "CbmStsWtQaTreeTable.h"
#include "CbmStsWtQaTreeTableNode.h"

#include <WEvent>

using namespace Database;
using namespace Layout;
using namespace Utils;
using namespace Wt;

namespace Views{
  
  CbmStsWtQaFolderTreeView::CbmStsWtQaFolderTreeView(WContainerWidget *parent)
    : CbmStsWtQaBaseView(parent)
    ,fVbox(nullptr)
    ,fPopup(nullptr)
    ,fTreeTable(nullptr)  
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::ctor()"; 

    // Data Initialisation
    if (!CbmStsWtQaDataObjects::isActive()) initDataObjects();
    
    // Widget Settings
    setOverflow(OverflowAuto);
    
    // Disable events 
    setAttributeValue
      ("oncontextmenu",
       "event.cancelBubble = true; event.returnValue = false; return false;"); 
  
    // Set initial size using %(size)    
    resize(WLength(90,WLength::Percentage),WLength(90,WLength::Percentage));

    // Css style
    setStyleClass("card");
    
    // Create the FolderTreeView Layout 
    createLayout();   

    // Events settings
    setEventSettings();
  }
  
  CbmStsWtQaFolderTreeView::~CbmStsWtQaFolderTreeView()
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::dtor";
  }
    
  void
  CbmStsWtQaFolderTreeView::createLayout(WContainerWidget* parent)
  {
    // Create the physical layout
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::CreateLayout()"; 
    
    fVbox = new Wt::WVBoxLayout();
    fTreeTable = new CbmStsWtQaTreeTable(parent);
    fVbox->addWidget(fTreeTable);
    
    setLayout(fVbox);
  }

  void
  CbmStsWtQaFolderTreeView::initDataObjects(){
    // Initialize the data objects
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::initDataObjects()";
    CbmStsWtQaDataObjects::instance().initialize();    
  }

  void
  CbmStsWtQaFolderTreeView::setEventSettings(){
    // Define Event Settings
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::setEventSettings()";
    mouseWentDown().connect(this, &CbmStsWtQaFolderTreeView::showPopup);
  }

  void
  CbmStsWtQaFolderTreeView::showPopup(const WMouseEvent& event){
    // Showing Browser Menu 
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::showPopup()";
    
    //<DB> here is a strange error coming when using
    // securized sessions ..     
    if (event.button() == WMouseEvent::RightButton) {
      // CHECK ME suboptimal
      if (fPopup) {delete fPopup; fPopup=NULL;} 
      
      if (!fPopup) {
        fPopup = new WPopupMenu();
        Wt::WMenuItem *item = fPopup->addItem("Show");
        // Defining actions 
        item->triggered().connect(
                                  std::bind(
                                            [=](){ selectedItem(item);}
                                            )
                                  );
      }
      
      if (fPopup->isHidden()){
        fPopup->popup(event);
      }else{
        fPopup->hide();
      }      
    }//! if (event)  
  }
  
  void
  CbmStsWtQaFolderTreeView::selectedItem(WMenuItem* mItem)
  {
    // Actions after a selected Item
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderTreeView::selectedItem() : "
                         << mItem->text();
    
    // Actions according to selected item
    const char* st = mItem->text().toUTF8().c_str();
    switch (str2int(st))
      {
        {
        case str2int("Show"):
          {
            showContent();
          }
          break;
        }
        
      }//!(switch)
  }
  
  void
  CbmStsWtQaFolderTreeView::showContent()
  {
    // Show Content of a Sensor batch
    WTree* aTree = fTreeTable->tree(); 
    // Get Tree Node
    CbmStsWtQaTreeTableNode* topNode = (CbmStsWtQaTreeTableNode*) fTreeTable->treeRoot();
    
    TreeNodeSet::const_iterator it = aTree->selectedNodes().begin();

    string batch_id;
    string vendor_id;

    for ( ; it != aTree->selectedNodes().end(); ++it)
      {       
        // Get batch_id
        WTreeNode *aNode = *it;
        batch_id = aNode->label()->text().toUTF8();
        // Get Vendor_id
        WTreeNode *pNode = aNode->parentNode();
        vendor_id = pNode->label()->text().toUTF8();
        QA_LOG(WLAYOUTS,DEBUG)  << "CbmStsWtQaFolderTreeView::showContent() " 
                                << " batch_id: " << batch_id  << " vendor_id: " << vendor_id;             
      }
    // Emit a message signal
    string message = vendor_id + ":" + batch_id;    
    getSignal(sig_browser)->get_str().emit(message);
  } 
  

}//!(namespace Views)

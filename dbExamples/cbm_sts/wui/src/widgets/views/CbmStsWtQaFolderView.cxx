#include "CbmStsWtQaFolderView.h"
#include "CbmStsWtQaLogger.h"
#include "CbmStsWtQaViewsUtils.h"


namespace Views{
  
  CbmStsWtQaFolderView::CbmStsWtQaFolderView(WContainerWidget *parent)
    : CbmStsWtQaBaseView(parent)
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderView::ctor()"; 
    
    // Widget Settings
    //setOverflow(OverflowAuto);
    
    // Disable events 
    setAttributeValue
      ("oncontextmenu",
       "event.cancelBubble = true; event.returnValue = false; return false;"); 
  
    //Set initial size to 300 width, auto-length
    resize(WLength(90,WLength::Percentage), WLength(90,WLength::Percentage));
    //resize(300,WLength::Auto);

    // Css style
    setStyleClass("card-1");

    // Create the FolderView Layout 
    createLayout();

  }
  
  CbmStsWtQaFolderView::~CbmStsWtQaFolderView()
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderView::dtor";
  }
  
  
  void
  CbmStsWtQaFolderView::createLayout(WContainerWidget* parent)
  {
    //
    // Create the main Folder Layout
    //
    
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderView::CreateLayout()"; 
    
    fPopup = new Wt::WPopupMenu();
    fStatus = new Wt::WText();
    fOut = new Wt::WText();
 
    // Margin
    fStatus->setMargin(10);
    fOut->setMargin(10);
    // Font
    fStatus->decorationStyle().font().setSize(WFont::Small);
    fOut->decorationStyle().font().setSize(WFont::XSmall);

    // Create Main "Actions" Menu
    createMenus(parent);
    
    // Layout definition  
    fVbox = new Wt::WVBoxLayout();
    fVbox->addWidget(fLeftMenu);

    // Add Tree View 
    fFolderTreeView = new CbmStsWtQaFolderTreeView(parent);
    fVbox->addWidget(fFolderTreeView,1);

    // Add footer message 
    fVbox->addWidget(fOut,0,Wt::AlignBottom | Wt::AlignRight);
    setLayout(fVbox);
  }

  
  void
  CbmStsWtQaFolderView::createMenus(WContainerWidget* parent)
  {
    //
    // Create the Left Top Menus and corresponding actions
    //    
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderView::CreateMenus()";
    
    fPopup->addItem("Connect")->triggered().connect(std::bind([=] () {
          fOut->setText("Connected");
        }));
    
    fPopup->addItem("Disconnect")->triggered().connect(std::bind([=] () {
          fOut->setText("Disconnected.");
        }));
    
    fPopup->addSeparator();
    
    fPopup->addItem("icons/house.png", "Home")->triggered().connect(std::bind([=] () {
          fOut->setText("");
        }));
    
    
    // Create a submenu for the fPopup menu.
    fPopup->addSeparator();
    fSubMenu = new Wt::WPopupMenu();
    
    fSubMenu->addItem("Contents")->triggered().connect(std::bind([=] () {
          fOut->setText("<p>This could be a link to /contents.html.</p>");
        }));
    
    fSubMenu->addItem("Index")->triggered().connect(std::bind([=] () {
          fOut->setText("<p>This could be a link to /index.html.</p>");
        }));
    
    fSubMenu->addSeparator();
    fSubMenu->addItem("About")->triggered().connect(std::bind([=] () {
          Wt::WMessageBox *messageBox = new Wt::WMessageBox
            ("About", "<p>This is a program to make connections.</p>",
             Wt::Information, Wt::Ok);
          messageBox->show();
          messageBox->buttonClicked().connect(std::bind([=] () {
                delete messageBox;
              }));
        }));
    
    // Assign the submenu to the parent fPopup menu.
    fPopup->addMenu("Help", fSubMenu);

    // Setup a Right-aligned menu.
    fLeftMenu = new Wt::WMenu();
    fLeftMenu->setStyleClass("nav nav-pills");
    
    Wt::WMenuItem *m_item = new Wt::WMenuItem("Actions");
    m_item->setMenu(fPopup);
    fLeftMenu->addItem(m_item);
    
    // React to an item selection
    fPopup->itemSelected().connect(std::bind([=] (Wt::WMenuItem *item) {
          fStatus->setText
            (Wt::WString::fromUTF8("Selected menu item: {1}.")
             .arg(item->text()));
        }, std::placeholders::_1));

  }

  void
  CbmStsWtQaFolderView::init()
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderView::init()";    
  }
  
  void
  CbmStsWtQaFolderView::setSignal(CbmStsWtQaSigStr* sig, signal_t type)
  {
    QA_LOG(WVIEWS,DEBUG) << "CbmStsWtQaFolderView::setSignal()";
    
    if(fFolderTreeView) fFolderTreeView->setSignal(sig, type);

    CbmStsWtQaBaseView::setSignal(sig, type);       
  }
  
  
}



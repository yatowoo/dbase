#include "FairDbWtRootBrowser.h"

#include <WContainerWidget>
#include <WApplication>
#include <WEnvironment>
#include <WSlider>
#include <WText>
#include <WBreak>
#include <WPushButton>
#include <WTree>
#include <WTreeTableNode>
#include <WTreeNode>
#include <WTreeView>
#include <WLineEdit>
#include <WLabel>
#include <WPopupMenu>
#include <WGridLayout>
#include <WVBoxLayout>
#include <WHBoxLayout>
#include <WFileUpLoad>
#include <WProgressBar>
#include <WMessageBox>
#include <WIconPair>
#include <WStringUtil>

#include "FairDbWtCanvasWidget.h"
#include "FairDbWtFileLoader.h"
#include "FairDbWtFileTreeTable.h"
#include "FairDbWtFileTreeTableNode.h"
#include "FairDbWtTFileNode.h"
#include "FairDbWtJsRootProxy.h"
#include "FairDbWtRootCanvas.h"
#include "FairDbWtConfigViewDialog.h"
#include "FairDbWtRtopFrame.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>

#include "TRoot.h"
#include "TClass.h"
#include "TH1F.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TString.h"
#include "TBufferJSON.h"

using namespace std;
using namespace Wt;
namespace fs = ::boost::filesystem;

map<string, TFile*> FairDbWtRootBrowser::fTFileMap;


FairDbWtRootBrowser::FairDbWtRootBrowser(WContainerWidget *root, bool showTitle)
  : WContainerWidget(root)
  ,fFileTreeTable(nullptr)
  ,fCurrentNode(nullptr)
  ,fCanvas(nullptr)
{
  cout << "-I- FairDbWtRootBrowser ctor " << endl;
  gROOT->SetBatch(kTRUE);
  // Create the Browser layout
  createLayout(root);
  acceptDrops(FairDbWtTFileNode::FileSelectionMimeType);
}


FairDbWtRootBrowser::~FairDbWtRootBrowser(){
  cout << "-I- ~FairDbWtRootBrowser" << endl;
  // Object deletion
  if (fFileTreeTable) {
    delete fFileTreeTable;
    fFileTreeTable=nullptr;
  }
  // Clear the map
  fTFileMap.clear();
}



void FairDbWtRootBrowser::createLayout(WContainerWidget * parent){

  cout << "-I- FairDbWtRootBrowser create layout  " << endl;
  // Get the app server.
  //WApplication* app = WApplication::instance();
  // create the layout
  setStyleClass("contents");

  fMainLayout = new WGridLayout(parent);
  WVBoxLayout *vbox = new WVBoxLayout(parent);


  // Top Right Tab Frame
  FairDbWtRtopFrame* rFrame_top = new FairDbWtRtopFrame(this, parent);
  // DB                           width          heigth
  //rFrame_top->resize(WLength(98,WLength::Percentage), 1000);
  rFrame_top->resize(B_WW, B_HH);
  rFrame_top->setOverflow(OverflowAuto);
  rFrame_top->setAttributeValue
        ("oncontextmenu",
        "event.cancelBubble = true; event.returnValue = false; return false;");
  fTabWidget_top = new WTabWidget(rFrame_top);
  fTabWidget_top->setStyleClass("Wt-tabs");
  // Some decoration definition
  //fTabWidget_top->decorationStyle().setCursor(PointingHandCursor);
  fTabWidget_top->decorationStyle().font().setSize(WFont::XSmall);

  // Add first a dummy widget
  rFrame_top->addWidget(fTabWidget_top);

  // DB Comment out
  rFrame_top->mouseWentDown().connect(this, &FairDbWtRootBrowser::showPopup_Rtop);

  // add rframe_top
  vbox->addWidget(rFrame_top);

  // Down Right Frame

  WContainerWidget *rFrame_down = new WContainerWidget();
  rFrame_down->resize(B_WW,B_HH-600);
  rFrame_down->setOverflow(OverflowAuto);
  rFrame_down->setAttributeValue
             ("oncontextmenu",
              "event.cancelBubble = true; event.returnValue = false; return false;");
  fTabWidget_down = new WTabWidget();
  fTabWidget_down->setStyleClass("Wt-tabs");

  rFrame_down->addWidget(fTabWidget_down);
  rFrame_down->mouseWentDown().connect(this, &FairDbWtRootBrowser::showPopup_Rdown);

  // add frame_down to vbox
  // DB comment out
  //vbox->addWidget(rFrame_down);
  vbox->setResizable(0, true);

  // Add RVBox in gridLayout
  fMainLayout->addLayout(vbox, 0, 1);

  // Add LVBox
  WContainerWidget *l_Frame = new WContainerWidget();
  l_Frame->resize(800,50);
  l_Frame->setAttributeValue
             ("oncontextmenu",
              "event.cancelBubble = true; event.returnValue = false; return false;");

  WVBoxLayout *l_vbox = new WVBoxLayout;
  l_Frame->setLayout(l_vbox);

  // Upper Left container
  fL_up = new WContainerWidget();
  fL_up->setOverflow(OverflowAuto);
  fL_up->mouseWentDown().connect(this, &FairDbWtRootBrowser::showPopup_Lup);
  fL_up->setMinimumSize(300,50);

  // Down Left container
  fL_down = new WContainerWidget();
  fL_down->decorationStyle().font().setSize(WFont::XSmall);
  fL_down->setOverflow(OverflowAuto);
  fL_down->mouseWentDown().connect(this, &FairDbWtRootBrowser::showPopup_Ldown);
  fL_down->setMinimumSize(300,50);

  // Asssembly
  l_vbox->addWidget(fL_up, 0, Wt::AlignTop);
  l_vbox->addWidget(fL_down, 0, Wt::AlignTop);
  //l_vbox->setResizable(0);

  // Directory  path selection
  WGridLayout *l_grid = new WGridLayout();
  l_grid->addWidget(new WText("Directory path:"), 0, 0);
  fPathEdit = new WLineEdit("");
  fPathEdit->setTextSize(30);
  l_grid->addWidget(fPathEdit, 0, 1, Wt::AlignLeft);

  fPathEdit->enterPressed()
      .connect(this, &FairDbWtRootBrowser::loadFileTree);

  fL_up->setLayout(l_grid);

  fL_up->resize(300,WLength::Auto);
  fL_down->resize(300,WLength::Auto);
  l_Frame->resize(350,WLength::Auto);

  fMainLayout->addWidget(l_Frame, 0, 0, 1, 1, Wt::AlignTop);


  // Properties
  fMainLayout->setColumnResizable(0);
  fMainLayout->setRowStretch(1, 1);
  fMainLayout->setColumnStretch(1, 1);

  // Check changes in size
  setLayoutSizeAware(true);

  setLayout(fMainLayout);
}


void FairDbWtRootBrowser::loadFileTree() {
 // Here algorithms for recognizing files
 cout << "-I- loadFiletree directory: " << fPathEdit->text().narrow() <<  endl;
 if (fFileTreeTable){
   fL_down->removeWidget(fFileTreeTable);
   fFileTreeTable=nullptr;
 }
  fFileTreeTable = new FairDbWtFileTreeTable(fPathEdit->text().narrow());
  fFileTreeTable->resize(300, WLength::Auto);
  fFileTreeTable->tree()->setSelectionMode(SingleSelection);
  fFileTreeTable->treeRoot()->setNodeVisible(false);
  fFileTreeTable->treeRoot()->setChildCountPolicy(WTreeNode::Enabled);
  // Evt handler
  fFileTreeTable->tree()->itemSelectionChanged()
                    .connect(this, &FairDbWtRootBrowser::treeTableChanged);

  fL_down->addWidget(fFileTreeTable);
}

void FairDbWtRootBrowser::treeTableChanged() {

  // Get the tree
  WTree* aTree = fFileTreeTable->tree();
  // Get Tree Node
  WTreeTableNode* tNode = (WTreeTableNode*) fFileTreeTable->treeRoot();

  for (aWTreeNodeSet::const_iterator i = aTree->selectedNodes().begin();
                                  i != aTree->selectedNodes().end(); ++i){
     WTreeNode *aNode=(*i);
     // Set the current node
     fCurrentNode = aNode;
     // Get label
     const string pName= aNode->label()->text().toUTF8();
     // DB Check if Single-Composite node
     if (aNode->displayedChildCount()>0 ) continue;

     cout << "-I- FairDbWtRootBrowser: selected node: "
          <<  aNode->label()->text().value() << endl;

      analyseSelectedNode(fFileTreeTable->getFullPathName(aNode->label()->text().narrow()));
  }//!(nodes)

}


void FairDbWtRootBrowser::analyseSelectedNode(const string& name){
 // Here will the node analysed in terms of Root Objects
 cout << "-I- FairDbWtRootBrowser analyseSelectedNode name:" << name << endl;

 fs::path p (name.c_str());

 // does p actually exist?
 if (fs::exists(p))
 {
   // is p a regular file?
   if (fs::is_regular_file(p)){
     cout << p << "-I- FairDbWtRootBrowser found file : size is "
          << fs::file_size(p) << '\n';
     // Additional test for ROOT file extension
     if (p.extension()==".root"){
       cout << "-I- FairDbWtRootBrowser: found a root file, analyzing ..." << endl;
       TFile * f = nullptr;
       f = fTFileMap[p.string()];
       if (!f){
         f = new TFile(p.string().c_str());
         fTFileMap[p.string()] = f;
       }
       TIter next(f->GetListOfKeys());
       TKey *key;
       while ((key=(TKey*)next())) {
         printf("key: %s points to an object of class: %s at %dn \n",
         key->GetName(),
         key->GetClassName(),key->GetSeekKey());
         // Test for TNtuple ot TTree
         if (
           (strcmp(key->GetClassName(),"TNtuple")==0)
           ||
           (strcmp(key->GetClassName(),"TTree")==0)
         )
         {
           FairDbWtTFileNode *treeNode =
           new FairDbWtTFileNode(key->GetName(),key->GetClassName());
           treeNode->setPath(p);
           if (fCurrentNode) fCurrentNode->addChildNode(treeNode);
           // Read the underlying TTree
           TTree* aTree = dynamic_cast<TTree*> (key->ReadObj());
           //cout << "-I- TNtuple found !" << aTree->GetName() << endl;
           TObjArray* t_arr = aTree->GetListOfLeaves();
           int n_leafs = t_arr->GetEntries();

           for (int i=0;i<n_leafs;i++){
             TLeaf* leaf = (TLeaf*) t_arr->At(i);
             cout << "-I- leafs:" << leaf->GetName()<< ":"
             << leaf->GetTypeName() << endl;
             FairDbWtTFileNode* tNode=
             new FairDbWtTFileNode(leaf->GetName(),TLeaf::Class()->GetName(),
                                    key->GetName(), true);
             tNode->setPath(p);
             if(treeNode) treeNode->addChildNode(tNode);
           }//! (for)

         }else{
           FairDbWtTFileNode *tfNode =
           new FairDbWtTFileNode(key->GetName(), key->GetClassName());
           tfNode->setPath(p);
           if (fCurrentNode) fCurrentNode->addChildNode(tfNode);
         }
       }//! TFile contents
     }// ! root extension
   }// ! regular file

   // is p a directory?
   else if (fs::is_directory(p)){
     cout << p << "-I- FairDbWtRootBrowser: node is a directory\n";
   }else{
     cout << p << "-I- FairDbWtRootBrowser path exists, but is neither a regular file nor a directory\n";
   }
 }//! is Exists
 else{
   cout << "-I- FairDbWtRootBrowser eventually is a TKey from file? " << name << endl;
   analyseTKey();
   return;
 }

}


void FairDbWtRootBrowser::analyseTKey(FairDbWtTFileNode* cNode){
  //Check the current Node name and Mother node
  WTreeNode* pNode = fCurrentNode->parentNode();
  cout << " -I- FairDbWtRootBrowser  analyseTKey current node : parent node "
  << fCurrentNode->label()->text()
  << " : "
  << pNode->label()->text() << endl;

  // Get Corresponding file node in cas of selected item
  if (!cNode) cNode = dynamic_cast<FairDbWtTFileNode*>(fCurrentNode);


  // Get Corresponding TFile
  const fs::path p = cNode->getPath();
  TFile * f = nullptr;
  f = fTFileMap[p.string()];
  if (!f) {
    cout << "-E- FairDbWtRootBrowser::analyseTKey cannnot find file at " << p.string() <<  endl;
  }else{
    cout << "-I- FairDbWtRootBrowser::analyseTKey found file " << f->GetName() << endl;
  }

  if (!cNode->isLeaf()){
    // Inspect Object
    TIter nextkey( f->GetListOfKeys() );
    TKey *key;
    while ( (key = (TKey*)nextkey())) {
      // Select corresponding key
      //cout << "key: " << key->GetName() << " name: " << fCurrentNode->label()->text().narrow().c_str() << endl;
      if( std::strcmp(key->GetName(), cNode->label()->text().narrow().c_str()) != 0 ) continue;

      TObject *obj = key->ReadObj();
      display(obj);
    }//!While()
  }else{
    cout << "Leaf: " << cNode->label()->text()
    << " correponding TTree: " <<  cNode->getTreeName() << endl;
    TTree *t = dynamic_cast<TTree*> (f->Get(cNode->getTreeName().c_str()));
    if (t){

      string h_name = cNode->getTreeName() + "_" +
                      cNode->label()->text().narrow().c_str();

      std::stringstream ss;
      ss << cNode->label()->text().narrow().c_str()
         << ">>" << h_name.c_str();

      t->Draw(ss.str().c_str());
      cout << "Draw done" << endl;

      TH1* hist = dynamic_cast<TH1*> (gROOT->FindObject(h_name.c_str()));
      hist->SetTitle(cNode->label()->text().narrow().c_str());
      if (hist) display(hist);
    }
  }//! isLeaf()

}

void FairDbWtRootBrowser::display(TObject *obj)
{
  // New display functionality
  // Look if tab exists , then use it with
  // the user layout

  // Display Object with its envelop
  int idx = fTabWidget_top->currentIndex();
  FairDbWtCanvasWidget * cont_main= nullptr;
  if (idx>=0){
    // Test if canvas-based presentation
    cont_main =
    dynamic_cast<FairDbWtCanvasWidget*> (fTabWidget_top->widget(idx));
    // Update Tab Text
    fTabWidget_top->setTabText(idx,WString(obj->GetName()));
  }

  // A Canvased tab is found
  if (cont_main){
    WGridLayout* gLayout = (WGridLayout*) cont_main->layout();
    // I suppose a layout already exists
    if (gLayout){
      int i_row = 0;
      int i_col = 0;
      cont_main->getFreeSlot(i_row,i_col);

      cout << "-I- display val ---> row, col " << i_row
      << " : " << i_col << endl;

      WContainerWidget* envelop =
      FairDbWtJsRootProxy::createContainerWidget(nullptr,obj,B_WW-100,B_HH-100);
      int n_col = cont_main->getColumns();
      int n_row = cont_main->getRows();

      //DB reinforce a concrete size to the envelop
      //container
      envelop->resize(B_WW-100,B_HH-100);
      gLayout->addWidget(envelop, i_row, i_col, Wt::AlignCenter);
    }

  }else{
    // single case
    cont_main =
    new FairDbWtCanvasWidget(nullptr, B_WW-100, B_HH-100, 1, 1);
    WGridLayout* gLayout = (WGridLayout*) cont_main->layout();
    WContainerWidget* envelop = nullptr;
    if (gLayout){
      int i_row = 0;
      int i_col = 0;
      cont_main->getFreeSlot(i_row,i_col);
      envelop = FairDbWtJsRootProxy::createContainerWidget(nullptr,obj,B_WW-100,B_HH-100);
      int n_col = cont_main->getColumns();
      int n_row = cont_main->getRows();
      envelop->resize(B_WW-100,B_HH-100);
      gLayout->addWidget(envelop, i_row, i_col, Wt::AlignCenter);
    }

    if (envelop){
      // DB use directly envelop container in the tab
      WMenuItem *tab = fTabWidget_top->addTab(cont_main, obj->GetName());
      int index = fTabWidget_top->indexOf(cont_main);
      WWidget* wi = fTabWidget_top->widget(index);
      fTabWidget_top->setCurrentIndex(index);
      tab->setCloseable(false);
    }
  }// !else
}

void FairDbWtRootBrowser::defineLayout(WContainerWidget* container, int col, int row){

  int columnCount     = col;
  int rowCount        = row;

  WGridLayout *grid = new WGridLayout();

  // Disable Grid spacing
  grid->setHorizontalSpacing(9);
  grid->setVerticalSpacing(9);
  //grid->setContentsMargins(20,20,20,20);

  // Divide the grid into equally-sized cells with stretch 0
  for (int c = 0; c < columnCount; ++c){
    grid->setColumnStretch(c, 1);
    grid->setColumnResizable(c);
    for (int r = 0; r < rowCount; ++r){
      grid->setRowStretch(r, 1);
      grid->setRowResizable(r);
      WContainerWidget *cw =
      FairDbWtJsRootProxy::createContainerWidget(nullptr,nullptr);
      cw->resize(B_WW-100,B_HH-100);
      grid->addWidget(cw, r, c, (Wt::AlignCenter | Wt::AlignMiddle));
    }
  }

  container->setLayout(grid);

  return;
}


void FairDbWtRootBrowser::showPopup_Lup(const WMouseEvent& event) {
  //<DB> here is a strange error coming when using
  // securized sessions ..

    if (event.button() == WMouseEvent::RightButton) {
    // cout << "FairDbWtRootBrowser::showPopup_Lup 1  --> fPopup" << fPopup << endl;
    // CHECK ME suboptimal
    if (fPopup) {delete fPopup; fPopup=NULL;}
    //cout << "FairDbWtRootBrowser::showPopup_Lup  2 --> fPopup" << fPopup << endl;

    if (!fPopup) {

      //cout << "FairDbWtRootBrowser::showPopup_Lup 3 --> fPopup" << fPopup << endl;
      fPopup = new WPopupMenu();

      fPopup->addItem("View")->triggered().connect(std::bind([=] () {
            inspectFile();
        }));
      //cout << "FairDbWtRootBrowser::showPopup_Lup 4 --> fPopup" << fPopup << endl;
      fPopup->addSeparator();
      fPopup->addItem("History");

     // fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction_Lup);
    }
      if (fPopup->isHidden()){
      //  cout << "FairDbWtRootBrowser::showPopup_Lup  5 --> fPopup" << fPopup << endl;
         fPopup->popup(event);
        }else{
         fPopup->hide();
       }

     }//! if (event)
}

void FairDbWtRootBrowser::showPopup_Ldown(const WMouseEvent& event) {
  //<DB> here is a strange error coming when using
  // securized sessions ..

    if (event.button() == WMouseEvent::RightButton) {
     //cout << "FairDbWtRootBrowser::showPopup_Ldown 1  --> fPopup" << fPopup << endl;
    // CHECK ME suboptimal
    if (fPopup) {delete fPopup; fPopup=NULL;}
    //cout << "FairDbWtRootBrowser::showPopup_Ldown  2 --> fPopup" << fPopup << endl;

    if (!fPopup) {
      //cout << "FairDbWtRootBrowser::showPopup_Ldown 3 --> fPopup" << fPopup << endl;
      fPopup = new WPopupMenu();

      fPopup->addItem("View")->triggered().connect(std::bind([=] () {
            inspectFile();
        }));
      //cout << "FairDbWtRootBrowser::showPopup_Ldown 4 --> fPopup" << fPopup << endl;
      fPopup->addSeparator();
      fPopup->addItem("History");

     // fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction_Lup);
    }
      if (fPopup->isHidden()){
      //cout << "FairDbWtRootBrowser::showPopup_Ldown  5 --> fPopup" << fPopup << endl;
         fPopup->popup(event);
        }else{
         fPopup->hide();
       }
     }//! if (event)
}


void FairDbWtRootBrowser::inspectFile() {
 // Here algorithms for recognizing files
 cout << "-I- FairDbWtRootBrowser::inspectFile()" << endl;
 // Get the tree
 WTree* aTree = fFileTreeTable->tree();
 // Get Tree Node
 WTreeTableNode* tNode = (WTreeTableNode*) fFileTreeTable->treeRoot();

 for (aWTreeNodeSet::const_iterator i = aTree->selectedNodes().begin();
                                 i != aTree->selectedNodes().end(); ++i){
    WTreeNode *aNode=(*i);
    // Set the current node
    fCurrentNode = aNode;
    // Get label
    const string pName= aNode->label()->text().toUTF8();
    // DB Check if Single-Composite node
    if (aNode->displayedChildCount()>0 ) continue;

    cout << "-I- FairDbWtRootBrowser:inspectFile selected node: "
         <<  aNode->label()->text().value() << endl;

    // analyseSelectedNode(fFileTreeTable->getFullPathName(aNode->label()->text().narrow()));
 }//!(nodes)

}


void FairDbWtRootBrowser::showPopup_Rtop(const WMouseEvent& event)
{
    if (event.button() == WMouseEvent::RightButton) {
    // CHECK ME suboptimal
    if (fPopup) {delete fPopup; fPopup=NULL;}

    if (!fPopup) {
      fPopup = new WPopupMenu();
      fPopup->addItem("icons/folder.gif", "Close");
      fPopup->addItem("icons/folder.gif", "Divide");
      fPopup->addSeparator();

      WPopupMenu *subMenu = new WPopupMenu();
      subMenu->addItem("Draw");
      subMenu->addItem("History");
      fPopup->addMenu("Actions", subMenu);

      fPopup->aboutToHide().connect(this, &FairDbWtRootBrowser::popupAction_Rtop);
    }

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event)
}

void FairDbWtRootBrowser::popupAction_Rtop()
{
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();

      if (text == "Close"){
        fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
        fPopupActionBox->buttonClicked().connect(this, &FairDbWtRootBrowser::dialogDone_Rtop);
        fPopupActionBox->show();
      } else if(text == "Divide"){
        fConfigDialog = new FairDbWtConfigViewDialog(fTabWidget_top);
      }


    } else {
      fPopup->hide();
    }
}

void FairDbWtRootBrowser::dialogDone_Rtop()
{
    if (fTabWidget_top->currentIndex()>=0)
      {
       int index =  fTabWidget_top->currentIndex();
       fTabWidget_top->removeTab(fTabWidget_top->widget(index));
       }
    if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
    }
}

void FairDbWtRootBrowser::showPopup_Rdown(const WMouseEvent& event)
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

      fPopup->aboutToHide().connect(this, &FairDbWtRootBrowser::popupAction_Rdown);
    }

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event)
}

void FairDbWtRootBrowser::popupAction_Rdown()
{
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();

      fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &FairDbWtRootBrowser::dialogDone_Rdown);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}

void FairDbWtRootBrowser::dialogDone_Rdown()
{
    cout << "-I- called dialogdone Rdown from base" << endl;
    if (fTabWidget_down->currentIndex()>=0){
       int index =  fTabWidget_down->currentIndex();
       fTabWidget_down->removeTab(fTabWidget_down->widget(index));
     }
    if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
    }
}


 void FairDbWtRootBrowser::dropEvent(WDropEvent event)
{
  cout << "-------- DROPED to Root Browser ---------- " << endl;
  if (event.mimeType() == FairDbWtTFileNode::FileSelectionMimeType) {
   cout << "-I- FairDbWtRootBrowser dropEvent called with mimeType" << endl;
  }
}

#include <WText>
#include <WMenu>
#include <WStackedWidget>
#include <WVBoxLayout>
#include <WHBoxLayout>
#include <WNavigationBar>
#include <Wt/WTreeTableNode>
#include <WPanel>
#include <WTable>

#include <iostream>

// Events Handling
#include <Wt/WBreak>
#include <Wt/WCssDecorationStyle>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/Utils>


#include "FileTreeTable.h"
#include "FairDbWtTreeView.h"
#include "FairDbWtParTreeTable.h"
#include "FairDbWtParTreeTableNode.h"
#include "FairDbWtPaintedImage.h"
#include "FairDbWtTCanvas.h"

// DB Test for Root histograms
#include "TColor.h"
#include "TImage.h"
#include "TImageDump.h"
#include "TVirtualPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFrame.h"


using namespace std;

using namespace Wt;

FairDbWtTreeView::FairDbWtTreeView(WContainerWidget *parent):
  WContainerWidget(parent)
{
 
     setStyleClass("maindiv");

     WGridLayout *layout = new WGridLayout(this);
     layout->addWidget(createTitle("Parameters"), 0, 0);
     layout->addWidget(createTitle("View"), 0, 1);
     layout->addWidget(TreeTableView(), 1, 0); 
     layout->setColumnResizable(0);
     

    // <DB> do not put anchor "this" to the Box Layout
     WVBoxLayout *vbox = new WVBoxLayout();
  
     // Top Right Frame
     WContainerWidget *rFrame_top = new WContainerWidget();  
     rFrame_top->setOverflow(OverflowAuto);  
     //rFrame_top->resize(WLength::Auto,WLength::Auto);   
     rFrame_top->setAttributeValue
             ("oncontextmenu",
              "event.cancelBubble = true; event.returnValue = false; return false;"); 
     fTabWidget_top = new WTabWidget(); 
     fTabWidget_top->setStyleClass("tabwidget");
     
     rFrame_top->addWidget(fTabWidget_top);
     rFrame_top->mouseWentDown().connect(this, &FairDbWtTreeView::showPopup);    
     vbox->addWidget(rFrame_top,1);

 
     //Down Right Frame 
     WContainerWidget *rFrame_down = new WContainerWidget();
     rFrame_down->setOverflow(OverflowAuto);    
     rFrame_down->setAttributeValue
             ("oncontextmenu",
              "event.cancelBubble = true; event.returnValue = false; return false;"); 
     fTabWidget_down = new WTabWidget(); 
     fTabWidget_down->setStyleClass("tabwidget");
     
     rFrame_down->addWidget(fTabWidget_down);
     rFrame_down->mouseWentDown().connect(this, &FairDbWtTreeView::showPopup);
   
     // ROOT Histos 
     //WWidget *whist = ghist();
     //WMenuItem *tab = fTabWidget_down->addTab(whist, "TGraph", WTabWidget::PreLoading);
    
     vbox->addWidget(rFrame_down,1);
     vbox->setResizable(0);

     // Add VBox
     layout->addLayout(vbox, 1, 1);
     layout->setRowStretch(1, 1);
     layout->setColumnStretch(1, 1);
     
       
     setLayout(layout);

}

FairDbWtTreeView::~FairDbWtTreeView(){

    if (fPopup) delete fPopup;
    if (fPopupActionBox) delete fPopupActionBox;
}


void FairDbWtTreeView::showMeMouse(const WMouseEvent& event) {
 if (event.button() == WMouseEvent::RightButton) {
    cout << "-I- RIGHT BUTTON FROM MOUSE" << endl;
}
if (event.button() == WMouseEvent::LeftButton) {
    cout << "-I- LEFT BUTTON FROM MOUSE" << endl;
}

}


WText* FairDbWtTreeView::createTitle(const WString& title) {
    WText *result = new WText(title);
    result->setInline(false);
    result->setStyleClass("title");
    return result;
}

FairDbWtParTreeTable* FairDbWtTreeView::TreeTableView(){

     //FileTreeTable *treeTable = new FileTreeTable(".");
     FairDbWtParTreeTable *treeTable = new FairDbWtParTreeTable();
     treeTable->resize(500, 1200);
     //treeTable->resize(500, WLength::Auto);
     treeTable->tree()->setSelectionMode(ExtendedSelection);
     treeTable->treeRoot()->setNodeVisible(false);
     treeTable->treeRoot()->setChildCountPolicy(WTreeNode::Enabled);

     treeTable->tree()->itemSelectionChanged()
                    .connect(this, &FairDbWtTreeView::TreeTableChanged); 
  


    fFileTreeTable=treeTable; 
    return treeTable; 
}

void FairDbWtTreeView::TreeTableChanged() {
// Get the tree
WTree* aTree = fFileTreeTable->tree(); 
// Get Tree Node
FairDbWtParTreeTableNode* tNode = (FairDbWtParTreeTableNode*) fFileTreeTable->treeRoot();
  
  for (WTreeNodeSet::const_iterator i = aTree->selectedNodes().begin(); 
                                  i != aTree->selectedNodes().end(); ++i){
     WTreeNode *aNode=(*i);

     cout << "-I Server: FairDbWtTreeView::TreeTableChanged() Node:" <<  aNode->label()->text().value() << endl;     
 
     string pName= aNode->label()->text().toUTF8();
     Wt::WTable* table = tNode->createTable(this,pName); 
     if (table){ 
       WMenuItem *tab = fTabWidget_top->addTab(table, pName, WTabWidget::PreLoading);
        int index = fTabWidget_top->indexOf(table);
        fTabWidget_top->setCurrentIndex(index);
        tab->setCloseable(false);
     }
  }

}

WTreeView* FairDbWtTreeView::folderView() {
    WTreeView *treeView = new FolderView();
    treeView->setRowHeight(25);   
    treeView->setAttributeValue
      ("oncontextmenu",
       "event.cancelBubble = true; event.returnValue = false; return false;");
     treeView->setModel(fFolderModel);
     treeView->resize(200, WLength::Auto);
     treeView->setSelectionMode(SingleSelection);
     treeView->expandToDepth(1);
     treeView->selectionChanged()
       .connect(this, &FairDbWtTreeView::folderChanged);

     //treeView->mouseWentUp().connect(this, &FairDbWtTreeView::showPopup);

    fFolderView = treeView;

    return treeView;
  }


void FairDbWtTreeView::folderChanged() {
    if (fFolderView->selectedIndexes().empty())
      return;

    WModelIndex selected = *fFolderView->selectedIndexes().begin();
    boost::any d = selected.data(UserRole);
    if (!d.empty()) {
      std::string folder = boost::any_cast<std::string>(d);

      // For simplicity, we assume here that the folder-id does not
      // contain special regexp characters, otherwise these need to be
      // escaped -- or use the \Q \E qutoing escape regular expression
      // syntax (and escape \E)
      fFileFilterModel->setFilterRegExp(folder);
    }
  }





void FairDbWtTreeView::showPopup(const WMouseEvent& event) {
    if (event.button() == WMouseEvent::RightButton) {
      // Select the item, it was not yet selected.
      //if (!fFolderView->isSelected(item))
      //fFolderView->select(item);

    if (!fPopup) {
      fPopup = new WPopupMenu();
      fPopup->addItem("icons/folder.gif", "Close");
      fPopup->addSeparator();
      
      WPopupMenu *subMenu = new WPopupMenu();
      subMenu->addItem("Draw");
      subMenu->addItem("History");
      fPopup->addMenu("Actions", subMenu);

    
      fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction);
    }

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event) 
}


void FairDbWtTreeView::popupAction() {
    if (fPopup->result()) {
      /*
       * You could also bind extra data to an item using setData() and
       * check here for the action asked. For now, we just use the text.
       */
      WString text = fPopup->result()->text();
      fPopup->hide();
         
      fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &FairDbWtTreeView::dialogDone);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}


void FairDbWtTreeView::dialogDone() {
    int index = fTabWidget_top->currentIndex();
    fTabWidget_top->removeTab(fTabWidget_top->widget(index));

    if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
    }
}



WStandardItem * FairDbWtTreeView::createFolderItem(const WString& location,
                  const std::string& folderId = std::string()){

    WStandardItem *result = new WStandardItem(location);

    if (!folderId.empty()) {
      result->setData(boost::any(folderId));
      result->setFlags(result->flags() | ItemIsDropEnabled);
      fFolderNameMap[folderId] = location;
    } else
      result->setFlags(result->flags().clear(ItemIsSelectable));

    result->setIcon("icons/folder.gif");

    return result;
}







void FairDbWtTreeView::populateFolders() {
    WStandardItem *level1, *level2;

    fFolderModel->appendRow(level1 = createFolderItem("San Fransisco"));
    level1->appendRow(level2 = createFolderItem("Investors", "sf-investors"));
    level1->appendRow(level2 = createFolderItem("Fellows", "sf-fellows"));

    fFolderModel->appendRow(level1 = createFolderItem("Sophia Antipolis"));
    level1->appendRow(level2 = createFolderItem("R&D", "sa-r_d"));
    level1->appendRow(level2 = createFolderItem("Services", "sa-services"));
    level1->appendRow(level2 = createFolderItem("Support", "sa-support"));
    level1->appendRow(level2 = createFolderItem("Billing", "sa-billing"));

    fFolderModel->appendRow(level1 = createFolderItem("New York"));
    level1->appendRow(level2 = createFolderItem("Marketing", "ny-marketing"));
    level1->appendRow(level2 = createFolderItem("Sales", "ny-sales"));
    level1->appendRow(level2 = createFolderItem("Advisors", "ny-advisors"));

    fFolderModel->appendRow(level1 = createFolderItem
                 (WString::fromUTF8("Frankfürt")));
    level1->appendRow(level2 = createFolderItem("Sales", "frank-sales"));

    fFolderModel->setHeaderData(0, Horizontal,
                 boost::any(std::string("SandBox")));
}


WWidget* FairDbWtTreeView::ghist(){
   
     gROOT->SetBatch(kTRUE);
     TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
     c1->SetGrid();

     const Int_t n = 20;
     Double_t x[n], y[n];
     for (Int_t i=0;i<n;i++) {
      x[i] = i*0.1;
      y[i] = 10*sin(x[i]+0.2);
      printf(" i %i %f %f \n",i,x[i],y[i]);
    }
    TGraph* gr = new TGraph(n,x,y);
    gr->SetLineColor(2);
    gr->SetLineWidth(4);
    gr->SetMarkerColor(4);
    gr->SetMarkerStyle(21);
    gr->SetTitle("a simple graph");
    gr->GetXaxis()->SetTitle("X title");
    gr->GetYaxis()->SetTitle("Y title");
    gr->Draw("ACP");

   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   c1->Modified();



    FairDbWtTCanvas *contents = new FairDbWtTCanvas(this,c1); 
     return contents;
}

WWidget* FairDbWtTreeView::rhist(){
 
   // DB test trivial functionality on Image for ROOT graphics
   // DB This example convert a Graph to a PNG to be sent to
   // client 

   // switch ROOT to batch mode
   gROOT->SetBatch(kTRUE);

   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
   //c1->ToggleEventStatus();
   //c1->SetRightMargin(0.2);
   //c1->SetLeftMargin(0.01);
   //c1->SetTopMargin(0.01);
   //c1->SetBottomMargin(0.01);
     
   //c1->SetFillColor(42);
   c1->SetGrid();

   const Int_t n = 20;
   Double_t x[n], y[n];
   for (Int_t i=0;i<n;i++) {
     x[i] = i*0.1;
     y[i] = 10*sin(x[i]+0.2);
     printf(" i %i %f %f \n",i,x[i],y[i]);
   }
   TGraph* gr = new TGraph(n,x,y);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->SetTitle("a simple graph");
   gr->GetXaxis()->SetTitle("X title");
   gr->GetYaxis()->SetTitle("Y title");
   gr->Draw("ACP");

   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   c1->Modified();
  

  // create gVirtualPS object
   TImageDump dmp("dummy.png");
  // image associated with image_dump  
   TImage *fore = dmp.GetImage();  
  // Resize canvas
   gPad->SetCanvasSize(1000, 1000);
        
  // paint gPad on fore image associated with TImageDump  
   gPad->Paint(); 
   fore->WriteImage("/Users/denis/fairroot/fairbase/dbase/dbWt/docroot/test_graph.png");
  
 
  // Back to Wt  
  //setLayoutSizeAware(true); 
  //ww->setOverflow(OverflowAuto);  

  WContainerWidget* ww = new WContainerWidget(); 
  //ww->setMargin(WLength::Auto, Top | Bottom);
  //ww->setMargin(WLength::Auto, Left | Right);

  FairDbWtPaintedImage* img = new FairDbWtPaintedImage();
  img->resize(fore->GetWidth(),fore->GetHeight());  
  img->setMargin(10, Top | Bottom);
  img->setMargin(10, Left | Right);
  ww->addWidget(img);  
 return ww; 
}



WWidget *FairDbWtTreeView::hist(){
 return 0;
}


//Event Handling
WWidget *FairDbWtTreeView::wMouseEvent()
{
  WContainerWidget *result = new WContainerWidget();

  //topic("WMouseEvent", result);
  //addText(tr("events-WMouseEvent"), result);
  WContainerWidget *c = new WContainerWidget(result);
  WHBoxLayout *hlayout = new WHBoxLayout;
  c->setLayout(hlayout);
  WContainerWidget *l = new WContainerWidget;
  WContainerWidget *r = new WContainerWidget;
  new WText("clicked<br/>doubleClicked<br/>mouseWentOut<br/>mouseWentOver",
        l);
  new WText("mouseWentDown<br/>mouseWentUp<br/>mouseMoved<br/>mouseWheel", r);
  hlayout->addWidget(l);
  hlayout->addWidget(r);
  c->resize(600, 300);
  l->decorationStyle().setBackgroundColor(Wt::gray);
  r->decorationStyle().setBackgroundColor(Wt::gray);
  // prevent that firefox interprets drag as drag&drop action
  l->setStyleClass("unselectable");
  r->setStyleClass("unselectable");
  l->clicked().connect(this, &FairDbWtTreeView::showClicked);
  l->doubleClicked().connect(this, &FairDbWtTreeView::showDoubleClicked);
  l->mouseWentOut().connect(this, &FairDbWtTreeView::showMouseWentOut);
  l->mouseWentOver().connect(this, &FairDbWtTreeView::showMouseWentOver);
  r->mouseMoved().connect(this, &FairDbWtTreeView::showMouseMoved);
  r->mouseWentUp().connect(this, &FairDbWtTreeView::showMouseWentUp);
  r->mouseWentDown().connect(this, &FairDbWtTreeView::showMouseWentDown);
  r->mouseWheel().connect(this, &FairDbWtTreeView::showMouseWheel);
  r->mouseWheel().preventDefaultAction(true);

  l->setAttributeValue
    ("oncontextmenu",
     "event.cancelBubble = true; event.returnValue = false; return false;");
  r->setAttributeValue
    ("oncontextmenu",
     "event.cancelBubble = true; event.returnValue = false; return false;");

  new WBreak(result);
  new WText("Last event: ", result);
  mouseEventType_ = new WText(result);
  new WBreak(result);
  mouseEventDescription_ = new WText(result);

  return result;
}



void FairDbWtTreeView::showClicked(const WMouseEvent &e)
{
  mouseEventType_->setText("clicked");
  describe(e);
}

void FairDbWtTreeView::showDoubleClicked(const WMouseEvent &e)
{
  mouseEventType_->setText("doubleClicked");
  describe(e);
}

void FairDbWtTreeView::showMouseWentOut(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentOut");
  describe(e);
}

void FairDbWtTreeView::showMouseWheel(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWheel");
  describe(e);
}

void FairDbWtTreeView::showMouseWentOver(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentOver");
  describe(e);
}

void FairDbWtTreeView::showMouseMoved(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseMoved");
  describe(e);
}

void FairDbWtTreeView::showMouseWentUp(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentUp");
  describe(e);
}

void FairDbWtTreeView::showMouseWentDown(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentDown");
  describe(e);
}


namespace  my_strutils{
  std::ostream &operator<<(std::ostream &o, Wt::WMouseEvent::Button b)
  {
    switch (b) {
    case WMouseEvent::NoButton:
      return o << "No button";
    case WMouseEvent::LeftButton:
      return o << "LeftButton";
    case WMouseEvent::RightButton:
      return o << "RightButton";
    case WMouseEvent::MiddleButton:
      return o << "MiddleButton";
    default:
      return o << "Unknown Button";
    }
  }

  std::ostream &operator<<(std::ostream &o, Wt::Key k)
  {
    switch(k) {
    default:
    case Key_unknown : return o << "Key_unknown";
    case Key_Enter : return o << "Key_Enter";
    case Key_Tab : return o << "Key_Tab";
    case Key_Backspace : return o << "Key_Backspace";
    case Key_Shift : return o << "Key_Shift";
    case Key_Control : return o << "Key_Control";
    case Key_Alt : return o << "Key_Alt";
    case Key_PageUp : return o << "Key_PageUp";
    case Key_PageDown : return o << "Key_PageDown";
    case Key_End : return o << "Key_End";
    case Key_Home : return o << "Key_Home";
    case Key_Left : return o << "Key_Left";
    case Key_Up : return o << "Key_Up";
    case Key_Right : return o << "Key_Right";
    case Key_Down : return o << "Key_Down";
    case Key_Insert : return o << "Key_Insert";
    case Key_Delete : return o << "Key_Delete";
    case Key_Escape : return o << "Key_Escape";
    case Key_F1 : return o << "Key_F1";
    case Key_F2 : return o << "Key_F2";
    case Key_F3 : return o << "Key_F3";
    case Key_F4 : return o << "Key_F4";
    case Key_F5 : return o << "Key_F5";
    case Key_F6 : return o << "Key_F6";
    case Key_F7 : return o << "Key_F7";
    case Key_F8 : return o << "Key_F8";
    case Key_F9 : return o << "Key_F9";
    case Key_F10 : return o << "Key_F10";
    case Key_F11 : return o << "Key_F11";
    case Key_F12 : return o << "Key_F12";
    case Key_Space : return o << "Key_Space";
    case Key_A : return o << "Key_A";
    case Key_B : return o << "Key_B";
    case Key_C : return o << "Key_C";
    case Key_D : return o << "Key_D";
    case Key_E : return o << "Key_E";
    case Key_F : return o << "Key_F";
    case Key_G : return o << "Key_G";
    case Key_H : return o << "Key_H";
    case Key_I : return o << "Key_I";
    case Key_J : return o << "Key_J";
    case Key_K : return o << "Key_K";
    case Key_L : return o << "Key_L";
    case Key_M : return o << "Key_M";
    case Key_N : return o << "Key_N";
    case Key_O : return o << "Key_O";
    case Key_P : return o << "Key_P";
    case Key_Q : return o << "Key_Q";
    case Key_R : return o << "Key_R";
    case Key_S : return o << "Key_S";
    case Key_T : return o << "Key_T";
    case Key_U : return o << "Key_U";
    case Key_V : return o << "Key_V";
    case Key_W : return o << "Key_W";
    case Key_X : return o << "Key_X";
    case Key_Y : return o << "Key_Y";
    case Key_Z : return o << "Key_Z";
    }
  }

  std::ostream &operator<<(std::ostream &o, Wt::WMouseEvent::Coordinates c)
  {
    return o << c.x << ", " << c.y;
  }
  std::string modifiersToString(const WFlags< KeyboardModifier >& modifiers)
  {
    std::stringstream o;
    if (modifiers & ShiftModifier) o << "Shift ";
    if (modifiers & ControlModifier) o << "Control ";
    if (modifiers & AltModifier) o << "Alt ";
    if (modifiers & MetaModifier) o << "Meta ";
    if (modifiers == 0) o << "No modifiers";
    return o.str();
  }
}



void FairDbWtTreeView::describe(const Wt::WMouseEvent &e)
{
  std::stringstream ss;
  ss << "Button: " << e.button() << "<br/>"
     << "Modifiers: " << my_strutils::modifiersToString(e.modifiers()) << "<br/>";
     //<< "Document coordinates: " << my_strutils::modifiersToString(e.document()) << "<br/>"
     //<< "Window coordinates: " << e.window() << "<br/>"
     //<< "Screen coordinates: " << e.screen() << "<br/>"
     //<< "Widget coordinates: " << e.widget() << "<br/>"
     //<< "DragDelta coordinates: " << e.dragDelta() << "<br/>"
     //<< "Wheel delta: " << e.wheelDelta() << "<br/>";
  mouseEventDescription_->setText(ss.str());
}








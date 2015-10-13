#include <WText>
#include <WMenu>
#include <WStackedWidget>
#include <WVBoxLayout>
#include <WHBoxLayout>
#include <WNavigationBar>
#include <WTreeTableNode>
#include <WStandardItemModel>

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
#include "TAxis.h"


using namespace std;

using namespace Wt;

FairDbWtTreeView::FairDbWtTreeView(WContainerWidget *parent):
  WContainerWidget(parent)
  ,fPopup(NULL)
  ,fPopupActionBox(NULL)
 {
  // <DB> here is the init of members
  // important if used in dtor.
 }

FairDbWtTreeView::~FairDbWtTreeView(){

  cout << "~FairDbTreeView 1 " << endl; 
  if (fPopup) delete fPopup;
  if (fPopupActionBox) delete fPopupActionBox;
  cout << "~FairDbTreeView 2 " << endl; 
}


void FairDbWtTreeView::createLayout(WContainerWidget *parent) {

  cout << "-------------------- FairDbTreeView  ctor   -------- " << endl;
  setStyleClass("contents");
    
  fMainLayout = new WGridLayout(parent);
  fMainLayout->addWidget(createTitle("Parameters"), 0, 0);
  fMainLayout->addWidget(createTitle("Views"), 0, 1);    
  
  
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
  fTabWidget_top->setStyleClass("Wt-tabs");
  
  // Some deco definition
  //fTabWidget_top->decorationStyle().setCursor(PointingHandCursor);
  //fTabWidget_top->decorationStyle().font().setSize(WFont::XSmall);

  rFrame_top->addWidget(fTabWidget_top);
  rFrame_top->mouseWentDown().connect(this, &FairDbWtTreeView::showPopup_Rtop);    
  vbox->addWidget(rFrame_top,1);

 
  //Down Right Frame 
  WContainerWidget *rFrame_down = new WContainerWidget();
  rFrame_down->setOverflow(OverflowAuto);    
  rFrame_down->setAttributeValue
             ("oncontextmenu",
              "event.cancelBubble = true; event.returnValue = false; return false;"); 
  fTabWidget_down = new WTabWidget(); 
  fTabWidget_down->setStyleClass("Wt-tabs");
     
  rFrame_down->addWidget(fTabWidget_down);
  rFrame_down->mouseWentDown().connect(this, &FairDbWtTreeView::showPopup_Rdown);    
       
  vbox->addWidget(rFrame_down,1);
  vbox->setResizable(0);

  // Add RVBox
  fMainLayout->addLayout(vbox, 1, 1);
  
  // Add LVBox
  WContainerWidget *l_Frame = new WContainerWidget();
  l_Frame->setAttributeValue
             ("oncontextmenu",
              "event.cancelBubble = true; event.returnValue = false; return false;"); 
  
  //<DB> this overflow creates problems 
  //l_Frame->setOverflow(OverflowAuto);  
  

  WVBoxLayout *l_vbox = new WVBoxLayout;
  l_Frame->setLayout(l_vbox);
  
  WContainerWidget *l_up = new WContainerWidget();
  WContainerWidget *l_down = new WContainerWidget();
  l_up->setOverflow(OverflowAuto);
  l_down->setOverflow(OverflowAuto);

  l_up->mouseWentDown().connect(this, &FairDbWtTreeView::showPopup_Lup);

  l_vbox->addWidget(l_up);
  l_vbox->addWidget(l_down);
  l_vbox->setResizable(0); 
  
  //setLayoutSizeAware(true);
  
  //l_up->decorationStyle().setBackgroundColor(Wt::lightGray);
  //l_down->decorationStyle().setBackgroundColor(Wt::green);
  
  // prevent that firefox interprets drag as drag&drop action
  //l_up->setStyleClass("unselectable");
  //l_down->setStyleClass("polished");
  //l_up->clicked().connect(this, &FairDbWtTreeView::showClicked);
   
  // Add Table tree
   l_up->addWidget(TreeTableView());
   l_up->resize(300,WLength::Auto); 


  // DB Check ME !!! !!!!!!!!!!!!!!!!!!!!!!!
  // Add example table view
 /*
  WContainerWidget *envelop = new WContainerWidget();
  envelop->decorationStyle().setBackgroundColor(Wt::white);
  envelop->setPadding(0); 
  envelop->setOverflow(OverflowAuto); 
  envelop->addWidget(TableView());  
  WMenuItem *tab =fTabWidget_down->addTab(envelop, "tableview");
  WMenuItem *tab0 =fTabWidget_down->addTab(wMouseEvent(), "hanlding events");
 */
    
  ///////////////////////////

  
  // Add Tab Object Info
  fTabWidget_info = new WTabWidget(this);
  fTabWidget_info->setStyleClass("Wt-tabs");
  
  fWObject_Info = new WContainerWidget(this);
  fWObject_Info->setPadding(8); 
  fWObject_Info->setOverflow(OverflowAuto);
  fWObject_Info->decorationStyle().font().setSize(WFont::XSmall);
  //fWObject_Info->setPadding(WLength(100), Left | Right );

  fConnections = new WContainerWidget(this);
  fConnections->setOverflow(OverflowAuto);
  fConnections->decorationStyle().font().setSize(WFont::XSmall);
  //fConnections->setPadding(WLength(100), Left | Right );
  fConnections->setPadding(8); 

  // Example test
  WText* title =new WText("connections status"); 
  title->decorationStyle().font().setSize(WFont::XSmall);
  
  WMenuItem *tab1 = fTabWidget_info->addTab(fWObject_Info,"Object Info", WTabWidget::PreLoading);
  tab1->setCloseable(false);
  WMenuItem *tab2 = fTabWidget_info->addTab(fConnections,"Connections", WTabWidget::PreLoading);
  tab2->setCloseable(false);

  //fTabWidget_info->resize(300,WLength::Auto); 
  l_down->addWidget(fTabWidget_info);
  //l_down->addWidget(fWObject_Info);
  l_down->resize(300,500);
  

  l_Frame->resize(350, WLength::Auto);
  fMainLayout->addWidget(l_Frame, 1, 0); 

  // Properties 
  fMainLayout->setColumnResizable(0);
  fMainLayout->setRowStretch(1, 1);
  fMainLayout->setColumnStretch(1, 1);
          
  setLayout(fMainLayout);
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
     Wt::WTable* table = tNode->createDataTable(this,pName); 
     if (table){ 
       WMenuItem *tab = fTabWidget_top->addTab(table, pName, WTabWidget::PreLoading);
        int index = fTabWidget_top->indexOf(table);
        fTabWidget_top->setCurrentIndex(index);
        tab->setCloseable(false);
     }
  }

}

void FairDbWtTreeView::showPopup_Lup(const WMouseEvent& event) {
  
  //<DB> here is a strange error coming when using
  // securized sessions .. 

    if (event.button() == WMouseEvent::RightButton) {
     cout << "FairDbWtTreeView::showPopup_Lup 1  --> fPopup" << fPopup << endl;   
    // CHECK ME suboptimal
    if (fPopup) {delete fPopup; fPopup=NULL;} 
    cout << "FairDbWtTreeView::showPopup_Lup  2 --> fPopup" << fPopup << endl;   

    if (!fPopup) {

      cout << "FairDbWtTreeView::showPopup_Lup 3 --> fPopup" << fPopup << endl;   
      fPopup = new WPopupMenu();

      fPopup->addItem("View")->triggered().connect(std::bind([=] () {
            InspectParameter();
        }));
      cout << "FairDbWtTreeView::showPopup_Lup 4 --> fPopup" << fPopup << endl;   
      fPopup->addSeparator();
      fPopup->addItem("History");
      
     // fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction_Lup);
    }

      if (fPopup->isHidden()){
        cout << "FairDbWtTreeView::showPopup_Lup  5 --> fPopup" << fPopup << endl;   
         fPopup->popup(event);
        }else{
         fPopup->hide();
       }

     }//! if (event) 
}


void FairDbWtTreeView::popupAction_Lup() {
    if (fPopup->result()) {
 
      WString text = fPopup->result()->text();
      fPopup->hide();
        
      if (text == "Inspect") cout << "-I- Inspect" << endl;
      if (text == "History") cout << "-I- History" << endl;
         
      //fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      //fPopupActionBox->buttonClicked().connect(this, &FairDbWtTreeView::dialogDone);
      //fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}



// DB Popups dialogs ( simple version ... try with boost::bind later)

void FairDbWtTreeView::showPopup_Rtop(const WMouseEvent& event) 
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
    
      fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction_Rtop);
    }

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event) 
}

void FairDbWtTreeView::popupAction_Rtop() 
{
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();
         
      fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &FairDbWtTreeView::dialogDone_Rtop);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}

void FairDbWtTreeView::dialogDone_Rtop() 
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

void FairDbWtTreeView::showPopup_Rdown(const WMouseEvent& event) 
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
    
      fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction_Rdown);
    }

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event) 
}

void FairDbWtTreeView::popupAction_Rdown() 
{
    if (fPopup->result()) {
      WString text = fPopup->result()->text();
      fPopup->hide();
         
      fPopupActionBox = new WMessageBox("Tab selected","Action: " + text, NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &FairDbWtTreeView::dialogDone_Rdown);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
}

void FairDbWtTreeView::dialogDone_Rdown() 
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





// ROOT Test function-------------------------------------------------------------
// <DB>  to be integrated 2015


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
  
  //l->clicked().connect(this, &FairDbWtTreeView::showClicked);
  l->clicked().connect(boost::bind(&FairDbWtTreeView::showClicked, this, _1));

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


WTableView* FairDbWtTreeView::TableView(){

// Create some Model
WStandardItemModel *model = new WStandardItemModel(this);
//headers
  model->insertColumns(model->columnCount(), 2);
  model->setHeaderData(0, boost::any(WString("Item")));
  model->setHeaderData(1, boost::any(WString("Sales")));
//data
  model->insertRows(model->rowCount(), 6);
  int row = 0;
  model->setData(row, 0, boost::any(WString("Blueberry")));
  model->setData(row, 1, boost::any(120));
  row++;
  model->setData(row, 0, boost::any(WString("Cherry")));
  model->setData(row, 1, boost::any(30));
  row++;
  model->setData(row, 0, boost::any(WString("Apple")));
  model->setData(row, 1, boost::any(260));
  row++;
  model->setData(row, 0, boost::any(WString("Boston Cream")));
  model->setData(row, 1, boost::any(160));
  row++;
  model->setData(row, 0, boost::any(WString("Other")));
  model->setData(row, 1, boost::any(40));
  row++;
  model->setData(row, 0, boost::any(WString("Vanilla Cream")));
  model->setData(row, 1, boost::any(120));
  row++;
//set all items to be editable and selectable
  for (int irow = 0; irow < model->rowCount(); ++irow)
    for (int col = 0; col < model->columnCount(); ++col)
      model->item(irow, col)->setFlags(ItemIsSelectable | ItemIsEditable); 




Wt::WTableView *tableView = new Wt::WTableView(this);
tableView->setModel(new VirtualModel(100, 10, tableView));
tableView->setColumnResizeEnabled(false);
tableView->setColumnAlignment(0, Wt::AlignCenter);
tableView->setHeaderAlignment(0, Wt::AlignCenter);
tableView->setRowHeaderCount(1); // treat first column as 'fixed' row headers
tableView->setSortingEnabled(false);
tableView->setAlternatingRowColors(true);
tableView->setRowHeight(28);
tableView->setHeaderHeight(28);
tableView->setSelectionMode(Wt::ExtendedSelection);
//tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
tableView->resize(900, 800);
//tableView->clicked().connect(this, &FairDbWtTreeView::editFile);


return tableView;

}

/*
void FairDbWtTreeView::editFile(const WModelIndex& item)
{
 // Here the Table Row Info

 cout << "-I--------------- item  row# " << item.row() <<  " column = " << item.column() << endl; 
const WAbstractItemModel* aModel  = item.model();
boost::any aOut = aModel->data(item);
 cout << "-I--------------- data : " << Wt::asString(aOut) << endl; 

}
*/






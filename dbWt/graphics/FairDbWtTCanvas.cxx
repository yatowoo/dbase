#include <Wt/WContainerWidget>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WGridLayout>
#include <Wt/WSlider>
#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WTree>
#include <Wt/WTreeTableNode>
#include <Wt/WTreeView>
#include <Wt/WLineEdit>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>

#include "FairDbWtTCanvas.h"
#include "FairDbWtTCanvasWidget.h"
#include "FairDbWtRootCanvas.h"
#include "FairDbWtFileLoader.h"
#include "FairDbWtFileTreeTable.h"

#include <iostream>

using namespace Wt;
using namespace std;

FairDbWtTCanvas::FairDbWtTCanvas(WContainerWidget *root, TCanvas* aCanvas,
                                 bool showTitle)
  : WContainerWidget(root)
{
  WApplication* app = WApplication::instance();
  WGridLayout *grid = new WGridLayout();

  FairDbWtFileTreeTable *treeTable = new FairDbWtFileTreeTable(".");
  treeTable->resize(500, 300);
  treeTable->tree()->setSelectionMode(ExtendedSelection);
  treeTable->treeRoot()->setNodeVisible(false);
  treeTable->treeRoot()->setChildCountPolicy(WTreeNode::Enabled);

  grid->addWidget(treeTable, 0 , 0);


  if (app->environment().javaScript()) {
    cout << "-I- javascript enabled "<< endl;
    WVBoxLayout *topLayout = new WVBoxLayout();
    topLayout->addLayout(grid, 0);
    WHBoxLayout *gitLayout = new WHBoxLayout();
    //gitLayout->addWidget(gitView_, 0);
    //gitLayout->addWidget(sourceView_, 1);
    topLayout->addLayout(gitLayout, 1);
    setStyleClass("maindiv");
    setLayout(topLayout);
  } else {
    cout << "-I- no javascript enabled "<< endl;
    setStyleClass("maindiv");
    WContainerWidget *top = new WContainerWidget();
  }

}

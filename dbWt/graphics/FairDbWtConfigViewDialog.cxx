#include "FairDbWtConfigViewDialog.h"
#include "FairDbWtRootBrowser.h"
#include "FairDbWtCanvasWidget.h"
#include "FairDbWtJsRootProxy.h"

#include <WCssDecorationStyle>
#include <iostream>

using namespace std;

FairDbWtConfigViewDialog::FairDbWtConfigViewDialog(WTabWidget* aTab)
        : WDialog("Divide"),
        fTabWidget(aTab)
  {

    resize(300, WLength::Auto);

    fNameEdit = new WLineEdit();
    fNameEdit->setText(WString("Canvas"));

    fRowEdit = new WLineEdit();
    fRowEdit->setText(WString("1"));
    fRowEdit->setValidator
      (new WIntValidator(1, 10, this));
    // size
    fColEdit = new WLineEdit();
    fColEdit->setText(WString("1"));
    fColEdit->setValidator
      (new WIntValidator(1, 10, this));

    WGridLayout *layout = new WGridLayout();

    WLabel *l;
    int row = 0;

    layout->addWidget(l = new WLabel("Name:"), row, 0);
    layout->addWidget(fNameEdit, row, 1);
    l->setBuddy(fNameEdit);
    ++row;

    layout->addWidget(l = new WLabel("Rows: "), row, 0);
    layout->addWidget(fRowEdit, row, 1);
    l->setBuddy(fRowEdit);
    ++row;

    layout->addWidget(l = new WLabel("Colums:"), row, 0);
    layout->addWidget(fColEdit, row, 1);
    l->setBuddy(fColEdit);
    ++row;

    WPushButton *b;

    WContainerWidget *buttons = new WContainerWidget();

    buttons->addWidget(b = new WPushButton("Divide"));

    b->clicked().connect(this, &WDialog::accept);
    contents()->enterPressed().connect(this, &WDialog::accept);

    buttons->addWidget(b = new WPushButton("Cancel"));
    b->clicked().connect(this, &WDialog::reject);


    layout->addWidget(buttons, row, 0, 0, 3, AlignCenter);
    layout->setColumnStretch(1, 1);

    contents()->setLayout(layout);

    finished().connect(this, &FairDbWtConfigViewDialog::handleFinish);

    show();
  }




void FairDbWtConfigViewDialog::handleFinish(DialogCode result)
  {

    if (result == WDialog::Accepted) {

      string s_row = fRowEdit->text().narrow();
      string s_col = fColEdit->text().narrow();

      int n_row, n_col;
      stringstream(s_row) >> n_row;
      stringstream(s_col) >> n_col;

      cout << "-I- FairDbWtConfigViewDialog handleFinish row: "
      << n_row
      << " : " << n_col << endl;

     // Create a custom widget here
      FairDbWtCanvasWidget *cont_main =
          new FairDbWtCanvasWidget(nullptr, B_WW-200, B_HH-200, n_row, n_col);

      //WCssDecorationStyle main_style;
      //main_style.setBackgroundColor(WColor(241,245,249));
      //cont_main->setDecorationStyle(main_style);
      cont_main->resize(WLength::Auto, WLength::Auto);

      cout << "-I- FairDbWtConfigViewDialog handleFinish row: "
      << n_row
      << " : " << n_col << endl;

      WMenuItem *tab = fTabWidget->addTab(cont_main, fNameEdit->text().narrow());
      int index = fTabWidget->indexOf(cont_main);
      fTabWidget->setCurrentIndex(index);
      tab->setCloseable(false);

    }

   // Destroy the dialog
   delete this;
  }

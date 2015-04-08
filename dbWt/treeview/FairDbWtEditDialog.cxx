#include "FairDbWtFileEditDialog.h"



FairDbWtFileEditDialog::FairDbWtFileEditDialog(WAbstractItemModel *model, const WModelIndex& item)
        : WDialog("Edit..."),
          model_(model),
          item_(item)
  {
    int modelRow = item_.row();

    resize(300, WLength::Auto);

    /*
     * Create the form widgets, and load them with data from the model.
     */

    // name
    nameEdit_ = new WLineEdit(asString(model_->data(modelRow, 1)));

    // type
    typeEdit_ = new WComboBox();
    typeEdit_->addItem("Document");
    typeEdit_->addItem("Spreadsheet");
    typeEdit_->addItem("Presentation");
    typeEdit_->setCurrentIndex
      (typeEdit_->findText(asString(model_->data(modelRow, 2))));

    // size
    sizeEdit_ = new WLineEdit(asString(model_->data(modelRow, 3)));
    sizeEdit_->setValidator
      (new WIntValidator(0, std::numeric_limits<int>::max(), this));

    // created
    createdPicker_ = new WDatePicker();
    createdPicker_->lineEdit()->validator()->setMandatory(true);
    createdPicker_->setFormat(FileModel::dateEditFormat);
    createdPicker_->setDate(boost::any_cast<WDate>(model_->data(modelRow, 4)));

    // modified
    modifiedPicker_ = new WDatePicker();
    modifiedPicker_->lineEdit()->validator()->setMandatory(true);
    modifiedPicker_->setFormat(FileModel::dateEditFormat);
    modifiedPicker_->setDate(boost::any_cast<WDate>(model_->data(modelRow, 5)));

    /*
     * Use a grid layout for the labels and fields
     */
    WGridLayout *layout = new WGridLayout();

    WLabel *l;
    int row = 0;

    layout->addWidget(l = new WLabel("Name:"), row, 0);
    layout->addWidget(nameEdit_, row, 1);
    l->setBuddy(nameEdit_);
    ++row;

    layout->addWidget(l = new WLabel("Type:"), row, 0);
    layout->addWidget(typeEdit_, row, 1);
    l->setBuddy(typeEdit_);
    ++row;

    layout->addWidget(l = new WLabel("Size:"), row, 0);
    layout->addWidget(sizeEdit_, row, 1);
    l->setBuddy(sizeEdit_);
    ++row;

    layout->addWidget(l = new WLabel("Created:"), row, 0);
    layout->addWidget(createdPicker_->lineEdit(), row, 1);
    layout->addWidget(createdPicker_, row, 2);
    l->setBuddy(createdPicker_->lineEdit());
    ++row;

    layout->addWidget(l = new WLabel("Modified:"), row, 0);
    layout->addWidget(modifiedPicker_->lineEdit(), row, 1);
    layout->addWidget(modifiedPicker_, row, 2);
    l->setBuddy(modifiedPicker_->lineEdit());
    ++row;

    WPushButton *b;
    WContainerWidget *buttons = new WContainerWidget();
   
    buttons->addWidget(b = new WPushButton("Save"));
   
    b->clicked().connect(this, &WDialog::accept);
    contents()->enterPressed().connect(this, &WDialog::accept);
   
    buttons->addWidget(b = new WPushButton("Cancel"));
    b->clicked().connect(this, &WDialog::reject);

    /*
     * Focus the form widget that corresonds to the selected item.
     */
    switch (item.column()) {
    case 2:
      typeEdit_->setFocus(); break;
    case 3:
      sizeEdit_->setFocus(); break;
    case 4:
      createdPicker_->lineEdit()->setFocus(); break;
    case 5:
      modifiedPicker_->lineEdit()->setFocus(); break;
    default:
      nameEdit_->setFocus(); break;
    }

    layout->addWidget(buttons, row, 0, 0, 3, AlignCenter);
    layout->setColumnStretch(1, 1);

    contents()->setLayout(layout);

    finished().connect(this, &FairDbWtFileEditDialog::handleFinish);

    show();
  }




void FairDbWtEditFileDialog::handleFinish(DialogCode result)
  {
    if (result == WDialog::Accepted) {
 
         WAbstractItemModel *m = model_;
         int modelRow = item_.row();

         WAbstractProxyModel *proxyModel = dynamic_cast<WAbstractProxyModel *>(m);
         if (proxyModel) {
           m = proxyModel->sourceModel();
           modellRow = proxyModel->mapToSource(item_).row();
        }

         m->setData(modelRow, 1, boost::any(nameEdit_->text()));
         m->setData(modelRow, 2, boost::any(typeEdit_->currentText()));
         m->setData(modelRow, 3, boost::any(boost::lexical_cast<int>
                      (sizeEdit_->text().toUTF8())));
         m->setData(modelRow, 4, boost::any(createdPicker_->date()));
         m->setData(modelRow, 5, boost::any(modifiedPicker_->date()));
    }

    delete this;
  }






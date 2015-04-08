#include "FairDbWtFileModel.h"

WString FairDbWtFileModel::dateDisplayFormat(WString::fromUTF8("MMM dd, yyyy"));
WString FairDbWtFileModel::dateEditFormat(WString::fromUTF8("dd-MM-yyyy"));


std::string FairDbWtFileModel::mimeType() const {
    return FolderView::FileSelectionMimeType;
  }







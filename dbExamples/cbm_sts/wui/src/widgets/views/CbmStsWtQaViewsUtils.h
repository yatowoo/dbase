#ifndef CBMSTSWTQAVIEWSUTILS_H
#define CBMSTSWTQAVIEWSUTILS_H

#include <WPushButton>

using namespace Wt;
using namespace std;

namespace Views{
  
  WPushButton *createColorButton(const char *className,
                                 const WString& text);
  
}

#endif //CBMSTSWTQAVIEWSUTILS_H

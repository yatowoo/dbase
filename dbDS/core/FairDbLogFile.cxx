#include "FairDbLogFile.h"

#include <iostream>

using namespace std;

ClassImp(FairDbLogFile)

FairDbLogFile::FairDbLogFile()
{
  for(int i = 0; i< 10 ; i++){
     amap.insert(std::pair<int, double>(i*10, 200+i));  
  }
}

FairDbLogFile::~FairDbLogFile(){}

void FairDbLogFile::dump(){

  // map
  std::map<int, double>::iterator low, high, prev;
  
  // this should be a time from DAQ (s) 
  for(int j=0;j<100; j++){
    
    int pos = j;
    cout << " lookup for DAQ time t = " << pos << endl;
    
    low = amap.lower_bound(pos);
    high = amap.upper_bound(pos);
    
    if (low == amap.end()) {
      cout << "-I- no Temperature found at Daq time" << pos << endl;
    } else if (low == amap.begin()) {
      std::cout << "-I- Found at begining  for time =" << low->first <<  '\n';
    } else {
      prev = low;
      --prev;
      // if ((pos - prev->first) < (low->first - pos)){
      //  std::cout << " lower_bound =" << prev->first <<  " : "
      //            << "upper_bound " << high->first  <<  " Temperature: " <<  prev->second << '\n';
      //}else{
      std::cout  << "@ pos = " << pos << " lower_bound =" <<  prev->first  <<  " : "
                  << " upper_bound "  <<  high->first  <<  " Temperature: " <<  high->second << '\n';

      // with find 
      auto search = amap.find(pos);
      std::cout <<  "@ pos = " << pos << " boundary " << search->first << "Temperature : " << search->second << std::endl;
      
      
      // }
    }
    
  }
}

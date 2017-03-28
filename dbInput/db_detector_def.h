/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

////////////////////////////////////////////////////////////////////////////
// FairDbDetector
//
// FairDbDetector defines Detector_t which is an enumeration of the
// legal detector configurations
//
////////////////////////////////////////////////////////////////////////////

#ifndef FAIRDBDETECTOR_H
#define FAIRDBDETECTOR_H

#ifndef ROOT_Rtypes
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Rtypes.h"
#endif
#endif

#include "TString.h"

class FairDbDetector
{
  public:
  
    public:  
     typedef enum EDetector {
       kUnknown          = 0x00,
       kCal              = 0x01,
       kSts              = 0x02,
       kRich             = 0x04,
       kTof              = 0x08
     } Detector_t;
  

  
    static Int_t       FullMask() {
      return kCal|kSts|kRich|kTof;
    }


    static const Char_t*  AsString(Detector_t detector) {
      switch (detector) {
      case kUnknown:
        return "Unknown";
        break;
      case kCal:
        return "Calorimeter";
        break;
      case kSts:
        return "Sts";
        break;
      case kRich:
        return "Rich";
        break;
      case kTof:
        return "Tof";
        break;
      default:
        return "?Unknown?";
        break;
      }
    }

    static FairDbDetector::Detector_t   CharToEnum(Char_t c) {
      switch(c) {
      case 'C':
      case '1':
      case 0x01:
        return kCal;
      case 'c':
      case '2':
      case 0x02:
        return kSts;
      case 'D':
      case 'd':
      case '4':
      case 0x04:
        return kRich;
      case 'G':
      case 'g':
      case '8':
      case 0x08:
        return kTof;
      default:
        return kUnknown;
      }
    }

    static Char_t*  MaskToString(Int_t mask) {
      static Char_t newstring[255] = "";
      Char_t* ptr = newstring;
      *ptr = 0;
      Int_t fullmask = FairDbDetector::FullMask();

      for (Int_t i=0; i<32; i++) {
        FairDbDetector::Detector_t adet = (FairDbDetector::Detector_t)(1<<i);
        if (mask & adet & fullmask) {
          const Char_t* toadd = FairDbDetector::AsString(adet);
          if (ptr != newstring) { *ptr++ = ','; }
          strcpy(ptr,toadd);
          ptr += strlen(toadd);
        }
      }
      *ptr++ = 0;

      return newstring;
    }


    static Int_t  StringToMask(const Char_t* chars, Int_t maxChar=0) {
      Int_t mask  = 0;
      TString thestring(chars);
      if (maxChar>0 && maxChar<thestring.Length()) { thestring.Resize(maxChar); }

      thestring.ToLower();
      if (thestring.Contains("C")) { mask |= kCal; }
      if (thestring.Contains("c")) { mask |= kSts; }
      if (thestring.Contains("D")) { mask |= kRich; }
      if (thestring.Contains("G")) { mask |= kTof; }

      return mask;
    }


    static FairDbDetector::Detector_t  StringToEnum(const Char_t* chars, Int_t maxChar=0) {

      Int_t mask = FairDbDetector::StringToMask(chars,maxChar);

      switch (mask) {
      case kUnknown:
        return kUnknown;
        break;
      case kCal:
        return kCal;
        break;
      case kSts:
        return kSts;
        break;
      case kRich:
        return kRich;
        break;
      case kTof:
        return kTof;
        break;
      default:
        return kUnknown;
        break;
      }

    }

    static FairDbDetector::Detector_t  GetDetType(Int_t detMask) {

      FairDbDetector::Detector_t  fDetType=FairDbDetector::kUnknown;

      if      ( detMask & FairDbDetector::kCal )       { fDetType = FairDbDetector::kCal;  }
      else if ( detMask & FairDbDetector::kSts)        { fDetType = FairDbDetector::kSts;  }
      else if ( detMask & FairDbDetector::kRich)       { fDetType = FairDbDetector::kRich; }
      else if ( detMask & FairDbDetector::kTof)        { fDetType = FairDbDetector::kTof;  }

      return fDetType;
    }


};


#endif // FAIRDBDETECTOR_H


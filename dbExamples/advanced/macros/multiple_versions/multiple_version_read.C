void multiple_version_read(){
  //
  //
  // Purpose: - Read multiple version of non-composite data
  //            from database table
  //          


  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);

  struct Test_t {
    Int_t    dateQuery;
    Int_t    timeQuery;
    Int_t    dateResStart;
    Int_t    timeResStart;
    Int_t    dateResEnd;
    Int_t    timeResEnd;
    Double_t Gain1;
    Double_t Gain2;
  };
  
  Test_t tests[] = {
    {20140120, 120000, 20140101,110000, 20140131,  10203, 10., 20.},
    {20140310, 123020, 20140131, 10204, 20150101,  10203,  2.,  8.},
    {20150102, 110000, 20150101, 10204, 20150104,  10203, 30., 33.},
    {20150105, 113000, 20150105,110000, 20150105, 130000, 1.0, 1.3},
    {20160311, 110000, 20150120, 40506, 20380119,  31407,  8.,  9.},
  };

  Bool_t ok_test=kTRUE;

  for (int itest=0; itest<5; ++itest ) {
    Test_t& test = tests[itest];
    Int_t date_q     = test.dateQuery;
    Int_t time_q     = test.timeQuery;
    Int_t date_start = test.dateResStart;
    Int_t time_start = test.timeResStart;
    Int_t date_end   = test.dateResEnd;
    Int_t time_end   = test.timeResEnd;
    Double_t gain1   = test.Gain1;
    Double_t gain2   = test.Gain2;
    
    
    ValTimeStamp t_start(date_start/10000, date_start/100%100, date_start%100,
                         time_start/10000, time_start/100%100, time_start%100);

    ValTimeStamp t_end(date_end/10000, date_end/100%100, date_end%100,
                       time_end/10000, time_end/100%100, time_end%100);

    
    Int_t par1_version=0;
    ValTimeStamp t_query(date_q/10000, date_q/100%100, date_q%100,
                         time_q/10000, time_q/100%100, time_q%100);

    ValCondition cond(FairDbDetector::kSts, DataType::kData,t_query);

    FairDbReader<FairDbExPar1> r_par1;
    r_par1.Activate(cond, par1_version);
    
    Int_t fIdSystem=0;
    Int_t fPedestal=0.;
    Int_t fGain1=0.;
    Int_t fGain2=0.;

    Int_t nRows = r_par1.GetNumRows();
    FairDbExPar1* cgd=NULL;    
    for (int i = 0; i < nRows; ++i) {
      cgd = (FairDbExPar1*) r_par1.GetRow(i);
      if (!cgd) { continue; }
      fIdSystem =  cgd->GetIdSystem();
      fPedestal =  cgd->GetPedestal();
      fGain1    =  cgd->GetGain1();
      fGain2    =  cgd->GetGain2();
    }
    
    cout << "*********************************************************************************" <<endl;
    cout << "-I- for t_query: " << t_query.AsString() << endl;
    cout << "-I- Parameter values found : " << endl;
    cgd->Print();

    // Fetch corresponding IoVs
    ValTimeStamp qt_start = r_par1.GetValidityRec()
                                         ->GetValInterval().GetTimeStart();
    ValTimeStamp qt_end   = r_par1.GetValidityRec()
                                           ->GetValInterval().GetTimeEnd();

    cout << "-I- Validity range: " << r_par1.GetValidityRec()->GetValInterval().AsString() << endl;
    cout << "-I- Time Start: " << qt_start.AsString() << " Time end: " << qt_end.AsString() <<  endl;
    cout << "*********************************************************************************" <<endl;
      
  
    // (1) Check consistency of validity ranges in results set
    if (    qt_start.GetDate()    != test.dateResStart
	    || qt_start.GetTime() != test.timeResStart
	    || qt_end.GetDate()   != test.dateResEnd
	    || qt_end.GetTime()   != test.timeResEnd ) {
      
        
      cout << "-E- Query date: " << t_query.AsString() << endl
	   << " has validity " << r_par1.GetValidityRec()->GetValInterval().AsString() << endl
	   << " but should have "
           << test.dateResStart << " " << test.timeResStart << " - "
           << test.dateResEnd   << " " << test.timeResEnd << endl;
        
      ok_test=kFALSE;
    }  

    // (2) Check consistency of fetched values in result set
    if ( nRows >= 0 ) {
      Double_t  gain1  =  r_par1.GetRow(nRows-1)->GetGain1();
      Double_t  gain2  =  r_par1.GetRow(nRows-1)->GetGain2();
      if (     gain1  != test.Gain1 
	   ||  gain2  != test.Gain2  ) {
	
        cout  << "-E- Query date: " << t_query.AsString() << endl
	      << " gave first,last gains  " << gain1
	      << "," << gain2
	      << " but should have given " << test.Gain1
	      << "," << test.Gain2 << endl;
        ok_test = kFALSE;
      }  
    }

   
    // (4) Using Meta Data Information  
    // Get Some Meta Data Info from corresponding FairDbValRecord 
    const FairDbValRecord* vrec = r_par1.GetValidityRec();
    UInt_t dbNo  = vrec->GetDbNo();
    UInt_t seqNo = vrec->GetSeqNo();
    cout << "-I- FairDbExPar1 read from db# " << dbNo << " @ seqNo: " << seqNo << endl;  

    // Another way to create a valid reader for FairDbExPar1 using  arguments i.e
    //  tableName  (className)
    //  seqNo      (sequence number)
    //  dbNo       (database entry) 
    
    // Create a FairDbReader with  arguments: FairDbExPar1 name (tablename), sequence number, db_entry  
    FairDbReader<FairDbExPar1> prSeqno("ExPar1", seqNo,dbNo);
    cout << "-I- Fetched number of rows# " << prSeqno.GetNumRows() << endl; 
    
  }//!for(itest)
  

  // Test Fetched Data Consistency
  if (!ok_test) cout << "-E- Inconsistent Fetched Data !" << endl;
  else cout << "-I- Reading test sucessfull !... " << endl; 


  
  // (5) Test Moving to adjacent validity intervals
  Test_t& test = tests[0];
  Int_t date = test.dateQuery;
  Int_t time = test.timeQuery;
  ValTimeStamp t_query2(date/10000, date/100%100, date%100,
                      time/10000, time/100%100, time%100);

  ValCondition vc2(FairDbDetector::kSts,DataType::kData,t_query2);
  FairDbReader<FairDbExPar1> rr_par1;
  rr_par1.Activate(vc2, par1_version);

  cout << " ******* Test moving to adjacent validity intervals ******* " << endl;

  // Date interval Stepping forward
  for (int step = 0; step < 5; ++step) {
    rr_par1.NextQuery();
    Int_t nextDateGot = rr_par1.GetValidityRec()->GetValInterval()
      .GetTimeStart().GetDate();
    Int_t nextTimeGot = rr_par1.GetValidityRec()->GetValInterval()
      .GetTimeStart().GetTime();
    cout << " -I- step: " << step << " Adjacent date forward stepping: " << nextDateGot
	 << " : " << nextTimeGot << endl; 
  }


  // Date interval Stepping backward
  for (int step = 0; step < 5; ++step) {
    rr_par1.NextQuery(kFALSE);
    nextDateGot = rr_par1.GetValidityRec()->GetValInterval()
      .GetTimeStart().GetDate();
    nextTimeGot = rr_par1.GetValidityRec()->GetValInterval()
      .GetTimeStart().GetTime();
    cout << " -I- step: " << step << " Adjacent date backward stepping: " << nextDateGot
	 << " : " << nextTimeGot << endl; 

  }


  cout << "*************** done ! ********* "<< endl;  



  sql_io->close();
}

void composite_read_update(){

  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);
  
  // Here you can try to use FairDb Rollback to invalidate
  // the update the data.
  // Be aware that the time given as input is the
  // transmission time (TIMETRANS) in the validation table 
  //sql_io->SetRollback("2016-04-13 08:43:00","EXPAR2");
  
  struct Test_t {
    Int_t    dateQuery;
    Int_t    timeQuery;
  };
  
  Test_t tests[] = {
    {20140110,000000}
    {20160110,000000}
  };
  
  for (int itest=0; itest<2; ++itest ) {
    
    
    Test_t& test = tests[itest];
    Int_t date_q    = test.dateQuery;
    Int_t time_q    = test.timeQuery;
    
    
    // Create the IoV
    ValTimeStamp t_query(date_q/10000, date_q/100%100, date_q%100,
                         time_q/10000, time_q/100%100, time_q%100);
    
    ValCondition cond(FairDbDetector::kSts, DataType::kData,t_query);

    cout << endl; 
    cout << "-I- iteration: " << itest << " Query timestamp " << t_query.AsString() << endl;  
    cout << endl; 
    
    // Select a version 
    Int_t par2_version = 0;
    
   

    // Create FairDbExPar2 Parameter
    FairDbExPar2 aPar2(FairDbDetector::kSts, DataType::kData,"ExPar2","ExPar2","Example");

    // Get Corresponding Reader from parameter
    FairDbReader<FairDbExPar2>& r_par2 = aPar2.GetReader();
    
    // Activate the reader for the condition  
    r_par2.Activate(cond, par2_version);
    
    
    Int_t nRows = r_par2.GetNumRows();

    cout << endl; 
    cout << "-I- Sequential Reading FairDbExPar2 table, found nb of rows: " << nRows << endl;
    FairDbExPar2* cgd=NULL;
    
    for (int i = 0; i < nRows; ++i) {
      cgd = (FairDbExPar2*) r_par2.GetRow(i);
      if (!cgd) { continue; }
      Int_t fIdSystem =  cgd->GetIdSystem();
      cout << " @ row = " << i << " found subsystem id: " << fIdSystem <<
        " with data: " << cgd->GetData() << endl;
    }
    
    cout << endl;
    cout << "-I- Random access FairDbExPar2 table via indexes ( subsystem id ) " << endl; 
    UInt_t indeces[]={2, 3, 11, 13};
    for (UInt_t idata = 0; idata<4; ++idata) {
      UInt_t index = indeces[idata];
      const FairDbExPar2* p2i = r_par2.GetRowByIndex(index);
      if (p2i){
	cout << " @ subsystem_id: " << index << " stored data: " << p2i->GetData() <<endl;
      }
      
    }
    
    
  }//!(itest)
  
   // Close DB connection
  sql_io->close();
  
}

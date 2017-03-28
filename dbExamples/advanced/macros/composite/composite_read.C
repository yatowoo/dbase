void composite_read(){
  //
  //
  // Purpose:  - Reading back composite data
  //           - Further data/validity interval access 
  
  
  
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
    Int_t    comp_id;
    Int_t    s_id1;
    Int_t    s_id2;
    Int_t    s_id3;
    Double_t p_data1;
    Double_t p_data2;
    Double_t p_data3;
  };
  
  Test_t tests[] = {
    {20140110,000000,20160131, 000000, 20140131,  000000, 195, 20, 30, 110, 102., 103., 111.}
    {20140110,000000,20160131, 000000, 20140131,  000000, 1098, 50, 80, 130, 105., 108., 113.}
  };
  
  for (int itest=0; itest<1; ++itest ) {
    Test_t& test = tests[itest];
    Int_t date_q    = test.dateQuery;
    Int_t time_q    = test.timeQuery;
    Int_t date_start = test.dateResStart;
    Int_t time_start = test.timeResStart;
    Int_t date_end   = test.dateResEnd;
    Int_t time_end   = test.timeResEnd;
    Int_t compId   = test.comp_id;
    Int_t s_id[3] = {test.s_id1, test.s_id2, test.s_id3};
    Int_t p_data[3] = {test.p_data1, test.p_data2, test.p_data3};
    
    
    ValTimeStamp t_start(date_start/10000, date_start/100%100, date_start%100,
                         time_start/10000, time_start/100%100, time_start%100);
    
    ValTimeStamp t_end(date_end/10000, date_end/100%100, date_end%100,
                       time_end/10000, time_end/100%100, time_end%100);
    
    
    Int_t par2_version=0;
    ValTimeStamp t_query(date_q/10000, date_q/100%100, date_q%100,
                         time_q/10000, time_q/100%100, time_q%100);
    
    
    
    ValCondition cond(FairDbDetector::kSts, DataType::kData,t_query);
    
    // Select a version 
    par2_version = 0;
    
 
    // Create FairDbExPar2 Parameter
    FairDbExPar2 aPar2(FairDbDetector::kSts, DataType::kData,"ExPar2","ExPar2","Example");

    // Get the corresponding Reader template
    FairDbReader<FairDbExPar2>& r_par2= aPar2.GetReader() ;

    // Activate the reader for the condition 
    r_par2.Activate(cond, par2_version);

    // Get the rows in corresponding table
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
    
    // Test getting a FairDbVaRecord for a row.
    ValTimeStamp tstamp2(2016, 02, 03, 12, 0, 0);
    ValCondition vc2(FairDbDetector::kSts, DataType::kData,tstamp2);
    Int_t startDateReq = 20010103;
    Int_t endDateReq   = 20010104;
    
    FairDbReader<FairDbExPar2> rp(vc2);
    const ValInterval& rng = rp.GetValidityRec()->GetValInterval(); 
    Int_t startDateGot = rng.GetTimeStart().GetDate();
    Int_t endDateGot   = rng.GetTimeEnd().GetDate();
    
    cout << endl;
    cout << "-I- Row validity record ------ " << endl;
    cout << "-I- Row Start - End date   " << startDateGot << " - " << endDateGot <<endl;
    cout << endl;	  


   
    // Collect up all validity records for the EXPAR2 table
    int dbEntry = 0; // first entry in list of DB    
    FairDbValRecordMap vrs("EXPAR2", dbEntry);
    cout << " -I- number of rows in validity table EXPAR2 " << vrs.GetNumRows() << endl;
    for (unsigned int irec = 0; irec < vrs.GetNumRows(); ++irec) 
      {
        const FairDbValRecord* vrec = vrs.GetObjTableMap(irec);
        const ValInterval& rg = vrec->GetValInterval(); 
        Int_t startDate = rg.GetTimeStart().GetDate();
        Int_t endDate   = rg.GetTimeEnd().GetDate();
        
        cout << endl;
        cout << "-I- Validity record ------ " << irec <<  endl;
        cout << "-I- Start - End date   " << startDate << " - " << endDate << endl;
        cout << endl;
        
        // Get the corresponding SeqNo and NRows 
        FairDbReader<FairDbConfigData> cfs("EXPAR2", *vrec);
        
        // Get seqNo number and corresponding number of Rows 
        int nRows = cfs.GetNumRows();
        int seqNo = vrec->GetSeqNo();
        int dbNo  = vrec->GetDbNo();      
        cout << "In Db entry# "
             << dbNo << "found  seqNo "
             << seqNo << " corresponding n_rows  in data table: " << nRows << endl;
        
        // Get the corresponding rows in data table         
        // FairDbReader<FairDbExPar2> rp("EXPAR2", *vrec);
        FairDbReader<FairDbExPar2> rp("EXPAR2", seqNo, dbNo);
        int numRows= rp.GetNumRows(); 
        cout << " found corresponding n_rows  in data table: " << numRows << endl;
        
        // Loop over the selected rows corresponding
        // to this validity record
        FairDbExPar2* cgd=NULL;    
        for (int i = 0; i < numRows; ++i) {
          cgd = (FairDbExPar2*) rp.GetRow(i);
          if (!cgd) { continue; }
          Int_t fIdSystem =  cgd->GetIdSystem();
          cout << " @ row = " << i << " found subsystem id: " << fIdSystem <<
            " with data: " << cgd->GetData() << endl;
        }
      }

    
  }//!(itest)
  
   // Close DB connection
  sql_io->close();
  
}

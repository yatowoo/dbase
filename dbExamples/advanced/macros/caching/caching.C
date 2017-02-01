
void caching(Int_t n_trials=3, Bool_t isCached=kTRUE){
 
  // Repeat fetching of data from DB using a validity condition
  // mutliple time and for multiple versions.
  // Show the internal usage of FairDb Caching Mechanism.
  
  
  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);


  // Create FairDbExPar2 Parameter
  FairDbExPar2 aPar2(FairDbDetector::kSts, DataType::kData,"ExPar2","ExPar2","Example");

  
  Int_t date_q = 20140110;
  Int_t time_q = 000000;
  
  ValTimeStamp t_query(date_q/10000, date_q/100%100, date_q%100,
		       time_q/10000, time_q/100%100, time_q%100);
  
  
  
  ValCondition cond(FairDbDetector::kSts, DataType::kData,t_query);
  
  
  
  // Test the FairDb Caching Mechanism
  FairDbCache * cache = (FairDbCache*)
    (const_cast<FairDbTableInterface&> (FairDbReader<FairDbExPar2>::GetTableInterface()).GetCache());
  
  
  if (cache ) {
    cout << "-I- cache in used: " << cache << endl;
    UInt_t numAccepted = cache->GetNumAccepted();
    UInt_t setSize = 0;  
    cout << " -I- nObjects initially in cache accepted: " << numAccepted << endl;      
    
    TStopwatch timer;
    timer.Start();
    
    for(Int_t trial=0; trial<n_trials;++trial){
      for(Int_t version=0; version<4; ++version ){

	// if not caching purge the internal cache
	if (!isCached) cache->Purge();

	// One can also test effect of Stale ( reforce loading of object) 
	//if (!isCached) cache->SetStale();
        
        // Get the corresponding Reader template
        FairDbReader<FairDbExPar2>& rr_par2= aPar2.GetReader() ;
        // Activate the reader for the condition 
        rr_par2.Activate(cond, version);
        
	//FairDbReader<FairDbExPar2> rr_par2(cond, version);
	setSize = rr_par2.GetNumRows(); 
	cout << "-I- read for version: " << version << " nRows: " << setSize << endl;
      }
    } 
    
    cout << "*************************************************************************"<<endl;
    cout << "-I- FairDb Cache- size: " << cache->GetCurSize() 
         << "  nObjs reused:    " << cache->GetNumReused()
         << "  nObjs accepted:  " << cache->GetNumAccepted()
	 << endl;
    cout << "*************************************************************************"<<endl;
    
  }//!(cache)
  
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << "Real time " << rtime << " s, CPU time " << ctime
       << "s" << endl << endl;  
  
 
}

    

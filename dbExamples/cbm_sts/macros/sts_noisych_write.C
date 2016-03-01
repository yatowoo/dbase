Int_t  sts_noisych_write()
{

  // Generate a unique RunID
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();

  // Create the Runtime Database ( parameter manager class )
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Create in memory the relevant container
  CbmStsDbSInfoPar* stsPar = (CbmStsDbSInfoPar*)(db->getContainer("StsDbSInfoPar"));

  // Set the SQL based IO as second input
  FairParTSQLIo* input_db = new FairParTSQLIo();  
  input_db->SetVerbosity(1);

  // Set Global SeqNo ( Replication Global Index Base )
  //inp2->SetGlobalSeqNoIn();

  // Shutdown Mode ( True, False )
  input_db->SetShutdown(kTRUE);


  // Open first input
  input_db->open();
  db->setFirstInput(input_db);

  // Set the output=input
  db->setOutput(input_db); 

  cout << "-I- STS Noise Info Paremeters Priming:  Opening database + Meta Data done via TSQLIO .. " << endl;
 

  // Define Noisy Channel
  Int_t noisyCh_n[5]  = {1, 4, 8, 10, 12 };
  Int_t noisyCh_p[10] = {1, 4, 8, 10, 12 , 28, 30, 58, 102, 508};


  for(Int_t iDet=0;iDet<5;iDet++){
   // Create an StsDbSNoisyChPar parameter
   CbmStsDbSNoisyChPar* noise_par = new CbmStsDbSNoisyChPar();	
	// Set the DetId
	noise_par->SetDetId(iDet);
	// n_side 0 ( 5 channels)
	for(Int_t i=0;i<5;i++){ 
	  noise_par->SetNoisyChannel(0, noisyCh_n[i]+iDet);
	}
	// p_side 1 (10 channel)
	for(Int_t i=0;i<10;i++){ 
	  noise_par->SetNoisyChannel(1, noisyCh_p[i]+iDet);
	}
	// Add the Det Noisy channel Info to the Main Parameter Class 
	cout << " Add noisy Channel at detId " << noise_par->GetDetId() << endl;
 	stsPar->AddNoisyChInfo( noise_par);
  }

  // Tell RuntimeDb Container has changed
  stsPar->setChanged(kTRUE); 

  cout << "  Nr of Det with NoisyChannel Info  # " <<  stsPar->GetNumSNoisyChInfo() << endl; 
  
  // Write just added container
  db->addRun(runId,-1);
  db->writeContainer(stsPar, db->getCurrentRun());
  
  // FairRuntimeDB deletion
  if (db) delete db;
  return 0;
}

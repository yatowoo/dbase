
Int_t lmd_indexer(){

  TStopwatch timer;
  timer.Start();

 
  // Create source with unpackers ----------------------------------------------
  FairLmdSource* source  = new FairLmdSource();
  // adding the LMD file
  source->AddFile("data/s221a_37_0397.lmd");
  source->AddFile("data/s221a_37_0398.lmd");
  source->AddFile("data/s221a_38_0399.lmd");
  //source->AddFile("data/s221a_39_0400.lmd");
  
   

  // Add Unpacker to scan lmd files
  R3BDBLmdUnpack *lmd_unpacker = (new R3BDBLmdUnpack());
  source->AddUnpacker(lmd_unpacker);
 
  // Create online run ---------------------------------------------------------
  FairRunOnline* run = new FairRunOnline(source);
 
  // DB Setup 
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();
  Double_t s= (Double_t) runId;
  ValTimeStamp t1(s);
     
  cout << " RUNID generated ---> " << runId << " date:" << t1.AsString("s") <<  endl;
  run->SetRunId(runId); 
  run->SetOutputFile("output.root");

  // Runtime data base
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // Set the SQL based IO as first input
  FairParTSQLIo* input_db = new FairParTSQLIo();  
  input_db->SetShutdown(kTRUE);
  input_db->open();  
  rtdb->setFirstInput(input_db);
  rtdb->setOutput(input_db);
  
  
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("parameter.root");
  rtdb->setSecondInput(parOut);
  rtdb->saveOutput();
  rtdb->print(); 
  // ---------------------------------------------------------------------------
  
  
  // Initialize ----------------------------------------------------------------
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  // ---------------------------------------------------------------------------
  
  
  // Run and Analyse LMD Files
  run->Run(-1);
  
  // Header report
  Int_t n_header = lmd_unpacker->GetNHeaderInfo();
  
  // Times report
  Int_t n_starts = lmd_unpacker->GetNStartTimes();
  Int_t n_stops = lmd_unpacker->GetNStopTimes();
  
  
  cout << " LMD_INDEXER: n_start, n_stop , n_header " << n_starts << " : " << n_stops << " : " << n_header <<endl;
  
  for (Int_t i=0; i<n_header;i++){     
    // Get File Start Time 
    ValTimeStamp* t_start = lmd_unpacker->GetStartTimesAt(i);
    if (t_start) cout << " LMD_INDEXER: START T: " << i << " : " <<  t_start->AsString("s") << endl;
    
    // Get File Stop Time
    if (n_stops>0){ 
      ValTimeStamp* t_end = lmd_unpacker->GetStopTimesAt(i);
      if (t_end) cout << " LMD_INDEXER STOP T: " << i << " : " <<  t_end->AsString("s") << endl;
    } 
    
    // Get File Header Info
    LmdHeaderInfo* info= lmd_unpacker->GetHeaderInfoAt(i);
    if (info) cout << info->file << endl;
    
    R3BDBLmdFileInfo fLmdInfo;
    fLmdInfo.SetExpLabel("s406"); 
    fLmdInfo.SetExpPhase("PRE");
    fLmdInfo.SetRunType("DATA");
    fLmdInfo.SetFileId(t_start->GetSec());
    
    fLmdInfo.SetStartTime(*t_start);
    //fLmdInfo.SetStartTime(*t_end);
    fLmdInfo.SetEndTime(ValTimeStamp::GetEOT());
    // Servers Time Offset -> if synchronized Offset=0s    
    fLmdInfo.SetOffsetTime(0.);   
    
    fLmdInfo.SetType(info->type);   
    fLmdInfo.SetSubType(info->subtype);
    fLmdInfo.SetLength(info->length);   
    fLmdInfo.SetFragmentation(info->frag);   
    fLmdInfo.SetFileLabel(info->label);   
    fLmdInfo.SetFileName(info->file);   
    fLmdInfo.SetFileComment(info->comment);   
    
    cout << " Commit Header Info# " << i  << endl;
    if (!fLmdInfo.Commit()) cout << "-E- LMD_INDEXER Error Writing Run Info " << endl;	
  } 
  
  
  // Define a Start Time ( taken from LMD Buffer Header)  
  ValTimeStamp* t_start = lmd_unpacker->GetStartTimesAt(1);
  
  // Read it back in 
  // make a extended query to get ConfigFileText
  ValTimeStamp tsStart(1970, 1, 1, 0, 0, 0);
  ValTimeStamp   tsEnd(2038, 1,18,19,14, 7);
  
  FairDbExtSqlContent extContextConfig(FairDbExtSqlContent::kStarts,tsStart,tsEnd,
				       FairDbDetector::kLand,   // any
				       DataType::kData);    // any
  
  
  string seqSelectConfig = Form("SEQNO=%d",t_start->GetSec());
  string tbl = fLmdInfo.GetTableName();
  
  FairDbReader<R3BDBLmdFileInfo> 
    rsConfig(tbl.c_str(),extContextConfig,FairDb::kDefaultVersion,
	     seqSelectConfig.c_str());
  
  Int_t nFiles = rsConfig.GetNumRows();
  cout << "Found nFiles: = " << nFiles << endl;
  const R3BDBLmdFileInfo* rpInfo = rsConfig.GetRow(0); 
  
  if (rpInfo) cout << " file_id:run # " << rpInfo->GetFileId() << " has time: " << rpInfo->GetStartTime().AsString("s") << endl;
  
  
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime<< "s" << endl << endl;
  
  return 0;
}


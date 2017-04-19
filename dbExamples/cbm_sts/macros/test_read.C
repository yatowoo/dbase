Int_t  test_read()
{

  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);

  StsSensor p;

  p.fill();
  
   return 0;
}

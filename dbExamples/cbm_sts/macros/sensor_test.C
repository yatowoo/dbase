bool table_exists(string table_name)
{
  Int_t db_entry = 0;
  TString name(table_name);
  name.ToUpper();
  FairDbConnectionPool* connectionPool = FairDbTableInterfaceStore::Instance().fConnectionPool;
  return connectionPool->TableExists(name.Data(), db_entry);
}

void create_db_tables()
{
  StsDefect defect; defect.CreateDbTable(0);
  StsDefectContext defectContext; defectContext.CreateDbTable(0);
  StsDefectType defectType; defectType.CreateDbTable(0);
  StsInspectionImage image; image.CreateDbTable(0);
  StsOpticalInspection inspection; inspection.CreateDbTable(0);
  StsSensor sensor; sensor.CreateDbTable(0);
  StsSensorBatch batch; batch.CreateDbTable(0);
  StsSensorLocation location; location.CreateDbTable(0);
  StsSensorType type; type.CreateDbTable(0);
  StsSensorVendor vendor; vendor.CreateDbTable(0);
}

void prime_defect_context()
{
  string logTitle = "Defect Context Priming";
  
  struct defect_context_t {
    Int_t    fId;
    char*    fContextName;
    Double_t fWeight;
  };

  defect_context_t defectContexts[] = {
    {0, "Strips",          0.5},
    {1, "AC Pads",         0.1},
    {2, "DC Pads",         1.},
    {3, "Bias Resistors",  1.},
    {4, "Guard Ring",      1.},
    {5, "Non-active zone", 0.05},
    {6, "Other",           0.05 }
  };

  StsDefectContext defectContext;
  for (int i=0; i<7; i++ ) {
    defectContext.SetLogTitle(logTitle);
    defectContext.SetCompId(i);
    defectContext.SetId(i);
    defectContext.SetContextName(defectContexts[i].fContextName);
    defectContext.SetWeight(defectContexts[i].fWeight);
    
    defectContext.store();
  }
}

void prime_defect_type()
{
  string logTitle = "Defect Type Priming";
  
  struct defect_type_t {
    Int_t    fId;
    char*    fTypeName;
    Double_t fWeight;
  };

  defect_type_t defectTypes[] = {
    {0, "Scratch",       0.5},
    {1, "Dust",          0.1},
    {2, "Metal Short",   1.},
    {3, "Metal Break",   1.},
    {4, "Implant open",  1.},
    {5, "Other",         0.05 }
  };

  StsDefectType defectType;
  for (int i=0; i<6; i++ ) {
    defectType.SetLogTitle(logTitle);
    defectType.SetCompId(i);
    defectType.SetId(i);
    defectType.SetTypeName(defectTypes[i].fTypeName);
    defectType.SetWeight(defectTypes[i].fWeight);

    defectType.store();
  }
}

void prime_defect()
{
  string logTitle = "Defect Priming";

  ValTimeStamp start = ValTimeStamp::GetBOT();
  ValTimeStamp end = ValTimeStamp::GetEOT();
  ValInterval context(FairDbDetector::kSts,DataType::kData,start, end, logTitle);

  StsDefect defect;
  FairDbWriter<StsDefect> w_defect;
  w_defect.Activate(context, 0, defect.GetVersion(), defect.GetDbEntry(), logTitle);
  for (int i=0; i<10; i++ ) {
    defect.SetLogTitle(logTitle);
    defect.SetCompId(0);
    defect.SetId(i);

    w_defect << defect;
  }

  w_defect.Close();
}

void prime_sensor_vendor()
{
  string logTitle = "Vendor Priming";
  
  struct vendor_type_t {
    Int_t    fId;
    char*    fVendorName;
    char*    fLocation;
    char*    fContactInfo;
    char*    fComment;
  };

  vendor_type_t vendors[] = {
    {0, "Hamamatsu",     "325-6, Sunayama-cho, Naka-ku, Hamamatsu City, Shizuoka Pref., 430-8587, Japan", "Phone:(81)53-452-2141 Fax:(81)53-456-7889",                 "They make good sensors"},
    {1, "CIS",           "CiS electronic GmbH, Europark Fichtenhain A15, D – 47807 Krefeld",              "Tel.: +49 21 51 / 37 87 – 0 Fax.: +49 21 51 / 37 87 – 11",  "They make good sensors"},
  };

  StsSensorVendor vendor;
  for (int i=0; i<2; i++ ) {
    vendor.SetLogTitle(logTitle);
    vendor.SetCompId(i);
    vendor.SetId(i);
    vendor.SetVendorName(vendors[i].fVendorName);
    vendor.SetLocation(vendors[i].fLocation);
    vendor.SetContactInfo(vendors[i].fContactInfo);
    vendor.SetComment(vendors[i].fComment);

    vendor.store();
  }
}

void prime_sensor_type()
{
  string logTitle = "Defect Type Priming";
 
  struct sensor_type_t {
    Int_t    fId;
    char*    fTypeName;
    char*    fProcessing;
    Double_t fWidth;
    Double_t fHeight;
    Double_t fAliMarkWidth;
    Double_t fAliMarkHeight;
    Int_t    fStripsPerSide;
    Double_t fPitch;
    Double_t fThickness;
    Double_t fStereoAngleP;
    Double_t fStereoAngleN;
    char*    fComment;
  };

  sensor_type_t types[] = {
    {0 , "CBM06C12",     "DSDM", 62000.,  124000., 121400.,  61400.,    1024, 58.,   280., 7.5, 0.,   ""},
    {1 , "CBM06C6",      "DSDM", 62000.,  62000.,  61400.,   61400.,    1024, 58.,   280., 7.5, 0.,   ""},
    {2 , "CBM06C4",      "DSDM", 62000.,  42000.,  41400.,   61400.,    1024, 58.,   280., 7.5, 0.,   ""},
    {3 , "CBM06C2",      "DSDM", 62000.,  22000.,  21400.,   61400.,    1024, 58.,   280., 7.5, 0.,   ""},
    {4 , "CBM06H12",     "DSDM", 62000.,  124000., 121400.,  61400.,    1024, 58.,   320., 7.5, 0.,   ""},
    {5 , "CBM06H6",      "DSDM", 62000.,  62000.,  61400.,   61400.,    1024, 58.,   320., 7.5, 0.,   ""},
    {6 , "CBM06H4",      "DSDM", 62000.,  42000.,  41400.,   61400.,    1024, 58.,   320., 7.5, 0.,   ""},
    {7 , "CBM06H2",      "DSDM", 62000.,  22000.,  21400.,   61400.,    1024, 58.,   320., 7.5, 0.,   ""},
    {8 , "CBM05C1",      "DSSM", 11925.,  11925.,  11175.,   11175.,    192,  50.,   0.,   90., 0.,   "Baby Sensor, Look up thickness"},
    {9 , "CBM03C6",      "DSDM", 62000.,  62000.,  61361.2,  60756.08,  1024, 60.,   0.,   7.5, 7.5,  "Look up thickness"},
    {10, "CBM01",        "DSDM", 54899.2, 53854.5, 54899.2,  53854.5,   1024, 50.7,  0.,   7.5, 7.5,  "Look up thickness"},
    {11, "CBM01B1",      "DSSM", 22359.,  22425.,  22359.,   22425.,    256,  80.,   0.,   90., 0.,   "Look up thickness"},
    {12, "CBM01B2",      "DSSM", 14865.5, 14916.5, 14865.5,  14916.5,   256,  50.7,  0.,   90., 0.,   "Look up thickness"}
  };

  StsSensorType sensorType;
  for (int i=0; i<13; i++ ) {
    sensorType.SetLogTitle(logTitle);
    sensorType.SetCompId(i);
    sensorType.SetId(types[i].fId);
    sensorType.SetTypeName(types[i].fTypeName);
    sensorType.SetProcessing(types[i].fProcessing);
    sensorType.SetWidth(types[i].fWidth);
    sensorType.SetHeight(types[i].fHeight);
    sensorType.SetAliMarkWidth(types[i].fAliMarkWidth);
    sensorType.SetAliMarkHeight(types[i].fAliMarkHeight);
    sensorType.SetStripsPerSide(types[i].fStripsPerSide);
    sensorType.SetPitch(types[i].fPitch);
    sensorType.SetThickness(types[i].fThickness);
    sensorType.SetStereoAngleP(types[i].fStereoAngleP);
    sensorType.SetStereoAngleN(types[i].fStereoAngleN);
    sensorType.SetComment(types[i].fComment);

    sensorType.store();
  }
}

void prime_sensor_location()
{
  string logTitle = "Sensor Location Priming";
 
  struct sensor_location_t {
    Int_t    fId;
    char*    fLocation;
    char*    fOwner;
    char*    fComment;
  };

  sensor_location_t sensorLocations[] = {
    {0, "GSI",           "J. Heuser",    "On Assembly"},
    {1, "EKU Tuebingen", "H.R. Schmidt", "On Optical Inspection"},
    {2, "JINR",          "Y. Murin",     "On Assembly"},
  };

  StsSensorLocation sensorLocation;
  for (int i=0; i<3; i++ ) {
    sensorLocation.SetLogTitle(logTitle);
    sensorLocation.SetCompId(i);
    sensorLocation.SetId(i);
    sensorLocation.SetLocation(sensorLocations[i].fLocation);
    sensorLocation.SetOwner(sensorLocations[i].fOwner);
    sensorLocation.SetComment(sensorLocations[i].fComment);

    sensorLocation.store();
  }
}

void prime_sensor_batch()
{
  string logTitle = "Sensor Batch Priming";
 
  struct sensor_batch_t {
    Int_t           fId;
    char*           fNumber;
    ValTimeStamp    fDate;
    char*           fComment;
  };

  sensor_batch_t sensorBatches[] = {
    {0, "331140",           ValTimeStamp::GetBOT(),    "Very good batch"},
    {1, "351135",           ValTimeStamp::GetBOT(),    "Very good batch"},
    {2, "351139",           ValTimeStamp::GetBOT(),    "Very good batch"},
    {3, "351141",           ValTimeStamp::GetBOT(),    "Very good batch"},
    {4, "351142",           ValTimeStamp::GetBOT(),    "Very good batch"},
    {5, "351152",           ValTimeStamp::GetBOT(),    "Very good batch"},
  };

  StsSensorBatch sensorBatch;
  for (int i=0; i<6; i++ ) {
    sensorBatch.SetLogTitle(logTitle);    sensorBatch.SetCompId(i);
    sensorBatch.SetCompId(i);
    sensorBatch.SetId(i);
    sensorBatch.SetNumber(sensorBatches[i].fNumber);
    sensorBatch.SetDate(sensorBatches[i].fDate);
    sensorBatch.SetComment(sensorBatches[i].fComment);

    sensorBatch.store();
  }
}

void print_sensor(StsSensor *sensor)
{
  if (!sensor)
    return;

  sensor->Print();
  TObjArray *inspections = sensor->GetOpticalInspections();
  if (inspections)
  {
    cout << "Listing sensor inspections:" << endl;
    for (Int_t i=0; i<inspections->GetEntriesFast(); i++)
    {
      cout << i << ":" << endl;
      ((StsOpticalInspection *)inspections->At(i))->Print();
    }
  }
  cout << endl;
}

void prime_sensor()
{
  string logTitle = "Sensor Priming";

  StsSensor sensor;
  for (int i=0; i<2; i++ ) {
    sensor.SetLogTitle(logTitle);
    sensor.SetCompId(i);
    sensor.SetId(i);
    sensor.SetReticleName("Main");
    sensor.SetWaferNumber(1);
    sensor.SetDate(ValTimeStamp::GetBOT());
    sensor.SetQuality(1.);
    sensor.SetQualityGrade("A++");
    sensor.SetComment("Good Sensor");

    sensor.store();
  }
}

void sensor_read(Int_t sensorId = 0, UInt_t runId = 0)
{
  StsSensor* sensor = StsSensor::GetSensorById(sensorId, runId);
  print_sensor(sensor);
}

void prime_optical_inspection()
{
  string logTitle = "Optical Inspection Priming";

  ValTimeStamp start = ValTimeStamp::GetBOT();
  ValTimeStamp end = ValTimeStamp::GetEOT();
  ValInterval context(FairDbDetector::kSts,DataType::kData,start, end, logTitle);

  StsOpticalInspection inspection;
  FairDbWriter<StsOpticalInspection> w_inspection;
  w_inspection.Activate(context, 0, inspection.GetVersion(), inspection.GetDbEntry(), logTitle);
  for (int i=0; i<2; i++ ) {
    inspection.SetLogTitle(logTitle);
    inspection.SetCompId(0);
    inspection.SetId(i);
    inspection.SetSensorId(0);
    inspection.SetSizeX(30);
    inspection.SetSizeY(20);
    inspection.SetDate(ValTimeStamp::GetBOT());
    inspection.SetSensorSide("n");
    inspection.SetMaxWarp(50.);
    inspection.SetWarp(new TGraph2D());
    inspection.SetThickness(280.);
    inspection.SetQuality(1.);
    inspection.SetQualityGrade("A++");
    inspection.SetPassed(kTRUE);
    inspection.SetComment("No comments");

    w_inspection << inspection;
  }

  w_inspection.Close();
}

void prime_inspection_image()
{
  string logTitle = "Inspection Image Priming";

  ValTimeStamp start = ValTimeStamp::GetBOT();
  ValTimeStamp end = ValTimeStamp::GetEOT();
  ValInterval context(FairDbDetector::kSts,DataType::kData,start, end, logTitle);

  StsInspectionImage image;
  FairDbWriter<StsInspectionImage> w_image;
  w_image.Activate(context, 0, image.GetVersion(), image.GetDbEntry(), logTitle);
  for (int i=0; i<5; i++ ) {
    image.SetLogTitle(logTitle);
    image.SetCompId(0);
    image.SetId(i);
    image.SetX(i);
    image.SetY(i);
    image.SetImageURI("smb://pipc40/Inspection Results/CBM06C6_331827-3/n_side/00_00.png");
    image.SetThumbnailURI("smb://pipc40/Inspection Results/CBM06C6_331827-3/n_side/thumbnails/00_00.png");
    image.SetOverlayURI("smb://pipc40/Inspection Results/CBM06C6_331827-3/n_side/results/00_00.png");

    w_image << image;
  }

  w_image.Close();
}

void inspection_read(int id = 0, UInt_t runId = 0)
{
  StsOpticalInspection* inspection = StsOpticalInspection::GetInspectionById(id, runId);
  if (!inspection)
    return;
  inspection->Print();

  StsSensor* sensor = inspection->GetSensor();
  if (!sensor)
    return;

  cout << "Sensor of this inspection: " << endl;
  sensor->Print();
}

void test_relations()
{
  StsSensorBatch *batch = StsSensorBatch::GetBatchById(0);
  batch->Print();
  StsSensor *sensor = batch->GetSensors()->At(0);
  sensor->Print();
  StsSensorLocation *location = sensor->GetLocation();
  location->Print();
  StsSensorType *type = sensor->GetType();
  type->Print();
  StsSensorVendor *vendor = sensor->GetVendor();
  vendor->Print();
  StsOpticalInspection *inspection = sensor->GetOpticalInspections()->At(0);
  inspection->Print();
  StsInspectionImage *image = inspection->GetInspectionImages()->At(0);
  image->Print();
  StsDefect *defect = image->GetDefects()->At(0);
  defect->Print();
  StsDefectType *defectType = defect->GetDefectType();
  defectType->Print();
  StsDefectContext *defectContext = defect->GetDefectContext();
  defectContext->Print();
}

void test_backward_relations()
{
  StsDefect *defect = StsDefect::GetDefectById(0);
  defect->Print();
  StsInspectionImage *image = defect->GetDefectImage();
  image->Print();
  StsOpticalInspection *inspection = defect->GetInspection();
  inspection->Print();
  StsSensor *sensor = inspection->GetSensor();
  sensor->Print();
  StsSensorBatch *batch = sensor->GetBatch();
  batch->Print();
}

void sensor_test()
{
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);

  create_db_tables();
  prime_defect_context();
  prime_defect_type();
  prime_defect();
  prime_sensor_vendor();
  prime_sensor_type();
  prime_sensor_location();
  prime_sensor_batch();
  prime_sensor();
  prime_optical_inspection();
  prime_inspection_image();

  test_relations();
  test_backward_relations();
}

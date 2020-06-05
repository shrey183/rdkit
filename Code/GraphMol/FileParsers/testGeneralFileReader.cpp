//
//   Copyright (C) 2002-2019 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#include "GeneralFileReader.h"
namespace io = boost::iostreams;

using namespace RDKit;

int testGeneralReader(){
	
	std::cout << "TESTING GENERAL READER" << std::endl;
	// Test with SDF files
	std::string rdbase = getenv("RDBASE");
	
	// Test SDF with default options
	std::string fname =
	      rdbase + "/Code/GraphMol/FileParsers/test_data/withHs.sdf";	
	SupplierOption opt_sdf; 	
  opt_sdf.takeOwnership = true;
  opt_sdf.sanitize = true;
  opt_sdf.removeHs = true;
  opt_sdf.strictParsing = true;
	GeneralFileReader gfr(fname, opt_sdf);
	MolSupplier* sdsup = gfr.read();

	unsigned int i = 0;
    while (!sdsup->atEnd()) {
      ROMol *nmol = sdsup->next();
    	std::cout << "Obtained next ROMol\n"; 
		  TEST_ASSERT(nmol || sdsup->atEnd());
      if (nmol) {
        delete nmol;
        i++;
      }
    }
    TEST_ASSERT(i == 16);
	/*
	// Test SMI with some options    
	fname = rdbase + "/Code/GraphMol/FileParsers/test_data/fewSmi.2.csv";
	SupplierOption opt_smi;
	opt_smi.delimiter = ",";
	opt_smi.smilesColumn = 1;
	opt_smi.nameColumn = 0;
	opt_smi.titleLine = true; 
	opt_smi.sanitize = true;
	GeneralFileReader gfr2(fname, opt_smi);
	MolSupplier* smiSup = gfr2.read();
	*/
	std::cout << "END OF TEST OF GENERAL READER" << std::endl;
	return 1;

}



int main() {
  RDLog::InitLogs();

  BOOST_LOG(rdErrorLog) << "\n-----------------------------------------\n";
  testGeneralReader();
  BOOST_LOG(rdErrorLog) << "Finished: testGeneralReader()\n";
  BOOST_LOG(rdErrorLog) << "-----------------------------------------\n\n";
	  
  return 0;
}



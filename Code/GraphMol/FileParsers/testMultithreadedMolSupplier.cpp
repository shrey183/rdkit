//
//  Copyright (C) 2020 Shrey Aryan
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#include <GraphMol/MonomerInfo.h>
#include <GraphMol/RDKitBase.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <RDGeneral/ConcurrentQueue.h>
#include <RDGeneral/FileParseException.h>
#include <RDGeneral/RDLog.h>
#include <RDGeneral/test.h>

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "MultithreadedMolSupplier.h"
#include "MultithreadedSmilesMolSupplier.h"

namespace io = boost::iostreams;
using namespace RDKit;

void testSmi() {
  std::string mname;
  std::string rdbase = getenv("RDBASE");
  std::string fname =
      rdbase + "/Code/GraphMol/FileParsers/test_data/fewSmi.csv";
  unsigned int i;
  MultithreadedSmilesMolSupplier sup(fname, ",", 1, 0, true);
  sup.startThreads();
  std::cout << "threads started" << std::endl;
  while (!sup.atEnd()) {
    std::cout << "we are calling next" << std::endl;
    ROMol* mol = sup.next();
    if (i == 3) {
      mol->getProp(common_properties::_Name, mname);
      CHECK_INVARIANT(mname == "4", "");
      mol->getProp("Column_2", mname);
      CHECK_INVARIANT(mname == "82.78", "");
    }
    delete mol;
    i++;
  }

  std::cout << "Number of lines = " << i << std::endl;

  TEST_ASSERT(i == 10);
}

int main() {
  RDLog::InitLogs();

  BOOST_LOG(rdErrorLog) << "\n-----------------------------------------\n";
  testSmi();
  BOOST_LOG(rdErrorLog) << "Finished: testSmi()\n";
  BOOST_LOG(rdErrorLog) << "-----------------------------------------\n\n";
  return 0;
}

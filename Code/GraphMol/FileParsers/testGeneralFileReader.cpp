#include <RDGeneral/test.h>
#include <GraphMol/RDKitBase.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <RDGeneral/FileParseException.h>
#include <RDGeneral/RDLog.h>
#include "GeneralFileReader.h"

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace io = boost::iostreams;
using namespace RDKit;

int testGeneralReader(){
	
	// Test with SDF files
	std::string rdbase = getenv("RDBASE");


// Open uncompressed SDF file format
{
	std::cout << "Testing Supplier for uncompressed SDF files\n";
	std::string fname = rdbase + "/Code/GraphMol/FileParsers/test_data/NCI_aids_few.sdf";	
	struct SupplierOption opt_sdf; 	
	GeneralFileReader gfr(fname, opt_sdf);
	MolSupplier* sdsup = gfr.getSupplier();

  unsigned int i = 0;
  while (!sdsup->atEnd()) {
    ROMol *nmol = sdsup->next();
    TEST_ASSERT(nmol || sdsup->atEnd());
    if (nmol) {
      TEST_ASSERT(nmol->hasProp(common_properties::_Name));
      TEST_ASSERT(nmol->hasProp("NCI_AIDS_Antiviral_Screen_Conclusion"));
      delete nmol;
      i++;
    }
  }
  TEST_ASSERT(i == 16);
}
			
// Open compressed SDF file format
{
	std::cout << "Testing Supplier for compressed SDF files\n";	
  fname = rdbase + "/Code/GraphMol/FileParsers/test_data/NCI_aids_few.sdf.gz";
	struct SupplierOption opt_sdf2;
  opt_sdf2.takeOwnership = false;
	GeneralFileReader gfr2(fname, opt_sdf2);
	MolSupplier* sdsup2 = gfr2.getSupplier();
  i = 0;
  while (!sdsup2->atEnd()) {
    ROMol *nmol = sdsup2->next();
    if (nmol) {
      TEST_ASSERT(nmol->hasProp(common_properties::_Name));
      TEST_ASSERT(nmol->hasProp("NCI_AIDS_Antiviral_Screen_Conclusion"));
      delete nmol;
      i++;
    }
  }
  TEST_ASSERT(i == 16);
}

// Open uncompressed MAE file format
{
    fname = rdbase + "/Code/GraphMol/FileParsers/test_data/props_test.mae";
	  struct SupplierOption opt_sdf3;
		GeneralFileReader gfr3(fname, opt_sdf3);
		MolSupplier* maesup = gfr3.getSupplier();
 
    std::unique_ptr<ROMol> nmol(maesup->next());
    TEST_ASSERT(nmol);

    // Test mol properties
    TEST_ASSERT(nmol->hasProp(common_properties::_Name));
    TEST_ASSERT(nmol->hasProp("b_sd_chiral_flag"));
    TEST_ASSERT(nmol->getProp<bool>("b_sd_chiral_flag") == false);
    TEST_ASSERT(nmol->hasProp("i_sd_NSC"));
    TEST_ASSERT(nmol->getProp<int>("i_sd_NSC") == 48);
    TEST_ASSERT(nmol->hasProp("s_m_entry_name"));
    TEST_ASSERT(nmol->getProp<std::string>("s_m_entry_name") ==
                "NCI_aids_few.1");
    TEST_ASSERT(nmol->hasProp("r_f3d_dummy"));
    TEST_ASSERT(abs(nmol->getProp<double>("r_f3d_dummy") - 42.123) < 0.0001);

    // Test atom properties
    TEST_ASSERT(nmol->getNumAtoms() == 19);
    for (int i = 0; i < 19; ++i) {
      const auto *atom = nmol->getAtomWithIdx(i);

      // The integer property is present for all atoms
      TEST_ASSERT(atom->hasProp("i_m_minimize_atom_index"));
      TEST_ASSERT(atom->getProp<int>("i_m_minimize_atom_index") == 1 + i);

      // The bool property is only defined for i < 10
      if (i < 10) {
        TEST_ASSERT(atom->hasProp("b_m_dummy"));
        TEST_ASSERT(atom->getProp<bool>("b_m_dummy") ==
                    static_cast<bool>(i % 2));
      } else {
        TEST_ASSERT(!atom->hasProp("b_m_dummy"));
      }

      // The real property is only defined for i >= 10
      if (i >= 10) {
        TEST_ASSERT(atom->hasProp("r_f3d_dummy"));
        TEST_ASSERT(abs(atom->getProp<double>("r_f3d_dummy") - (19.1 - i)) <
                    0.0001);
      } else {
        TEST_ASSERT(!atom->hasProp("r_f3d_dummy"));
      }

      // All atoms have the string prop
      TEST_ASSERT(atom->hasProp("s_m_dummy"));
      TEST_ASSERT(atom->getProp<std::string>("s_m_dummy") ==
                  std::to_string(19 - i));
    }

    TEST_ASSERT(maesup->atEnd());
} 	

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



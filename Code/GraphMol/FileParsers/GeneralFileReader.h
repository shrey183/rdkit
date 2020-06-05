#ifndef GENERAL_FILE_READER_H
#define GENERAL_FILE_READER_H
#include <RDGeneral/test.h>
#include <GraphMol/RDKitBase.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>

#include "MolSupplier.h"
#include "MolWriters.h"
#include "FileParsers.h"
#include "FileParserUtils.h"
#include <RDGeneral/FileParseException.h>
#include <RDGeneral/BadFileException.h>
#include <RDGeneral/RDLog.h>
#include <GraphMol/MonomerInfo.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/Depictor/RDDepictor.h>
#include "SupplierOption.h"


using namespace RDKit;

class GeneralFileReader{
	
public:
	std::string fname; // file name
	SupplierOption opt; 
public:
	GeneralFileReader(std::string fileName); // use default options
	GeneralFileReader(std::string fileName, SupplierOption options);
	bool valid(std::string format);
	MolSupplier* read();
	
};




#endif

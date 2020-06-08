#ifndef GENERAL_FILE_READER_H
#define GENERAL_FILE_READER_H
#include <string>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "SupplierOption.h"

#include "MolSupplier.h"
#include <RDGeneral/BadFileException.h>
#include <RDStreams/streams.h>


using namespace RDKit;
namespace fs = boost::filesystem;


class GeneralFileReader{
	
public:
	std::string fname; 	// file name
	SupplierOption opt; // options for the Mol Supplier 
public:
	GeneralFileReader(std::string fileName);
	GeneralFileReader(std::string fileName, SupplierOption options);
		
	/*
		Function to check the validity of the file and compression format
	*/
	bool valid(std::string file_format, std::string compression_format);

	/*
		Get MolSupplier Object based on the file and compression format
	*/
	MolSupplier* getSupplier();
	
};




#endif

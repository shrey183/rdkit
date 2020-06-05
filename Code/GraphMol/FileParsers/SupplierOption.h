#ifndef SUPPLIER_OPTION_H
#define SUPPLIER_OPTION_H

#include <string>
#include <list>
#include <memory>
#include <iostream>


using namespace RDKit;

class SupplierOption{

public:
	// List of all possible options for Forward SDMol Supplier
	bool takeOwnership;
	bool sanitize;
	bool removeHs;
	bool strictParsing;
	
	// List of all possible options for SMILES Forward Supplier	
	std::string delimiter;
	int smilesColumn;
	int nameColumn;
	bool titleLine;

	
	SupplierOption(bool takeOwnership_ = true, bool sanitize_ = true,
			bool removeHs_ = true, bool strictParsing_ = false, 
			std::string delimiter_ = " ", int smilesColumn_ = 0,
			int nameColumn_ = 1, bool titleLine_ = true){	
	takeOwnership = takeOwnership_;
	sanitize = sanitize_;
	removeHs = removeHs_;
	strictParsing = strictParsing_;
	delimiter = delimiter_;
	smilesColumn = smilesColumn_;
	nameColumn = nameColumn_;
	titleLine = titleLine_;
	}

};






















#endif


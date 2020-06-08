#ifndef SUPPLIER_OPTION_H
#define SUPPLIER_OPTION_H

#include <string>
#include <list>
#include <memory>
#include <iostream>



class SupplierOption{

public:
	bool takeOwnership;
	bool sanitize;
	bool removeHs;
	bool strictParsing;
	
	std::string delimiter;
	int smilesColumn;
	int nameColumn;
	bool titleLine;

	std::string nameRecord;
	int confId2D;
	int confId3D;	

	bool with_stream; // if opening the file with a stream object

	/*
		Constructor covers options for the following suppliers:
			- ForwardSDMolSupplier
			- SDMolSupplier
			- SmilesMolSupplier
			- TDTMolSupplier
			- MaeMolSupplier
	*/
	SupplierOption(bool with_stream = false, bool takeOwnership = true, 
								bool sanitize = true, bool removeHs = true, 
								bool strictParsing = true, std::string delimiter = "\t",
								int smilesColumn = 0, int nameColumn = 1, 
								bool titleLine = true, std::string nameRecord = "",
								int confId2D = -1, int confId3D = 0){
	this->with_stream = with_stream;	
	this->takeOwnership = takeOwnership;
	this->sanitize = sanitize;
	this->removeHs = removeHs;
	this->strictParsing = strictParsing;
	this->delimiter = delimiter;
	this->smilesColumn = smilesColumn;
	this->nameColumn = nameColumn;
	this->titleLine = titleLine;
	this->nameRecord = nameRecord;
	this->confId2D = confId2D;
	this->confId3D = confId3D;
	}
	
};






















#endif


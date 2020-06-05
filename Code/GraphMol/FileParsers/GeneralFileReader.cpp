#include "GeneralFileReader.h"


GeneralFileReader::GeneralFileReader(std::string fileName){
	fname = fileName;
}

GeneralFileReader::GeneralFileReader(std::string fileName, SupplierOption options){
	fname = fileName;
	opt = options;
}

bool GeneralFileReader::valid(std::string format){
	return format == "sdf" || format == "smi";

}

MolSupplier* GeneralFileReader::read(){

	// Given the filename and options, we want to read the file using
	// appropriate supplier  
	
	// First get the format of the file

	std::cout << "Generating Supplier..." << std::endl;

	std::size_t found = fname.find(".");
	
	std::string format;	
	if (found != std::string::npos){
		int dot = fname.find(".");
		format = fname.substr(dot + 1); 
		if(!valid(format)) throw std::invalid_argument("Recieved Invalid File Format");
	}
	else{
		throw std::invalid_argument("Recieved Invalid File Format");	
	}
	
	std::cout << "Valid File Format" << std::endl;

	// At this stage we have a valid file format
	std::ifstream strm(fname.c_str());	
	
	// CASE 1: SDF Format
	if(format == "sdf") {
		std::cout << "File Format: " << format << std::endl;
		ForwardSDMolSupplier* sdsup = new ForwardSDMolSupplier(&strm, opt.takeOwnership, opt.sanitize, opt.removeHs, opt.strictParsing);	
		return sdsup;
	
	}

	// CASE 2: Smiles Format
	else if(format == "smi"){
		std::cout << "File Format: "<< format << std::endl;
		SmilesMolSupplier* smsup = new SmilesMolSupplier(fname, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
		return smsup;
	}


}

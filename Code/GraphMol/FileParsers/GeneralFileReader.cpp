#include "GeneralFileReader.h"


GeneralFileReader::GeneralFileReader(std::string fileName){
	fname = fileName;
}

GeneralFileReader::GeneralFileReader(std::string fileName, SupplierOption options){
	fname = fileName;
	opt = options;
}

bool GeneralFileReader::valid(std::string file_format, std::string compression_format=""){
	std::vector<std::string> file_formats{"sdf", "mae", "smi", "tdt"};
	std::vector<std::string> compression_formats{"gz"};

	bool flag_fileFormat = (std::find(file_formats.begin(), file_formats.end(), file_format)!= file_formats.end()); 	
	bool flag_compressionFormat = true;
  if(compression_format.compare("") != 0)
    flag_compressionFormat = std::find(compression_formats.begin(),compression_formats.end(), compression_format)!= compression_formats.end();
	
	return flag_fileFormat && flag_compressionFormat;

}

MolSupplier* GeneralFileReader::getSupplier(){
	
	// find the file and compression format
	fs::path p(fname);
	std::string fileName = p.filename().string();
	int dots = std::count(fileName.begin(), fileName.end(), '.');	
  std::string file_format = "";
	std::string compression_format = "";
	if(dots == 0 || dots > 2) throw BadFileException("Recieved Invalid File Format, no extension or compression"); 
		
	else if(dots == 1){
	// there is not compression format
	int pos = fileName.find(".");
	file_format = fileName.substr(pos + 1);
  if(!valid(file_format))  throw BadFileException("Recieved Invalid File Format");
	}

  else if(dots == 2){
  // there is a compression format
  int p1 = fileName.find(".");
  int p2 = fileName.find(".", p1 + 1);
  file_format = fileName.substr(p1 + 1, p2 - p1 - 1);
  compression_format = fileName.substr(p2 + 1); 
	if(!valid(file_format, compression_format)) throw BadFileException("Recieved Invalid File or Compression Format");
  }

  // At this stage we have a valid file format	
	//std::cout << "File Format: " << file_format << std::endl;
	//std::cout << "Compression Format: " << compression_format << std::endl;
	
  // Open the file object with ifstream
  if(compression_format.compare("") == 0){
   	std::ifstream* strm = new std::ifstream(fname.c_str());	
	  
    // CASE 1: Use Forward SDMol Supplier to open sdf file formats
	  if(file_format == "sdf") {
		  ForwardSDMolSupplier* sdsup = new ForwardSDMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs, opt.strictParsing);	
		  return sdsup;
	  
	  }

	  // CASE 2: Smiles Format
	  else if(file_format == "smi"){
		  SmilesMolSupplier* smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
		  return smsup;
	   }

    // CASE 3: MAE Mol Supplier 
    else if(file_format == "mae"){
      MaeMolSupplier* maesup = new MaeMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs);	
		  return maesup;
    }

    // CASE 4: TDT Mol Supplier
    else if(file_format == "tdt"){
      TDTMolSupplier* tdtsup = new TDTMolSupplier(strm, opt.takeOwnership, opt.nameRecord, opt.confId2D, opt.confId3D, opt.sanitize);	
		  return tdtsup;
    }
  }

  else{
			//std::ifstream* strm = new std::ifstream(fname.c_str());
			std::ifstream* strm = new std::ifstream(fname.c_str());	

			//auto* strm = new gzstream(fname);
			
			if(file_format == "sdf"){
      ForwardSDMolSupplier* sdsup = new ForwardSDMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs, opt.strictParsing);	
		  return sdsup;
	  }

	  // CASE 2: Smiles Format
	  else if(file_format == "smi"){
		  SmilesMolSupplier* smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
		  return smsup;
	   }

    // CASE 3: MAE Mol Supplier 
    else if(file_format == "mae"){
      MaeMolSupplier* maesup = new MaeMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs);	
		  return maesup;
    }

    // CASE 4: TDT Mol Supplier
    else if(file_format == "tdt"){
      TDTMolSupplier* tdtsup = new TDTMolSupplier(strm, opt.takeOwnership, opt.nameRecord, opt.confId2D, opt.confId3D, opt.sanitize);	
		  return tdtsup;
    }
	}
}

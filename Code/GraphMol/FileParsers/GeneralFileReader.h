#ifndef GENERAL_FILE_READER_H
#define GENERAL_FILE_READER_H
#include <string>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "MolSupplier.h"
#include <RDGeneral/BadFileException.h>
#include <RDStreams/streams.h>

namespace fs = boost::filesystem;

namespace RDKit{
	struct SupplierOption{
		bool takeOwnership = true;
		bool sanitize = true;
		bool removeHs = true;
		bool strictParsing = true;

		std::string delimiter = "\t";
		int smilesColumn = 0;
		int nameColumn = 1;
		bool titleLine = true;

		std::string nameRecord = "";
		int confId2D = -1;
		int confId3D = 0;
	};

  class GeneralFileReader{
	  public:
			std::string fname;
		  std::string file_format, compression_format;

		struct SupplierOption opt;	// options for the Mol Supplier 
		public:
			GeneralFileReader(std::string fileName);
		  GeneralFileReader(std::string fileName, struct SupplierOption options);

		  /*
		    Function to check the validity of the file and compression format
	    */
		  bool valid();

		  /*
		    Function to set the file and compression format
		  */
		  void setFormat();

		  /*
		    Get MolSupplier Object based on the file and compression format
	    */
		  RDKit::MolSupplier* getSupplier();
  };

	GeneralFileReader::GeneralFileReader(std::string fileName){
		fname = fileName;
		file_format = "";
		compression_format = "";
	}

	GeneralFileReader::GeneralFileReader(std::string fileName, struct SupplierOption options){
		fname = fileName;
		file_format = "";
		compression_format = "";
		opt = options;
	}

	bool GeneralFileReader::valid(){

		std::vector<std::string > file_formats{ "sdf", "mae", "smi", "csv", "txt", "tsv", "tdt"};
		std::vector<std::string > compression_formats{ "gz" };

    // Case 1: Unconventional format types
		if(file_format.compare("maegz") == 0){
			file_format = "mae";
			compression_format = "gz";
			return true;
		}

	
		// Case 2: Either the filename has a file format or compression format or both
		bool flag_fileFormat = std::find(file_formats.begin(), file_formats.end(), file_format) != file_formats.end();
		
		if (compression_format.compare("") != 0){
			bool flag_compressionFormat = std::find(compression_formats.begin(), compression_formats.end(), compression_format) 
			                              != compression_formats.end();
			                              
			// if the compression type is not valid then 
			if(flag_compressionFormat == false){
				file_format = compression_format;
				compression_format = "";
				flag_fileFormat = std::find(file_formats.begin(), file_formats.end(), file_format) != file_formats.end();
			}	
    }
		return flag_fileFormat;
	}

	void RDKit::GeneralFileReader::setFormat(){
		fs::path p(fname);
		std::string fileName = p.filename().string();
		int dots = std::count(fileName.begin(), fileName.end(), '.');

		if (dots == 0) throw std::invalid_argument("Recieved Invalid File Format, no extension or compression");

		else if (dots == 1){
			// there is a file format but no compression format
			int pos = fileName.find(".");
			file_format = fileName.substr(pos + 1);
			if (!valid()) throw std::invalid_argument("Recieved Invalid File Format");
		}
		else{
			// there is a file and compression format
			int n = fileName.length();
			int p1 = fileName.rfind(".");
			int p2 = fileName.rfind(".", p1 - 1);
			file_format = fileName.substr(p2 + 1, (p1 - p2) - 1);
			// possible compression format
			compression_format = fileName.substr(p1 + 1, (n - p1) + 1);
			if (!valid()) throw std::invalid_argument("Recieved Invalid File or Compression Format");
		}
	}

	RDKit::MolSupplier *RDKit::GeneralFileReader::getSupplier(){

    setFormat();

		if (compression_format.compare("") == 0){
			std::ifstream *strm = new std::ifstream(fname.c_str());

			if (file_format == "sdf"){
				ForwardSDMolSupplier *sdsup = new ForwardSDMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs, opt.strictParsing);
				return sdsup;
			}

			else if (file_format == "smi"){
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}

			else if (file_format == "csv"){
				opt.delimiter = ",";
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}

			else if (file_format == "txt"){
				opt.delimiter = "\t";
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}

			else if (file_format == "tsv"){
				opt.delimiter = "\t";
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}
 
			else if (file_format == "mae"){
				MaeMolSupplier *maesup = new MaeMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs);
				return maesup;
			}

			else if (file_format == "tdt"){
				TDTMolSupplier *tdtsup = new TDTMolSupplier(strm, opt.takeOwnership, opt.nameRecord, opt.confId2D, opt.confId3D, opt.sanitize);
				return tdtsup;
			}
		}
		else
		{
			auto *strm = new gzstream(fname);

			if (file_format == "sdf"){
				ForwardSDMolSupplier *sdsup = new ForwardSDMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs, opt.strictParsing);
				return sdsup;
			}

			else if (file_format == "smi"){
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}

			else if (file_format == "csv"){
				opt.delimiter = ",";
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}

			else if (file_format == "txt"){
				opt.delimiter = "\t";
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}

			else if (file_format == "tsv"){
				opt.delimiter = "\t";
				SmilesMolSupplier *smsup = new SmilesMolSupplier(strm, opt.takeOwnership, opt.delimiter, opt.smilesColumn, opt.nameColumn, opt.titleLine, opt.sanitize);
				return smsup;
			}
 
			else if (file_format == "mae"){
				MaeMolSupplier *maesup = new MaeMolSupplier(strm, opt.takeOwnership, opt.sanitize, opt.removeHs);
				return maesup;
			}

			else if (file_format == "tdt"){
				TDTMolSupplier *tdtsup = new TDTMolSupplier(strm, opt.takeOwnership, opt.nameRecord, opt.confId2D, opt.confId3D, opt.sanitize);
				return tdtsup;
			}
		}
	}
}
#endif

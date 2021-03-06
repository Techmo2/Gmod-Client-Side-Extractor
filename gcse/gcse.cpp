// gcse.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <Bootil\Bootil.h>
#include <vector>
#include <string>
using namespace Bootil;

std::vector<std::string> params;
int count;
BString gm_dir;
BString out_dir;

int main(int argc, char** argv)
{
	std::cout << std::endl;

	// Make sure we have some command line arguments
	if (argc < 2) {
		std::cout << "Usage: gcse -g <gmod directory> -o <output directory>" << std::endl;
		return 1;
	}

	// Push parameters into vector to make them easier to work with
	for (int i = 1; i < argc; i++) {
		params.push_back(std::string(argv[i]));
	}

	gm_dir = "";
	out_dir = "";
	::count = 0;

	// Parse command line options
	for (int i = 0; i <= params.size(); i++) {
		std::string current_param = params[i];
		if (current_param.compare("-g") == 0) {
			if (i >= params.size()) {
				std::cout << "Error: -g switch given, but no path specified" << std::endl;
				return 1;
			}

			gm_dir = params[i + 1];
			std::cout << "Using gmod directory: '" << gm_dir << "'" << std::endl;
		}

		else if (current_param.compare("-o") == 0) {
			if (i >= params.size()) {
				std::cout << "Error: -o switch given, but no path specified" << std::endl;
				return 1;
			}

			out_dir = params[i + 1];
			std::cout << "Using output directory: '" << out_dir << "'" << std::endl;
		}
	}

	// Be sure both paths were provided
	if (gm_dir == (BString)"") {
		std::cout << "Error: no gmod directory specified" << std::endl;
		return 1;
	}

	if (out_dir == (BString)"") {
		std::cout << "Error: no output directory specified" << std::endl;
		return 1;
	}

	std::cout << "Reading cached CS Scripts from: '" << gm_dir << "/garrysmod/cache/lua'" << std::endl;

	BString in_folder = gm_dir + "/garrysmod/cache/lua";
	String::List fileList;
	File::GetFilesInFolder(in_folder, fileList, false);

	std::cout << "Decompressing" << std::endl;

	// Decompress each file
	BOOTIL_FOREACH_CONST(f, fileList, String::List)
	{
		AutoBuffer inBuf;
		File::Read(in_folder + "/" + *f, inBuf);

		// Extract with LZMA ignoring the first 4 bytes
		AutoBuffer outBuf;
		Compression::LZMA::Extract(inBuf.GetBase(4), inBuf.GetSize() - 4, outBuf);

		// Write decompressed lua to lua file
		File::Write(out_dir + "/" + *f, outBuf);

		// Give status and update count
		std::cout << in_folder << "/" << *f << " -> " << out_dir << "/" << *f << std::endl;
		::count++;
	}

	std::cout << "Extracted " << ::count << " lua scripts to " << out_dir << std::endl;

	return 0;
}
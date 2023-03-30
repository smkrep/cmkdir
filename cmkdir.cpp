#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void printHelp() { //this method prints out the instructions
	std::cout << "\n\nUsage: cmkdir [OPTION] DIRECTORY...\n";
	std::cout << "Create the directory(ies), if it/they do not exist\n\n";
	std::cout << "Options\n-p, --parents     create parent directories if needed\n-h, --help        display help\n\n";
}

void tokenize(std::string const& str, const char separator, std::vector<std::string>& out) //this method tokenizes the string using /
{																						   //as a separator
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(separator, end)) != std::string::npos)
	{
		end = str.find(separator, start);
		out.push_back(str.substr(start, end - start));
	}
}

int main(int argc, char* argv[]) {

	std::string cmdName = "cmkdir", helpShort = "-h", helpLong = "--help", parentsShort = "-p", parentsLong = "--parents";
	const char separator = '/';
	std::vector<std::string> tokens;

	bool helpRequested = (argc == 2 && ( !strcmp( argv[1],helpShort.c_str() ) || !strcmp( argv[1], helpLong.c_str() )) 
		&& ( !strcmp(argv[0], cmdName.c_str()) ) ); //boolean interpretation of an occasion when "help" display is requested


	bool makeDir = (argc >= 2 && ( !strcmp(argv[0], cmdName.c_str()) ) 
		&& (strcmp(argv[1], parentsShort.c_str()) && strcmp(argv[1], parentsLong.c_str()))); //boolean interpretation of an occasion
																	     //when creation of dir/dirs is requested


	bool makeDirTree = (argc == 3 && (!strcmp(argv[1], parentsShort.c_str()) || !strcmp(argv[1], parentsLong.c_str())) 
		&& (!strcmp(argv[0], cmdName.c_str()))); //boolean interpretation of an occasion when creating a
												 //tree of directories is requested


	if (helpRequested) { //displaying instructions
		printHelp();
		return 0;
	}
	else if (makeDir) { //creating a directory (or directories) in current working directory

		for (int i = 1; i < argc; i++) {

			bool dirExists = false;

			tokenize(argv[i], separator, tokens);

			if (tokens.size() > 1) {
				std::cout << "\nArgument " << argv[i] << " is invalid\n";
				tokens.clear();
				continue;
			}

			tokens.clear();

			for (auto entry : fs::directory_iterator(".")) {

				if (entry.is_directory() && fs::path(entry).filename().string() == std::string(argv[i])) {

					std::cout << "\nThe directory " << fs::path(entry).filename().string() << " already exists\n";
					dirExists = true;
					break;
					
				}
			}

			if (dirExists) {
				continue;
			}
			else {
				fs::path path = fs::current_path().string() + "/" + std::string(argv[i]);
				fs::create_directory(path);
				std::cout << "\nCreated directory: " << argv[i] << "\n";
			}	
		}
	}
	else if (makeDirTree) { //creating a tree of directories

		tokenize(argv[2], separator, tokens);

		if (tokens.size() <= 1) {
			std::cout << "\nArgument " << argv[2] << " is invalid\n";
			tokens.clear();
			return 0;
		}


		fs::path dirTree = fs::current_path().string() + "/";

		for (auto var : tokens) {
			dirTree += var;
			fs::create_directory(dirTree);
			dirTree += "/";
		}

		std::cout << "\nTree " << argv[2] << " created successfully\n";
		
	}
	else {
		std::cout << "\nInvalid input detected, type 'cmkdir -h' or 'cmkdir --help' for more information\n";
	}
}
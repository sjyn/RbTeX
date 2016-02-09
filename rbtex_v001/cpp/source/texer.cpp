#include "../header/texer.h"
#include "../header/includes.h"
#include "../header/strutil.h"

Texer::Texer(const std::string& texFile){
	file = texFile;
	auxFile = file + RB_OUTPUT;
	rubyFile = file + RB_FIL;
}

/*
* A method that scans the TeX document for the RbTeX environments
* and copies them into rbtex_imdt.rb for later use.
*/
void Texer::scan(){
	std::ifstream scanner(file);
	std::ofstream writer(rubyFile);
	unsigned long long lino = 0;
	if(writer.is_open() && scanner.is_open()){
		std::string line;
		writer << REQUIRE_01 << std::endl;
		while(std::getline(scanner,line)){
			lino++;
			if(sutl::contains(RUB_ENV_02_START,line)){
				while(getline(scanner,line)){
					lino++;
					if(sutl::contains(RUB_ENV_02_END,line))
						break;
					writer << line << " #!!~~" << lino << std::endl;
				}
				writer << std::endl;
			} else if (sutl::contains(RUB_ENV_03_START,line)){
				size_t s = line.find("{");
				size_t e = line.find("}");
				std::string rbfn = line.substr(s + 1, e - s - 1);
				if(rbfn != ""){
					std::ifstream rbFile(rbfn);
					std::string rbFileContent;
					while(getline(rbFile,rbFileContent)){
						writer << rbFileContent << std::endl;
					}
				}
			}
		}
		scanner.close();
		writer << "Tex.return_control" << std::endl;
		writer.close();
	} else {
		throw std::string("Could not open files");
	}
}

void Texer::rubify(){
	std::ofstream write(auxFile);
	if(write.is_open()){
		write.close();
		if(system(std::string("ruby " + rubyFile + " ").c_str()) == 0){
			// std::ifstream scanner(auxFile);
		} else {
			throw std::string("Error Compiling Ruby Code!");
		}
	} else {
		throw std::string("No Such File: ") + std::string(auxFile);
	}
}

void Texer::postprocess(){
	std::ifstream scanner(auxFile);
}

void Texer::cleanup(){
	std::remove(rubyFile.c_str());
}
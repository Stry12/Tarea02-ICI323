#include <global.hh>
#include <iostream>
#include <getopt.h>


int main(){
	std::vector<std::string> textInMemory;
	
	std::string fileName;
	fileName = "data/quijote.txt";
    std::ifstream file(fileName); 
	
    if (!file) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return(EXIT_FAILURE);
    }
	
    std::string line;
    while (std::getline(file, line)) {
		textInMemory.push_back(line);
	}
	file.close();

    
}
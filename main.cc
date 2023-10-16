#include <global.hh>


std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) {
            result += c;
        }
    }
    return(result);
}

// Función para dividir un texto en palabras
std::vector<std::string> splitText(const std::string &text) {
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(text);
    while (iss >> word) {
		
        //std::string lowercaseWord = word;
        for (char &c : word) {
            c = std::tolower(c);
        }
		
		word = removePunctuation(word);
        words.push_back(word);
    }
    return words;
}

int main() {
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
	

    std::map<std::string, int> wordHistogram;
	for(auto line : textInMemory){
        // Dividir cada línea del texto en palabras
        std::vector<std::string> words = splitText(line);
        
        // Incrementar el conteo de cada palabra en el histograma
        for (const std::string &word : words) {
			wordHistogram[word]++;
        }
    }

    // Mostrar el histograma de palabras
    for (const auto &entry : wordHistogram) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }


    return(EXIT_SUCCESS);
}



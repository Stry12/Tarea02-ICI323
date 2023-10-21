#include <sstream>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include <vector>
#include <thread>
#include <map>
#include <mutex>

std::map<std::string, int> wordHistogram;
std::mutex histogramMutex;

std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) {
            result += c;
        }
    }
    return result;
}

std::vector<std::string> splitText(const std::string& text) {
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(text);
    while (iss >> word) {
        for (char &c : word) {
            c = std::tolower(c);
        }
        word = removePunctuation(word);
        words.push_back(word);
    }
    return words;
}

void processHistogram(std::vector<std::string>& textInMemory, size_t start, size_t end) {
    std::map<std::string, int> localHistogram;

    for (size_t i = start; i < end; i++) {
        const std::string& line = textInMemory[i];
        std::vector<std::string> words = splitText(line);

        for (const std::string& word : words) {
            localHistogram[word]++;
        }
    }

    // Agregar el histograma local al histograma global utilizando un mutex
    std::lock_guard<std::mutex> lock(histogramMutex);
    for (const auto& entry : localHistogram) {
        wordHistogram[entry.first] += entry.second;
    }
}

int main(int argc, char* argv[]) {
    int opt;
    bool noOptionsProvided = true;
    int numthreads = 1;
    std::string filename = "";

    struct option longOptions[] = {
        {"file", required_argument, nullptr, 'f'},
        {"threads", required_argument, nullptr, 't'},
        {"help", no_argument, nullptr, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "f:t:h:", longOptions, NULL)) != -1) {
        noOptionsProvided = false;
        switch (opt) {
            case 'f':
                filename = optarg;
                std::cout << "Uso -f: " << filename << std::endl;
                break;
            case 't':
                numthreads = atoi(optarg);
                std::cout << "Uso -t: " << numthreads << std::endl;
                break;
            case 'h':
                std::cout << "Modo de uso: ./histograma_mt --threads N --file FILENAME [--help]\n--threads: cantidad de threads a utilizar. Si es 1, entonces ejecuta la versión secuencial.\n--file: archivo a procesar.\n--help: muestra este mensaje y termina." << std::endl;
                break;
            case '?':
                std::cout << "Modo de uso: ./histograma_mt --threads N --file FILENAME [--help]\n--threads: cantidad de threads a utilizar. Si es 1, entonces ejecuta la versión secuencial.\n--file: archivo a procesar.\n--help: muestra este mensaje y termina." << std::endl;
                return EXIT_FAILURE;
        }
    }

    if (noOptionsProvided) {
        std::cout << "Modo de uso: ./histograma_mt --threads N --file FILENAME [--help]\n--threads: cantidad de threads a utilizar. Si es 1, entonces ejecuta la versión secuencial.\n--file: archivo a procesar.\n--help: muestra este mensaje y termina." << std::endl;
        return EXIT_FAILURE;
    } else {
        if (filename.empty()) {
            std::cerr << "No se proporcionó un nombre de archivo (-f)." << std::endl;
            return EXIT_FAILURE;
        }
        std::vector<std::string> textInMemory;
        try {
            std::ifstream file(filename);
            if (!file) {
                std::cerr << "No se pudo abrir el archivo." << std::endl;
                return EXIT_FAILURE;
            }

            std::string line;
            while (std::getline(file, line)) {
                textInMemory.push_back(line);
            }
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "Error al procesar el archivo: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Procesando con " << numthreads << " threads." << std::endl;
        int inicio = 0;
        int avance = textInMemory.size() / numthreads;

        std::vector<std::thread> threads;

        for (int i = 0; i < numthreads; i++) {
            int fin = inicio + avance;
            if (i == numthreads - 1) {
                fin = textInMemory.size(); // La última hebra maneja las líneas restantes
            }
            threads.push_back(std::thread(processHistogram, std::ref(textInMemory), inicio, fin));
            inicio = fin;
        }

        for (std::thread& thread : threads) {
            thread.join();
        }

        for (const auto &entry : wordHistogram) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
    }
    return EXIT_SUCCESS;
}

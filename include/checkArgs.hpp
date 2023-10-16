#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
	// 1) Modificar esta sección
	const std::string optString = "n:t:l:h";
	
	const std::string opciones = "-n tam_problema -t threads -l letra_a_buscar [-h]";

	const std::string descripcion  = "Descripción:\n"
		                             "\t-n   Tamaño del problema.\n"
									 "\t-t   Cantidad de threads a utilizar.\n"
									 "\t-l   Letra a buscar y contabilizar.\n"
									 "\t-h   Muestra esta ayuda y termina.\n";
	
	typedef struct args_t{
		uint32_t tamProblema;
		uint32_t numThreads;
		uint8_t  letra;
	} args_t;
	
	// 2) Modificar constructor
	// 3) Modificar ciclo "getopt" en método checkArgs::getArgs()
	// 4) Recuerde que para compilar nuevamente, y por tratarse
	//    de un archivo header, debe hacer primero un make clean
	
	args_t  parametros;
	
	int argc;
	char **argv;

	
public:
	checkArgs(int _argc , char **_argv);
	~checkArgs();
	args_t getArgs();
	
private:
	void printUsage();
	
	
};

checkArgs::checkArgs(int _argc , char **_argv){
	parametros.tamProblema = 0;
	parametros.numThreads  = 0;
	parametros.letra       = 0;

	argc = _argc;
	argv = _argv;
	
}

checkArgs::~checkArgs(){
	
}

checkArgs::args_t checkArgs::getArgs(){
	int opcion;
	
	while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
		switch (opcion) {
			case 'n':
					parametros.tamProblema = atoi(optarg);
					break;
			case 't':
					parametros.numThreads = atoi(optarg);
					break;
			case 'l':
					parametros.letra = *(optarg);
					break;
			case 'h':
			default:
					printUsage();
					exit(EXIT_FAILURE);
		}
	}

	if ( parametros.tamProblema == 0 ||
		 parametros.numThreads  == 0 || parametros.letra == 0){
		printUsage();
		exit(EXIT_FAILURE);
	}
	
	return(parametros);
}

void checkArgs::printUsage(){
	std::cout << "Uso: " <<
		argv[0] << " " << opciones << " " << descripcion << std::endl;
}



#endif

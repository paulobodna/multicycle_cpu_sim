#include <stdlib.h>
#include <stdio.h>

const int R = 256;	// tamanho da RAM em bytes

int main (int argc, char *argv[]) {

	// declaracao de variaveis e ponteiros

	int i, sim_error = 0;
	int **RAM = NULL;
	FILE *fp = NULL;
	unsigned int inst;

	if (argc < 2) {
		printf("uso: ./cpu_multi_code <arquivo>.bin\n");
		return 1;
	}

	RAM = (int **) calloc (R, sizeof(int *));
	for (i = 0; i < R; ++i) RAM[i] = (int *) calloc (8, sizeof(int));

	// leitura do arquivo binario

	fp = fopen(argv[1], "r");

	/*
		sim_error lida com erros surgidos durante a simulacao, causados pela decodificacao de uma
		instrucao invalida. O seu valor eh definido a depender do erro ocorrido. Se sim_error = 0,
		nenhum erro apareceu durante a execucao.
	*/

	while ((fscanf(fp, "%u", &inst) != EOF) || (sim_error > 0)) {

	}						
	
	fclose(fp);
	for (i = 0; i < R; ++i) free(RAM[i]);
	free(RAM);

	return 0;
}

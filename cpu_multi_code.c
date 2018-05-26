
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int R = 256;	// tamanho da RAM em bytes


int* dec_to_bin(unsigned int n){

	int* bin = NULL;
	int size = R;

	bin = (int *) malloc (size * sizeof(int));

	while (size > 0) {
		bin[size-1] = n % 2;
		n /= 2;
		--size;
	}

	
return bin;
}

int bin_to_dec(int* bin_num, int end_pos, int start_pos){ // a chamada dessa função pode ocorrer quando a word de 32 bits for colocada no IR

	int dec = 0;
	int exp = 0;

	for (int i = end_pos; i >= start_pos; i--) dec += bin_num[i] * pow(2, exp++);

return dec;
}



int main (int argc, char *argv[]) {

	// declaracao de variaveis e ponteiros

	int i, sim_error = 0;
	int **RAM = NULL;
	FILE *fp = NULL;
	
	int* bin_word = NULL;
	unsigned int dec_word;


	if (argc < 2) {
		printf("uso: ./cpu_multi_code <arquivo>.bin\n");
		return 1;
	}

	RAM = (int **) calloc (R, sizeof(int *));
	for (i = 0; i < R; ++i) RAM[i] = (int *) calloc (8, sizeof(int));

	bin_word = (int*) calloc(32, sizeof(int));

	// leitura do arquivo binario

	fp = fopen(argv[1], "r");

	/*
		sim_error lida com erros surgidos durante a simulacao, causados pela decodificacao de uma
		instrucao invalida. O seu valor eh definido a depender do erro ocorrido. Se sim_error = 0,
		nenhum erro apareceu durante a execucao.
	*/

	while ((fscanf(fp, "%u", &dec_word) != EOF) || (sim_error > 0)) {
		bin_word = dec_to_bin(dec_word);
		/* atribuir cada palavra à matriz que representa a RAM : TODO	*/


	}						
	
	fclose(fp);
	for (i = 0; i < R; ++i) free(RAM[i]);
	free(RAM);

	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct alu {
	int zero;
	int result;
} ALU;

typedef struct ir {
	int *inst;
} IR;

typedef struct regs {
	int *registers, *MDR;
	int PC, A, B, ALUout;
} REGS;

const int R = 256;	// tamanho da RAM em bytes

int init_cpu(int ***RAM, IR **ir, REGS **regs) {
	*RAM = (int **) calloc (R, sizeof(int *));
	for (int i = 0; i < R; i++) (*RAM)[i] = (int *) calloc (8, sizeof(int));

	*ir = (IR *) calloc(32, sizeof(IR));
	*regs = (REGS *) calloc(32, sizeof(REGS)); 

    return 1;
}

int free_cpu(int ***RAM, IR **ir, REGS **regs) {
	for (int i = 0; i < R; i++) free((*RAM)[i]);
	free(*RAM);
	*RAM = NULL;
    
	free(*ir);
	*ir = NULL;

	free(*regs);
	*regs = NULL;

	return 1;
}

int dec_to_bin(unsigned int n, int *bin){
	int size = 32;

	while (size > 0) {
		bin[size-1] = n % 2;
		n /= 2;
		size--;
	}

	return 1;
}

// a chamada dessa função pode ocorrer quando a word de 32 bits for colocada no IR
int bin_to_dec(int* bin_num, int start_pos, int end_pos) { 
	int dec = 0, exp = 0;

	for (int i = end_pos; i >= start_pos; i--) dec += bin_num[i] * pow(2, exp++);

	return dec;
}

int RAM_write(int **RAM, int start_pos, int *bin) { 
	int i, j; 
	for (i = 0, j = start_pos; i < 32; i+= 8, j++) {
        	memcpy(RAM[j], &(bin[i]), 8*sizeof(int));
	}

	return 1;
}

int readfile(char *file_name, int **RAM) {
	unsigned int inst;

	FILE *fp = NULL;
	fp = fopen(file_name, "r");

	if (fp == NULL) {
        	printf("Arquivo inexistente.\n");
		return 0;
    	}

	int *bin = (int *) calloc (32, sizeof(int));
	for (int i = 0; fscanf(fp, "%u", &inst) != EOF; i += 4) {
	        dec_to_bin(inst, bin);
        	RAM_write(RAM, i, bin);    
	}		
    				
	free(bin);
	fclose(fp);
	return 1;
}

ALU *alu_run(int a, int b, int ALUcontrol) {
	ALU *alu = (ALU *) malloc (sizeof(ALU));

	if (ALUcontrol == 2) {
		alu.result = a + b;
	} else if (ALUcontrol == 6) {
		alu.result = a - b;
	} else if (ALUcontrol == 0) {
		alu.result = a & b;
	} else if (ALUcontrol == 1) {
		alu.result = a | b;
	} else if (ALUcontrol == 7) {
		alu.result = (a < b) ? 1 : 0;
	}

	alu.zero = (alu.result == 0) ? 1 : 0;
	return alu;
}

int main (int argc, char *argv[]) {

	// declaracao de variaveis e ponteiros

	int **RAM = NULL;
	IR *ir = NULL;
	REGS *regs = NULL;

	if (argc < 2) {
		printf("uso: ./cpu_multi_code <arquivo>.bin\n");
		return 1;
	}

	// iniciando modulos da cpu
	init_cpu(&RAM, &ir, &regs);
	// leitura do arquivo
	if (readfile(argv[1], RAM)) {
		for (i = 0; i < ; i += 4) {
			
		}
//        for(int i = 0; i < R; i ++) {
//            printf("[%d] - %d", i, bin_to_dec(RAM[i], 0, 7)); // impressao em decimal
////            for(int j = 0; j < 8; j++) { // impressao em binario
////                printf("%d", RAM[i][j]); //
////            }                             //
//            printf("\n");
//        }
//        printf("\n");
    //    run_simulation(RAM, ir, regs);
	}

	free_cpu(&RAM, &ir, &regs);

	return 0;
}

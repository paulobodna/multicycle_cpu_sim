#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct uc {
    int *state; // estado atual
    int *ns; // próximo estado
    int *ctrl; // bits de controle, segundo a especificação;
} UC;

typedef struct alu {
	int zero;
	int result;
} ALU;

typedef struct ir {
	int *inst;
} IR;

typedef struct regs {
	int *registers;
	int MDR, PC, A, B, ALUout;
} REGS;

const int R = 256;	// tamanho da RAM em bytes

int init_cpu(int ***RAM, IR **ir, REGS **regs, UC **uc) {
	*RAM = (int **) calloc (R, sizeof(int *));
	for (int i = 0; i < R; i++) (*RAM)[i] = (int *) calloc (8, sizeof(int));

	*ir = (IR *) malloc (sizeof(IR));
	(*ir)->inst = (int *) calloc (32, sizeof(int));

	*regs = (REGS *) malloc (sizeof(REGS));
	(*regs)->registers = (int *) calloc (32, sizeof(int));

	*uc = (UC *) malloc (sizeof(UC));
	(*uc)->state = (int *) calloc(4, sizof(int));
	(*uc)->ns = (int *) calloc(4, sizof(int));
	(*uc)->ctrl = (int *) calloc(19, sizof(int));

	return 1;
}

int free_cpu(int ***RAM, IR **ir, REGS **regs, UC *uc) {
	for (int i = 0; i < R; i++) free((*RAM)[i]);
	free(*RAM);
	*RAM = NULL;

	free((*ir)->inst);
	free(*ir);
	*ir = NULL;

	free((*regs)->registers);
	free(*regs);
	*regs = NULL;

	free((*uc)->state);
	free((*uc)->ns);
	free((*uc)->ctrl);
	free(*uc);
	*uc = NULL;

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

int bin_to_dec(int* bin_num, int start_pos, int end_pos) {
	int dec = 0, exp = 0;

	for (int i = end_pos; i >= start_pos; i--) dec += bin_num[i] * pow(2, exp++);

	return dec;
}

int update_uc(UC *uc, IR *ir) {

  //state = next state
  memcpy(uc->state, uc->ns, 4 * sizeof(int));

  //condicao de saida da simulacao
  int check = bin_to_dec(uc->state, 0, 5);
  if(check == 1) {
    check = bin_to_dec(ir->inst, 0, 5);
    if(check > 25) {
      printf("Instrucao invalida.\n");
      return 0;
    }
  }

  // atualizando proximo estado
  uc->ns[0] =
  uc->ns[1] =
  uc->ns[2] =
  uc->ns[3] =

  //atualizando bits de controles
  uc->ctrl[0] =
  uc->ctrl[1] =
  uc->ctrl[2] =
  uc->ctrl[3] =
  uc->ctrl[4] =
  uc->ctrl[5] =
  uc->ctrl[6] =
  uc->ctrl[7] =
  uc->ctrl[8] =
  uc->ctrl[9] =
  uc->ctrl[10] =
  uc->ctrl[11] =
  uc->ctrl[12] =
  uc->ctrl[13] =
  uc->ctrl[14] =
  uc->ctrl[15] =
  uc->ctrl[16] =
  uc->ctrl[17] =
  uc->ctrl[18] =

  return 1;
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

void datapath(int **RAM, IR **ir, REGS **regs) {
	int *opcode, *rs, *rt, *rd, *shamt, *fcode, *imm, *target;
	regs->PC = 0;

	while (1) {
		// PASSO 1: busca da instrução
		for (int i = 0; i < 4; ++i)
			memcpy(ir->inst+(i*8), RAM[i+PC], 8*sizeof(int));
		opcode = (int *) malloc (6 * sizeof(int));
		memcpy(opcode, ir->inst, 6*sizeof(int));
		if (opcode == 0) {	// R-type instruction
			rs = (int *) malloc (5 * sizeof(int));
			memcpy(rs, ir->inst+6, 5*sizeof(int));
//			rs = bin_to_dec(ir->inst, 6, 10);

			rt = (int *) malloc (5 * sizeof(int));
			memcpy(rt, ir->inst+11, 5*sizeof(int));
//			rt = bin_to_dec(ir->inst, 11, 15);

			rd = (int *) malloc (5 * sizeof(int));
			memcpy(rd, ir->inst+16, 5*sizeof(int));
//			rd = bin_to_dec(ir->inst, 16, 20);

			shamt = (int *) malloc (5 * sizeof(int));
			memcpy(shamt, ir->inst+21, 5*sizeof(int));
//			shamt = bin_to_dec(ir->inst, 21, 25);

			fcode = (int *) malloc (5 * sizeof(int));
			memcpy(fcode, ir->inst+26, 5*sizeof(int));
//			fcode = bin_to_dec(ir->inst, 26, 31);
		} else if (opcode == ...) { // I-type instruction
//			rs = bin_to_dec(ir->inst, 6, 10);
			rs = (int *) malloc (5 * sizeof(int));
			memcpy(rs, ir->inst+6, 5*sizeof(int));

//			rt = bin_to_dec(ir->inst, 11, 15);
			rt = (int *) malloc (5 * sizeof(int));
			memcpy(rt, ir->inst+11, 5*sizeof(int));

//			imm = bin_to_dec(ir->inst, 16, 31);
			imm = (int *) malloc (16 * sizeof(int));
			memcpy(imm, ir->inst+16, 16*sizeof(int));
		} else {	// J-type instruction
//			target = bin_to_dec(ir->inst, 6, 31);
			target = (int *) malloc (28 * sizeof(int));
			memcpy(target, ir->inst+6, 28*sizeof(int));
		}
		memcpy(regs->MDR, ir->inst, 32*sizeof(int));
//		regs->PC += 4;

		// PASSO 2: decodificação da instrução e busca dos registradores
		memcpy(regs->A, rs, 5*sizeof(int));
		memcpy(regs->B, rt, 5*sizeof(int));
//		regs->A = rs, regs->B = rt;
//		regs->ALUout = regs->PC + (bin_to_dec(ir->inst, 16, 31) << 2);
	}


}

int main (int argc, char *argv[]) {

	// declaracao de variaveis e ponteiros

	int **RAM = NULL;
	IR *ir = NULL;
	REGS *regs = NULL;
	UC *uc = NULL;

	if (argc < 2) {
		printf("uso: ./cpu_multi_code <arquivo>.bin\n");
		return 1;
	}

	// iniciando modulos da cpu
	init_cpu(&RAM, &ir, &regs, &uc);
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

	free_cpu(&RAM, &ir, &regs, &uc);

	return 0;
}

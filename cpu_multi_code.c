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
	unsigned MDR;
	int PC, A, B, ALUout;
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

// a chamada dessa função pode ocorrer quando a word de 32 bits for colocada no IR
unsigned bin_to_dec(int* bin_num, int start_pos, int end_pos) { 
	unsigned dec = 0, exp = 0;

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

void alu_run(ALU *alu, int a, int b, int ALUcontrol) {
	if (ALUcontrol == 2) {
		alu->result = a + b;
	} else if (ALUcontrol == 6) {
		alu->result = a - b;
	} else if (ALUcontrol == 0) {
		alu->result = a & b;
	} else if (ALUcontrol == 1) {
		alu->result = a | b;
	} else if (ALUcontrol == 7) {
		alu->result = (a < b) ? 1 : 0;
	}

	alu->zero = (alu->result == 0) ? 1 : 0;
	return alu;
}

void PC_update(int *PC, REGS *regs,) {

}

void datapath(int **RAM, IR *ir, REGS *regs, UC *uc) {
	int s, address, *MemData, ReadReg1, ReadReg2, WriteReg, ALUin1, ALUin2;
	regs->PC = 0;

	while (1) {
		s = bin_to_dec(uc->ctrl, 0, 1); 	// RegDst
		if (s == 0) {
			WriteReg = bin_to_dec(ir->inst, 11, 15);
		} else if (s == 1) {
			WriteReg = bin_to_dec(ir->inst, 16, 20);
		} else if (s == 2) {
			WriteReg = 31; // $ra
		}

		if (uc->ctrl[2]) {			// RegWrite
			ReadReg1 = bin_to_dec(ir->inst, 6, 10);
			ReadReg2 = bin_to_dec(ir->inst, 11, 15);
			regs->A = regs->registers[ReadReg1];
			regs->B = regs->registers[ReadReg2];
		}

		if (!uc->ctrl[3]) {			// ALUSrcA
			ALUin1 = regs->PC;
		} else if (uc->ctrl[3]) {
			ALUin1 = regs->A;
		}

		s = bin_to_dec(uc->ctrl, 4, 5); 	// ALUSrcB
		if (s == 0) {
			ALUin2 = regs->B;
		} else if (s == 1) {
			ALUin2 = 4;
		} else if (s == 2) {
			ALUin2 = bin_to_dec(ir->inst, 16, 31);
		} else if (s == 3) {
			ALUin2 = bin_to_dec(ir->inst, 16, 31) << 2;	
		}

		s = bin_to_dec(uc->ctrl, 8, 9); 	// PCSource
		if (s == 0) {
			regs->PC = alu->result;
		} else if (s == 1) {
			regs->PC = regs->ALUout;
		} else if (s == 2) {
			regs->PC = bin_to_dec(ir->inst, 6, 31) << 2;
		} else if (s == 3) {
			regs->PC = regs->A;
		}

		if (!uc->ctrl[12]) {			// IorD
			address = regs->PC;
		} else if (uc->ctrl[12]) {
			address = regs->ALUout;
		}

		if (uc->ctrl[13]) { 			// MemRead
			for (int i = 0; i < 4; ++i)
				memcpy(MemData, RAM[i+address], 8*sizeof(int));
			memcpy(regs->MDR, ir->inst, 32*sizeof(int));
		}
		
		if (uc->ctrl[16]) {			// IRWrite
			memcpy(ir->inst, MemData, 32*sizeof(int));
		}


	}


}

void sim_output(int **RAM, IR *ir, REGS *regs) {
	int *aux = (int *) calloc (32, sizeof(int));
	int a = 0, b = 32, c = 64, d = 96;
	printf("PC=%d\tIR=%u\tMDR=%u\n", regs->PC, bin_to_dec(ir->inst, 0, 31), bin_to_dec(regs->MDR, 0, 31));
	printf("A=%d\tB=%d\tAluOut=%d\n", regs->A, regs->B, regs->ALUout);
	printf("Controle=???\n\n"); // TODO

	printf("Banco de Registradores\n");
	printf("R00(r0)=%d\tR08(t0)=%d\tR16(s0)=%d\tR24(t8)=%d\n", regs->registers[0], regs->registers[8], regs->registers[16], regs->registers[24]);
	printf("R01(at)=%d\tR09(t1)=%d\tR17(s1)=%d\tR25(t9)=%d\n", regs->registers[1], regs->registers[9], regs->registers[17], regs->registers[25]);
	printf("R02(v0)=%d\tR10(t2)=%d\tR18(s2)=%d\tR26(k0)=%d\n", regs->registers[2], regs->registers[10], regs->registers[18], regs->registers[26]);
	printf("R03(v1)=%d\tR11(t3)=%d\tR19(s3)=%d\tR27(k1)=%d\n", regs->registers[3], regs->registers[11], regs->registers[19], regs->registers[27]);
	printf("R04(a0)=%d\tR12(t4)=%d\tR20(s4)=%d\tR28(gp)=%d\n", regs->registers[4], regs->registers[12], regs->registers[20], regs->registers[28]);
	printf("R05(a1)=%d\tR13(t5)=%d\tR21(s5)=%d\tR29(sp)=%d\n", regs->registers[5], regs->registers[13], regs->registers[21], regs->registers[29]);
	printf("R06(a2)=%d\tR14(t6)=%d\tR22(s6)=%d\tR30(s8)=%d\n", regs->registers[6], regs->registers[14], regs->registers[22], regs->registers[30]);
	printf("R07(a3)=%d\tR15(t7)=%d\tR23(s7)=%d\tR31(ra)=%d\n", regs->registers[7], regs->registers[15], regs->registers[23], regs->registers[31]);
	printf("\n");
	
	for (int i = 0; i < 8; ++i) {
			// linha 1
			memcpy(aux, RAM[a++], 8*sizeof(int));
			memcpy(aux+8, RAM[a++], 8*sizeof(int));
			memcpy(aux+16, RAM[a++], 8*sizeof(int));
			memcpy(aux+24, RAM[a++], 8*sizeof(int));
			
			if (a < 10) printf("[0%d]=%u\t\t", a, bin_to_dec(aux, 0, 31));
			else printf("[%d]=%u\t\t", a, bin_to_dec(aux, 0, 31));
			// linha 2
			memcpy(aux, RAM[b++], 8*sizeof(int));
			memcpy(aux+8, RAM[b++], 8*sizeof(int));
			memcpy(aux+16, RAM[b++], 8*sizeof(int));
			memcpy(aux+24, RAM[b++], 8*sizeof(int));
			
			printf("[%d]=%u\t\t", b, bin_to_dec(aux, 0, 31));
			// linha 3
			memcpy(aux, RAM[c++], 8*sizeof(int));
			memcpy(aux+8, RAM[c++], 8*sizeof(int));
			memcpy(aux+16, RAM[c++], 8*sizeof(int));
			memcpy(aux+24, RAM[c++], 8*sizeof(int));
			
			printf("[%d]=%u\t\t", c, bin_to_dec(aux, 0, 31));
			// linha 4
			memcpy(aux, RAM[d++], 8*sizeof(int));
			memcpy(aux+8, RAM[d++], 8*sizeof(int));
			memcpy(aux+16, RAM[d++], 8*sizeof(int));
			memcpy(aux+24, RAM[d++], 8*sizeof(int));
			
			printf("[%d]=%u\n", d, bin_to_dec(aux, 0, 31));
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
		sim_output(RAM, ir, regs); 
		/*for (i = 0; i < ; i += 4) {
			
		}*/

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

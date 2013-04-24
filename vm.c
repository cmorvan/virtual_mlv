/* ***************************************************************************
 * This file is part of virtual_mlv.
 *
 * virtual_mlv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * virtual_mlv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with virtual_mlv.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 *
 *  Authors: S. Lombardy, N. Bedon, C. Morvan
 *
 *************************************************************************** */


#include"mystack.h"
#include"actions.h"
#include "vm.h"


int reg1,reg2; /* The registers */
int base=0;
int *prog;  /* The code itself */
int prog_counter=0; /* Ordinal counter */
extern int prog_length; 
int debug=0;

int vm_neg(void){
  MAKE_OPE_UN(-reg1);
}


int vm_add(void){
  MAKE_OPE_BIN(reg1+reg2);
}


int vm_sub(void){
  MAKE_OPE_BIN(reg1-reg2);
}


int vm_mult(void){
  MAKE_OPE_BIN(reg1*reg2);
}


int vm_div(void){
  if(reg2==0){fprintf(stderr,"Division par 0\n"); return 1;}
  MAKE_OPE_BIN(reg1/reg2);
}


int vm_mod(void){
  if(reg2==0){fprintf(stderr,"Modulo par 0\n"); return 1;}
  MAKE_OPE_BIN(reg1%reg2);
}


int vm_equal(void){
    MAKE_OPE_BIN(reg1==reg2);
}


int vm_noteq(void){
    MAKE_OPE_BIN(reg1!=reg2);
}


int vm_low(void){
    MAKE_OPE_BIN(reg1<reg2);
}

int vm_leq(void){
    MAKE_OPE_BIN(reg1<=reg2);
}

int vm_great(void){
    MAKE_OPE_BIN(reg1>reg2);
}

int vm_geq(void){
    MAKE_OPE_BIN(reg1>=reg2);
}

int vm_set(int n){
    MAKE_OPE_UN(n);
}

int vm_load(void){
  if(dirload(reg1 ,&reg1)){
    fprintf(stderr,"LOAD: accès pile illégal\n"); return 1;
  }
  return 0;
}

int vm_loadr(void){
  if(dirload(reg1 +base ,&reg1)){
    fprintf(stderr,"LOADR: accès pile illégal\n"); return 1;
  }
  return 0;
}

int vm_save(void){
  if(dirsave(reg2, reg1)){
    fprintf(stderr,"SAVE: accès pile illégal\n"); return 1;
  }
   return 0;
}   

int vm_saver(void){
  if(dirsave(reg2 + base, reg1)){
    fprintf(stderr,"SAVER: accès pile illégal\n"); return 1;
  }
  return 0;
}   

int vm_swap(void){
  reg1^=reg2; reg2^=reg1; reg1^=reg2;
  return 0;
}

int vm_read(void){
  printf("Read:");
  reg1 = low_read();
  return 0;
}

int low_read(){
  /* reads character as long as they are digits
     Constructs the relevant integer
   */
  char tmp[LENGTH_INT+1]; /* +1 pour '/0' */
  int neg=1, i=0, cur;
  while ((cur = getchar())==' ');
  if (cur == (int)'-'){
    neg=-1;
    cur = getchar();
  }
  while ((('0' <= cur)&&(cur <= '9'))&& (i<LENGTH_INT) ){
    tmp[i++]=cur;
    cur = getchar();
  }
  tmp[i]='\0';
  /* We suppress trailling spaces and \n,  nothing else*/
  while (' ' == cur){
    cur = getchar();
  }
  if ('\n' != cur){
    /* If not '\n' then unget */
    ungetc(cur, stdin);
  }
  return atoi(tmp)*neg;
}

int vm_write(void){
  printf("Write:%d\n",reg1);
  return 0;
}

int vm_readch(void){
  reg1 = getchar();
  return 0;
}
int vm_writech(void){
  putchar(reg1);
  return 0;
}

int vm_jump(int n){
  if(n>=prog_length || n<0){
    fprintf(stderr,"JUMP: branchement illégal\n"); return 1;
  }
  prog_counter=n;
  return 0;
}

int vm_jumpf(int n){
  if(reg1==0){
    if(n>=prog_length || n<0){
      fprintf(stderr,"JUMPF: branchement illégal\n"); return 1;
    }
  prog_counter=n;
  }
  return 0;
}

int vm_halt(void){
  free(prog); free_stack();
  exit(EXIT_SUCCESS);
}

int vm_call(int n){
  if(push(prog_counter+1)){
    fprintf(stderr,"Stack overflow\n"); return 1;
  }
  if(push(base)){
    fprintf(stderr,"Stack overflow\n"); return 1;
  }
  base=stack_size();
  return(vm_jump(n));
}

int vm_return(void){
  while(stack_size()>base) if(pop(NULL)){fprintf(stderr,"RETURN: Pile vide\n"); return 1;}
  pop(&base);
  pop(&prog_counter);
  return 0;
}

int vm_alloc(int n){
  int i;
  for(i=0;i<n;i++)
    if(push(0)){
      fprintf(stderr,"ALLOC: Stack overflow\n"); return 1;
    }
  return 0;
}

int vm_free(int n){
  int i;
  for(i=0;i<n;i++) if(pop(NULL)){fprintf(stderr,"FREE: Pile vide\n"); return 1;}
  return 0;
}

int vm_push(void){
    if(push(reg1)){
      fprintf(stderr,"PUSH: Stack overflow\n"); return 1;
    }
  return 0;
}

int vm_pop(void){
  if(pop(&reg1)){fprintf(stderr,"FREE: Pile vide\n"); return 1;}
  return 0;
}



int vm_execute(void){
  while(1){
    switch(prog[prog_counter++]){
    case VM_NEG: vm_neg(); break;
    case VM_ADD: vm_add(); break;
    case VM_SUB: vm_sub(); break;
    case VM_MULT: vm_mult();break;
    case VM_DIV: vm_div(); break;
    case VM_MOD: vm_mod(); break;
    case VM_EQUAL: vm_equal(); break;
    case VM_NOTEQ: vm_noteq(); break;
    case VM_LOW  : vm_low(); break;
    case VM_LEQ  : vm_leq(); break;
    case VM_GREAT: vm_great(); break;
    case VM_GEQ  : vm_geq(); break;
    case VM_SET  : vm_set(prog[prog_counter++]); break;
    case VM_LOAD : vm_load(); break;
    case VM_LOADR: vm_loadr(); break;
    case VM_SAVE : vm_save(); break;
    case VM_SAVER: vm_saver(); break;
    case VM_JUMP : vm_jump(prog[prog_counter]); break;
    case VM_JUMPF: vm_jumpf(prog[prog_counter++]); break;
    case VM_PUSH : vm_push(); break;
    case VM_POP  : vm_pop(); break;
    case VM_SWAP : vm_swap(); break;
    case VM_READ : vm_read(); break;
    case VM_WRITE: vm_write(); break;
    case VM_READCH : vm_readch(); break;
    case VM_WRITECH: vm_writech(); break;
    case VM_HALT : vm_halt(); break;
    case VM_CALL : vm_call(prog[prog_counter]); break;
    case VM_RETURN: vm_return(); break;
    case VM_ALLOC: vm_alloc(prog[prog_counter++]); break;
    case VM_FREE : vm_free(prog[prog_counter++]); break;
    default: return 1;
    }
    if(debug){
      display_stack();printf("prog line: %d  register 1: %d   register 2: %d\n",prog_counter,reg1,reg2);
      getchar();
    }
  }
  return 0;
}

int main(int argc, char** argv){
  int i=1;
  FILE *input;
  if (argc>=2 && strcmp(argv[1],"-debug")==0){
    debug=1; i++;
  }
  if(argc>i+1){
    fprintf(stderr,"usage: %s [-debug] [<fichier>]\n",argv[0]);
    return 1;
  }
  if(argc==i+1)
    input=fopen(argv[i],"r");
  else
    input=stdin;
  /* Using the fuction generated from loadprog.y*/
  loadprog(argv[i],input);
  if (input!=stdin) fclose(input);
  if(debug==1){
    printf("Segment de code\n");
    for(i=0;i<prog_length; i++)
      printf("%d  ",prog[i]);
  }
  printf("\n");
  vm_execute();
  return 0;
}



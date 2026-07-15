#include "inttypes.h"

#define PROCESS_STACK_SIZE 512
#define PROCESS_NAME_FORMAT "[process #pid] State: state"

enum process_state {
  READY,
  CHOSEN,
  SLEEPING,
};

enum process_registers_index {
    EBX = 0,
    ESP = 1,
    EBP = 2,
    ESI = 3,
    EDI = 4
};

struct Process {
  int pid;
  char name[20];
  enum process_state state;
  // ebx, esp, ebp, esi, edi
  uint32_t register_table[5];
  int32_t call_stack[PROCESS_STACK_SIZE];
};



void initialize_process(struct Process *p2, char *name, void (*process_fn)());
void idle_process_initialization(struct Process *p);

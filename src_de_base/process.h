#include "inttypes.h"


#define PROCESS_STACK_SIZE 512

/*
 * The number of registers to save and restore on a context switch.
 *
 * They are the %EBX, %ESP, %EBP, %ESI and %EDI non-scratch registers,
 * on x86 architecture.
 */
#define NUMBER_OF_REGISTERS 5
#define PROCESS_NAME_FORMAT "[process #pid] State: state"

enum process_state {
    UNINITIALIZED,
    READY,
    CHOSEN,
    SLEEPING,
    ZOMBIE
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
  uint32_t register_table[NUMBER_OF_REGISTERS];
  int32_t call_stack[PROCESS_STACK_SIZE];
  uint32_t waking_time;
};

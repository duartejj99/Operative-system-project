# Bugbuster list

This is a list of bugs, but more important a list of bug's explanations
and solution to track the knowledge I learnt solving the bugs of this project.
## Screen driver bugs

**Cursor placement and incorrect data type pointer**

On the screen driver there is a memory zone that controls what is display on the screen.
This memory zone is divided on cells. In each cell, there is the information of the character to display, 
and the format to display it, which take as a whole 2 bytes.

**Bug observation**

I observed the characters were displayed on the screen but the cursor didn't appear on the next case where the next character will be written.

Strangely I observed that if I placed the cursor in other case other than that one, it appeared. And while debugging, it appeared at the moment I set the character and its format on the cursor position, but once the cursor advanced, it didn't appear anymore.

**Explanation**

In my implementation, while writing the characters I was pointing at the memory zones with a uint32_t * pointer  to write the 16 bit value to cells of size of 16 bits. This created a situation where 

1. The correct char information was written on the current cell
2. The next adjacent cell was overwritten with zeros, since I have a uint32_t * pointer which indicates my data size is 32 bits.
3. At the end of my writing, the next cell, where the cursor placement should happen but was not seen, I had the cell as [0x00, 0x00].
4. The [0x00, 0x00] cell indicated that my background color was black and my text color was BLACK as well.
5. Doing some verification, I found out the color of the cursor is the same as the letter. And since everything was black, it was not being seen! (I played with gdb to confirm everything of the above and figure it out)


**Solution**

The solution is to only use a pointer of the size of your real data, not a bigger one, it can corrupt data even if that only means an invisible cursor.

PD: I discovered the memory multi-byte lecture is little-endian.


## Constants declaration on C

**Problem**

I tried to use a constant `SCREEN_WIDTH` that was originally declared on the file screen.c, into screen.h. 
For it to be included on time.c that could serve from it just by including screen.h.

I had an linkage error message that says that the `SCREEN_WIDTH` constant was being defined multiples times:
On the time.o and start.o files, the error messages says that the first declaration was on screen.h.


**Google it**

After an extensive google search I found that you dont define variables on headers files.

On quotes:
"If you put a definition of a global variable in a header file, then this definition will go to every .c file that includes this header, and you will get multiple definition error because a varible may be declared multiple times but can be defined only once."

If for some reason you need a constant that absolutely needs to be defined on the header, use the word static:
This guarantees that the constant is defined only one on your whole program and can be accessed multiple times.

Source:https://stackoverflow.com/questions/17764661/multiple-definition-of-linker-error


## Struct initialization and assignment

#### **Context**

When Initializing my process data struct, I have to **initialize** the value of that process **stack pointer register** `esp` to point into the top of its own **stack**. 
And also that stack top should have the **return address** where the program should start (or resume), once a context switch has been done.

At the end of the context switch, when the `ret` instruction is executed, the value pointed by `esp` is **pop** onto the **program counter** to indicate the next instruction to execute.



**Bug Observation**: 
A process struct had the following fields:
```
Process
|
|--------> register_table[5] // register_table[1] corresponds to ESP value.
|
|
|--------> call_stack[512] // call_stack[511] is the top of the stack
```

At initialization:

call_stack[PROCESS_STACK_SIZE - 1] should have the process_fn_address
register_table[ESP] should be equal to & call_stack[PROCESS_STACK_SIZE -1].


1. Process fn address was 0x110003.
2. Successfully added onto the top of the stack. :white_check_mark:
3. Top stack addr was 0x11353c :white_check_mark:
4.  *(esp) should be equal to process_fn_addr1. It did :white_check_mark:
5. esp should be equal then to the top stack addr (0x11353c) :x:


**BUT I expected `esp` being 0x11353c but it was 0x11db1c**. I suspected something was wrong.


**Explanation**

Upon initialization, I pass a struct pointer, and the initialization function set its values.
Once its finished, the values were correclty set and `esp` actually had the same value as the
stack end.

Surprisingly, that value was `0x1d320`. This observation was done on a debugging session, and
inside the end of the initialization fn


After asking chatgpt, I realized that I did a copy of the structure I initialized.
I thought the **struct assignment** meant only a copy of a pointer, but it copies the whole
struct!!!

That means that when I check the struct copy `os_processes[1]`, its `register_table[ESP]` value points
to the **stack of the ORIGINAL copy** (`process1`), and not its own stack.


**Solution**

Instead of creating a struct and passsing it to the initialization function.
I passed the pointer to the corresponding case of os_processes[].


## Stack Overflow - Stack restoration bug

This is the biggest bug I've ever attacked in this context.

### **Context**


While working on the **dynamic process creation** I found an indeterministic crash. The program worked, but could crash with apparently subtle changes like

1. declaring a variable 
2. Replacing a variable for another one on a function call.

**Observation**: This little changes resulted on a crash on the program.

I was not happy by just living the project on a state of "working" with really luck more than understanding.


On that context, I debugged and found out the trouble maker could be on the ```new_process()``` function but without any reason on why.

### **Explaining**
 
 I was doing a really funny stack overflow

I had a Process Control Block array, with each control block containing the dedicated stack zone of each process (512 cases of 4 bytes each = 2048 bytes). Each stack, being by logic, smaller than the whole size of the control block (2100 bytes).

**Corruption process**

1. `Process 4` is created.
2. `Process 4` is about to spawn another process with `new_process()`
3. `new_process()` call reserves stack space for the local variables. 
4. There is a variable `process` of type `struct Process` declared inside `new_process()`
5. Size of variable `process` is bigger than `Process 4`'s stack space 
5. `Stack pointer` results on pointing to `Process 3`
6. `Process 4's return address` corruption happens when calling `sprintf(...)`



**Conclusion**: `process` variable corrupted the adjacent Process 3 stack allocated on the PCB array. Explaining perfectly the bug.
 
 ### **Solution**

I created an struct Process and assigned to the corresponding PCB array slot that already had a memory allocated for the struct.

Once I stopped allocating the struct, and instead used a pointer to the already reserved PCB array slot to initialize it, every problem dissapeared, my little kernel was finally functional.

This was the final step for the solution!
I'm proud.

### **Learnings**
1.  Dont allocate big structs on a limited stack size.
 This was the original bug I had in my project 5 years ago. I just checked it.
2.  Learning how the stack worked, and how function calls work in assembly, saved this project.

### For more information

1. Check the [Security section]() on the [Call stack](https://en.wikipedia.org/wiki/Call_stack) wiki page
2. Check [Stack buffer overflow](https://en.wikipedia.org/wiki/Stack_buffer_overflow)

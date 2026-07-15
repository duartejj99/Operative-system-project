Lessons

# Computer architecture


In order to understand the current project section (processes), I had to put my hands and head into
understanding computer architecture, CPU, registers and stack management upon a program execution.


I was able to understand how a cpu is really used when executing a programs code.
I was able to understand how the stack is managed, why is so important on context switching.
I was able to simulate the stack mechanism with a simple example that let me
I was able to learn the conventions on the x86 architecture to understand the return address recovery
I was able to learn a little more about assembly, and its call and ret instructions (What they really do).



1.  I understood how parameters are the first thing to be pushed into the stack by the caller (and in which order)
2.  I understood how `call fn_name` automatically push the return address into the stack.
3.  I understood how the current `ebp` is saved by the callee into the stack
4.  I understood how `ebp` is moved now into the `esp` position to create a new stack frame
5.  I understood that the esp can grow and shrink on demand while allocating space for the local variables.
6.  I understood how at the end of a fn, `ebp` and `esp` could be pointing at the same @,
    in this case, the old `ebp`.
6.  I understood how at the end the old `ebp` (the one saved on the stack on step 3) is restored on the
    callee
7.  I understood how after that `esp` was pointing at the return address
8.  I understood when we arrive to `ret`, this return address is pop into the program counter.
9.  I understood how the stack is cleaned out of the parameters, as a caller responsability
    Ex: If I pushed two 32bits parameters, then after `call fn_name`, as a caller I will put `ADD ESP, 8`

    
For more info: https://wiki.osdev.org/Stack [Stack architecture on x86]
For more info: https://youtu.be/u_-oQx_4jvo?si=ZkfGff86CFe-TsbX [How Assembly function works]


## Next step

I understood how the context switch works explicitaly being called by one of the processes.

But what happen when an interruption is called?

What happen to the stack pointer?
What happen to the registers?

1. I see that eax, edx, ecx are saved and restored before and after the interruption
2. Entering the interruption we saved eflags and also the RETURN ADDR (#1) to resume the interrupted process.
3. WE CALL THE INTERRUPTION (CLOCK)
4. Here again a return address should be pushed into the stack.
5. The interruption handle happens and at the end we should CTX_SW.
6. This means we are going to save the CURRENT STACK POINTER (#sp1) that is in the end of the interruption, before it arrives to IRET
7. Take the new chosen PROCESS STACK POINTER (#sp2) and this I imagine it was also interrupted at some point
8. Which means, he will get out of the INTERRUPTION HANDLER INTO THE TRAITANT.S, but with a different RETURN ADDRESS(#2) and then 
9. Resumes the execution but to a different process.

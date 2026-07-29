# Operative system's mini kernel 

A Mini kernel with a shared time notion implemented with a simple preemtive scheduler, clock interruption treatment and a screen driver. Realized from the Operative system conception project [course](https://ensimag.grenoble-inp.fr/fr/formation/projet-de-conception-de-syst-egrave-mes-d-exploitation-fondements-4mmpcsef) at Ensimag.

## Setup

A Dockerfile will be added later to the project.

## How to use

Currently a script called `launch.sh` that only works on Fedora (hahaha), I should automatize for debian based machines as well.


In order to compile and execute the process you should launch:

```./launch.sh```

This will:

1. Launch a console executing QEMU with the correct configuration
2. Launch GDB connecting its session with the QEMU open port
3. (Only for Fedora) Launch a vncviewer to see QEMU emulated system. This is not necessary on debian based


After this steps you can continue the execution in gdb and play around with breakpoints and inspect the values of the program to explore what is happening.

## Why to do this project?

First, I would to begin with a quote from Grégory Mounié, professor of Operating Systems at ENSIMAG:


> _"Algorithmics plays a crucial role on a program performance, but is not the only factor. The program's interactions with hardware can change its execution time on a x1000 or x10000 factor. All the interactions with the hardware layer goes through the operating system ...  Therefore, it is mandatory to understand them in order to avoid "programming against one's own field" and being able to comprehend what happens"_ - **Mr Gregory Mounié**



That been said, I took this project again because it forced me to dive into learn **C**, **computer architecture**, got deeper in **programs fundamentals**, and **Operative system theory** in order to understand the project and what I was doing.


By understanding as well the ideas that Mr Mounié points at, I would be able to better analyze concepts as performance and security as the result of the orchestrator role attribued to an OS. For example, understanding problematics such as cache miss issues, or ```syscalls``` and ```ctx_sw``` overhead issues.

### Ensimag's influence

Also, I genuinely believe the ENSIMAG learning program makes you a well-prepared professional. Therefore, I thought by finishing the project, I could learn more than the first time, renforcing and completing my current knowledge on the already mentionned aspects.


## Next steps

This project will have a continuation where aside what is done here. There will be:

1. A more complex scheduler policy
2. Complex relationships between processes and its children processes
3. Communication system between processes
4. Separation between kernel mode and user mode
5. Virtual memory mechanism.
6. Syscalls development.


The continuation is specified on the [PCSEA course](https://ensimag.grenoble-inp.fr/fr/formation/projet-de-conception-de-syst-egrave-me-d-exploitation-approfondissement-4mmpcsea) at Ensimag.

## References
1. For more info about the project: [Ensimag's operation system project](https://ensimag.grenoble-inp.fr/fr/formation/projet-de-conception-de-syst-egrave-mes-d-exploitation-fondements-4mmpcsef)
2. For more info about the interruption configuration: [Programmable Interval Timer](https://wiki.osdev.org/Programmable_Interval_Timer)
3. For more info about the stack architecture: [Stack architecture on x86](https://wiki.osdev.org/Stack)
4. For more info about the stack architecture and calling conventions: [Stack architecture on x86](https://wiki.osdev.org/Stack)
5. For more info about how functions are executed on CPU: [How CPUs Run Functions](https://youtu.be/7YyALikxAlU?si=yEGeB4k6L4uRCAZe)
6. For more information on stack buffer overflow: [Stack buffer overflow](https://en.wikipedia.org/wiki/Stack_buffer_overflow)
7. For more information about context switching: [Context switching](https://wiki.osdev.org/Context_Switching)

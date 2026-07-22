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

First, I want to begin with a look to the answer to this question given by Mr Mounié, a specialized professor on Operative Systems:

>"L’algorithme joue un rôle crucial dans la performance d’un programme, mais ce n’est pas le seul facteur. Les interactions du programme avec le matériel peuvent changer son temps d’exécution de plusieurs ordres de grandeurs, jusqu’à ralentir votre programme d’un facteur 1000 ou 10000. Toutes les interactions avec le matériel passent par le système d’exploitation, y compris les caches du processeur, pour des problèmes de couleurs dans les caches multi-voies. Il faut donc les comprendre pour éviter de “programmer contre son camp”, et être capable de comprendre ce qui se passe." - Gregory Mounié


That been said, I took this project again because it forced me to learn C, computer architecture, program fundamentals, and Operative system theory in order to understand the project and what I was doing (the last one was for fun).

Also, I genuinely believe the ENSIMAG learning program makes you a well-prepared professional. Therefore, I thought by finishing the project, I could learn more than the first time, renforcing and completing my current knowledge on the already mentionned aspects.

By understanding as well the ideas that Mr Mounié points at, I would be able to better analyze concepts as performance. Being conscious of problematics such as cache miss issues, or syscall and ctx_sw overhead issues. As well as the role on performance, security, orchestrator attribued to an Operative System.

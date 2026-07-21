# Operative system's mini kernel 

A Mini kernel with a shared time notion implemented with a simple preemtive scheduler, clock interruption treatment and a screen driver.

## Setup


A Dockerfile will be added later to the project.

## How to use

Currently a script called `launch.sh` that only works onFedora (hahaha), I should automatize for debian based machines as well.


In order to compile and execute the process you should launch:

```./launch.sh```

This will:

1. Launch a console executing QEMU with the correct configuration
2. Launch GDB connecting its session with the QEMU open port
3. (Only for Fedora) Launch a vncviewer to see QEMU emulated system. This is not necessary on debian based


After this steps you can continue the execution in gdb and play around with breakpoints and inspect the values of the program to explore what is happening.

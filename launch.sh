#bin/sh
konsole -e bash  -c  "cd src_de_base/ && make debug; exec bash" & 
gdb -ex 'target remote:1234' -ex  'b kernel_start'  src_de_base/kernel.bin 


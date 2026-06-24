#bin/sh
#If any command fails, we stop script execution
set -e

PROJECT_FOLDER=src_de_base/

cd $PROJECT_FOLDER
# Compile the project latest version
make


konsole -e bash  -c  "make debug; exec bash"  &
konsole -e bash  -c  "sleep 0.5"
vncviewer localhost:5900 &
gdb -ex 'target remote:1234' -ex  'b kernel_start'  kernel.bin
ps axf | grep "make debug" | grep -v grep | awk '{print "kill -9 " $1}' | sh

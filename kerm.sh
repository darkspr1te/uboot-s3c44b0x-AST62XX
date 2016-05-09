#!/usr/bin/kermit +
  if defined \%1 echo "Argument 1: \%1"
  if defined \%2 echo "Argument 2: \%2"
  if defined \%3 echo "Argument 3: \%3"
  if defined \%4 echo "etc..."
  
  
 set line /dev/ttyUSB0
set speed 115200
set serial 8n1

# General C-Kermit settings.  These probably don't need to change.
set flow-control none
set file type bin
set carrier-watch off
set prefixing all
set modem none

#echo "Prepared to boot new kernel.  Reset the board now."
# This is the string that my board outputs to allow the user to
# gain access to the U-Boot console.  Change this to suit your
# setup.
#input 60 "Hit any key to stop autoboot"
# If your board wants you to press a different key to get to
# U-Boot, edit this line.
output " "
input 5 "iGeek_44b0x=>"
# Here, 0x800000 is the memory address into which the kernel
# should be loaded.
lineout "loadb 0xc000000"
send \%1
#input 5 "iGeek_44b0x=>"
#lineout "bootm 0xc000000"
lineout "erase 1:0-2"
lineout "cp.b 0xc000000 0x0 0x28000"

#we use go 0xc000000 instead as bootm looks for kernel magic
##lineout "go 0xc000000"
connect
#exit

#!/bin/bash

##########################################################
#    Script to generate ubifs filesystem image.     #
##########################################################

##### ubinize configuration file
config_file=rootfs_ubinize.cfg

##### Function to check result of the command
check_result() {
if [ $? -ne 0 ]
then
    echo "FAILED"
else
    echo "SUCCESSFUL"
fi
}

######  Function to check whether an application exists
check_program() {
for cmd in "$@"
do
        which ${cmd} > /dev/null 2>&1
        if [ $? -ne 0 ]
        then
                echo
                echo "Cannot find command \"${cmd}\""
                echo
                exit 1
        fi
done
}

if [ $# -ne 5 ]
then
    echo
    echo 'Usage: create-ubifs.sh [page_size_in_bytes] [pages_per_block] [partition_size_in_bytes] [blocks_per_device] [path_to_rootfs]'
    echo
    exit
fi

page_size_in_bytes=$1
echo "Page size                                                 [$page_size_in_bytes]bytes."
pages_per_block=$2
echo "Pages per block                                           [$pages_per_block]"
partition_size_in_bytes=$3
echo "File-system partition size                                [$partition_size_in_bytes]bytes."
blocks_per_device=$4
echo "Blocks per device                                         [$blocks_per_device]"
path_to_rootfs=$5

# wear_level_reserved_blocks is 1% of total blcoks per device
wear_level_reserved_blocks=`expr $blocks_per_device / 100`
echo "Reserved blocks for wear level                            [$wear_level_reserved_blocks]"

#logical_erase_block_size is physical erase block size minus 2 pages for UBI
logical_pages_per_block=`expr $pages_per_block - 1`
logical_erase_block_size=`expr $page_size_in_bytes \* $logical_pages_per_block`
echo "Logical erase block size                                  [$logical_erase_block_size]bytes."

#Block size = page_size * pages_per_block
block_size=`expr $page_size_in_bytes \* $pages_per_block`
echo "Block size                                                [$block_size]bytes."

#physical blocks on a partition = partition size / block size
partition_physical_blocks=`expr $partition_size_in_bytes / $block_size`
echo "Physical blocks in a partition                            [$partition_physical_blocks]"

#Logical blocks on a partition = physical blocks on a partitiion - reserved for wear level
patition_logical_blocks=`expr $partition_physical_blocks - $wear_level_reserved_blocks`
echo "Logical blocks in a partition                             [$patition_logical_blocks]"

#File-system volume = Logical blocks in a partition * Logical erase block size
fs_vol_size=`expr $patition_logical_blocks \* $logical_erase_block_size`
echo "File-system volume                                        [$fs_vol_size]bytes."

page_size_in_bytes_slp=`expr $page_size_in_bytes / 2`
echo "SubData offset                            		[$page_size_in_bytes_slp]"
echo
echo "Generating configuration file..."
echo "[rootfs-volume]"  > $config_file
echo "mode=ubi" >> $config_file
echo "image=rootfs_ubifs.img" >> $config_file
echo "vol_id=0" >> $config_file
echo "vol_size=$fs_vol_size" >> $config_file
echo "vol_type=dynamic" >> $config_file
echo "vol_name=system" >> $config_file
echo

# Note: Check necessary program for installation
#echo -n "Checking necessary program for installation......"
#check_program mkfs.ubifs ubinize
#echo "Done"

#Generate ubifs image
echo -n "Generating ubifs..."
mkfs.ubifs -x lzo -m $page_size_in_bytes -e $logical_erase_block_size -c $patition_logical_blocks -o rootfs_ubifs.img -d $path_to_rootfs
check_result
echo -n "Generating ubi image out of the ubifs..."
ubinize -o ubi.img -m $page_size_in_bytes -p $block_size -s $page_size_in_bytes_slp $config_file -v
check_result

rm -f rootfs_ubifs.img
rm -f $config_file

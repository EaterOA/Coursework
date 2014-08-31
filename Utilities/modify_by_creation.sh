#!/bin/bash

# Usage: modify_by_creation.sh
#
# A script that touches all regular files in an NTFS mounted directory in the
# order of creation time. At least in Nautilus, there is no option to sort by
# creation, so this is more or less a hack to force the modification time to be
# in the same order as creation. If you actually care about modification time,
# this script might not be useful for you.
#
# This script was tested on Ubuntu 14.04 with ntfs-3g 2013.1.13AR.1 external
# FUSE 29. It works for me, I make no guarantees it will work for you.

NL=$'\n'
IFS=${NL}

echo "Getting list of regular files in current directory..."
names=`find . -maxdepth 1 -type f`
total=`echo "${names}" | wc -l`

echo "Checking requirements..."
if [ -z "$names" ]; then
    echo "No regular files found." >&2
    exit 1
fi
command -v getfattr >/dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "getfattr (attr) must be installed" >&2
    exit 1
fi
echo "${names}" | head -n 1 | xargs getfattr -h -e hex -n system.ntfs_times_be >/dev/null || exit 1

echo "Retrieving NTFS creation times..."
list=""
for i in $names; do
    list="${list}`getfattr -h -e hex -n system.ntfs_times_be "${i}" | grep -Eo "0x[0-9a-f]{16}" | xargs printf "%d"`"
    list="${list} ${i}${NL}"
done

echo "Sorting by creation times..."
list=`echo "${list}" | sort -V -k 1,1`

echo "Begin modification time adjustment:"
counter=1
for i in $list; do
    name=`echo "${i}" | cut --complement -f1 -d " "`
    echo "${counter}/${total}: ${name}..."
    sleep 1
    touch "${name}"
    let counter+=1
done

echo "Done"

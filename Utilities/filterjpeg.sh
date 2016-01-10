#!/bin/bash

# filterjpeg.sh 
#
# Poor man's way of filtering out jpeg files from a list of files. Can be used
# for e.g. extracting image files from browser cache.

err_exit() {
    cat >&2 <<EOF
Usage: filterjpeg.sh

Reads a list of files from stdin and prints only those that are jpeg files
EOF
    exit 1
}

while read f; do
    # guess based on the jpeg format string; 32 bits of reliability

    format="$(dd status=none skip=6 count=4 bs=1 if="$f")"
    if [ "$format" == "Exif" -o "$format" == "JFIF" ]; then
        echo "$f"
    fi
done

#!/bin/bash

# egest.sh
#
# The companion script to ingest.sh. It takes a bunch of files, concatenates
# them, and attempts to decrypt it into a tarball which is then decompressed.
#
# For usage, see: ./egest.sh -h
# (or just look at the text right below)

err_exit() {
    cat >&2 <<EOF
Usage: ./egest.sh [-d] <path to file or directory>

Options:
    -d      delete all archive files if recovery succeeds
EOF
    exit 1
}

delete=0
while getopts ":d" opt; do
    case $opt in
        d)
            delete=1
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

if (($# == 0)); then
    echo "ERROR: You need at least one argument" >&2
    exit 1
fi

for f in "$@"; do
    if [ ! -e "$f" ]; then
        echo "ERROR: $1 cannot be found" >&2
    fi
done

cat "$@" |
gpg |
tar -xz

if [ $? -eq 0 -a $delete -eq 1 ]; then
    rm -- "$@"
fi

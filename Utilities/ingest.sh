#!/bin/bash

# ingest.sh
#
# This script takes a path to a file or directory and does 3 things:
# 1) tars/gzips it
# 2) asks the user for a key and encrypts the tarball with AES-256
# 3) splits the encrypted tarball up into chunks
# The point of this is to be able to conveniently perform client-side
# encryption on a file or directory before transfering it or backing it up in
# some insecure storage (e.g. cloud storage services). The companion script
# egest.sh performs the opposite operations to recover the original data.
#
# For usage, see: ./ingest.sh -h
# (or just look at the text right below)

err_exit() {
    cat >&2 <<EOF
Usage: ./ingest.sh [-ocrneaC] <path to file or directory>

Options:
    -o      specify output name prefix (default "out")
    -c      specify chunk size to split on (default "1024m"), see split(1)
    -r      specify a gpg recipient instead of using a symmetric key
    -n      tell gpg not to sign the encrypted tarball
    -a      tell gpg to use --armor
    -e      don't split the encrypted tarball
    -C      specify a directory for tar to change into first
EOF
    exit 1
}

prefix="out"
chunk="1024m"
sign="--sign"
encryption="--symmetric --cipher-algo"
encryption_arg="AES256"
split=1
armor=""
dir="."
dirchange=0
while getopts ":o:c:r:neaC:" opt; do
    case $opt in
        o)
            prefix="$OPTARG"
            ;;
        c)
            chunk="$OPTARG"
            ;;
        r)
            encryption="--encrypt --recipient"
            encryption_arg="$OPTARG"
            ;;
        n)
            sign=""
            ;;
        e)
            split=0
            ;;
        a)
            armor="--armor"
            ;;
        C)
            dir="$OPTARG"
            dirchange=1
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

if (($# != 1)); then
    err_exit
fi
testpath="$1"
if [ $dirchange == 1 ]; then
    testpath="$dir/$1"
fi
if [ ! -e "$testpath" ]; then
    echo "ERROR: $testpath cannot be found" >&2
    exit 1
fi

if (($split == 1)); then
    tar -cz -C "$dir" -- "$1" |
    gpg $sign $encryption "$encryption_arg" $armor --force-mdc |
    split -db "$chunk" - "${prefix}.part"
else
    tar -cz -C "$dir" -- "$1" |
    gpg $sign $encryption "$encryption_arg" $armor --force-mdc --output "$prefix"
fi

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
Usage: ./ingest.sh [-ocrne] <path to file or directory>

Options:
    -o      specify output name prefix (default "out")
    -c      specify chunk size to split on (default "1024m"), see split(1)
    -r      specify a gpg recipient instead of using a symmetric key
    -n      tell gpg not to sign the encrypted tarball
    -e      don't split the encrypted tarball
EOF
    exit 1
}

prefix="out"
chunk="1024m"
sign="--sign"
encryption="--symmetric --cipher-algo AES256"
split=1
while getopts ":o:c:r:ne" opt; do
    case $opt in
        o)
            prefix="$OPTARG"
            ;;
        c)
            chunk="$OPTARG"
            ;;
        r)
            encryption="--encrypt --recipient $OPTARG"
            ;;
        n)
            sign=""
            ;;
        e)
            split=0
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
if [ ! -e "$1" ]; then
    echo "ERROR: $1 cannot be found" >&2
    exit 1
fi

if (($split == 1)); then
    tar -cz -- "$1" |
    gpg $sign $encryption --force-mdc --armor |
    split -db "$chunk" - "${prefix}.part"
else
    tar -cz -- "$1" |
    gpg $sign $encryption --force-mdc --armor --output "$prefix"
fi

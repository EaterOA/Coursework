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
Usage: ./ingest.sh [-osn] <path to file or directory>

Options:
    -o      specify output name prefix (default "out")
    -s      specify chunk size to split on (default "1024m"), see split(1)
    -n      tell gpg not to sign the encrypted output
EOF
    exit 1
}

prefix="out"
chunk="1024m"
sign="--sign"
while getopts ":o:c:n" opt; do
    case $opt in
        o)
            prefix="$OPTARG"
            ;;
        c)
            chunk="$OPTARG"
            ;;
        n)
            sign=""
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

tar -cz -- "$1" |
gpg "$sign" --force-mdc --cipher-algo AES256 --symmetric --armor |
split -db "$chunk" - "${prefix}.part"

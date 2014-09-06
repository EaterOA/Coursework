#!/bin/bash

err_exit() {
    cat >&2 <<EOF
Usage: ./ingest.sh [-osn] <path to file or directory>

Options:
    -o: Specify output name prefix (default "out")
    -s: Specify chunk size to split on (default "1024m"),
        see split(1) for parameters
    -n: Tell gpg not to sign the encrypted output
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

#!/bin/bash

# randstr.sh
#
# Uses /dev/urandom to generates a string of random printable characters, with
# a default length of 16. Useful for generating random passwords.

err_exit() {
    cat >&2 <<EOF
Usage: randstr.sh [-an] [-c number_of_characters]

Options:
    -a      generate alpha characters
    -n      generate numeric characters
    -c      specify the number of characters to generate (default 16)
EOF
    exit 1
}

numchars="16"
alpha=""
numeric=""
printable="[:print:]"
while getopts ":anc:" opt; do
    case $opt in
        n)
            printable=""
            numeric="[:digit:]"
            ;;
        a)
            printable=""
            alpha="[:alpha:]"
            ;;
        c)
            numchars="$OPTARG"
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

tr -cd "${printable}${alpha}${numeric}" < /dev/urandom | head -c "$numchars" ; echo

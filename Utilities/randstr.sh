#!/bin/bash

# randstr.sh
#
# Uses /dev/urandom to generate a string of random printable characters, with
# a default length of 16. Useful for generating random passwords.

err_exit() {
    cat >&2 <<EOF
Usage: randstr.sh [-an] [number_of_characters]

Randstr provides a simple way to construct a random sequence of characters.
Perfect for passwords. The number of characters can be adjusted with an integer
argument. If unspecified, the default is 16.

Options:
    -a      generate alpha characters
    -n      generate numeric characters
    -s      generate special characters
EOF
    exit 1
}

numchars="16"
filtered=false
fields="[:alpha:][:digit:]"
while getopts ":ans" opt; do
    if [ "$filtered" = false ]; then
        fields=""
        filtered=true
    fi
    case $opt in
        n)
            fields="${fields}[:digit:]"
            ;;
        a)
            fields="${fields}[:alpha:]"
            ;;
        s)
            fields="${fields}[:punct:]"
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

# numchars argument must be an integer
if [ ! -z "$1" ]; then
    re='^[0-9]+$'
    if ! [[ "$1" =~ $re ]]; then
        err_exit
    fi
    numchars="$1"
fi

tr -cd "$fields" < /dev/urandom | head -c "$numchars" ; echo

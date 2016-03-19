#!/bin/bash

# getip.sh
#
# This script queries an external website to figure out the public ip of the
# current host
#
# For usage, see: ./getip.sh -h

err_exit() {
    cat >&2 <<EOF
Usage: getip.sh [-t <timeout>]

Options:
    -t      specify a timeout in # of seconds for requests
EOF
    exit 1
}

site='ipv4bot.whatismyipaddress.com'
timeout=0
while getopts ":t:" opt; do
    case $opt in
        t)
            timeout=$OPTARG
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

timeout -- "$timeout" /bin/sh -c "wget -qO - '$site' | egrep -o '([0-9]+\.){3}[0-9]+'"
if (($? == 124)); then
    echo "Request timed out for $site at $timeout second(s)" >&2
    exit 1
fi

#!/bin/bash

# Usage: dns.sh <hostname> [hostname]...
# This script will check every hostname argument provided with the host program,
# giving you a simple answer of Yay or Nay for whether that host is reachable.
# If an A record is found, the IP is also printed. Outputs are colored using
# ANSI color codes.
# If used with the watch program, make sure to set watch's -c flag.

BLACK="\033[0;30m"
RED="\033[0;31m"
GREEN="\033[0;32m"
ORANGE="\033[0;33m"
BLUE="\033[0;34m"
PURPLE="\033[0;35m"
CYAN="\033[0;36m"
LGRAY="\033[0;37m"
GRAY="\033[1;30m"
LRED="\033[1;31m"
LGREEN="\033[1;32m"
YELLOW="\033[1;33m"
LBLUE="\033[1;34m"
LPURPLE="\033[1;35m"
LCYAN="\033[1;36m"
WHITE="\033[1;37m"
NC="\033[0m"

for h in "$@"; do

    # No feeding host with options please
    if [[ $h == -* ]]; then
        continue
    fi

    # Testing the hostname
    echo -n "Checking ${h}... "
    output=`host "$h"`
    echo "$output" |
    grep "not found" >/dev/null

    # If not found
    if [ $? -eq 0 ]; then
        echo -en "${LRED}Nay";
    # If found
    else
        echo -en "${LGREEN}Yay ";
        echo "$output" |
        grep -ZEom 1 "(([0-9]{1,3})\.){3}[0-9]{1,3}" |
        xargs echo -n
    fi

    echo -e "${NC}"
done

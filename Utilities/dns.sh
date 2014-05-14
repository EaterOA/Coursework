#!/bin/bash

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
 
nslookup linux.ucla.edu 131.179.104.1   \
  | grep '^Address:.*40$' >/dev/null    \
  && echo -e "${LGREEN}yay${NC}"        \
  || echo -e "${LRED}nay${NC}"

nslookup 131.179.104.40 131.179.104.1   \
  | grep 'name = hiroshima' >/dev/null  \
  && echo -e "${LGREEN}yay${NC}"        \
  || echo -e "${LRED}nay${NC}"

nslookup linux.ucla.edu 131.179.104.10  \
  | grep '^Address:.*40$' >/dev/null    \
  && echo -e "${LGREEN}yay${NC}"        \
  || echo -e "${LRED}nay${NC}"

nslookup 131.179.104.40 131.179.104.10  \
  | grep 'name = hiroshima' >/dev/null  \
  && echo -e "${LGREEN}yay${NC}"        \
  || echo -e "${LRED}nay${NC}"
 

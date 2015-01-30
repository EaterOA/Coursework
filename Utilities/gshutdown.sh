#!/bin/bash

# gshutdown.sh
#
# This script allows the user to poweroff their computer without typing in the
# root password.
# For now, it only works for Ubuntu 14.04+ (or any distro with using
# systemd-logind) under the Unity or Gnome desktop environments
#
# For usage, see: ./gshutdown -h

err_exit() {
    cat >&2 <<EOF
Usage: gshutdown.sh [-n]

Options:
    -n      do not show gnome prompt
EOF
    exit 1
}

noprompt=0
while getopts ":n" opt; do
    case $opt in
        n)
            noprompt=1
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

if [ $noprompt -eq 0 ]; then
    gnome-session-quit --power-off
else
    dbus-send --system --print-reply --dest=org.freedesktop.login1 /org/freedesktop/login1 "org.freedesktop.login1.Manager.PowerOff" boolean:true
fi

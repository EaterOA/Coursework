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
Usage: gshutdown.sh [-nr]

Options:
    -n      do not show gnome prompt
    -r      reboot
EOF
    exit 1
}

ypflag="--power-off"
npflag="PowerOff"
noprompt=0
while getopts ":nr" opt; do
    case $opt in
        n)
            noprompt=1
            ;;
        r)
            ypflag="--reboot"
            npflag="Reboot"
            ;;
        \?)
            err_exit
            ;;
    esac
done
shift $(($OPTIND-1))

if [ $noprompt -eq 0 ]; then
    gnome-session-quit $ypflag
else
    dbus-send --system --print-reply --dest=org.freedesktop.login1 /org/freedesktop/login1 "org.freedesktop.login1.Manager.$npflag" boolean:true
fi

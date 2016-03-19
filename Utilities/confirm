#!/bin/bash

if [ -z "$1" ]; then
    question_text="Are you sure? [y/N] "
else
    question_text="$1 [y/N] "
fi

read -r -p "$question_text" response
[ "${response,,}" = "y" ]

#!/bin/bash

# Delete all carriage returns
sed -E "s/\r//g" -i $@

# Remove all trailing spaces
sed -E "s/[[:space:]]+$//g" -i $@

# Make sure newline is present at eof
sed '$a\' -i $@

# Replace tabs with four spaces
sed -E "s/\t/    /g" -i $@

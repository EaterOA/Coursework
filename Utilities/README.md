Utilities
==========

A (miserable) collection of utility scripts for use in extraordinary situations (i.e. almost never).

- dns.sh: Tests DNS resolution. Useful when you suddenly feel like mucking with your bind configs and need to monitor your domain and/or connectivity to the Internet but don't want to read `host`'s ugly output and prefer to sacrifice useful information for this script's pretty colors.
- tidyfy.sh: Cleans up source files by removing those pesky carriage returns, trailing spaces, non-newline-eofs, and tabs.
- modify_by_creation.sh: Touches regular files in an NTFS mounted directory in the order of creation times. Sometimes useful for dual-booting noobs like me who want to sort stuff by creation time.
- ingest.sh: Encrypts a target file or folder. More specifically, it creates a gzipped tarball out of the target, asks for a passphrase for gpg to AES-256-encrypt the tarball with, and splits the tarball into chunks. Cause I don't trust Dropbox.
- egest.sh: Companion script to ingest.sh for recovery of the original file(s)
- getip.sh: A very slow script that looks for your public ip
- gshutdown.sh: Invokes poweroff without root password, just like clicking the shutdown button from your favorite desktop environment*
- randstr.sh: Generates a string of random characters.

*Unless your favorite desktop environment is not my favorite desktop
environment.

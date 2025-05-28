# Hashes a file, ignoring all whitespace and comments. Use for
# verifying that code was correctly typed.
cpp -dD -P -fpreprocessed <file.cpp> | tr -d '[:space:]'| md5sum |cut -c-6
# Add current directory to PATH
export PATH="$PATH:$(pwd)"
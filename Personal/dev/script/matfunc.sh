#!/bin/bash

# Check funciton name

if [ -z "$1" ]; then 
    echo "Provide a function name"
    exit 1
fi

FUNCTION_NAME=$1

# create function file
cat <<EOL > ${FUNCTION_NAME}.m
function ${FUNCTION_NAME}()
    disp("hello from matlab");
end
EOL

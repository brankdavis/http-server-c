#!/bin/bash

# Check if argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <number_of_times>"
  exit 1
fi

# Validate that the argument is a positive integer
if ! [[ "$1" =~ ^[0-9]+$ ]] || [ "$1" -le 0 ]; then
  echo "Error: Argument must be a positive integer."
  exit 1
fi

# Loop and run the command n times in background
for ((i = 1; i <= $1; i++)); do
  (sleep 3 && printf "GET / HTTP/1.1\r\n\r\n") | nc localhost 4221 &
done

echo "Started $1 background requests."


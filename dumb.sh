#!/bin/bash

# Specify the directory containing the files
DIRECTORY="src"

# Output file
OUTPUT_FILE="file.txt"

# Clear the output file if it exists
>"$OUTPUT_FILE"

# Iterate through all .cpp files in the specified directory
for FILE in "$DIRECTORY"/*.cpp; do
  # Check if it's a file (not a directory with .cpp in name)
  if [ -f "$FILE" ]; then
    # Append the content of the file to output file
    cat "$FILE" >>"$OUTPUT_FILE"
  fi
done

echo "All .cpp files have been concatenated into $OUTPUT_FILE."

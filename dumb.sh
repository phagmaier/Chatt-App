#!/bin/bash

# Specify the directory containing the files
DIRECTORY="src"

# Output file
OUTPUT_FILE="file.txt"

# Clear the output file if it exists
>"$OUTPUT_FILE"

# Iterate through all files in the specified directory
for FILE in "$DIRECTORY"/*; do
  # Check if it's a file and not the excluded file
  if [ -f "$FILE" ] && [ "$(basename "$FILE")" != "raygui.h" ]; then
    # Append the content of the file to output file
    cat "$FILE" >>"$OUTPUT_FILE"
  fi
done

echo "All text has been concatenated into $OUTPUT_FILE, excluding raygui.h."

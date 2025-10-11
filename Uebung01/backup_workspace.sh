#!/usr/bin/env bash

# default Paths
INPUT_DIR="$HOME/Workspace"
OUTPUT_DIR="$HOME/Backups"
INTERMEDIAT_DIR="$1"
ZIP_NAME="$2"

# Conventional Usage Function
usage() {
  echo "Usage: $0 <tmp location> <Backup Name> [-i input_directory] [-o output_directory]"
  echo "  -i    Specify the input directory (default: $INPUT_DIR)"
  echo "  -o    Specify the output directory (default: $OUTPUT_DIR)"
  exit 2
}

# Function to get absolute path
abspath() {
  (cd "$1" >/dev/null 2>&1 && pwd) || return 1
}

if [[ $# -lt 2 ]]; then
  echo "Error: Not enough arguments provided."
  usage
fi

# get Optional Param
shift 2
while getopts ":i:o:" opt; do
  case $opt in
  i)
    INPUT_DIR="$OPTARG"
    echo "Input directory set to: $INPUT_DIR"
    ;;
  o)
    OUTPUT_DIR="$OPTARG"
    echo "Output directory set to: $OUTPUT_DIR"
    ;;
  :)
    # This triggers when a required argument is missing
    echo "Error: Option -$OPTARG requires an argument." >&2
    usage
    ;;
  \?)
    echo "Error: Invalid option -$OPTARG" >&2
    usage
    ;;
  esac
done

# -----------------------------------------
# ----------- Check Directories -----------
# -----------------------------------------

# Check if input directory exists
if [[ ! -d "$INPUT_DIR" ]]; then
  echo "Error: Input directory '$INPUT_DIR' does not exist."
  usage
fi
INPUT_DIR="$(abspath "$INPUT_DIR")"

# Check and create output directory if it doesn't exist
if [[ ! -d "$OUTPUT_DIR" ]]; then
  echo "Output directory '$OUTPUT_DIR' does not exist. Now creating it."
  mkdir -p "$OUTPUT_DIR"
  if [[ $? -ne 0 ]]; then
    echo "Error: Failed to create temporary location '$OUTPUT_DIR'."
    exit 2
  fi
  echo "Temporary location '$OUTPUT_DIR' created successfully."
fi
OUTPUT_DIR="$(abspath "$OUTPUT_DIR")"

# Check and create temporary location if it doesn't exist
if [[ ! -d "$INTERMEDIAT_DIR" ]]; then
  echo "Temporary location '$INTERMEDIAT_DIR' does not exist. Now creating it."
  mkdir -p "$INTERMEDIAT_DIR"
  if [[ $? -ne 0 ]]; then
    echo "Error: Failed to create temporary location '$INTERMEDIAT_DIR'."
    exit 2
  fi
  echo "Temporary location '$INTERMEDIAT_DIR' created successfully."
fi
INTERMEDIAT_DIR="$(abspath "$INTERMEDIAT_DIR")"

# -----------------------------------------
# ----------- create Backup ---------------
# -----------------------------------------

PARENT_DIR="$(dirname "$INPUT_DIR")"
DIR_NAME="$(basename "$INPUT_DIR")"

ZIP_NAME="${ZIP_NAME}_$(date +%Y%m%d_%H%M%S)"

(
  cd "$PARENT_DIR" && zip -r "$INTERMEDIAT_DIR/$ZIP_NAME.zip" "$DIR_NAME"
)

if [[ $? -eq 0 ]]; then
  echo "ZIP created successfully"
else
  echo "ZIP creation failed!"
  exit 2
fi

# -----------------------------------------
# ----------- Moving Backup ---------------
# -----------------------------------------

if [[ "$INTERMEDIAT_DIR" != "$OUTPUT_DIR" ]]; then
  mv -- "$INTERMEDIAT_DIR/$ZIP_NAME.zip" "$OUTPUT_DIR/"
  if [[ $? -eq 0 ]]; then
    echo "Backup moved successfully to $OUTPUT_DIR"
  else
    echo "Failed to move backup to $OUTPUT_DIR"
    exit 2
  fi
fi

echo "Backup process completed successfully."

#!/bin/bash

set -e  # Exit immediately if any command fails
set -x  # Enable command tracing (debug output)

OUTPUT_DIR="output"
DIST_DIR="dist"

echo "Creating distribution directory: $DIST_DIR"
mkdir -p "$DIST_DIR"

MANIFEST_FILE="$DIST_DIR/manifest.json"
echo "[" > "$MANIFEST_FILE"

first=1

echo "Iterating over subdirectories in $OUTPUT_DIR..."
for dir in "$OUTPUT_DIR"/*; do
  if [ ! -d "$dir" ]; then
    echo "Skipping non-directory: $dir"
    continue
  fi

  platform=$(basename "$dir")
  echo "Processing platform: $platform"

  case "$platform" in
    esp32dev)
      chip="ESP32"
      ;;
    esp32c3dev)
      chip="ESP32-C3"
      ;;
    esp32s3dev)
      chip="ESP32-S3"
      ;;
    *)
      chip="ESP32"
      echo "Unknown platform: $platform, defaulting chip to ESP32"
      ;;
  esac

  firmware_path="$dir/firmware.bin"
  if [ ! -f "$firmware_path" ]; then
    echo "Warning: Firmware not found for $platform at $firmware_path"
    continue
  fi

  [ $first -eq 0 ] && echo "," >> "$MANIFEST_FILE" || first=0

  echo "Adding entry for $platform to manifest..."
  cat <<EOF >> "$MANIFEST_FILE"
{
  "name": "$platform",
  "chipFamily": "$chip",
  "parts": [
    {
      "path": "$platform/firmware.bin",
      "offset": 0
    }
  ]
}
EOF

  echo "Copying firmware to $DIST_DIR/$platform..."
  mkdir -p "$DIST_DIR/$platform"
  cp "$firmware_path" "$DIST_DIR/$platform/"
done

echo "]" >> "$MANIFEST_FILE"
echo "Manifest written to $MANIFEST_FILE"
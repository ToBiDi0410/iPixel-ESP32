#!/bin/bash

set -e

OUTPUT_DIR="output"
DIST_DIR="dist"

mkdir -p "$DIST_DIR"

MANIFEST_FILE="$DIST_DIR/manifest.json"
echo "[" > "$MANIFEST_FILE"

first=1
for dir in "$OUTPUT_DIR"/*; do
  platform=$(basename "$dir")

  case "$platform" in
    esp32dev) chip="ESP32" ;;
    esp32c3dev) chip="ESP32-C3" ;;
    esp32s3dev) chip="ESP32-S3" ;;
    *) chip="ESP32" ;;
  esac

  [ $first -eq 0 ] && echo "," >> "$MANIFEST_FILE" || first=0

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
  mkdir -p "$DIST_DIR/$platform"
  cp "$dir/firmware.bin" "$DIST_DIR/$platform/"
done

echo "]" >> "$MANIFEST_FILE"
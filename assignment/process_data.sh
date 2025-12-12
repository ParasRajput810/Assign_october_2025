#!/usr/bin/env bash
echo "=== Solver Output Processing ==="

if [ ! -f solver_output.tar.gz ]; then
echo "Error: 'solver_output.tar.gz' not found in $(pwd)" >&2
exit 1
fi

echo "Found archive: solver_output.tar.gz"

tar -xzf solver_output.tar.gz

echo "Extraction done"


echo "Renaming extracted file"
original_solver_file=$(tar -tf solver_output.tar.gz)
tar -xzf solver_output.tar.gz "$original_solver_file"

mv "$original_solver_file" solver_output.txt

echo "Renamed to solver_output.txt"
echo "Cleaning the data..."

grep -E '^[[:space:]]*[0-9]+' solver_output.txt \
  | sed -E 's/[[:space:]]*\|[[:space:]]*/,/g; s/^[[:space:]]+//; s/[[:space:]]+$//' \
  > solver_data.csv

echo "CSV file created: solver_data.csv"

echo "Making csv from txt..."

grep -E '^[[:space:]]*[0-9]+' solver_output.txt \
  | sed -E 's/[[:space:]]*\|[[:space:]]*/,/g; s/^[[:space:]]+//; s/[[:space:]]+$//' \
  > solver_data.csv

echo "Packing files into tar..."

tar -czf processed_outputs.tar.gz solver_output.txt solver_data.csv

echo "Done with processed_outputs.tar.gz"
echo "Cleaning is completed!"
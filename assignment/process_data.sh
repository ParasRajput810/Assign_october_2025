#!/usr/bin/env bash
echo "=== Solver Output Processing ==="

if [ ! -f solver_output.tar.gz ]; then
echo "Error: 'solver_output.tar.gz' not found in $(pwd)" >&2
exit 1
fi

echo "Found archive: solver_output.tar.gz"

tar -xzf solver_output.tar.gz

echo "Extraction done"

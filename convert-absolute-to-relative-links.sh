#!/bin/bash

# Loop through all provided directories
for dir in "$@"; do
    # Find and convert absolute symbolic links to relative
    find "$dir" -type l | while read link; do
        target=$(readlink "$link")
        if [[ "$target" = /* ]]; then
            link_dir=$(dirname "$link")
            relative_target=$(realpath --relative-to="$link_dir" "$target")
            ln -sf "$relative_target" "$link"
        fi
    done
done

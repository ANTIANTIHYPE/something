#!/bin/bash

pull() {
    local path="$1"
    
    cd "$path" || {
        echo "Error: Failed to enter directory: $path" >&2
        return 1
    }
    
    if git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
        if git pull origin HEAD; then
            echo "Successfully pulled in repository: $path"
        fi
    fi
}

find ~ -type d -name ".git" | while IFS= read -r gitdir; do
    path="$(dirname "$gitdir")"
    pull "$path" || {
        echo "Error occurred while processing: $path" >&2
    }
done

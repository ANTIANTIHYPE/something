#!/bin/bash

# This script pulls updates from the remote repository for every local Git repository found on the computer

pull() {
    local path="\$1"
    echo "Pulling in repository: $path"
    cd "$path" || {
        echo "Failed to enter directory: $path"
        return
    }
    if git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
        git pull origin HEAD || {
            echo "Failed to pull in repository: $path"
        }
    fi
}

find ~ -type d -name ".git" | while read -r gitdir; do
    path="$(dirname "$gitdir")"
    pull "$path"
done

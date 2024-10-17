for submodule in linux vm1 vm2; do
    cd $submodule
    git fetch origin                                   # Fetch the latest changes from the remote
    if git show-ref --quiet refs/heads/dev/upg/kernel_absorb; then
        git checkout dev/upg/kernel_absorb           # If branch exists, switch to it
    else
        git checkout -b dev/upg/kernel_absorb origin/dev/upg/kernel_absorb  # Create and switch to it
    fi
    cd ..
done
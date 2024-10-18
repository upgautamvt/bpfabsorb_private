# Define the branch name
branch_name="dev/upg/kernel_absorb"

# Loop through each submodule
for submodule in linux vm1 vm2; do
    cd $submodule || exit  # Change directory to the submodule
    git fetch origin  # Fetch the latest changes from the remote
    if git show-ref --quiet refs/heads/$branch_name; then
        git checkout $branch_name  # If branch exists, switch to it
    else
        git checkout -b $branch_name origin/$branch_name  # Create and switch to it
    fi
    cd ..  # Return to the main repository directory
done

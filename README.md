# Fully Made Version of DKAN 
This is a version of DKAN that has already been "made" with drush make. It includes Drupal core as well as the DKAN installation profile which can be found in "profiles/dkan".

To install, follow the same directions as Drupal core: https://www.drupal.org/documentation/install

See the main DKAN repository for further instructions, support, and community: http://github.com/GetDKAN/dkan

# DKAN on Pantheon

This is a fork based on [Pantheon DROPs](https://github.com/pantheon-systems/drops-7)

## How to update Drupal Drop from GetDKAN Git Repository

Just for the first time, add the GetDKAN base drop repo as a remote
```bash
git remote add getdkan https://github.com/GetDKAN/dkan-drops-7.git
```

Any time you want to integrate their changes into this repo

```bash
# Make a branch so we can test if their work pass our testing
git checkout -b updating_from_getdkan_drops
# Pull their master into your branch (solve conflicts if any)
git merge getdkan/master -X theirs
# Push changes and wait for travis to run the build on the 'updating_from_getdkan_drops' branch.  
git push origin updating_from_getdkan_drops
```

Fix any issues with the build (if any) pushing commits. When everything is ok squash all your fix commits into one. Then:

```bash
# Checkout master
git checkout master
# Rebase changes from your branch
git rebase updating_from_getdkan_drops
# Push
git push origin master
# Delete integration branch
git push origin :updating_from_getdkan_drops
```

## How to update dkan profile

```bash
# Make a branch so we can test the travis build sep
git checkout -b rebuilding_dkan_profile
# Run dkan update script
cd scripts
./rebuild-dkan.sh
# Add, Commit, Push and check the travis build for the 'rebuilding_dkan_profile' branch
git add ../profiles/dkan -A
git commit -m "Rebuilding dkan"
git push origin rebuilding_dkan_profile
```

Fix any issues with the build (if any) pushing commits. When everything is ok squash all your fix commits into one. Then:

```bash
# Checkout master
git checkout master
# Rebase changes from your branch
git rebase rebuilding_dkan_profile
# Push
git push origin master
# Delete integration branch
git push origin :rebuilding_dkan_profile
```


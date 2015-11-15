# bash shell script to set environment variables

# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Benchpress root directory.
if [ -d "doc" ] && [ -d "src" ] && [ -d "pygments" ] && [ -d "testing" ] && [ -d "util" ]
   then
      echo "Updating PATH to include $PWD"
      export PATH="$PWD/util/build/":"$PATH"
   else
      echo "Error: setenv must be sourced from within the Benchpress root directory"
fi

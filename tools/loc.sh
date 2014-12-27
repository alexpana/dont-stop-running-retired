#/bin/bash
cd ..
git ls-files | grep -e ".*\.cpp" -e ".*\.h" -e ".*\.cmake" | xargs cat | wc -l

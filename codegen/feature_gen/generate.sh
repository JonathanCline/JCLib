#!

HERE=${0%generate.sh}

if [[ -z "{$1}" ]]; then
    echo "missing output file path!"
    exit
fi

# Just testing for now
lua $HERE/feature_gen.lua $HERE/feature_list.lua $1

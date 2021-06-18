output="log.txt"
passed=0
failed=0
expected_ret_value=1
test_dir="recovery/*"
rm -f $output

function run_tests() {
    for file in $test_dir 
    do
        ../src/compil < $file
        if [ $? -eq $expected_ret_value ]
        then
            echo -e "$file \033[32mpassed\033[0m"
            echo "$file: passed" >> $output
            ((passed++))
        else
            echo -e "$file \033[31mfailed\033[0m\n"
            echo "$file: failed" >> $output
            ((failed++))
        fi
    done
}

cd ../src/
make clean
make
cd ../tests/
run_tests

expected_ret_value1
test_dir="bad/*"
run_tests

expected_ret_value=0
test_dir="good/*"
run_tests

echo "tests passed: $passed" >> $output
echo "tests failed: $failed" >> $output

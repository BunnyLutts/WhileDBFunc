print "Start testing...";
let files = ls tests | get name;
print $"Find ($files | length) tests...";
let result = $files | each {|path| print $"Testing: ($path)" | open $path | ./bin/main};
print "Test finished. Results saved to test_result.csv";
let parsed = $result | parse --regex "__PARSE_RESULT_BEGIN__ (?P<parsed>.*?) __PARSE_RESULT_END__ __INPTR_RESULT_BEGIN__ (?P<interpreted>.*?) __INPTR_RESULT_END__";
let formatted = $files | wrap test | merge $parsed;
$formatted | to csv | save -f test_result.csv;
print $formatted
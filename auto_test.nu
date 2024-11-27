print "Start testing...";
let files = ls tests | get name;
print $"Find ($files | length) tests...";
let result = $files | each {|path| print $"Testing: ($path)" | open $path | ./bin/main};
print "Test finished. Results saved to test.result";
$files | wrap test | merge ($result | wrap result) | each {|pack| $"($pack.test):\n($pack.result)\n"} | save -f test.result
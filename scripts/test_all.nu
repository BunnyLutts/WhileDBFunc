print "Start testing...";
let files = ls tests | sort-by name | get name;
print $"Find ($files | length) tests...";
let result = $files | each {|path| print $"Testing: ($path)" | ./bin/main $path};
let raw = $files | each {|path| open $path};
print "Test finished. Results saved to test_result.json";
let parsed = $result | parse "__PARSE_RESULT_BEGIN__\n{parsed}\n__PARSE_RESULT_END__\n__INPTR_RESULT_BEGIN__\n{interpreted}\n__INPTR_RESULT_END__";
let formatted = $files | wrap test | merge ($raw | each {|cnt| $cnt | lines } | wrap raw) | merge $parsed;
let env_info = {os_version: (uname), gcc_version: (gcc --version | lines | $in.0), ld_version: (ld -v), make_version: (make -v | lines | $in.0), nu_version: (nu -v)};
let output = {env_info: $env_info, version: (git show | lines | $in.0), build_sha512sum:(sha512sum bin/main), test_result: $formatted};
$output | to json | save -f test_result.json;
print $output
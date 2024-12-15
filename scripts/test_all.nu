source utility.nu;
print "Start testing...";
let files = ls tests | sort-by name | get name;
print $"Find ($files | length) tests...";
let raw = $files | each {|path| open $path};
let seperated = $raw | parse "__INPUT_BEGIN__\n{input}\n__INPUT_END__\n__SRC_BEGIN__\n{src}\n__SRC_END__";
let combined = $files | wrap test | (merge $seperated);
let result = $combined | each {|pack| print $"Testing: ($pack.test)"; $pack.src | save -f temp.src; $pack.input | ./bin/main temp.src};
rm temp.src;
let cresult = $combined | each {|pack| print $"Verifing: ($pack.test)"; $pack.src | save -f temp.src; w2c temp.src; gcc -g w2c_output.c; $pack.input | ./a.out}
rm temp.src w2c_output.c a.out;
print "Test finished. Results saved to test_result.json";
let parsed = $result | parse "__PARSE_RESULT_BEGIN__\n{parsed}\n__PARSE_RESULT_END__\n__INPTR_RESULT_BEGIN__\n{interpreted}\n__INPTR_RESULT_END__";
let formatted = $combined | merge $parsed | merge ($cresult | wrap c-verification);
let compared = $files | wrap test | merge ($formatted | each {|x| $x.interpreted == $x.c-verification} | wrap compared);
let env_info = {os_version: (uname), gcc_version: (gcc --version | lines | $in.0), ld_version: (ld -v), make_version: (make -v | lines | $in.0), nu_version: (nu -v), racket_version: (racket -v)};
let output = {env_info: $env_info, version: (git show | lines | $in.0), build_sha512sum:(sha512sum bin/main), test_result: $formatted};
print "Compared result:";
print $compared;
$output | to json | save -f test_result.json;
print $output
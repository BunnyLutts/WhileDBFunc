def test_one [file_name] {
  let result = (./bin/main $file_name);
  let raw = open $file_name;
  let parsed = $result | parse --regex "__PARSE_RESULT_BEGIN__\n(?P<parsed>.*?)\n__PARSE_RESULT_END__\n__INPTR_RESULT_BEGIN__\n(?P<interpreted>.*?)\n__INPTR_RESULT_END__";
  let formatted = {test: $file_name , raw: ($raw | lines ), parsed: $parsed.0.parsed, interpreted: $parsed.0.interpreted};
  let env_info = {os_version: (uname), gcc_version: (gcc --version | lines | $in.0), ld_version: (ld -v), make_version: (make -v | lines | $in.0), nu_version: (nu -v)};
  let output = {env_info: $env_info, version: (git show | lines | $in.0), build_sha512sum:(sha512sum bin/main), test_result: $formatted};
  $output | to json | save -f test_result.json;
  print $output;
}
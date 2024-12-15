def w2c [p] {
  open $p | parse "!INPUT:{input}!SRC:\n{src}" | get src | save -f temp.src;
  racket verification/w2c.rkt temp.src | "// Source: " + $p + "\n" + (open verification/header.c) + "\n\n" + $in | clang-format | save -f w2c_output.c;
  rm temp.src
}
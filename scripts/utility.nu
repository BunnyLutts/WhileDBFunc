def w2c [p] {
  racket verification/w2c.rkt $p | "// Source: " + $p + "\n" + (open verification/header.c) + "\n\n" + $in | clang-format | save -f w2c_output.c;
}
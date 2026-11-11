#!/bin/bash
# Compiles and runs the networking tests (libcxx/test/std/{io,...}/*.pass.cpp) with the installed compiler.
# usage: run_net_tests.sh <install-prefix>
P="$1"; fail=0; pass=0; skipped=0
SDK="$(xcrun --show-sdk-path)"
out=/tmp/nettests; mkdir -p $out
with_timeout() { perl -e 'alarm shift; exec @ARGV' "$@"; }
for d in io io_buffer io_stream io_bridge io_context io_timer io_signal net io_file net_tls; do
  for f in libcxx/test/std/$d/*.pass.cpp libcxx/test/libcxx/$d/*.pass.cpp; do
    [ -f "$f" ] || continue
    if grep -qE '^// UNSUPPORTED:.*(apple|darwin|macos)' "$f"; then echo "SKIP $f"; skipped=$((skipped+1)); continue; fi
    if grep -qE '^// REQUIRES: linux *$' "$f"; then echo "SKIP(linux-only) $f"; skipped=$((skipped+1)); continue; fi
    if grep -qE '^// UNSUPPORTED:.*c\+\+29' "$f"; then echo "SKIP(not c++29) $f"; skipped=$((skipped+1)); continue; fi
    n=$(echo "$f" | tr '/' '_')
    flags="-std=c++29 -fexperimental-library"
    case "$f" in *header_requires_flag*) flags="-std=c++29";; esac   # the flag gate must be off without the flag
    mode="link"; case "$f" in *.compile.pass.cpp) mode="syntax";; esac
    common="-Wall -Wextra -Wno-unused-parameter -nostdlib++ -isysroot $SDK -isystem $P/include/c++/v1 -I libcxx/test/support -I $(dirname "$f")"
    if [ $mode = syntax ]; then
      if "$P/bin/clang++" $flags $common -fsyntax-only "$f" 2> "$out/$n.compile.log"; then echo "PASS $f"; pass=$((pass+1)); else echo "FAIL(compile) $f"; grep -E "error" "$out/$n.compile.log" | head -5; fail=$((fail+1)); fi
      continue
    fi
    if "$P/bin/clang++" $flags $common -fuse-ld=lld -L"$P/lib" -lc++ -lc++abi -Wl,-rpath,"$P/lib" "$f" -o "$out/$n" 2> "$out/$n.compile.log"; then
      if (cd "$out" && with_timeout 90 "./$n" > "$out/$n.run.log" 2>&1); then echo "PASS $f"; pass=$((pass+1)); else echo "FAIL(run) $f"; tail -5 "$out/$n.run.log"; fail=$((fail+1)); fi
    else echo "FAIL(compile) $f"; grep -E "error" "$out/$n.compile.log" | head -5; fail=$((fail+1)); fi
  done
done
echo "net tests: pass=$pass fail=$fail skipped=$skipped"
[ $fail -eq 0 ]

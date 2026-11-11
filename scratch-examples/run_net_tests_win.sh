#!/bin/bash
# Compiles and runs the networking tests (libcxx/test/{std,libcxx}/{io,...}/*.pass.cpp) with the installed clang-cl.
# usage: run_net_tests_win.sh <install-prefix (mixed path)> [dirs...]
P="$1"; shift
dirs="${*:-io io_buffer io_stream io_bridge io_context io_timer io_signal net io_file net_tls}"
fail=0; pass=0; skipped=0
out=/tmp/nettests; mkdir -p $out
export MSYS2_ARG_CONV_EXCL="*"
CL="$P/bin/clang-cl.exe"
for d in $dirs; do
  for f in libcxx/test/std/$d/*.pass.cpp libcxx/test/libcxx/$d/*.pass.cpp; do
    [ -f "$f" ] || continue
    if grep -qE '^// UNSUPPORTED:.*(windows|msvc|mingw)' "$f"; then echo "SKIP $f"; skipped=$((skipped+1)); continue; fi
    if grep -E '^// REQUIRES:' "$f" | grep -E 'linux|darwin|freebsd|io-backend' | grep -qvE 'windows'; then echo "SKIP(not windows) $f"; skipped=$((skipped+1)); continue; fi
    if grep -qE '^// UNSUPPORTED:.*c\+\+29' "$f"; then echo "SKIP(not c++29) $f"; skipped=$((skipped+1)); continue; fi
    n=$(echo "$f" | tr '/' '_')
    flags="-Xclang -fexperimental-library"
    case "$f" in *header_requires_flag*) flags="";; esac
    mode="link"; case "$f" in *.compile.pass.cpp) mode="syntax";; esac
    common="-clang:-std=c++29 -MD -EHsc -w -Xclang -nostdinc++ -I $P/include/c++/v1 -I libcxx/test/support -I $(dirname "$f")"
    if [ $mode = syntax ]; then
      if "$CL" $flags $common -fsyntax-only "$f" > "$out/$n.compile.log" 2>&1; then echo "PASS $f"; pass=$((pass+1)); else echo "FAIL(compile) $f"; grep -E "error" "$out/$n.compile.log" | head -5; fail=$((fail+1)); fi
      continue
    fi
    if "$CL" $flags $common -fuse-ld=lld "$f" -Fe:"$out/$n.exe" -link "$P/lib/libc++.lib" msvcprt.lib ws2_32.lib > "$out/$n.compile.log" 2>&1; then
      if (cd "$out" && timeout 90 "./$n.exe" > "$out/$n.run.log" 2>&1); then echo "PASS $f"; pass=$((pass+1)); else echo "FAIL(run) $f"; tail -5 "$out/$n.run.log"; fail=$((fail+1)); fi
    else echo "FAIL(compile) $f"; grep -E "error" "$out/$n.compile.log" | head -5; fail=$((fail+1)); fi
  done
done
echo "net tests: pass=$pass fail=$fail skipped=$skipped"
[ $fail -eq 0 ]

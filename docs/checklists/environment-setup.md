# Environment Setup Checklist
gcc --version
gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

gabiota@pop-os:~/personal/projects/code/time$ make --version
GNU Make 4.3
Built for x86_64-pc-linux-gnu
Copyright (C) 1988-2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
gabiota@pop-os:~/personal/projects/code/time$ git --version
git version 2.43.0

python3 --version
Python 3.12.3

./emsdk activate latest
Resolving SDK alias 'latest' to '5.0.2'
Resolving SDK version '5.0.2' to 'sdk-releases-0a320d2395858e63288b3632b81535444ca2c59d-64bit'
Setting the following tools as active:
   node-22.16.0-64bit
   releases-0a320d2395858e63288b3632b81535444ca2c59d-64bit

Next steps:
- To conveniently access emsdk tools from the command line,
  consider adding the following directories to your PATH:
    /home/gabiota/personal/projects/code/time/emsdk
    /home/gabiota/personal/projects/code/time/emsdk/upstream/emscripten
- This can be done for the current shell by running:
    source "/home/gabiota/personal/projects/code/time/emsdk/emsdk_env.sh"
- Configure emsdk in your shell startup scripts by running:
    echo 'source "/home/gabiota/personal/projects/code/time/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile
gabiota@pop-os:~/personal/projects/code/time/emsdk$ source "/home/gabiota/personal/projects/code/time/emsdk/emsdk_env.sh"
Setting up EMSDK environment (suppress these messages with EMSDK_QUIET=1)
Adding directories to PATH:
PATH += /home/gabiota/personal/projects/code/time/emsdk
PATH += /home/gabiota/personal/projects/code/time/emsdk/upstream/emscripten

Setting environment variables:
PATH = /home/gabiota/personal/projects/code/time/emsdk:/home/gabiota/personal/projects/code/time/emsdk/upstream/emscripten:/home/gabiota/.local/bin:/home/gabiota/.nvm/versions/node/v22.21.1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
EMSDK = /home/gabiota/personal/projects/code/time/emsdk
EMSDK_NODE = /home/gabiota/personal/projects/code/time/emsdk/node/22.16.0_64bit/bin/node

 emcc --version
clang: warning: future releases of the clang compiler will prefer GCC installations containing libstdc++ include directories; '/usr/lib/gcc/x86_64-linux-gnu/13' would be chosen over '/usr/lib/gcc/x86_64-linux-gnu/14' [-Wgcc-install-dir-libstdcxx]
clang: warning: future releases of the clang compiler will prefer GCC installations containing libstdc++ include directories; '/usr/lib/gcc/x86_64-linux-gnu/13' would be chosen over '/usr/lib/gcc/x86_64-linux-gnu/14' [-Wgcc-install-dir-libstdcxx]
shared:INFO: (Emscripten: Running sanity checks)
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 5.0.2 (dc80f645ee70178c11666de0c3860d9e064d50e4)
Copyright (C) 2026 the Emscripten authors (see AUTHORS.txt)
This is free and open source software under the MIT license.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## System Tools

- [x] gcc installed
  ```bash
  gcc --version
  ```

- [x] make installed
  ```bash
  make --version
  ```

- [x] git installed
  ```bash
  git --version
  ```

- [x] Python 3 installed (for local HTTP server)
  ```bash
  python3 --version
  ```

## Emscripten SDK

- [x] emsdk cloned
  ```bash
  git clone https://github.com/emscripten-core/emsdk.git
  cd emsdk
  ```

- [x] emsdk latest installed
  ```bash
  ./emsdk install latest
  ```

- [x] emsdk activated
  ```bash
  ./emsdk activate latest
  ```

- [x] emsdk in PATH (add to .bashrc or .profile)
  ```bash
  source /path/to/emsdk/emsdk_env.sh
  ```

- [x] emcc works
  ```bash
  emcc --version
  ```

## Editor

- [x] VS Code C/C++ extension installed (or preferred editor configured for C)

## Git Repository

- [x] Git repo initialized
  ```bash
  cd /home/gabiota/personal/projects/code/time
  git init
  ```

- [ ] .gitignore created (already exists in project root)

- [ ] First commit made
  ```bash
  git add -A
  git commit -m "chore(init): project scaffold — Time visual artwork"
  ```

## Test Framework

- [ ] Unity test framework vendored in tests/unity/
  - Download from: https://github.com/ThrowTheSwitch/Unity
  - Copy `src/unity.c`, `src/unity.h`, `src/unity_internals.h` into `tests/unity/`
## Build Verification

- [ ] Hello world compiles natively
  ```bash
  make native
  ./build/time
  # Expected output: "Time - A Visual Study"
  ```

- [ ] Hello world compiles to WASM
  ```bash
  make wasm
  ```

- [ ] WASM loads in browser
  ```bash
  make serve
  # Open http://localhost:8080 in browser
  ```

- [ ] Test runner compiles and runs
  ```bash
  make test
  # Expected: "0 Tests 0 Failures 0 Ignored" (or similar)
  ```

---

**All checks passing = Phase 0 complete. Ready to start Phase 1.**

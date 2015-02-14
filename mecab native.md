Install PNaCl SDK

Configure to be built using PNaCl's Clang toolchain

https://developer.chrome.com/native-client/devguide/devcycle/building

Use gnu++11 as standard library, because `newlib` relies on GNU extensions.

```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CXX="pnacl-clang++"
```


// This might help:
https://groups.google.com/forum/#!topic/native-client-discuss/E5lECqm7tx8
http://magnushoff.com/pnacl.html

We will run all our make and configure in the root directory of the `mecab-emscripten` repository:
https://github.com/fasiha/mecab-emscripten

Add Pepper headers:

```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include" CXX="pnacl-clang++"
```

Add Pepper library path for linking:

```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release"
```

Specify libraries to pass to linker:

Don't use -lppapi; doesn't seem to exist at the moment? I used lppapi_simple at a guess.

```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release" LIBS="-lppapi_cpp -lppapi_simple"
```

We don't need shared anyway (I hope?), so get rid of Makefile switches that clang doesn't understand

```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release" LIBS="-lppapi_cpp -lppapi_simple" --disable-shared
```

```shell
// libless just in case useful:
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CXX="pnacl-clang++" --disable-shared
```

Default AR and ARFLAGS (Clang, cru) might work, but you need to add support to PNaCl for BSD long filenames.

# AR_FLAGS="-crfu"
# AR="pnacl-ar"
Mecab's `configure` needs to tell LLVM archiver to use `f` modifier, to avoid BSD long filenames
Err, `f` flag doesn't work as advertised.
Guess we just use the default AR and AR_FLAGS.
Additionally can't figure out how to use `pnacl-ar` for archiving. So I will use the default clang.
Instead of avoiding BSD long filename, I added support to `artools.py` in PNaCl:

```python
# BSD style long names (totally supported, baby)
    if self.name.startswith('#1/'):
      parse_name_length = re.search(r"#1/(\d+)", self.name)
      name_length_bytes_dec = parse_name_length.group(1)
      # how many bytes to read after header
      name_length_bytes = int(name_length_bytes_dec)

      self.file_contents_bytes = self.size-name_length_bytes
      self.size = self.file_contents_bytes

      file_data = fp.read(name_length_bytes)
      self.name = file_data
      self.is_bsd_long_name = True
```

Uh, move this code to be ABOVE the check for `self.is_bsd4_symtab = (self.name == '__.SYMDEF SORTED')`.

On Mac OS X, neeed to comment out all references to the `bind_at_load` linker switch (this is in `libtool` and `ltmain.sh`), since PNaCl toolchain doesn't recognise it:

```shell
	# Don't allow lazy linking, it breaks C++ global constructors
	# But is supposedly fixed on 10.4 or later (yay!).
	# if test "$tagname" = CXX ; then
	#   case ${MACOSX_DEPLOYMENT_TARGET-10.0} in
	#     10.[0123])
	#       func_append compile_command " ${wl}-bind_at_load"
	#       func_append finalize_command " ${wl}-bind_at_load"
	#     ;;
	#   esac
	# fi
```

Download & extract PNaCl iconv lib from naclports Continuous Build server (for example http://gsdview.appspot.com/naclports/builds/pepper_41/trunk-253-g089940f/packages/libiconv_1.14_pnacl.tar.bz2)
Add pnacl iconv lib to lib path and include path:
```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_cpp -lppapi_simple" --disable-shared
```

Use PNaCl AR:
```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_cpp -lppapi_simple" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar"
```

Use PNaCl ranlib:
```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_cpp -lppapi_simple" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib"
```

Now having problems with multiply-defined symbols at link-time.

Get rid of stdc++ lib from `./configure`:
```shell
# LIBS="-lstdc++  $LIBS"

…

# if test "x$ac_cv_lib_stdcpp_main" = xyes; then :
#   MECAB_LIBS="-lstdc++"
# fi
```

Try ppapi lib (fails `./configure` check of whether your C compiler works):

Use PNaCl ranlib:
```shell
./configure --with-charset=utf8 CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_cpp -lppapi_simple" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib"
```

Use `PNaCl-clang` for C, set cross-compiling to `maybe`:

```shell
./configure --with-charset=utf8 CC="pnacl-clang" CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_cpp -lppapi -lppapi_simple" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib" cross_compiling=maybe
```

Maybe try without iconv, since it is perhaps failing to find this dependency at runtime?

`--without-libiconv-prefix`
```shell
./configure --with-charset=utf8 CC="pnacl-clang" CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_cpp -lppapi -lppapi_simple" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib" cross_compiling=maybe --without-libiconv-prefix
```

And also maybe put ipadic/ folder next to it?

Copy LIBS from tutorial:
`-lppapi_simple -lppapi -lppapi_cpp -lnacl_io`
```shell
./configure --with-charset=utf8 CC="pnacl-clang" CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib" LIBS="-lppapi_simple -lppapi -lppapi_cpp -lnacl_io" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib" cross_compiling=maybe --without-libiconv-prefix
```

Provide pthread (from tutorial):

```shell
./configure --with-charset=utf8 CC="pnacl-clang" CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib -pthread" LIBS="-lppapi_simple -lppapi -lppapi_cpp -lnacl_io" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib" cross_compiling=maybe --without-libiconv-prefix
```

make also:

```shell
./configure --with-charset=utf8 CC="pnacl-clang" CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib -pthread" LIBS="-lppapi_simple -lppapi -lppapi_cpp -lnacl_io" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib" cross_compiling=maybe --without-libiconv-prefix && make clean && make && pnacl-finalize src/mecab -o /Users/birch/Documents/nacl_sdk/pepper_41/getting_started/me/mecab.final.pexe
```

I think I saw the linker still try to link to libiconv despite the --without-libiconv-prefix switch; maybe I'm using it wrong. it complained that my libiconv wasn't portable.

Change mecab.cpp to talk to Pepper:

```c++
//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#include "mecab.h"
#include "winmain.h"
#include <iostream>
#include <unistd.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>
#include <ppapi_simple/ps_main.h>

int main(int argc, char **argv) {
  return mecab_do (argc, argv);
}

int ppapi_simple_main(int argc, char* argv[]) {
    for (int i=0; i<argc; ++i) {
        // std::cout << "Argument " << i << ": " << argv[i] << std::endl;
        std::cout << mecab_do(argc, &argv[i]);
    }

    // std::cerr << "Standard error output appears in the debug console\n";

    // Since PNaCl modules aren't supposed to terminate, we need to give
    // Pepper some breathing room to convey all the messages:
    sleep(3);

    return 0;
}

PPAPI_SIMPLE_REGISTER_MAIN(ppapi_simple_main)
```

```shell
make && pnacl-finalize src/mecab -o /Users/birch/git/mecab-emscripten/web/demo/mecab.final.pexe
```

```shell
make && pnacl-finalize mecab -o /Users/birch/git/mecab-emscripten/web/demo/mecab.final.pexe
```

```shell
./configure --with-charset=utf8 CC="pnacl-clang" CXXFLAGS="-std=gnu++11 -O2" CFLAGS="-O2" CPPFLAGS="-I/Users/birch/Documents/nacl_sdk/pepper_41/include -I/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/include" CXX="pnacl-clang++" LDFLAGS="-L/Users/birch/Documents/nacl_sdk/pepper_41/lib/pnacl/Release -L/Users/birch/git/mecab-emscripten/lib/libiconv_1.14_pnacl/payload/lib -pthread" LIBS="-lppapi_simple -lppapi -lppapi_cpp -lnacl_io" --disable-shared AR_FLAGS="-crfu" AR="pnacl-ar" RANLIB="pnacl-ranlib" cross_compiling=maybe --without-libiconv-prefix && make clean && make && pnacl-finalize src/mecab -o /Users/birch/git/mecab-emscripten/web/demo/mecab.final.pexe
```
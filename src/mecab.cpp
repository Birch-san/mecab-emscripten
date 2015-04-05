//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
// #include <stdio.h>
// #include <stdlib.h>
#include "mecab.h"
#include "winmain.h"
#include <iostream>
#include <unistd.h>
// #include <ppapi_simple/ps_main.h>

// Global variables
// int g_argc;
// char **g_argv;

// int main(int argc, char **argv) {
//   g_argc = argc;
//   g_argv = argv;
//   // return mecab_do (argc, argv);
// }

/*int ppapi_simple_main(int argc, char** argv) {
    for (int i=0; i<argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
        // std::cout << mecab_do(argc, &argv[i]);
    }

	std::cout << "yo" << std::endl;
    std::cout << mecab_do2(*argv) << std::endl;

    // std::cerr << "Standard error output appears in the debug console\n";

    // Since PNaCl modules aren't supposed to terminate, we need to give
    // Pepper some breathing room to convey all the messages:
    sleep(3);

    return 0;
}

PPAPI_SIMPLE_REGISTER_MAIN(ppapi_simple_main)*/
#include "url_loader.h"

namespace pp {
Module* CreateModule() { return new URLLoaderModule(); }
}  // namespace pp
//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
// #include <stdio.h>
// #include <stdlib.h>
#include <sstream>
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
#include "url_loader_handler.h"

void URLLoaderInstance::HandleMessage(const pp::Var& var_message) {
  if (!var_message.is_string()) {
    return;
  }
  std::string message = var_message.AsString();
  if (message.find(kLoadUrlMethodId) == 0) {
    // The argument to getUrl is everything after the first ':'.
    size_t sep_pos = message.find_first_of(kMessageArgumentSeparator);
    if (sep_pos != std::string::npos) {
      std::string url = message.substr(sep_pos + 1);
      printf("URLLoaderInstance::HandleMessage('%s', '%s')\n",
             message.c_str(),
             url.c_str());
      fflush(stdout);
      URLLoaderHandler* handler = URLLoaderHandler::Create(this, url);
      if (handler != NULL) {
        // Starts asynchronous download. When download is finished or when an
        // error occurs, |handler| posts the results back to the browser
        // vis PostMessage and self-destroys.
        handler->Start();
      }
    }
  }
}

void URLLoaderInstance::PostMessage(const pp::Var& message) {
  // return mecab_do (argc, argv);
  // super::PostMessage(message);
  // pp::Var var_result(fname + "\n" + text);

  // for (int i=0; i<argc; ++i) {
      // std::cout << "Argument " << i << ": " << argv[i] << std::endl;
      // std::cout << mecab_do(argc, &argv[i]);
  // }

  char args[] = "-help";

  std::stringstream ss;

  // //put arbitrary formatted data into the stream
  // ss << 4.5 << ", " << 4 << " whatever";
  // ss << "yo" << std::endl;
  int success = mecab_do2(args);
  // ss << mecab_do3(argv) << std::endl;

  // //convert the stream buffer into a string
  // std::string str = ss.str();

  std::string str = success ? "success" : "failure";
  // str.append(mecab_do2(*g_argv));

  pp::Var var_result(str);

  // pp::Var var_result("what up");

  super::PostMessage(var_result);
}
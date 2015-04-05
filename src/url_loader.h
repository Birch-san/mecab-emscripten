// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_LOADER_H_
#define URL_LOADER_H_

// #include <ppapi/cpp/instance.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>

// #include "url_loader_handler.h"

// These are the method names as JavaScript sees them.
namespace {
const char* const kLoadUrlMethodId = "getUrl";
static const char kMessageArgumentSeparator = ':';
}  // namespace

class URLLoaderInstance : public pp::Instance {
 public:
  explicit URLLoaderInstance(PP_Instance instance) : pp::Instance(instance) {}
  virtual ~URLLoaderInstance() {}

  // Called by the browser to handle the postMessage() call in Javascript.
  // The message in this case is expected to contain the string 'getUrl'
  // followed by a ':' separator, then the URL to fetch.  If a valid message
  // of the form 'getUrl:URL' is received, then start up an asynchronous
  // download of URL.  In the event that errors occur, this method posts an
  // error string back to the browser.
  virtual void HandleMessage(const pp::Var& var_message);
  using pp::Instance::PostMessage;
  void PostMessage(const pp::Var& message);
 private:  // Prevents erroneous use by other classes.
  typedef pp::Instance super;
};

class URLLoaderModule : public pp::Module {
 public:
  URLLoaderModule() : pp::Module() {}
  virtual ~URLLoaderModule() {}

  // Create and return a URLLoaderInstance object.
  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new URLLoaderInstance(instance);
  }
};


#endif  // URL_LOADER_H_

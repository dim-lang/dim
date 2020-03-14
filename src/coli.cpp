// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Option.h"
#include "Parser.h"
#include "Parser.tab.hpp"
#include "Scanner.h"
#include "Semant.h"
#include "Token.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  Log::initialize();
  Option conf(argc, argv);
  CINFO("starting, argc:{} argv[0]:{}", argc, argv[0]);

  if (conf.hasHelp()) {
    std::cout << conf.help() << std::endl;
    return 0;
  }

  if (conf.hasVersion()) {
    std::cout << conf.version() << std::endl;
    return 0;
  }

  if (conf.hasFileNames()) {
    std::vector<std::string> fileNameList = conf.fileNames();
    for (int i = 0; i < (int)fileNameList.size(); i++) {
      std::string fileName = fileNameList[i];
      CINFO("fileName:{}", fileName);
      Scanner scanner;
      scanner.pushBuffer(fileName);
      int p = scanner.parse();
      CINFO("parse: p:{}, currentBuffer: {}, yy_scaninfo: {}", p,
            scanner.currentBuffer(), (void *)scanner.yy_scaninfo());
      CASSERT(p == 0, "parse fail:{}", p);
      Semant *semant = new Semant(scanner.translateUnit());
      semant->build();
      semant->check();
      delete semant;
    }
  }

  return 0;
}

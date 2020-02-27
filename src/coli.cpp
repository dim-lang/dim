// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Log.h"
#include "Option.h"
#include "Parser.h"
#include "Parser.tab.hpp"
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
      std::string moduleName = fileToModule(fileName);
      CINFO("fileName:{} moduleName:{}", fileName, moduleName);
      Scanner scanner;
      if (!scanner.push(moduleName)) {
        CASSERT(false, "tkPushImport {} fail", moduleName);
        return 0;
      }
      int yp = yyparse(scanner.scaninfo, &scanner);
      CINFO("yyparse: yp:{}, scanner.scaninfo:{}", yp,
            (void *)scanner.scaninfo);
      CASSERT(yp == 0, "yyparse fail:{}", yp);
      Semant *semant = new Semant(scanner.program);
      semant->build();
      semant->check();
      delete semant;
    }
  }

  return 0;
}

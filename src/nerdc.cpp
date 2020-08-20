// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Ast.h"
#include "Buffer.h"
#include "Ir.h"
#include "Log.h"
#include "Option.h"
#include "Parser.h"
#include "Parser.tab.h"
#include "Scanner.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  Log::initialize();
  Option conf(argc, argv);
  LOG_INFO("starting, argc:{} argv[0]:{}", argc, argv[0]);

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
      LOG_INFO("fileName:{}", fileName);
      Scanner scanner;
      scanner.pushBuffer(fileName);
      int p = scanner.parse();
      LOG_INFO("parse: p:{}, currentBuffer: {}, yy_scaninfo: {}", p,
               scanner.currentBuffer(), (void *)scanner.yy_scaninfo());
      EX_ASSERT(p == 0, "parse fail:{}", p);
      IrContext context(fileName);
      IrTranslateUnit tunit(&context, scanner.translateUnit());
    }
  }

  return 0;
}

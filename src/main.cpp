#include <iostream>

#include "config.h"

int main(int argc, char ** argv, const char ** envp) {
    Config::instance()->parse(argc, argv, envp);
    return 0;
}

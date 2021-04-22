#ifndef REAL_FRAMEWORK_COMMAND_H_
#define REAL_FRAMEWORK_COMMAND_H_

#include <functional>
#include "util.h"

namespace rea {

class DSTDLL ICommand{
public:
    ICommand(){}
    ICommand(std::function<bool(void)> aRedo, std::function<bool(void)> aUndo) : undo(aUndo), redo(aRedo){}
    std::function<bool(void)> undo;
    std::function<bool(void)> redo;
};

}

#endif

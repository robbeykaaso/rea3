#ifndef REA_COMMAND_H_
#define REA_COMMAND_H_

#include <functional>
#include "util.h"

namespace rea2 {

class DSTDLL ICommand{
public:
    ICommand(){}
    ICommand(std::function<bool(void)> aRedo, std::function<bool(void)> aUndo) : undo(aUndo), redo(aRedo){}
    std::function<bool(void)> undo;
    std::function<bool(void)> redo;
};

}

#endif

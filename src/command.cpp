#include "command.h"
#include "rea.h"
#include <vector>

namespace rea {

class commandManager{
public:
    commandManager(){
        rea::pipeline::instance()->add<ICommand, pipePartial>([this](stream<ICommand>* aInput){
            for (int i = int(m_commands.size()) - 1; i > m_cur; --i)
                m_commands.pop_back();
            m_commands.push_back(aInput->data());
            m_cur++;
            aInput->out();
        }, Json("name", "addCommand"));

        rea::pipeline::instance()->add<bool, pipePartial>([this](stream<bool>* aInput){
            if (aInput->data()){
                if (m_cur < int(m_commands.size() - 1)){
                    if (m_commands[size_t(m_cur + 1)].redo())
                        m_cur++;
                }
            }else{
                if (m_cur >= 0)
                    if (m_commands[size_t(m_cur)].undo())
                        m_cur--;
            }
            aInput->out();
        }, Json("name", "doCommand"));
    }
private:
    int m_cur = - 1;
    std::vector<ICommand> m_commands;
};

static commandManager command_manager;

}

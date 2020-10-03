#ifndef __LOOPLINE_COMMAND_HPP__
#define __LOOPLINE_COMMAND_HPP__

#include <functional>

namespace loopline
{
    class Command
    {
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
        virtual void undo() = 0;

    private:
    };

    class NullCommand : public Command
    {
    public:
        NullCommand() {}

        virtual void execute() {}
        virtual void undo() {}
    };

    class LambdaCommand : public Command
    {
    public:
        LambdaCommand(std::function<void()> exec, std::function<void()> un = [](){}) : exec(exec), un(un) {}
        virtual ~LambdaCommand() {}

        virtual void execute()
        {
            exec();
        }

        virtual void undo()
        {
            un();
        }

    private:
        std::function<void()> exec, un;
    };

    class UndoCommand : public Command
    {
    public:
        UndoCommand(std::vector<Command *> *commandList, std::vector<Command *> *undoList) : list(commandList), undoList(undoList) {}

        virtual void execute()
        {
            if (list->empty())
                return;

            list->back()->undo();
            undoList->push_back(list->back());

            list->pop_back();
        }

        virtual void undo()
        {
            if (undoList->empty())
                return;

            undoList->back()->execute();
            list->push_back(undoList->back());

            undoList->pop_back();
        }

    private:
        std::vector<Command *> *list;
        std::vector<Command *> *undoList;
    };

    class RedoCommand : public Command
    {
    public:
        RedoCommand(UndoCommand *undoCommand) : undoCommand(undoCommand) {}

        virtual void execute()
        {
            undoCommand->undo();
        }

        virtual void undo()
        {
        }

    private:
        UndoCommand *undoCommand;
    };
}; // namespace loopline

#endif
#pragma once

#include <vector>

#include "source.h"
#include "indent.h"
#include "parseMode.h"
#include "scope.h"

namespace parser
{

struct Enviroment
{
    Source source;
    Indent indent;
    std::vector<std::shared_ptr<IParseMode>> modeStack;
    std::vector<std::shared_ptr<IScope>> scopeStack;
    Value externObj;

    explicit Enviroment(char const* source_, std::size_t length);

    void pushMode(std::shared_ptr<IParseMode> pMode);
    void popMode();

    void pushScope(std::shared_ptr<IScope> pScope);

    void popScope();

    std::shared_ptr<IParseMode> currentMode();
    IScope& currentScope();
    IScope const& currentScope() const;
    std::shared_ptr<IScope>& currentScopePointer();
    std::shared_ptr<IScope> const& currentScopePointer() const;
    int compareIndentLevel(int level);
    IScope& globalScope();
    IScope const& globalScope()const;
};

}
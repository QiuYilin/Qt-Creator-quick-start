#pragma once
#include <vector>
#include <memory>

struct Step
{
    bool SomeBoolMember1 {false};
    int SomeIntMember1 {0};
};

struct Definition
{
    std::vector<std::shared_ptr<Step>> Steps;

    bool SomeBoolMember1 {false};
    int SomeIntMember1 {0};
};

struct Data
{
    std::vector<std::shared_ptr<Definition>> definitions;
};
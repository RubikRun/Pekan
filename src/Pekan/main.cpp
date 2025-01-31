#include "PekanEngine.h"
using Pekan::PekanEngine;

int main()
{
    PekanEngine engine;
    if (!engine.init())
    {
        return -1;
    }
    if (!engine.runExercise0000())
    {
        return -1;
    }
    engine.cleanup();
    return 0;
}
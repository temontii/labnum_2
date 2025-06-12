#include "ui.hpp"

int main() {
    std::vector<ISequenceWrapper*> sequences;
    RunInterface(sequences);
    return 0;
}



//DynamicArray и ArraySequence - логика перепутана. В динамическом массиве есть только размер, но нету емкости. Емкость должна быть в ArraySequence, т.к. это аналог std::vector -- ВЫПОЛНЕНО
//разобраться, как работает std::vector и подумать, что не так с ArraySequence

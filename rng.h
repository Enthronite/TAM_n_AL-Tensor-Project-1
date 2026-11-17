#ifndef RANDOM_NUMBER_CREATOR_H
#define RANDOM_NUMBER_CREATOR_H

class RandomNumberCreator <%
    private:
        std::mt19937 gen;
    public:
        RandomNumberCreator() : gen(std::random_device<%%>()) <%%>
        int get_random_number(int min, int max) <%
            std::uniform_int_distribution<int> dist(min, max);
            return dist(gen);
        %>
%>;

#endif

// the only thing I'm worried about is that at then end, compiling all this is going to be a nightmare
// answer: as long as you don't put anything in another cpp file, it's fine

#include <cstring>
#include <sstream>
#include <vector>

using namespace std;

class Process
{
    public:
        string name;
        int arrivalTime;
        int serviceTime;
        int finishTime;
        int turnaroundTime;
        float normaTAT;
        int waitingTime;
        int is_complete = 0;
        int remainingServiceTime = 0;
        int startTime = 0;
        vector<char> trace;

};

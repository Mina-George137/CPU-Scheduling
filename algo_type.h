using namespace std;

class AlgoType
{
   public:
        int algoNumber;
        int quantum;
        AlgoType(int x, int y);

};

AlgoType::AlgoType(int x, int y) {
  algoNumber = x;
  quantum = y;
}

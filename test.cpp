#include <iostream>
using namespace std;

int main(){
    bool canMove[4] = {true, true, true, true};

    cout <<(canMove[0] + canMove[1] + canMove[2] + canMove[3]) <<endl;

    return 0;
}
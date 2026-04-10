
#include <iostream>
using namespace std;

int dif(int num1, int num2)
{
    return num1 - num2;
}


int sum(int num1, int num2) 
{
    return num1 + num2;
}

int main()
{
    int firstNum = 3;
    int secondNum = 5;

    int resultSum = sum(firstNum, secondNum);
    int resultDif = dif(firstNum, secondNum);

    cout << "Результат суми: " << resultSum << '\n';
    cout << "Результат різниці: " << resultDif;
    
    return 0;
}
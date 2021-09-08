TextCalculator was a small project from my Numerical Calculation Class. 
TextCalculator could calculate equation from string with variable from 'a' to 'z' and the value could be set by user.

Struct TextCalculator {
  double value;          -> calculation value
  char text[255];        -> mathematical string equation
  double constant[26];   -> constant for variable 'a' to 'z'
}

double textCalculatorCalculate(TextCalculator *textCalculator)
* translate string equation and doing calculation
* 'a' to 'a' in string equation will change to value from constant 
* return calculation value

void textCalculatorInit(TextCalculator* textCalculator, char* text)
* initialize string equation of textCalculator
* set value to 0

void textCalculatorSetConstant(TextCalculator* textCalculator, char constantName, double value)
* set constant[constantName-'a'] to value
* constantName argument range from 'a' to 'z', 'a' value stored in constant[0], 'z' in constant[25]

double textCalculatorCalculateSigmaTreshold(TextCalculator *textCalculator, char sigmaVariable, double treshold)
* Calculate Taylor Series
* sigmaVariable was the variable that used for incease of iteration (sigmaVariable range from 'a' to 'z').
* function will return calculation when approximation error smaller than treshold
* return calculation value of Taylor Series

double textCalculatorCalculateSigmaOrde(TextCalculator *textCalculator, char sigmaVariable, int orde)
* Same with textCalculatorCalculateSigmaTreshold but iteration set to orde value
* function will return calculation after perform orde iteration
* return calculation value of Taylor Series

double textCalculatorCalculateIntersection(TextCalculator *textCalculator, char intersectionVariable, double leftLimit, double rightLimit)
* calculate value of intersectionVariable to make equation equals to 0
* intersectionVariable range from 'a' to 'z'
* the value was limited from leftLimit to rightLimit, so it can't calculate value that make interectionVariable become 0 outside limit area
* leftLimit must be smaller than rightLimit
* return value of intersectionVariable in string equation that make the equation zero

double eConstantValue()
* calculate value of e (Natural Number constant)
* just need to be called once to be initialized
* return value of e

double piConstanValue()
* calculate value of pi (pi in circle)
* just need to be called once to be initialized
* quite slow (around 1s) to initialized (could be faster but less precision, but if you want faster just change the treshold in code)
* return pi value

double ln(double value)
* return value of ln (or log e)

double logt(double value, double base)
* return base log of value

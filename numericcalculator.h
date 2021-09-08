#ifndef NUMERICCALCULATOR_H
#define NUMERICCALCULATOR_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "math.h"
#include "float.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus
struct __csnd
{
    double value;
    struct __csnd* prev;
};

void __csndif(struct __csnd* __node, struct __csnd* __node_prev, double value)
{
    __node->value = value;
    __node->prev = __node_prev;
}

struct __csd
{
    struct __csnd* top;
};

void __csdif(struct __csd* __stack)
{
    __stack->top = NULL;
}

void __csd_push_back(struct __csd* __stack, double value)
{
    struct __csnd* newNode = (struct __csnd*)malloc(sizeof(struct __csnd));
    __csndif(newNode, __stack->top, value);
    __stack->top = newNode;
}

void __csd_pop_back(struct __csd* __stack)
{
    if (__stack->top)
    {
        struct __csnd* garbageCollector = __stack->top;
        __stack->top = __stack->top->prev;
        free(garbageCollector);
    }
}


void __csdea(struct __csd* __stack)
{
    struct __csnd* garbageCollector = __stack->top;
    while(__stack->top)
    {
        garbageCollector = __stack->top;
        __stack->top = __stack->top->prev;
        free(garbageCollector);
    }
}

struct __csnc
{
    char ascii;
    struct __csnc* prev;
};

void __csncif(struct __csnc* __node, struct __csnc* __node_prev, char ascii)
{
    __node->ascii = ascii;
    __node->prev = __node_prev;
}

struct __csc
{
    struct __csnc* top;
};

void __cscif(struct __csc* __stack)
{
    __stack->top = NULL;
}

void __csc_push_back(struct __csc* __stack, char value)
{
    struct __csnc* newNode = (struct __csnc*)malloc(sizeof(struct __csnc));
    __csncif(newNode, __stack->top, value);
    __stack->top = newNode;
}

void __csc_pop_back(struct __csc* __stack)
{
    if (__stack->top)
    {
        struct __csnc* garbageCollector = __stack->top;
        __stack->top = __stack->top->prev;
        free(garbageCollector);
    }
}

void __cscea(struct __csc* __stack)
{
    struct __csnc* garbageCollector = __stack->top;
    while(__stack->top)
    {
        garbageCollector = __stack->top;
        __stack->top = __stack->top->prev;
        free(garbageCollector);
    }
}

double eConstantValue();
double piConstanValue();

typedef struct TextCalculator
{
    char* text;
    double xValue;
    double value;
    double constant[26];
} TextCalculator;

void textCalculatorInit(TextCalculator* textCalculator, char* text)
{
    textCalculator->text = text;
    textCalculator->value = 0;
}

void textCalculatorSetConstant(TextCalculator* textCalculator, char constantName, double value)
{
    textCalculator->constant[constantName-'a'] = value;
}

void textCalculatorProcessSymbol(struct __csc* stackAscii, struct __csd* stackDecimal)
{
    double temp = stackDecimal->top->value;
    __csd_pop_back(stackDecimal);
    if (stackAscii->top->ascii == '+')
    {
        stackDecimal->top->value += temp;
    }
    else if (stackAscii->top->ascii == '-')
    {
        stackDecimal->top->value -= temp;
    }
    else if (stackAscii->top->ascii == '*')
    {
        stackDecimal->top->value *= temp;
    }
    else if (stackAscii->top->ascii == '/')
    {
        stackDecimal->top->value /= temp;
    }
    else if (stackAscii->top->ascii == '^')
    {
        stackDecimal->top->value = pow(stackDecimal->top->value, temp);
    }
    else if (stackAscii->top->ascii == '$')
    {
        stackDecimal->top->value = pow(temp, 1.0/stackDecimal->top->value);
    }
    else if (stackAscii->top->ascii == '_')
    {
        __csd_push_back(stackDecimal, -1.0 * temp);
    }
    __csc_pop_back(stackAscii);
}

double factorial(double value)
{
    if (value < 0)
    {
        fprintf(stderr, "\nError: factorial below 0\n");
        exit(-1);
    }
    else if (value <= 1)
    {
        return 1;
    }
    return value * factorial(value-1);
}

double absolute(double val)
{
    if (val < 0)
    {
        return -1 * val;
    }
    else return val;
}

double textCalculatorProcess(TextCalculator* textCalculator)
{
    struct __csd stackDecimal;
    __csdif(&stackDecimal);
    struct __csc stackAscii;
    __cscif(&stackAscii);

    bool stillNumber = false;
    bool derivative = false;
    int textLength = strlen(textCalculator->text);
    for (int i = 0; i < textLength; i++)
    {
        if (textCalculator->text[i] >= '0' && textCalculator->text[i] <= '9')
        {
            if (!stillNumber && derivative)
            {
                __csd_push_back(&stackDecimal, (double)(textCalculator->text[i] - '0'));
            }
            else if (stillNumber || derivative)
            {
                stackDecimal.top->value = stackDecimal.top->value * 10 + (double)(textCalculator->text[i] - '0');
            }
            else
            {
                __csd_push_back(&stackDecimal, (double)(textCalculator->text[i] - '0'));
            }
            stillNumber = true;
        }
        else
        {
            if (derivative)
            {
                if (textCalculator->text[i] >= 'a' && textCalculator->text[i] <= 'z')
                {
                    __csd_push_back(&stackDecimal, textCalculator->constant[textCalculator->text[i]-'a']);
                }
                int top = stackDecimal.top->value;
                __csd_pop_back(&stackDecimal);
                double value = 1;
                for (int i = 0; i < top; i++)
                {
                    value *= stackDecimal.top->value;
                    stackDecimal.top->value -= 1;
                }
                while(stackAscii.top && (
                        stackAscii.top->ascii != '(' &&
                        stackAscii.top->ascii != '+' &&
                        stackAscii.top->ascii != '-')
                        )
                {
                    textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                }
                __csc_push_back(&stackAscii, '*');
                __csd_push_back(&stackDecimal, value);
                derivative = false;
                if (textCalculator->text[i] >= 'a' && textCalculator->text[i] <= 'z')
                {
                    continue;
                }
            }
            if (textCalculator->text[i] == 'D')
            {
                derivative = true;
            }
            else if ((textCalculator->text[i] >= 'a' && textCalculator->text[i] <= 'z') && !stillNumber)
            {
                __csd_push_back(&stackDecimal, textCalculator->constant[textCalculator->text[i]-'a']);
            }
            else if (textCalculator->text[i] == ')')
            {
                while(stackAscii.top->ascii != '(')
                {
                    textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                }
                __csc_pop_back(&stackAscii);
            }
            else if (textCalculator->text[i] == '(')
            {
                __csc_push_back(&stackAscii, '(');
            }
            else if (textCalculator->text[i] == '%')
            {
                stackDecimal.top->value /= 100;
            }
            else if (textCalculator->text[i] == '!')
            {
                stackDecimal.top->value = factorial(stackDecimal.top->value);
            }
            else if (textCalculator->text[i] == '+')
            {
                if (!stackAscii.top || (stackAscii.top &&
                                        stackAscii.top->ascii == '(')
                                        )
                {
                    __csc_push_back(&stackAscii, '+');
                }
                else
                {
                    while(stackAscii.top && stackAscii.top->ascii != '(')
                    {
                        textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                    }
                    __csc_push_back(&stackAscii, '+');
                }
            }
            else if (textCalculator->text[i] == '-')
            {
                if (i == 0 || (i > 0 && (textCalculator->text[i-1] < '0' || textCalculator->text[i-1] > '9') &&
                               textCalculator->text[i-1] != ')' && (textCalculator->text[i-1] < 'a' || textCalculator->text[i-1] > 'z'))
                               )
                {
                    __csc_push_back(&stackAscii, '_');
                }
                else
                {
                    if (!stackAscii.top || (stackAscii.top &&
                                            stackAscii.top->ascii == '(')
                                            )
                    {
                        __csc_push_back(&stackAscii, '-');
                    }
                    else
                    {
                        while(stackAscii.top && stackAscii.top->ascii != '(')
                        {
                            textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                        }
                        __csc_push_back(&stackAscii, '-');
                    }
                }
            }
            else if (textCalculator->text[i] == '*' || (textCalculator->text[i] >= 'a' && textCalculator->text[i] <= 'z'))
            {
                if (!stackAscii.top || (stackAscii.top && (
                                        stackAscii.top->ascii == '(' ||
                                        stackAscii.top->ascii == '+' ||
                                        stackAscii.top->ascii == '-' ||
                                        stackAscii.top->ascii == '_'))
                                        )
                {
                    __csc_push_back(&stackAscii, '*');
                }
                else
                {
                    while(stackAscii.top && (
                                            stackAscii.top->ascii != '(' &&
                                            stackAscii.top->ascii != '+' &&
                                            stackAscii.top->ascii != '-' &&
                                            stackAscii.top->ascii != '_')
                                            )
                    {
                        textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                    }
                    __csc_push_back(&stackAscii, '*');
                }
                if (textCalculator->text[i] >= 'a' && textCalculator->text[i] <= 'z')
                {
                    __csd_push_back(&stackDecimal, textCalculator->constant[textCalculator->text[i]-'a']);
                }
            }
            else if (textCalculator->text[i] == '/')
            {
                if (!stackAscii.top || (stackAscii.top && (
                                        stackAscii.top->ascii == '(' ||
                                        stackAscii.top->ascii == '+' ||
                                        stackAscii.top->ascii == '-' ||
                                        stackAscii.top->ascii == '_'))
                                        )
                {
                    __csc_push_back(&stackAscii, '/');
                }
                else
                {
                    while(stackAscii.top && (
                                            stackAscii.top->ascii != '(' &&
                                            stackAscii.top->ascii != '+' &&
                                            stackAscii.top->ascii != '-' &&
                                            stackAscii.top->ascii != '_')
                                            )
                    {
                        textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                    }
                    __csc_push_back(&stackAscii, '/');
                }
            }
            else if (textCalculator->text[i] == '^')
            {
                if (!stackAscii.top || (stackAscii.top && (
                                        stackAscii.top->ascii == '(' ||
                                        stackAscii.top->ascii == '+' ||
                                        stackAscii.top->ascii == '-' ||
                                        stackAscii.top->ascii == '*' ||
                                        stackAscii.top->ascii == '/' ||
                                        stackAscii.top->ascii == '_'))
                                        )
                {
                    __csc_push_back(&stackAscii, '^');
                }
                else
                {
                    while(stackAscii.top && (
                                            stackAscii.top->ascii != '(' &&
                                            stackAscii.top->ascii != '+' &&
                                            stackAscii.top->ascii != '-' &&
                                            stackAscii.top->ascii != '*' &&
                                            stackAscii.top->ascii != '/' &&
                                            stackAscii.top->ascii != '_')
                                            )
                    {
                        textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                    }
                    __csc_push_back(&stackAscii, '^');
                }
            }
            else if (textCalculator->text[i] == '$')
            {
                if (!stackAscii.top || (stackAscii.top && (
                                        stackAscii.top->ascii == '(' ||
                                        stackAscii.top->ascii == '+' ||
                                        stackAscii.top->ascii == '-' ||
                                        stackAscii.top->ascii == '*' ||
                                        stackAscii.top->ascii == '/' ||
                                        stackAscii.top->ascii == '_'))
                                        )
                {
                    __csc_push_back(&stackAscii, '$');
                }
                else
                {
                    while(stackAscii.top && (
                                            stackAscii.top->ascii != '(' &&
                                            stackAscii.top->ascii != '+' &&
                                            stackAscii.top->ascii != '-' &&
                                            stackAscii.top->ascii != '*' &&
                                            stackAscii.top->ascii != '/' &&
                                            stackAscii.top->ascii != '_')
                                            )
                    {
                        textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
                    }
                    __csc_push_back(&stackAscii, '$');
                }
            }

            stillNumber = false;
        }
    }

    if (derivative)
    {
        int top = stackDecimal.top->value;
        __csd_pop_back(&stackDecimal);
        double value = 1;
        for (int i = 0; i < top; i++)
        {
            value *= stackDecimal.top->value;
            stackDecimal.top->value -= 1;
        }
        while(stackAscii.top && (
                stackAscii.top->ascii != '(' &&
                stackAscii.top->ascii != '+' &&
                stackAscii.top->ascii != '-')
                )
        {
            textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
        }
        __csc_push_back(&stackAscii, '*');
        __csd_push_back(&stackDecimal, value);
        derivative = false;
    }

    while(stackAscii.top)
    {
        textCalculatorProcessSymbol(&stackAscii, &stackDecimal);
    }

    double returnValue = stackDecimal.top->value;

    __cscea(&stackAscii);
    __csdea(&stackDecimal);

    return returnValue;
}

double textCalculatorCalculate(TextCalculator* textCalculator)
{
    textCalculator->value = textCalculatorProcess(textCalculator);
    return textCalculator->value;
}

double textCalculatorCalculateSigmaTreshold(TextCalculator* textCalculator, char sigmaVariable, double treshold)
{
    textCalculatorSetConstant(textCalculator, sigmaVariable, 0);
    double prevAprox = textCalculatorCalculate(textCalculator);
    textCalculatorSetConstant(textCalculator, sigmaVariable, 1);
    double currentAprox = prevAprox + textCalculatorCalculate(textCalculator);
    int currentIteration = 2;
    while(currentIteration == 2 || absolute(absolute(currentAprox) - absolute(prevAprox)) / absolute(currentAprox) > treshold)
    {
        prevAprox = currentAprox;
        textCalculatorSetConstant(textCalculator, sigmaVariable, currentIteration);
        currentAprox += textCalculatorCalculate(textCalculator);
        currentIteration += 1;
    }
    return currentAprox;
}

double textCalculatorCalculateSigmaOrde(TextCalculator* textCalculator, char sigmaVariable, int orde)
{
    textCalculatorSetConstant(textCalculator, sigmaVariable, 0);
    double prevAprox = textCalculatorCalculate(textCalculator);
    textCalculatorSetConstant(textCalculator, sigmaVariable, 1);
    double currentAprox = prevAprox + textCalculatorCalculate(textCalculator);
    if (orde == 1)
    {
        return prevAprox;
    }
    else if (orde == 2)
    {
        return currentAprox;
    }
    for (int i = 2; i < orde; i++)
    {
        prevAprox = currentAprox;
        textCalculatorSetConstant(textCalculator, sigmaVariable, i);
        currentAprox += textCalculatorCalculate(textCalculator);
    }
    return currentAprox;
}

double textCalculatorCalculateIntersection(TextCalculator *textCalculator, char intersectionVariable, double leftLimit, double rightLimit)
{
    textCalculatorSetConstant(textCalculator, intersectionVariable, leftLimit);
    double leftValue = textCalculatorCalculate(textCalculator);

    textCalculatorSetConstant(textCalculator, intersectionVariable, rightLimit);
    double rightValue = textCalculatorCalculate(textCalculator);

    if (leftValue >= 0 && rightValue <= 0)
    {
        double temp = leftLimit;
        leftLimit = rightLimit;
        rightLimit = temp;
    }
    else if (!(leftValue <= 0 && rightValue >= 0))
    {
        return 0;
    }

    double half = (leftLimit + rightLimit)/2;
    double prevhalf;
    textCalculatorSetConstant(textCalculator, intersectionVariable, half);
    double currentValue = textCalculatorCalculate(textCalculator);
    if (currentValue == 0)
    {
        return half;
    }
    else
    {
        if (currentValue < 0)
        {
            leftLimit = half;
        }
        else
        {
            rightLimit = half;
        }
    }
    do
    {
        prevhalf = half;
        half = (leftLimit+rightLimit)/2;
        textCalculatorSetConstant(textCalculator, intersectionVariable, half);
        currentValue = textCalculatorCalculate(textCalculator);
        if (currentValue < 0)
        {
            leftLimit = half;
        }
        else
        {
            rightLimit = half;
        }
    }
    while(absolute(absolute(half)-absolute(prevhalf))/absolute(half) > 0.0000001);
    return half;
}


double ln(double value)
{
    double returnValue = 0;

    TextCalculator textCalculator;
    textCalculatorInit(&textCalculator, "(x^n)/n!");

    double left = -50;
    double right = 50;
    double half = 0;
    textCalculatorSetConstant(&textCalculator, 'x', 1);
    double currentValue = textCalculatorCalculateSigmaOrde(&textCalculator, 'n', 101);
    textCalculatorSetConstant(&textCalculator, 'x', 0);
    double prevValue = textCalculatorCalculateSigmaOrde(&textCalculator, 'n', 101);
    while(absolute(absolute(currentValue)-absolute(prevValue))/absolute(currentValue) > 0.0000001)
    {
        half = (left+right)/2;
        prevValue = currentValue;
        textCalculatorSetConstant(&textCalculator, 'x', half);
        currentValue = textCalculatorCalculateSigmaOrde(&textCalculator, 'n', 101);
        if (currentValue - value == 0)
        {
            break;
        }
        else if (currentValue - value < 0)
        {
            left = half;
        }
        else
        {
            right = half;
        }
    }
    return half;
}

double logt(double value, double base)
{
    return ln(value)/ln(base);
}

double eConstantValue()
{
    static bool init = false;
    static double value = 0;
    if (!init)
    {
        TextCalculator textCalculator;
        textCalculator.text = "1/(x!)";
        double currentApprox = 1;
        double prevApprox = 0;
        double treshold = 0.000000001;
        int currentIteration = 1;
        while(((currentApprox - prevApprox) / currentApprox) > treshold)
        {
            prevApprox = currentApprox;
            textCalculatorSetConstant(&textCalculator, 'x', currentIteration);
            currentApprox += textCalculatorCalculate(&textCalculator);
            currentIteration += 1;
        }
        value = currentApprox;
        init = true;
    }
    return value;
}

double piConstanValue()
{
    static bool init = false;
    static double value = 0;
    if (!init)
    {
        TextCalculator textCalculator;
        textCalculator.text = "((-1)^x)/(x*2+1)";
        double currentApprox = 1;
        double prevApprox = 0;
        double treshold = 0.000001;
        int currentIteration = 1;
        while(absolute(absolute(currentApprox) - absolute(prevApprox))/absolute(currentApprox) > treshold)
        {
            prevApprox = currentApprox;
            textCalculatorSetConstant(&textCalculator, 'x', currentIteration);
            currentApprox += textCalculatorCalculate(&textCalculator);
            currentIteration += 1;
        }
        value = 4*currentApprox;
        init = true;
    }
    return value;
}

#if __cplusplus
}
#endif // __cplusplus

#endif // NUMERICCALCULATOR_H

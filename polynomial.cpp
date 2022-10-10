#include "polynomial.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cfloat>
#include <math.h>

using std::istream;
using std::ostream;
using std::string;
using std::stringstream;
using std::fixed;
using std::setprecision;
using std::showpos;
using std::pow;

Polynomial::Polynomial(size_t degree) : _degree(degree){
	_coefficients = new float[_degree + 1];
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = 0.0;
	}
}
Polynomial::Polynomial(size_t degree, const float* coefficients): _degree(degree){
	_coefficients = new float[_degree + 1];
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = coefficients[i];
	}
}
Polynomial::Polynomial(const Polynomial& polynomial): _degree(polynomial._degree){
	_coefficients = new float[_degree + 1];
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = polynomial._coefficients[i];
	}
}
Polynomial::~Polynomial(){
	// DO THIS FIRST TO PREVENT MEMORY LEAKS!
    delete[] _coefficients;
}
const Polynomial Polynomial::Sum(const Polynomial& rhs)const
{
    // makes a copy of this in retVal
    Polynomial retVal(*this);
    // checks which has higher degree and adds coefficients
    if(_degree > rhs._degree)
    {
        for(size_t i = 0; i < rhs._degree + 1; i++)
        {
            retVal._coefficients[i] += rhs._coefficients[i];
        }
    }
    else
    {
        //sets retVal to the higher degree (right-hand side in this case)
        retVal._degree = rhs._degree;
        for (size_t i = 0; i < _degree + 1; i++)
        {
            retVal._coefficients[i] += rhs._coefficients[i];
        }
    }

    return retVal;
}
const Polynomial Polynomial::Subtract(const Polynomial& rhs)const
{
    // makes a copy of this in retVal
    Polynomial retVal(*this);
    // checks which has higher degree and subtracts coefficients
    if(_degree > rhs._degree)
    {
        for(size_t i = 0; i < rhs._degree +1; i++)
        {
            retVal._coefficients[i] -= rhs._coefficients[i];
        }
    }
    else
    {
        retVal._degree = rhs._degree;
        for (size_t i = 0; i < _degree + 1; i++)
        {
            retVal._coefficients[i] -= rhs._coefficients[i];
        }
    }

    return retVal;
}
const Polynomial Polynomial::Minus()const{
	Polynomial retVal(*this);
	for (size_t i = 0; i < _degree + 1; i++) {
		retVal._coefficients[i] *= -1;
	}
	return retVal;
}
const Polynomial Polynomial::Multiply(const Polynomial& rhs)const{
    // makes retVal the size of both the polynomial arrays combined - 1
    Polynomial retVal(_degree + rhs._degree);

    // loops through the left side array and multiplys the each term by every term on the right side
    for(size_t i = 0; i < _degree + 1; i++)
    {
        // multiplies each term by every term on the right
        // ex. term1 * rhs.term1 ,, loop ,, term1 * rhs.term2 ,, etc...
        for(size_t j = 0; j < rhs._degree + 1; j++)
        {
            retVal._coefficients[i + j] += _coefficients[i] * rhs._coefficients[j];
        }
    }

    return retVal;
}
const Polynomial Polynomial::Divide(const Polynomial& rhs)const{
	return Polynomial(0);
}
const Polynomial Polynomial::Derive()const{
	Polynomial retVal(_degree - 1);

    for(size_t i = _degree; i > 0; i--)
    {
        retVal._coefficients[i-1] = _coefficients[i] * i;

    }

    return retVal;
}
float Polynomial::Evaluate(float x)const{
	float retVal = 0;

    // evaluates the polynomial with the given value x plugged in
    for(size_t i = 0; i < _degree + 1; i++)
    {
        // if degree is 0, just adds coefficient,
        // otherwise raises x to the degree, then multiplies it by the coefficient
        if(i == 0)
            retVal += _coefficients[i];
        else
            retVal += pow(x, i) * _coefficients[i];
    }

    return retVal;
}
float Polynomial::Integrate(float start, float end)const{
	return FLT_MAX;
}
const Polynomial& Polynomial::operator=(const Polynomial& rhs){
	if (&rhs == this){
		return *this;
	}
	if (_degree != rhs._degree){
		if (_coefficients){
			delete[] _coefficients;
		}
		_degree = rhs._degree;
		_coefficients = new float[_degree + 1];
	}
	for (size_t i = 0; i < _degree + 1; i++) {
		_coefficients[i] = rhs._coefficients[i];
	}
	return *this;
}
bool Polynomial::Equals(const Polynomial& rhs)const{
	if (_degree != rhs._degree){
		return false;
	}
	for (size_t i=0; i < _degree; i++){
		if (abs(_coefficients[i] - rhs._coefficients[i]) > 0.0001){
			return false;
		}
	}
	return true;
}
string Polynomial::ToString()const{
	stringstream ss;
	for (size_t i = _degree; i > 0; i--) {
		ss << showpos << fixed << setprecision(2) << _coefficients[i] << "x^" << i << " ";
	}
	ss << showpos << fixed << setprecision(2) << _coefficients[0];
	return ss.str();
}
ostream& Polynomial::Write(ostream& output)const{
	output << _degree << " ";
	for (size_t i = 0; i < _degree + 1; i++) {
		output << _coefficients[i] << " ";
	}
	return output;
}
istream& Polynomial::Read(istream& input){
	size_t degree;
	input >> degree;
	if (input.fail()){
		return input;
	}
	float* coefficients = new float[degree + 1];
	for (size_t i = 0; i < degree + 1; i++) {
		input >> coefficients[i];
		if (input.fail()){
			delete[] coefficients;
			return input;
		}
	}

	if (degree != _degree){
		if (_coefficients){
			delete[] _coefficients;
		}
		_degree = degree;
		_coefficients = coefficients;
	}else{
		for (size_t i = 0; i < _degree + 1; i++) {
			_coefficients[i] = coefficients[i];
		}
		delete[] coefficients;
	}
	return input;
}

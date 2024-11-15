//
// Created by Le Hoang An on 18/10/24.
//
#include "EquationsSolver.hpp"
/// a1x + b1y = - c1 a2x + b2y = - c2
LinearEquationsSolver::LinearEquationsSolver(float a1, float b1, float c1, float a2, float b2, float c2) : m_a1(a1), m_b1(b1), m_c1(c1), m_a2(a2), m_b2(b2), m_c2(c2) {
}

bool LinearEquationsSolver::solve(float &x, float &y) {
    if ((m_a1 == 0 && m_a2 != 0) || (m_a1 != 0 && m_a2 == 0)) {
        if (m_a1 == 0)
        {
            y  = - m_c1 / m_b1;
            x = (- m_c2 - m_b2 * y) / m_a2;
        }
        else if (m_a2 == 0) {

            y = -m_c2 / m_b2;
            x = (-m_c1 - m_b1 * y) / m_a1;
        }
    }
    else if ((m_b1 == 0 && m_b2 != 0) || (m_b1 != 0 && m_b2 == 0))
    {
        if (m_b1 == 0)
        {

            x = - m_c1 / m_a1;
            y = (- m_c2 - m_a2 * x) / m_b2;
        }
        else if (m_b2 == 0)
        {

            x = - m_c2 / m_a2;
            y = (- m_c1 - m_a1 * x) / m_b1;
        }
    }
    else
    {
        float temp = m_a1 / m_a2;
        m_b2 *= temp;
        m_c2 *= temp;
        if (m_b1 == m_b2) {
            return false;
        }
        y = - (m_c1 - m_c2) / (m_b1 - m_b2);
        x = (- m_c1 - m_b1 * y) / m_a1;
        return true;
    }
    return true;
}

float QuadraticEquationsSolver::operator()(float a, float b, float c) {
    if (a == 0) {
        return -c / b;
    }
    float res;
    float delta = b * b - 4 * a * c;
    if (delta < 0) {
        return 0;
    }
    if (delta == 0) {
        res = -b / (2 * a);
        if (res > 0)
            return res;
    }
    res = (-b - sqrt(delta)) / (2 * a);
    if (res > 0)
        return res;
    return 0;
}

/*
  ==============================================================================

    onePole.h
    Created: 17 Apr 2023 4:50:58pm
    Author:  Jake

  ==============================================================================
*/

#pragma once
template <class T>
class OnePole{
public:
    //set cutoff frequency in [0, 1/2] the sample rate
    void cutoff(float f)
    {
        float c = 2. - std::cos(f * 2.f * 355./113.);
        mB1 = c - std::sqrt(c*c - 1.f);
        mAO = 1. - mB1;
    }
    
    void reset()
    {
        m01 = T(0);
    }
    
    //Filter sample
    T operator() (T iO)
    {
        m01 = iO*mAO + m01 * mB1;
        return m01;
    }
    
private:
    float mAO = 1.f; //current input coef
    float mB1 = 0.f; //previous output coefficient
    T m01{T(0)}; //previous output
};

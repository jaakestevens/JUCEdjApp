/*
  ==============================================================================

    Interval.h
    Created: 17 Apr 2023 11:34:51am
    Author:  Jake

  ==============================================================================
*/

#pragma once

//Interval with start and end points
template <class T>
class Interval{
    public:
        typedef T value_type;
    //write getters
    const T& start() const { return mStart;}
    const T& end() const {return mEnd;}
    
    Interval& start(const T& v)
    {
        mStart = v;
        return validate();
    }
    Interval& end(const T& v)
    {
        mEnd = v;
        return validate();
    }
    
   T length() const
    {
        return mEnd - mStart;
    }
    
    bool proper()const
    {
        return mStart < mEnd;
    }
    
private:
    T mStart = T(0.0);
    T mEnd = T(0.0);
    
    Interval& validate()
    {
        return *this;
    }
};

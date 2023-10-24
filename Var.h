#ifndef VAR_H
#define VAR_H

#define MAX_VARS 32

#include <string>

template <class T>
class Var
{
public:
    Var(const T &var, const std::string &var_name)
    {
        _var = var;

        _registered_var_names[_var_idx] = var_name;
        _registered_vars[_var_idx] = this;
        _var_idx++;
    }

    void set(const T &var)
    {
        _var = var;
    }

    const T &get()
    {
        return _var;
    }

    static std::string getInstancesName(const uint8_t &idx);

    static uint8_t getInstancesCount();

private:
    T _var;

    static std::string _registered_var_names[MAX_VARS];
    static Var<T> *_registered_vars[MAX_VARS];
    static uint8_t _var_idx;
};

template <typename T>
uint8_t Var<T>::_var_idx = 0;

template <typename T>
std::string Var<T>::_registered_var_names[MAX_VARS];

template <typename T>
Var<T> *Var<T>::_registered_vars[MAX_VARS];

template <typename T>
std::string Var<T>::getInstancesName(const uint8_t &idx)
{   
    if(idx >= _var_idx)
        return _registered_var_names[_var_idx -1];

    return _registered_var_names[idx];
}

template <typename T>
uint8_t Var<T>::getInstancesCount()
{
    return _var_idx;
}

#endif
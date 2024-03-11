#ifndef PID_H
#define PID_H

class PID{

    public:

    struct K
    {
        float p;
        float i;
        float d;
        /* data */
    };
    

    PID(float* feedback, float* target, const K& k, const float& Ts, const float& minControl, const float& maxControl){
        _feedback = feedback;
        _target = target;
        _error_sum = 0;
        _k = k;
        _ts = Ts;
        NormaliseKParametersTs();

        _MAX_CONTROL = maxControl;
        _MIN_CONTROL = minControl;
    }

    void setKParams(K k){
        _k = k;
        NormaliseKParametersTs();
    }

    float getMinControl(){
        return _MIN_CONTROL;
    }

    float getMaxControl(){
        return _MAX_CONTROL;
    }

    void setMinControl(const float& minControl){
        _MIN_CONTROL = minControl;
    }

    void setMaxControl(const float& maxControl){
        _MAX_CONTROL = maxControl;
    }

    const K getKParams(){
        DenormaliseKParametersTs();
        K out = {_k.p, _k.i, _k.d};
        NormaliseKParametersTs();
        return out;
    }


    float computeControl(){

        _error = *_target - *_feedback; // For p
        _error_sum += _k.i *_error; // For i
        float _d_feedback = *_feedback - _feedback_previous; // For d


        _control = (_k.p * _error) + (_error_sum) - (_k.d * _d_feedback);

        _error_previous = _error;
        _feedback_previous = *_feedback;


        if(_control > _MAX_CONTROL){
            _control =  _MAX_CONTROL;
        }
        else if(_control < _MIN_CONTROL){
            _control = _MIN_CONTROL;
        }

        return _control;

    }

    float getError(){
        return _error;
    }

    float getControl(){
        return _control;
    }


    void restart(){
        _feedback_previous = *_feedback;
        _error_sum = _control;
    }


    private:

    inline void NormaliseKParametersTs(){
        _k.i *= _ts;
        _k.d /= _ts;
    }

    inline void DenormaliseKParametersTs(){
        _k.i /= _ts;
        _k.d /= _ts;
    }

    float* _feedback;
    float _feedback_previous;

    float* _target;

    float _MAX_CONTROL;
    float _MIN_CONTROL;

    float _ts;
    K _k;
    float _error;
    float _error_sum;
    float _error_previous;
    float _control;

};



#endif
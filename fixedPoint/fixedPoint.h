#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#define FIX_SHIFT 14
#define FIX_MUL (1L<<FIX_SHIFT)
#define FIX_UINT(x) ((x) << FIX_SHIFT)
#define FIX_INT(x) ((x) * FIX_MUL)
#define FIX_FLOAT(x) ((x) * FIX_MUL)
#define expandFix(x) x.fix / FIX_MUL
#define FIX(x) (int32_t&)x

class intFP {
public:    
    int32_t fix = 0;
    intFP(){}
    //intFP(intFP& val) {fix = val.fix;}
    intFP(uint32_t val) {fix = FIX_UINT(val);}
    intFP(int32_t val) {fix = FIX_INT(val);}
    intFP(uint16_t val) {fix = FIX_UINT(val);}
    intFP(int16_t val) {fix = FIX_INT(val);}
    intFP(uint8_t val) {fix = FIX_UINT(val);}
    intFP(int8_t val) {fix = FIX_INT(val);}
    intFP(double val) {fix = FIX_FLOAT(val);}
    intFP(int32_t val1, int32_t val2) {fix = FIX_INT(val1) / val2;}

    intFP& operator = (uint32_t val) {fix = FIX_UINT(val);}
    intFP& operator = (int32_t val) {fix = FIX_INT(val);}
    intFP& operator = (uint16_t val) {fix = FIX_UINT(val);}
    intFP& operator = (int16_t val) {fix = FIX_INT(val);}
    intFP& operator = (uint8_t val) {fix = FIX_UINT(val);}
    intFP& operator = (int8_t val) {fix = FIX_INT(val);}
    intFP& operator = (double val) {fix = FIX_FLOAT(val);}

    intFP& operator += (intFP val) {fix += val.fix; return *this;}
    intFP& operator += (uint32_t val) {fix += FIX_UINT(val); return *this;}
    intFP& operator += (int32_t val) {fix += FIX_INT(val); return *this;}
    intFP& operator += (uint16_t val) {fix += FIX_UINT(val); return *this;}
    intFP& operator += (int16_t val) {fix += FIX_INT(val); return *this;}
    intFP& operator += (uint8_t val) {fix += FIX_UINT(val); return *this;}
    intFP& operator += (int8_t val) {fix += FIX_INT(val); return *this;}
    intFP& operator += (double val) {fix += FIX_FLOAT(val); return *this;}

    intFP operator + (intFP val) {return intFP(*this) += val;}
    intFP operator + (uint32_t val) {return intFP(*this) += val;}
    intFP operator + (int32_t val) {return intFP(*this) += val;}
    intFP operator + (uint16_t val) {return intFP(*this) += val;}
    intFP operator + (int16_t val) {return intFP(*this) += val;}
    intFP operator + (uint8_t val) {return intFP(*this) += val;}
    intFP operator + (int8_t val) {return intFP(*this) += val;}
    intFP operator + (double val) {return intFP(*this) += val;}

    intFP& operator -= (intFP val) {fix -= val.fix; return *this;}
    intFP& operator -= (uint32_t val) {fix -= FIX_UINT(val); return *this;}
    intFP& operator -= (int32_t val) {fix -= FIX_INT(val); return *this;}
    intFP& operator -= (uint16_t val) {fix -= FIX_UINT(val); return *this;}
    intFP& operator -= (int16_t val) {fix -= FIX_INT(val); return *this;}
    intFP& operator -= (uint8_t val) {fix -= FIX_UINT(val); return *this;}
    intFP& operator -= (int8_t val) {fix -= FIX_INT(val); return *this;}
    intFP& operator -= (double val) {fix -= FIX_FLOAT(val); return *this;}

    intFP operator - (intFP val) {return intFP(*this) -= val;}
    intFP operator - (uint32_t val) {return intFP(*this) -= val;}
    intFP operator - (int32_t val) {return intFP(*this) -= val;}
    intFP operator - (uint16_t val) {return intFP(*this) -= val;}
    intFP operator - (int16_t val) {return intFP(*this) -= val;}
    intFP operator - (uint8_t val) {return intFP(*this) -= val;}
    intFP operator - (int8_t val) {return intFP(*this) -= val;}
    intFP operator - (double val) {return intFP(*this) -= val;}

    intFP& operator *= (intFP val) {fix *= val.fix; return *this;}
    intFP& operator *= (uint32_t val) {fix *= (val); return *this;}
    intFP& operator *= (int32_t val) {fix *= (val); return *this;}
    intFP& operator *= (uint16_t val) {fix *= (val); return *this;}
    intFP& operator *= (int16_t val) {fix *= (val); return *this;}
    intFP& operator *= (uint8_t val) {fix *= (val); return *this;}
    intFP& operator *= (int8_t val) {fix *= (val); return *this;}
    intFP& operator *= (double val) {fix *= (val); return *this;}

    intFP operator * (intFP val) {return intFP(*this) *= val;}
    intFP operator * (uint32_t val) {return intFP(*this) *= val;}
    intFP operator * (int32_t val) {return intFP(*this) *= val;}
    intFP operator * (uint16_t val) {return intFP(*this) *= val;}
    intFP operator * (int16_t val) {return intFP(*this) *= val;}
    intFP operator * (uint8_t val) {return intFP(*this) *= val;}
    intFP operator * (int8_t val) {return intFP(*this) *= val;}
    intFP operator * (double val) {return intFP(*this) *= val;}

    intFP& operator /= (intFP val) {fix /= val.fix; return *this;}
    intFP& operator /= (uint32_t val) {fix /= (val); return *this;}
    intFP& operator /= (int32_t val) {fix /= (val); return *this;}
    intFP& operator /= (uint16_t val) {fix /= (val); return *this;}
    intFP& operator /= (int16_t val) {fix /= (val); return *this;}
    intFP& operator /= (uint8_t val) {fix /= (val); return *this;}
    intFP& operator /= (int8_t val) {fix /= (val); return *this;}
    intFP& operator /= (double val) {fix /= (val); return *this;}

    intFP operator / (intFP val) {return intFP(*this) /= val;}
    intFP operator / (uint32_t val) {return intFP(*this) /= val;}
    intFP operator / (int32_t val) {return intFP(*this) /= val;}
    intFP operator / (uint16_t val) {return intFP(*this) /= val;}
    intFP operator / (int16_t val) {return intFP(*this) /= val;}
    intFP operator / (uint8_t val) {return intFP(*this) /= val;}
    intFP operator / (int8_t val) {return intFP(*this) /= val;}
    intFP operator / (double val) {return intFP(*this) /= val;}

    intFP& operator %= (intFP val) {fix %= val.fix; return *this;}
    intFP& operator %= (uint32_t val) {fix %= FIX_UINT(val); return *this;}
    intFP& operator %= (int32_t val) {fix %= FIX_INT(val); return *this;}
    intFP& operator %= (uint16_t val) {fix %= FIX_UINT(val); return *this;}
    intFP& operator %= (int16_t val) {fix %= FIX_INT(val); return *this;}
    intFP& operator %= (uint8_t val) {fix %= FIX_UINT(val); return *this;}
    intFP& operator %= (int8_t val) {fix %= FIX_INT(val); return *this;}

    intFP operator % (intFP val) {return intFP(*this) %= val;}
    intFP operator % (uint32_t val) {return intFP(*this) %= val;}
    intFP operator % (int32_t val) {return intFP(*this) %= val;}
    intFP operator % (uint16_t val) {return intFP(*this) %= val;}
    intFP operator % (int16_t val) {return intFP(*this) %= val;}
    intFP operator % (uint8_t val) {return intFP(*this) %= val;}
    intFP operator % (int8_t val) {return intFP(*this) %= val;}

    int32_t toInt() {return (fix >= 0) ? (fix >> FIX_SHIFT) : ((int32_t)fix / FIX_MUL);}
    float toFloat() {return (float)fix / FIX_MUL;}  
private:
};

intFP toFix(double val) {intFP buf(val); return buf;}
intFP toFix(uint32_t val) {intFP buf(val); return buf;}
intFP toFix(int32_t val) {intFP buf(val); return buf;}
intFP toFix(uint16_t val) {intFP buf(val); return buf;}
intFP toFix(int16_t val) {intFP buf(val); return buf;}
intFP toFix(uint8_t val) {intFP buf(val); return buf;}
intFP toFix(int8_t val) {intFP buf(val); return buf;}
intFP toFix(int val1, int val2) {intFP buf(val1, val2); return buf;}
#endif
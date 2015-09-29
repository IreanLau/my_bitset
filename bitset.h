#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

//#define _THROW(x, y)	throw x(y)

template<size_t _N>	//无类型参数
class bitset
{
	typedef unsigned long _Ty;
public:
	typedef bool element_type;

	/*class reference 用于对已经构造的Bitset中某一位进行操作*/
	class reference {
		friend class bitset<_N>;
	public:
		reference& operator=(bool _X)
		{
			_Pbs->set(_Off,_X);
			return (*this);
		}
		reference& operator=(const reference& _Bs)
		{
			_Pbs->set(_Off,(bool)_Bs);
			return (*this);
		}
		reference& flip()
		{
			_Pbs->flip(_Off);
			return (*this);
		}
		bool operator~() const
		{
			return (!_Pbs->test(_Off));
		}
		operator bool() const
		{
			return (_Pbs->test(_Off));
		}
	private:
		reference(bitset<_N>& _X, size_t _P)
			: _Pbs(&_X), _Off(_P) {}
		bitset<_N> *_Pbs;
		size_t _Off;
	};


public:
	bool at(size_t _P) const
	{
		if (_N <= _P)
			_Xran();
		return (test(_P));
	}
	reference at(size_t _P)
	{
		if (_N <= _P)
			_Xran();
		return (reference(*this, _P));
	}
	bool operator[](size_t _P) const
	{
		return (test(_P));
	}
	reference operator[](size_t _P)
	{
		return (reference(*this, _P));
	}
public:
	bitset()
	{
		_Tidy();
	}

	bitset(unsigned long _X)
	{
		_Tidy();
		for (size_t _P = 0; _X != 0 && _P < _N; _X >>= 1, ++_P)
		{
			if (_X &(_Ty)1)
				set(_P);
		}
	}


	/*字符串从右向左插入*/
	explicit bitset(const string& _S, size_t _P = 0,
		size_t _L = (size_t)(-1))
	{
		size_t _I;
		if (_S.size() < _P)
			_Xran();
		if (_S.size() - _P < _L)
			_L = _S.size() - _P;	/*控制边界*/
		if (_N < _L)
			_L = _N;	/*控制边界*/
		_Tidy(), _P += _L;
		for (_I = 0; _I < _L; ++_I)
			if (_S[--_P] == '1')
				set(_I);
			else if (_S[_P] != '0')/*不为0或1则抛异常*/
				_Xinv();
	}

	bitset<_N>& operator&=(const bitset<_N>& _R)
	{
		for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] |= _R._W(_I);
		return (*this);
	}

	bitset<_N>& operator|=(const bitset<_N>& _R)
	{
		for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] |= _R._W(_I);/*将拆分开的数组逐个操作*/
		return (*this);
	}
	bitset<_N>& operator^=(const bitset<_N>& _R)
	{
		for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] ^= _R._W(_I);
		return (*this);
	}


	/*先以32bit为单元移动，再以bit为单位移动*/
	bitset<_N>& operator<<=(size_t _P)
	{
		if (_P < 0)
			return (*this >>= -_P);
		const int _D = _P / _Nb;
		if (_D != 0)
			for (int _I = _Nw; 0 <= _I; --_I)
				_A[_I] = _D <= _I ? _A[_I - _D] : 0;	/*移动位数 > 限定位数，（为0）元素置0,否则将A[i-1]赋值给a[i]*/
		if ((_P %= _Nb) != 0)
		{
			for (int _I = _Nw; 0 < _I; --_I)
				_A[_I] = (_A[_I] << _P)
				| (_A[_I - 1] >> (_Nb - _P));	/*解决[i]的尾部，和[i-1]头都有数据的情况*/
			_A[0] <<= _P, _Trim();
		}
		return (*this);
	}

	bitset<_N>& operator>>=(size_t _P)
	{
		if (_P < 0)
			return (*this <<= -_P);
		const int _D = _P / _Nb;
		if (_D != 0)
			for (int _I = 0; _I <= _Nw; ++_I)
				_A[_I] = _D <= _Nw - _I ? _A[_I + _D] : 0;/*移动位数 > 限定位数，（为0）元素置0,否则将A[i]赋值给a[i-1]*/
		if ((_P %= _Nb) != 0)
		{
			for (int _I = 0; _I < _Nw; ++_I)
				_A[_I] = (_A[_I] >> _P)
				| (_A[_I + 1] << (_Nb - _P));	/*解决[i]的尾部，和[i-1]头都有数据的情况*/
			_A[_Nw] >>= _P;
		}
		return (*this);
	}

	/*所有位置1*/
	bitset<_N>& set()
	{
		/*置数*/
		_Tidy(~(_Ty)0);
		return *this;
	}

	bitset<_N>& set(size_t _P, bool _X = true)
	{
		if (_P >= _N)
			_Xran();
		if (_X)
			_A[_P / _Nb] |= ((_Ty)1 << (_P % _Nb));	// | 置数
		else
			_A[_P / _Nb] &= ~((_Ty)1 << (_P % _Nb));// & 清零

		return (*this);
	}



	size_t my_count()const
	{
		size_t sum = 0;
		size_t count = 0;
		for (int _I = _Nw; 0 <= _I; --_I)
		{
			_Ty _X = _A[_I];
			count = 0;
			while (_X)
			{
				count++;
				_X &= (_X - 1);
			}
			sum += count;
		}
		return sum;
	}

	size_t count()const
	{
		size_t _V = 0;
		for (int _I = _Nw; 0 <= _I; --_I)
			for (_Ty _X = _A[_I]; _X != 0; _X >>= 4)
				_V += "\0\1\1\2\1\2\2\3"
				"\1\2\2\3\2\3\3\4"[_X & 0xF];	//   \的作用 ： 字符转数字（\0 就是 0）
		return (_V);
	}

	/*测试某一位是否为1*/

	bool test(size_t _P)const
	{
		if (_P >= _N)
			_Xran();
		return ((_A[_P / _Nb] & ((_Ty)1 << _P % _Nb)) != 0);
	}

	bitset<_N>& reset()
	{
		_Tidy();
		return (*this);
	}


	bitset<_N>& reset(size_t _P)
	{
		return (set(_P, 0));
	}
	bitset<_N> operator~() const
	{
		return (bitset<_N>(*this).flip());
	}



	/*按位取反*/
	bitset<_N>& flip()
	{
		for (int _I = _Nw; _I >= 0; --_I)
			_A[_I] = ~_A[_I];
		_Trim();
		return (*this);
	}

	bitset<_N>& flip(size_t _P) //_P 下标
	{
		if (_N <= _P)
			_Xran();
		_A[_P / _Nb] ^= (_Ty)1 << _P % _Nb;
		return (*this);
	}

	/*判断是否有1出现*/
	bool any() const
	{
		for (int _I = _Nw; 0 <= _I; --_I)
			if (_A[_I] != 0)
				return (true);
		return (false);
	}

	bool none() const
	{
		//return (*this) == 0;
		return (!any());
	}

	size_t size() const
	{
		return (_N);
	}

	bool operator==(const bitset<_N>& _R) const
	{
		for (int _I = _Nw; 0 <= _I; --_I)
			if (_A[_I] != _R._W(_I))
				return (false);
		return (true);
	}

	bool operator!=(const bitset<_N>& _R) const
	{
		return (!(*this == _R));
	}


	unsigned long to_ulong() const
	{
		enum {
			_Assertion = 1 /
			(sizeof(unsigned long) % sizeof(_Ty) == 0)
		};/*防止自定义类型Ty > ulong;或不等于ulong*/
		int _I = _Nw;
		for (; (sizeof(unsigned long) / sizeof(_Ty)) <= _I; --_I)
			if (_A[_I] != 0)		/*如果超出32位，不能转化，abort*/
					_Xoflo();	
			unsigned long _V = _A[_I];
		for (; 0 <= --_I; )
			_V = _V << _Nb | _A[_I];	/*<< 优先于 |  防止系统的unsigned long，是自定义 _Ty的倍数*/
		return (_V);
	}

	string to_string() const
	{
		string _S;
		_S.reserve(_N);
		for (size_t _P = _N; 0 < _P; )
			_S += test(--_P) ? '1' : '0';/*控制下标，前置--*/
		return (_S);
	}



	friend ostream& operator<<(ostream& _O, const bitset<_N>& _R)
	{
		for (size_t _P = _N; 0 < _P; )
			_O << (_R.test(--_P) ? '1' : '0');
		return (_O);
	}

	
private:
	enum {
		_Nb = CHAR_BIT * sizeof(_Ty),	
		_Nw = _N == 0 ? 0 : (_N - 1) / _Nb
	};	//num of word,num of bit
	

	/*置数*/
	void _Tidy(_Ty _X = 0)
	{
		for (int _I = _Nw; _I >= 0; --_I)
			_A[_I] = _X;
		if (_X != 0)
			_Trim();
	}


	/*把超出范围的置数 重新赋值为0*/
	void _Trim()
	{
		if (_N%_Nb != 0)
			_A[_Nw] &= ((_Ty)1 << _N%_Nb) - 1;
	}

	void _Xinv() const
	{
		_THROW(invalid_argument, "invalid bitset<N> char");
	}
	void _Xoflo() const
	{
		_THROW(overflow_error,
			"bitset<N> conversion overflow");
	}
	void _Xran() const
	{
		_THROW(out_of_range, "invalid bitset<N> position");
	}

	_Ty _A[_Nw + 1];
};

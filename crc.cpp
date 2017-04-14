#include<vector>
#include "crc.hpp"

using std::vector;




class Polinom{
	int degree;
	vector<bool> polinom;
public:
	Polinom(const char*  _polinom,int len):polinom(len*8,false){
		for(int i=0; i<len*8; i++){
			polinom[i] = (_polinom[i/8] & (1<<(i%8)))>>(i%8);
		}
		for(int i=len*8-1;i >=0; i--){
			if(polinom[i]){
				degree=i;
				break;
			}
		}
	}
	Polinom(unsigned int _polinom,int _degree):degree(_degree),polinom(_degree,false){
		for(int i=0; i<sizeof(int); i++){
			polinom[i] = (_polinom & (1<<i))>>i;
		}
		polinom[_degree] = true;
	}
	Polinom():degree(0),polinom(1,false){}
	friend bool operator<(Polinom A,Polinom B){
		if(A.degree > B.degree)
			return true;
		if(A.degree < B.degree)
			return false;
		int degree =A.degree;
		for(int i=degree-1; i>=0; i++){
			if(A.polinom[i]&&!B.polinom[i])
				return true;
		}
		return false;
	}

	friend bool operator==(Polinom A,Polinom B){
		return !((A<B)||(B<A));
	}

	Polinom operator<<(int shift){
		Polinom res = *this;
		res.degree += shift;
		res.polinom.insert(res.polinom.begin(),vector<false>(shift,false));
		return res;
	}

	Polinom operator-(Polinom p){
		Polinom res = *this;
		int deg  = this->degree < p.degree ? this->degree : p.degree ;
		for(int i=deg; i >= 0 ; i--){
			res[i] = (this->polinom[i])^(p.polinom[i]);
			if(i == res.degree && !res[i]){
				res.degree--;
			}
		}
		return res;
	}

	Polinom operator%(Polinom p){
		Polinom r = *this;
		int shift;
		while(  r.degree != 0 && r.degree >= p.degree ){
			shift = r.degree - p.degree;
			r = r - (p << shift);
		}
		return r;
	}

	void toChar(unsigned char* buff){
		for(int i=0;i<this->degree;i+=8){
			char c = 0;
			for(int j=0;j<8;j++){
				c = c+(polinom[i+j])<<j;
			}
			buff[i] = c;
		}
};


/*
 * param msg - Pointer to message to encode
 * param len - Length of message in bytes
 * p - Polynom value
 * g - Polynom grade
 * Output param e_msg - Pointer to message with CRC
 * Return: void
 * */
void encode(unsigned char* msg, int len, unsigned int p, int g ,unsigned char* e_msg){
	Polinom M(msg,len),G(p,g);
	Polinom T = (M<<g) - ((M<<g)%G);
	T.toChar(e_msg);
}

/*
 * param msg - Pointer to message with CRC
 * param len - Length of message
 * p - Polynom value
 * g - Polynom grade
 * Return:  1 - CRC is OK, 0 - CRC Error
 * */
bool validate(unsigned char* msg, int len, unsigned int p, int g){
	Polinom zero,G(p,g),M(msg,len);
	return (zero == (M%G));
}

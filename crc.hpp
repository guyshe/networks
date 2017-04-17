#include<vector>
#include<iostream>
#include<cassert>
#include<cstdio>
using std::vector;
using std::cout;
using std::endl;

#ifndef CRC_HPP_
#define CRC_HPP_


void encode(unsigned char* msg, int len, unsigned int p, int g ,unsigned char* e_msg);

bool validate(unsigned char* msg, int len, unsigned int p, int g);

class Polinom{
	int degree;
	vector<bool> polinom;
	public:
		Polinom(unsigned char*  _polinom,int len):polinom(len*8,false),degree(-1){
			for(int i=0; i<len*8; i++){
				polinom[i] = (_polinom[i/8] & (1<<(i%8)))>>(i%8);
				// if(i==0)
				// printf("%x & %x = %d\n",(_polinom[i/8]),(1<<(i%8)),(_polinom[i/8] & (1<<(i%8))));
			}
			for(int i=len*8-1;i >=0; i--){
				if(polinom[i]){
					degree=i;
					break;
				}
			}
		}
		Polinom(unsigned int _polinom,int _degree):degree(_degree),polinom(_degree+1,false){
			for(int i=0; i<_degree; i++){
				polinom[i] =  (_polinom & (1<<i))>>i;
			}
			polinom[_degree]=true;
		}
		Polinom():degree(-1),polinom(1,false){}

		friend bool operator<(Polinom A,Polinom B){
			if(A.degree < B.degree)
				return true;
			if(A.degree > B.degree)
				return false;
			int degree = A.degree;
			for(int i=degree-1; i>=0; i--){
				if(!A.polinom[i]&&B.polinom[i])
					return true;
				if(A.polinom[i]&&!B.polinom[i])
					return false;
			}
			return false;
		}

		friend bool operator==(Polinom A,Polinom B){
			return !((A<B)||(B<A));
		}

		Polinom operator<<(int shift){
			Polinom res = *this;
			res.degree += shift;
			vector<bool> tmp(shift,false);
			(res.polinom).insert((res.polinom).begin(),tmp.begin(),tmp.end());
			return res;
		}

		friend Polinom operator-(Polinom a,Polinom b){
			Polinom res = a.degree < b.degree  ? b : a ;
			int deg  = a.degree < b.degree ? a.degree : b.degree ;
			for(int i=deg; i >= 0 ; i--){
				res.polinom[i] = (a.polinom[i])^(b.polinom[i]);

			}
			res.degree = -1;
			for(int i=(a.degree + b.degree - deg); i >= 0; i--){
				if(res.polinom[i]){
					res.degree = i;
					break;
				}
			}
			return res;
		}

		Polinom operator%(Polinom p){
			Polinom r = *this;
			int shift;
			while( r.degree != 0 &&  r.degree >= p.degree ){
				shift = r.degree - p.degree;
				// r.print();
				// (p << shift).print();
				r = r - (p << shift);
			}
			return r;
		}
		void print(){
			for(int i=degree;i>=0;i--){
				std::cout<<polinom[i];
			}
			std::cout<<std::endl;
		}
		void toChar(unsigned char* buff){
			for(int i=0;i<=this->degree;i+=8){
				char c = 0;
				for(int j=0;j<8;j++){
                    if(i+j>this->degree){
                        break;
                    }
					c = c+((polinom[i+j])<<j);
				}
				buff[i/8] = c;
			}
		}
		int getDegree(){
			return this->degree;
		}
};




#endif /* CRC_HPP_ */

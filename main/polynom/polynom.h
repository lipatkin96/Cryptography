//
// Created by artem on 8/9/16.
//

#ifndef CRYPTOGRAPHY_POLYNOMIAL_H
#define CRYPTOGRAPHY_POLYNOMIAL_H

#include <vector>
#include <iostream>

namespace Polynomial {

	template <class T>
	class Polynom {

	private:
		vector<T> polynom;

	public:
		void move(Polynom &&temp) {
			polynom = move(temp.polynom);
		}

		explicit
		Polynom(){
			polynom.push_back(static_cast<T>(0));
		}
		explicit
		Polynom(const vector<T> &vec) {

			for (int i = 0; i < vec.size(); i++)
				polynom.push_back(vec[i]);
		}
		explicit
		Polynom(const size_t size) : polynom(size) {}
		//copy constructor
		Polynom(const Polynom &obj) : polynom(obj.polynom) {}
		//move constructor
		Polynom(Polynom &&temp) { this->move(temp); }

		/*---------------Operators-------------*/

//why cannot do it?			- 	because such occasion cannot happen
		/*const Polynom& operator= (Polynom &&temp) {
			if ((*this) != temp) {
				cout << &temp.polynom[0] << " ";
				this->move(temp);
				cout << &polynom[0] << " ";
				//custom = temp.custom;
			}
			return (*this);
		}*/

		const Polynom& operator= (const Polynom &obj) {
			if ((*this) != obj) {
				cout << &obj.polynom[0] << " ";
				polynom = obj.polynom;
				cout << &polynom[0] << " ";
			}
			return (*this);
		}

		Polynom operator+ (const Polynom &obj) const {
			const Polynom &longPolynom = (size() > obj.size()) ? (*this) : obj;
			const Polynom &shortPolynom = (size() <= obj.size()) ? (*this) : obj;
			Polynom sum(longPolynom);

			for (int i = 0; i < shortPolynom.size(); i++)
				sum[i] += shortPolynom[i];

							//the power of polynom can decrease by addition
			size_t newSize = sum.size();
			for (size_t i = sum.size() - 1; (i >= 0) && (sum[i] == 0); i--)
				--newSize;

			sum.resize(newSize);

			return sum;
		};

		const Polynom& operator+= (const Polynom &obj) { return ((*this) = ((*this) + obj)); }

		Polynom operator- () const {
			Polynom inverse(size());

			for (int i = 0; i < size(); i++)
				inverse[i] = -polynom[i];

			return inverse;
		}

		Polynom operator- (const Polynom &obj) const { return (*this) + (-obj); }

		Polynom operator* (const Polynom &obj) const {
			Polynom mul((size() - 1) + (obj.size() - 1) + 1);

			for (int i = 0; i < size(); i++)
				for (int j = 0; j < obj.size(); j++)
					mul[i + j] += (*this)[i] * obj[j];

			return mul;
		}

		const Polynom& operator*= (const Polynom &obj) { return ((*this) = ((*this) * obj)); }

		const T &operator[](const int i) const {
			if (i < 0 || i >= polynom.size())
				throw out_of_range("Polynom Exception: out of range");
			return polynom[i];
		}
				//code duplication - bad!!!
		T &operator[](const int i) {
			if (i < 0 || i >= polynom.size())
				throw out_of_range("Polynom Exception: out of range");
			return polynom[i];
		}

		bool operator==(const Polynom &obj) const {
			if (size() != obj.size())
				return false;

			for (int i = 0; i < size(); i++)
				if (static_cast<T>(0) != polynom[i] - obj[i])
					return false;
			return true;
		}

		bool operator!=(const Polynom &obj) const {
			return (!((*this) == obj));
		}

		void push_back(T elem) { polynom.push_back(elem); }

		const size_t size() const { return polynom.size(); }

		void resize(const size_t size) { polynom.resize(size); }

		//I need different representations of polynom
		void print(ostream &o = cout, const char delimiter = ' ') const {
			//OutputVector::print(polynom, o, separator);

			if (size() > 1)
				cout << "(";

			for (int i = 0; i < size(); i++)	{
				if (i == 0)
					o << polynom[i];
				else if (polynom[i] != static_cast<T>(0)) {
					o << delimiter << " + " << delimiter;
					if (polynom[i] != static_cast<T>(1))
						o << polynom[i];
					o << "x";
					if (i != 1)
						o << "^" << i;
				}
			}

			if (size() > 1)
				cout << ")";
		}


		/*---------------Friendship-----------------*/

		template<class S>
		friend ostream &operator<< (ostream &o, const Polynom<S> &p);
	};

	template <class S>
	ostream &operator<< (ostream &o, const Polynom<S> &p){
		p.print(o);
		return o;
	}
}

#endif //CRYPTOGRAPHY_POLYNOMIAL_H
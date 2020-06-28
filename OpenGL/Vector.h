#pragma once
#include <array>
#include <cassert>
#include <initializer_list>

template <typename T, size_t Size>
class Vector {
	std::array<T, Size> data;
public:
	// Construct
	Vector() : Vector(T(0)) {}

	Vector(T value) {
		std::fill(data.begin(), data.end(), value);
	}

	template<typename Castable>
	Vector(std::initializer_list<Castable> init) {
		assert(init.size() == Size);
		size_t index = 0;
		for (const auto& i : init) {
			data[index] = i;
			++index;
		}
	}

	// Access
	T& operator[](size_t i) { return data[i]; }
	const T& operator[](size_t i) const { return data[i]; }

	T& x() { return data[0]; }
	const T& x() const { return data[0]; }
	T& y() { return data[1]; }
	const T& y() const { return data[1]; }
	T& z() { return data[2]; }
	const T& z() const { return data[2]; }
	T& w() { return data[3]; }
	const T& w() const { return data[3]; }

	T& r() { return data[0]; }
	const T& r() const { return data[0]; }
	T& g() { return data[1]; }
	const T& g() const { return data[1]; }
	T& b() { return data[2]; }
	const T& b() const { return data[2]; }
	T& a() { return data[3]; }
	const T& a() const { return data[3]; }

	// Operations

	friend Vector operator+(const Vector& a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] + b[i];
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator+(const Vector& a, Castable b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] + b;
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator+(Castable a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a + b[i];
		}
		return res;
	}
	friend Vector& operator+=(const Vector& a, const Vector& b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] += b[i];
		}
		return a;
	}
	template<typename Castable>
	friend Vector& operator+=(const Vector& a, Castable b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] += b;
		}
		return a;
	}

	friend Vector operator-(const Vector& a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] - b[i];
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator-(const Vector& a, Castable b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] - b;
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator-(Castable a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a - b[i];
		}
		return res;
	}
	friend Vector& operator-=(const Vector& a, const Vector& b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] -= b[i];
		}
		return a;
	}
	template<typename Castable>
	friend Vector& operator-=(const Vector& a, Castable b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] -= b;
		}
		return a;
	}

	friend Vector operator*(const Vector& a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] * b[i];
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator*(const Vector& a, Castable b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] * b;
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator*(Castable a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a * b[i];
		}
		return res;
	}
	friend Vector& operator*=(const Vector& a, const Vector& b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] *= b[i];
		}
		return a;
	}
	template<typename Castable>
	friend Vector& operator*=(const Vector& a, Castable b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] *= b;
		}
		return a;
	}

	friend Vector operator/(const Vector& a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] / b[i];
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator/(const Vector& a, Castable b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] / b;
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator/(Castable a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a / b[i];
		}
		return res;
	}
	friend Vector& operator/=(const Vector& a, const Vector& b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] /= b[i];
		}
		return a;
	}
	template<typename Castable>
	friend Vector& operator/=(const Vector& a, Castable b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] /= b;
		}
		return a;
	}

	friend Vector operator%(const Vector& a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] % b[i];
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator%(const Vector& a, Castable b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a[i] % b;
		}
		return res;
	}
	template<typename Castable>
	friend Vector operator%(Castable a, const Vector& b) {
		Vector res;
		for (size_t i = 0; i < Size; i++) {
			res[i] = a % b[i];
		}
		return res;
	}
	friend Vector& operator%=(const Vector& a, const Vector& b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] %= b[i];
		}
		return a;
	}
	template<typename Castable>
	friend Vector& operator%=(const Vector& a, Castable b) {
		for (size_t i = 0; i < Size; i++) {
			a[i] %= b;
		}
		return a;
	}
};

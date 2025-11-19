#include <iostream>
#include <memory>
#include <vector>
#include <cstdint>
#include <type_traits>

struct BitMemory {

	typedef std::intmax_t int1_t;

public:
	std::size_t Size()const  {
		return (Bits.size())*8;
	}
	void Resize(std::size_t N) {

		auto A = N / 8;
		auto B = N & 8 ? 1 : 0;

		Bits.resize(A + B);
	}
	bool Rise(std::size_t Idx) {
		if (Size() < Idx) return  false;
		//std::intmax_t A = 0;
		///A = (Idx % 8) ? 1 : 0;
		std::size_t dx = Idx / 8;
		Bits[dx] |= 1 << (Idx % 8);
		return true;
	}
	bool Drop(std::size_t Idx){
		if (Size()<Idx) { return false; }
		//std::intmax_t A = 0;
		//A = (Idx % 8) ? 1 : 0;
		std::size_t Dx = Idx / 8;
		auto B = Bits[Dx] & (1 << (Idx % 8));
		Bits[Dx] &= ~B;
		return true;
	}

	bool Get(std::size_t Idx) {
		if (Size() < Idx) { return false; }
		std::intmax_t A = 0;
		A = (Idx % 8) ? 1 : 0;
		std::size_t Dx = Idx / 8;
		return Bits[Dx] & 1 << (Idx % 8);
		
	}
	bool Set(std::size_t Idx,int1_t B) {
		if (B) {
			return Rise(Idx);
		}
		else {
			return Drop(Idx);
		}

		return false;
	}
	bool Toggle(std::size_t Idx) {
		if (this->Get(Idx)) {
			return Drop(Idx);
		}
		else {
			return Rise(Idx);
		}

		return false;
	}

	const int1_t operator[](std::intmax_t Idx) {
		return Get(Idx);
	}

protected:
	std::vector<std::uint8_t> Bits;
};

void Print(BitMemory& In) {

	for (std::size_t i = 0; i < In.Size(); i++) {
		std::cout << In.Get(i);
	}
	std::cout << std::endl;
}

struct BitVector {
	std::size_t Size() {
		return Uses;
	}

	std::size_t Capacity() {
		return Bits.Size();
	}

	void ReCap(std::size_t N) {//this is cap bit.
		Bits.Resize(N);
		if (Uses > N) { Uses = N; }
	}

	void Resize(std::size_t N) {
		if (Bits.Size() < N) {
			Bits.Resize(N);
		}
		Uses = N;
	}
	bool Rise(std::size_t N) {
		if (Size() < N) { return false; }
		return Bits.Rise(N);
	}
	bool Drop(std::size_t N) {
		if (Size() < N) { return false; }
		return Bits.Drop(N);
	}
	bool Get(std::size_t N) {
		if (Size() < N) { return false; }
		return Bits.Get(N);
	}
	bool Get(std::size_t N,BitMemory::int1_t b) {
		if (Size() < N) { return false; }
		return Bits.Set(N,b);
	}
	bool Toggle(std::size_t N) {
		if (Size() < N) { return false; }
		return Bits.Toggle(N);
	}
protected:
	BitMemory Bits;
	std::size_t Uses = 0;
};

void Print(BitVector& In) {
	for (std::size_t i = 0; i < In.Size(); i++) {
		std::cout << In.Get(i);
	}
	std::cout << std::endl;
}

int main() {
	BitMemory BM;

	BM.Resize(13);

	Print(BM);

	BitVector BV;

	BV.Resize(13);

	Print(BV);

	BV.Toggle(6);
	BV.Toggle(12);
	Print(BV);
	BV.Toggle(6);
	BV.Toggle(12);
	Print(BV);

	return 0;
}
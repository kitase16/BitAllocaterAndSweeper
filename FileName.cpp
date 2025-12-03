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

	const int1_t Index(std::size_t N) {
		return Get(N) ? 1 : 0;
	}

	const int1_t operator[](std::intmax_t Idx) {
		return Index(Idx);
	}

protected:
	std::vector<std::uint8_t> Bits;
};

void Print(BitMemory& In) {

	for (std::size_t i = 0; i < In.Size(); i++) {
		std::cout << In.Index(i);
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
	bool Set(std::size_t N,BitMemory::int1_t b) {
		if (Size() < N) { return false; }
		return Bits.Set(N,b);
	}
	bool Toggle(std::size_t N) {
		if (Size() < N) { return false; }
		return Bits.Toggle(N);
	}
	const BitMemory::int1_t Index(std::size_t N) {
		if (Size()<N) { return 0; }
		return Get(N) ? 1 : 0;
	}
	const BitMemory::int1_t operator[](std::size_t Idx){
		return Index(Idx);
	}
	void Push(BitMemory::int1_t b) {
		Resize(Size() + 1);
		Set(Size() - 1, b);
	}

	void Pop() {
		if (!Size()) { return; }
		Resize(Size() - 1);
	}
protected:
	BitMemory Bits;
	std::size_t Uses = 0;
};

void Print(BitVector& In) {
	for (std::size_t i = 0; i < In.Size(); i++) {
		std::cout << In.Index(i);
	}
	std::cout << std::endl;
}

class MemoryAndSweeper {
public:

//	MemoryAndSweeper(std::size_t N) {
//		BV.Resize(N);
//	}

	void Self(std::shared_ptr<MemoryAndSweeper> S) {
		if (this != S.get()) { return; }
		Self_ = S;
	}

	std::size_t Size() {
		return BV.Size();
	}

	std::size_t Capacity() {
		return BV.Capacity();
	}

	void ReCap(std::size_t N) {
		BV.ReCap(N);
	}
	void Resize(std::size_t N) {
		BV.Resize(N);
	}
	
	void Add(std::size_t N) {
		for (std::size_t i = 0; i < N; i++) {
			BV.Push(0);
		}
	}
	void Sub(std::size_t N) {
		for (std::size_t i = 0; i < N; i++) {
			BV.Pop();
		}
	}
	BitMemory::int1_t Index(std::size_t Index) {
		return BV.Index(Index);
	}

	void Set(std::size_t P, BitMemory::int1_t b) {
		if (b) {
			BV.Rise(P);
		}
		else {
			BV.Drop(P);
		}
	}
	BitMemory::int1_t Get(std::size_t P) {
		return BV.Get(P);
	}
	void Rise(std::size_t Pos) {
		BV.Rise(Pos);
	}
	void Drop(std::size_t Pos) {
		BV.Drop(Pos);
	}
	void Toggle(std::size_t Pos) {
		BV.Toggle(Pos);
	}
	struct MemoryHolder;
	std::shared_ptr<MemoryHolder> Child(std::size_t Sz) {

		if (SMH.size() == 0) {
				auto M = std::make_shared<MemoryHolder>(Self_, 0);
				M->SetSz(Sz);
				return M;		

		}

		std::size_t Po = 0;
		for (std::size_t i = 0; i < SMH.size(); i++) {
			std::size_t Pa = Po + Sz;
			if (SMH[i]->Position() > Pa) {
				auto M = std::make_shared<MemoryHolder>(Self_, Pa);
				M->SetSz(Sz);
				return M;
			}
			Po = SMH[i]->Position();
		}
		auto M = std::make_shared<MemoryHolder>(Self_, SMH.back()->Position()+SMH.back()->Size());
		M->SetSz(Sz);
		return M;
	}

	struct MemoryHolder {
		MemoryHolder(std::shared_ptr<MemoryAndSweeper> Mas, std::size_t P) :MAS(Mas), Pos(P) {}
		std::size_t Position() { return Pos; }
		std::size_t Size() { return Sz; }
		bool IsSize() { return MAS->Size() >= Pos + Sz; }
		BitMemory::int1_t Index(std::size_t Pos) {
			if (!IsSize()) { return 0; }
			return (*MAS).Index(Position() + Pos);
		}
		void Set(std::size_t P, BitMemory::int1_t b) {
			if (!IsSize()) { return; }		
			MAS->Set(Position() + P, b);
		}

		BitMemory::int1_t operator[](std::size_t Idx) {
			return Index(Idx);
		}

	//protected:
		void SetSz(std::size_t SZ) {
			Sz = SZ;
		}

		virtual ~MemoryHolder() {
			for (std::size_t i = 0; i < Sz; i++) {
				Set(i, 0);
			}
		}

	protected:
		std::size_t Pos = 0;
		std::size_t Sz = 0;
		std::shared_ptr<MemoryAndSweeper> MAS;
	};

protected:
	BitVector BV;
	std::shared_ptr<MemoryAndSweeper> Self_;
	std::vector<std::shared_ptr<MemoryHolder>> SMH;
};
void Print2(MemoryAndSweeper& In) {
	for (std::size_t i = 0; i < In.Size(); i++) {
		std::cout << In.Index(i);
	}
	std::cout << std::endl;
}
void Print2(MemoryAndSweeper::MemoryHolder& In) {
	for (std::size_t i = 0; i < In.Size(); i++) {
		std::cout << In.Index(i);
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

	std::shared_ptr<MemoryAndSweeper> MAS = std::make_shared<MemoryAndSweeper>();
	MAS->Self(MAS);

	MAS->ReCap(1 << 16);
	MAS->Resize(1 << 11);

	{
		auto C = MAS->Child(32);

		Print2(*C.get());
		C->Set(8, 1);
		Print2(*C.get());
		Print2(*MAS.get());
	}
		Print2(*MAS.get());
	return 0;
}
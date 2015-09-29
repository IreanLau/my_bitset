#include "bitset.h"

void main()
{
//	bitset<14> bt(1);
////	bt.set();
//	cout << bt << endl;
////	cout << bt.test(0) << endl;
//	cout << bt.count() << endl;
//	cout << bt.my_count() << endl;
//	//bt.flip();
//
//	cout << bt << endl;
//	cout << bt.any() << endl;
//	cout << bt.none() << endl;
//
//
//	bitset<30> bt1("10111");
//	
//	cout << bt1 << endl;
////	bt1.reset();
//	cout << bt1.to_ulong() << endl;


	//cout <<oct<<((size_t)(-1)) << endl;
	//cout << hex << ((size_t)(-1)) << endl;

	bitset<4>bt1(15);
	bitset<7>bt2(28);

	cout << bt1 << endl;
	cout << bt2 << endl;

	bt1[2] = 0;
	cout << bt1 << endl;
	
}
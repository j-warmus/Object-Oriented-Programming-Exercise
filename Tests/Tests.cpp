#include "pch.h"
#include "CppUnitTest.h"

#include <fstream>

#define private public

#include "../OOP Exercise/InputStreamInterface.cpp"
#include "../OOP Exercise/TradeManager.cpp"
#include "../OOP Exercise/InfoOutputter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(InputStreamTests)
	{
	public:
		TEST_METHOD(GetValuesHasValue)
		{
			CSVInputInterface testinterface;
			std::string in("51300607155,dba,1,229");
			auto out = testinterface.parseInfoFromString(in);
			Assert::IsTrue(out.has_value());
		}
		TEST_METHOD(GetValuesEmpty)
		{
			CSVInputInterface testinterface;
			std::string in("");
			auto out = testinterface.parseInfoFromString(in);
			Assert::IsFalse(out.has_value());
		}
		TEST_METHOD(GetValuesCorrectly){
			CSVInputInterface testinterface;
			std::string in("51300607155,dba,1,229");
			auto out = testinterface.parseInfoFromString(in);
			inputData expected{ std::make_tuple(51300607155, std::string("dba"), 1, 229) };
			auto [ea, eb, ec, ed] = expected;
			auto [aa, ab, ac, ad] = out.value();
			Assert::AreEqual(ea, aa);
			Assert::AreEqual(eb, ab);
			Assert::AreEqual(ec, ac);
			Assert::AreEqual(ed, ad);
		}
	};
	TEST_CLASS(ManagerTests) {
		TEST_METHOD(UpdateEntry) {
			outputEntry entry;
			entry.lastTime = 100;
			entry.maxPrice = 10;
			entry.maxTimeGap = 5;
			entry.symbol = "aaa";
			entry.volume = 1000;
			entry.weightedAvgPrice = 10;

			inputData newdata{ make_tuple(110, std::string("aaa"), 1000, 20 )};

			std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
			HashTableTradeManager m(std::move(interface));
			m.update_entry(newdata, entry);
			Assert::AreEqual((microseconds)110, entry.lastTime);
			Assert::AreEqual((unsigned int)20, entry.maxPrice);
			Assert::AreEqual((microseconds)10, entry.maxTimeGap);
			Assert::AreEqual(std::string("aaa"), entry.symbol);
			Assert::AreEqual(2000, entry.volume);
			Assert::AreEqual(15, (int)entry.weightedAvgPrice);
		}
		TEST_METHOD(AddNewEntry) { 
			inputData newdata{ make_tuple(110, std::string("aaa"), 1000, 20) };

			std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
			HashTableTradeManager m(std::move(interface));
			m.addToMap(newdata);
			auto entry = m.table["aaa"];
			Assert::AreEqual(std::string("aaa"), entry.symbol);
			Assert::AreEqual((microseconds)0, entry.maxTimeGap);
			Assert::AreEqual((microseconds)110, entry.lastTime);
			Assert::AreEqual(1000, entry.volume);
			Assert::AreEqual((double)20, entry.weightedAvgPrice);
			Assert::AreEqual((unsigned int)20, entry.maxPrice);
		}
		TEST_METHOD(UpdateExistingEntry) {
			inputData newdata1{ make_tuple(110, std::string("aaa"), 1000, 20) };
			inputData newdata2{ make_tuple(115, std::string("aaa"), 1000, 30) };

			std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
			HashTableTradeManager m(std::move(interface));
			m.addToMap(newdata1);
			m.addToMap(newdata2);

			auto entry = m.table["aaa"];
			Assert::AreEqual(std::string("aaa"), entry.symbol);
			Assert::AreEqual((microseconds)5, entry.maxTimeGap);
			Assert::AreEqual((microseconds)115, entry.lastTime);
			Assert::AreEqual(2000, entry.volume);
			Assert::AreEqual((double)25, entry.weightedAvgPrice);
			Assert::AreEqual((unsigned int)30, entry.maxPrice);
		}
		TEST_METHOD(DumpData) {
			inputData newdata{ make_tuple(110, std::string("aaa"), 1000, 20) };

			std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
			HashTableTradeManager m(std::move(interface));
			m.addToMap(newdata);
			auto v = m.dumpData();
			auto entry = v[0];
			Assert::AreEqual(std::string("aaa"), entry.symbol);
			Assert::AreEqual((microseconds)0, entry.maxTimeGap);
			Assert::AreEqual((microseconds)110, entry.lastTime);
			Assert::AreEqual(1000, entry.volume);
			Assert::AreEqual((double)20, entry.weightedAvgPrice);
			Assert::AreEqual((unsigned int)20, entry.maxPrice);
		}
		TEST_METHOD(DumpSorted) {
			inputData newdata1{ make_tuple(110, std::string("aaa"), 1000, 20) };
			inputData newdata2{ make_tuple(115, std::string("aab"), 1000, 30) };
			inputData newdata3{ make_tuple(115, std::string("aac"), 1000, 30) };
			inputData newdata4{ make_tuple(115, std::string("zzb"), 1000, 30) };

			std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
			HashTableTradeManager m(std::move(interface));
			m.addToMap(newdata1);
			m.addToMap(newdata4);
			m.addToMap(newdata3);
			m.addToMap(newdata2);

			auto v = m.dumpData();
			Assert::AreEqual(std::string("aaa"), v[0].symbol);
			Assert::AreEqual(std::string("aab"), v[1].symbol);
			Assert::AreEqual(std::string("aac"), v[2].symbol);
			Assert::AreEqual(std::string("zzb"), v[3].symbol);
		}
		TEST_METHOD(SampleInput) {
			std::string s{
				R""""(	52924702,aaa,13,1136
						52924702,aac,20,477
						52925641,aab,31,907
						52927350,aab,29,724
						52927783,aac,21,638
						52930489,aaa,18,1222
						52931654,aaa,9,1077
						52933453,aab,9,756)""""
			};
			std::stringstream fs(s);

			std::unique_ptr<InputInterface> interface { std::make_unique<CSVInputInterface>() };
			HashTableTradeManager m(std::move(interface));
			m.generateDataFromStream(fs);

			auto v = m.dumpData();
			Assert::AreEqual(3, (int)v.size());

			Assert::AreEqual(std::string("aaa"), v[0].symbol);
			Assert::AreEqual(std::string("aab"), v[1].symbol);
			Assert::AreEqual(std::string("aac"), v[2].symbol);

			Assert::AreEqual((microseconds)5787, v[0].maxTimeGap);
			Assert::AreEqual(40, v[0].volume);
			Assert::AreEqual(1161, (int)v[0].weightedAvgPrice);
			Assert::AreEqual((unsigned int)1222, v[0].maxPrice);

			Assert::AreEqual((microseconds)6103, v[1].maxTimeGap);
			Assert::AreEqual(69, v[1].volume);
			Assert::AreEqual(810, (int)v[1].weightedAvgPrice);
			Assert::AreEqual((unsigned int)907, v[1].maxPrice);

			Assert::AreEqual((microseconds)3081, v[2].maxTimeGap);
			Assert::AreEqual(41, v[2].volume);
			Assert::AreEqual(559, (int)v[2].weightedAvgPrice);
			Assert::AreEqual((unsigned int)638, v[2].maxPrice);

		}
	};

	TEST_CLASS(OutputterTests) {
		TEST_METHOD(TestStringOutputSingleLine) {
			CSVInfoOutputter c;
			outputEntry entry{"aba", 4, 5, 6, 7.6, 8};
			std::vector<outputEntry> v;
			v.push_back(entry);
			std::string s = c.writeToString(v);

			Assert::AreEqual(std::string("aba,4,6,7,8\n"), s);
		}
		TEST_METHOD(TestStringOutputMultiLine) {
			CSVInfoOutputter c;
			outputEntry entry1{ "aba", 4, 5, 6, 7.6, 8 };
			outputEntry entry2{ "bba", 14, 15, 16, 17.6, 18 };
			std::vector<outputEntry> v;
			v.push_back(entry1);
			v.push_back(entry2);
			std::string s = c.writeToString(v);

			Assert::AreEqual(std::string("aba,4,6,7,8\nbba,14,16,17,18\n"), s);
		}
		TEST_METHOD(FileWrite) {
			CSVInfoOutputter c;
			outputEntry entry{ "aba", 4, 5, 6, 7.6, 8 };
			std::vector<outputEntry> v;
			v.push_back(entry);
			bool b = c.writeToFile("testout.csv",v);

			Assert::IsTrue(b);
		}
	};
}

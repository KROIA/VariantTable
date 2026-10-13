#pragma once

#include "UnitTest.h"
#include "VariantTable.h"
//#include <QObject>
//#include <QCoreapplication>





class TST_simple : public UnitTest::Test
{
	TEST_CLASS(TST_simple)
public:
	TST_simple()
		: Test("TST_simple")
	{
		ADD_TEST(TST_simple::test1);
		ADD_TEST(TST_simple::test2);
		ADD_TEST(TST_simple::cellColor);

	}

private:

	// Tests
	TEST_FUNCTION(test1)
	{
		TEST_START;

		int a = 0;
		TEST_MESSAGE("is a == 0?");
		TEST_ASSERT(a == 0);

		
	}




	TEST_FUNCTION(test2)
	{
		TEST_START;

		int a = 0;
		TEST_ASSERT_M(a == 0, "is a == 0?");

		int b = 0;
		if (b != 0)
		{
			TEST_FAIL("b is not 0");
		}

		// fails if a != b
		TEST_COMPARE(a, b);
	}

	TEST_FUNCTION(cellColor)
	{
		TEST_START;

		VariantTable::LineEdit cell("text");

		// A cell without a color reports an invalid color, so nothing is painted over it.
		TEST_ASSERT_M(!cell.getColor().isValid(), "a fresh cell has no color set");

		cell.setColor(QColor(255, 0, 0));
		TEST_ASSERT_M(cell.getColor().isValid(), "the color set is reported as valid");
		TEST_COMPARE(cell.getColor(), QColor(255, 0, 0));

		// Setting an invalid color returns the cell to the uncolored state.
		cell.setColor(QColor());
		TEST_ASSERT_M(!cell.getColor().isValid(), "setColor(QColor()) clears the color");
	}

};

TEST_INSTANTIATE(TST_simple);
/*
 * File:   newtestclass.h
 * Author: Ivan
 *
 * Created on May 29, 2013, 4:12:11 PM
 */

#ifndef NEWTESTCLASS_H
#define	NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class newtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclass);

    CPPUNIT_TEST(testParseData);
    CPPUNIT_TEST(testParseData_partialBuffer);
    CPPUNIT_TEST(testParseData_BeginFromNotPacket);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclass();
    virtual ~newtestclass();
    void setUp();
    void tearDown();

private:
    void testParseData();
    void testParseData_partialBuffer();
    void testParseData_BeginFromNotPacket();
};

#endif	/* NEWTESTCLASS_H */


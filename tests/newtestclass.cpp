/*
 * File:   newtestclass.cpp
 * Author: Ivan
 *
 * Created on May 29, 2013, 4:12:11 PM
 */

#include "newtestclass.h"
#include "../Parsing.h"
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

newtestclass::newtestclass() {
}

newtestclass::~newtestclass() {
}

void newtestclass::setUp() {
}

void newtestclass::tearDown() {
}

void newtestclass::testParseData() {
    std::string buffer = "$GPGGA,160023.69,1130.832,N,04344.045,E,2,04,\
2.6,100.00,M,-33.9,M,,0000*7C\r\n$GPGLL,1130.915,\
N,04344.052,E,160024.70,A*0C\r\n$GPGGA,160025.71,\
1130.998,N,04344.060,E,2,04,2.7,100.00,M,-33.9,M,\
,0000*74\r\n$GPRMC,160026.73,A,1131.081,N,04344.067,\
E,300.00,4.96,110213,0.0,E*5A\r\n$GPGGA,160025.71,";

    CParsing cParsing;
    bool result = cParsing.parseData(buffer);

    CPPUNIT_ASSERT(result);

    bool expectedIsValidGPGGAData = true;
    bool resultIsValidGPGGAData = cParsing.isValidGPGGAData();
    CPPUNIT_ASSERT(expectedIsValidGPGGAData && resultIsValidGPGGAData);

    bool expectedIsValidGPRMCData = true;
    bool resultIsValidGPRMCData = cParsing.isValidGPRMCData();
    CPPUNIT_ASSERT(expectedIsValidGPRMCData && resultIsValidGPRMCData);

    double expectedAltitude = 100.0;
    double resultAltitude = cParsing.getAltitude();
    double deltaAltitude = expectedAltitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAltitude, resultAltitude, deltaAltitude);

    double expectedLatitude = 11.0 + (31.081 / 60.0);
    double resultLatitude = cParsing.getLatitude();
    double deltaLatitude = expectedLatitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLatitude, resultLatitude, deltaLatitude);

    double expectedLongitude = 43.0 + (44.067 / 60.0);
    double resultLongitude = cParsing.getLongitude();
    double deltaLongitude = expectedLongitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLongitude, resultLongitude, deltaLongitude);

    int expectedNSatellites = 4;
    int resultNSatellites = cParsing.getNSatellites();
    CPPUNIT_ASSERT_EQUAL(expectedNSatellites, resultNSatellites);

    double expectedSpeed = 300.0 * 1.852;
    double resultSpeed = cParsing.getSpeed();
    double deltaSpeed = expectedSpeed / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSpeed, resultSpeed, deltaSpeed);

    std::tm time2013;
    time2013.tm_hour = 16;
    time2013.tm_min = 00;
    time2013.tm_sec = 26;
    time2013.tm_mday = 11;
    time2013.tm_mon = 2-1;
    time2013.tm_year = 113;
    int expectedTime = std::difftime( std::mktime(&time2013), 0 );
    int resultTime = cParsing.getTime();
    CPPUNIT_ASSERT_EQUAL(expectedTime, resultTime);
}

void newtestclass::testParseData_partialBuffer() {
    std::vector<std::string> bufferArr;
    bufferArr.push_back("$GPGGA,160023.69,1130.832,N,04344.045,E,2,04,");
    bufferArr.push_back("2.6,100.00,M,-33.9,M,,0000*7C\r\n$GPGLL,1130.915,");
    bufferArr.push_back("N,04344.052,E,160024");
    bufferArr.push_back(".70,A*0C\r\n$GPGGA,160025.71,");
    bufferArr.push_back("1130.998,N,04344.060,E");
    bufferArr.push_back(",2,04,2.7,100.00,M,-33.9,M,");
    bufferArr.push_back(",0000*74\r\n$GP");
    bufferArr.push_back("RMC,160026.73,A,1131.081,N,04344.067,");
    bufferArr.push_back("E,300.00,4.");
    bufferArr.push_back("96,110213,0.0,E*5A\r\n$GP");

    bool result = false;
    CParsing cParsing;

    for (unsigned int i = 0; i < bufferArr.size(); i++) {
        result = cParsing.parseData(bufferArr[i]);
        if (result) {
            break;
        }
    }

    CPPUNIT_ASSERT(result);

    bool expectedIsValidGPGGAData = true;
    bool resultIsValidGPGGAData = cParsing.isValidGPGGAData();
    CPPUNIT_ASSERT(expectedIsValidGPGGAData && resultIsValidGPGGAData);

    bool expectedIsValidGPRMCData = true;
    bool resultIsValidGPRMCData = cParsing.isValidGPRMCData();
    CPPUNIT_ASSERT(expectedIsValidGPRMCData && resultIsValidGPRMCData);

    double expectedAltitude = 100.0;
    double resultAltitude = cParsing.getAltitude();
    double deltaAltitude = expectedAltitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAltitude, resultAltitude, deltaAltitude);

    double expectedLatitude = 11.0 + (31.081 / 60.0);
    double resultLatitude = cParsing.getLatitude();
    double deltaLatitude = expectedLatitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLatitude, resultLatitude, deltaLatitude);

    double expectedLongitude = 43.0 + (44.067 / 60.0);
    double resultLongitude = cParsing.getLongitude();
    double deltaLongitude = expectedLongitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLongitude, resultLongitude, deltaLongitude);

    int expectedNSatellites = 4;
    int resultNSatellites = cParsing.getNSatellites();
    CPPUNIT_ASSERT_EQUAL(expectedNSatellites, resultNSatellites);

    double expectedSpeed = 300.0 * 1.852;
    double resultSpeed = cParsing.getSpeed();
    double deltaSpeed = expectedSpeed / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSpeed, resultSpeed, deltaSpeed);

    std::tm time2013;
    time2013.tm_hour = 16;
    time2013.tm_min = 00;
    time2013.tm_sec = 26;
    time2013.tm_mday = 11;
    time2013.tm_mon = 2-1;
    time2013.tm_year = 113;
    int expectedTime = std::difftime( std::mktime(&time2013), 0 );
    int resultTime = cParsing.getTime();
    CPPUNIT_ASSERT_EQUAL(expectedTime, resultTime);
}

void newtestclass::testParseData_BeginFromNotPacket() {
    std::vector<std::string> bufferArr;
    bufferArr.push_back(".70,A*0C\r\n$GPGGA,160023.69,1130.832,N,04344.045,E,2,04,");
    bufferArr.push_back("2.6,100.00,M,-33.9,M,,0000*7C\r\n$GPGLL,1130.915,");
    bufferArr.push_back("N,04344.052,E,160024");
    bufferArr.push_back(".70,A*0C\r\n$GPGGA,160025.71,");
    bufferArr.push_back("1130.998,N,04344.060,E");
    bufferArr.push_back(",2,04,2.7,100.00,M,-33.9,M,");
    bufferArr.push_back(",0000*74\r\n$GP");
    bufferArr.push_back("RMC,160026.73,A,1131.081,N,04344.067,");
    bufferArr.push_back("E,300.00,4.");
    bufferArr.push_back("96,110213,0.0,E*5A\r\n");
    bufferArr.push_back("$GPGGA,160023.69,1130.832,N,04344.045,E,2,04,");
    bufferArr.push_back("2.6,100.00,M,-33.9,M,,0000*7C\r\n$GPGLL,1130.915,");
    bufferArr.push_back("N,04344.052,E,160024");
    bufferArr.push_back(".70,A*0C\r\n$GPGGA,160025.71,");
    bufferArr.push_back("1130.998,N,04344.060,E");
    bufferArr.push_back(",2,04,2.7,100.00,M,-33.9,M,");
    bufferArr.push_back(",0000*74\r\n$GP");
    bufferArr.push_back("RMC,160026.73,A,1131.081,N,04344.067,");
    bufferArr.push_back("E,300.00,4.");
    bufferArr.push_back("96,110213,0.0,E*5A\r\n");

    bool result = false;
    CParsing cParsing;

    for (unsigned int i = 0; i < bufferArr.size(); i++) {
        result = cParsing.parseData(bufferArr[i]);
        if (result) {
            break;
        }
    }

    CPPUNIT_ASSERT(result);

    bool expectedIsValidGPGGAData = true;
    bool resultIsValidGPGGAData = cParsing.isValidGPGGAData();
    CPPUNIT_ASSERT(expectedIsValidGPGGAData && resultIsValidGPGGAData);

    bool expectedIsValidGPRMCData = true;
    bool resultIsValidGPRMCData = cParsing.isValidGPRMCData();
    CPPUNIT_ASSERT(expectedIsValidGPRMCData && resultIsValidGPRMCData);

    double expectedAltitude = 100.0;
    double resultAltitude = cParsing.getAltitude();
    double deltaAltitude = expectedAltitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAltitude, resultAltitude, deltaAltitude);

    double expectedLatitude = 11.0 + (31.081 / 60.0);
    double resultLatitude = cParsing.getLatitude();
    double deltaLatitude = expectedLatitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLatitude, resultLatitude, deltaLatitude);

    double expectedLongitude = 43.0 + (44.067 / 60.0);
    double resultLongitude = cParsing.getLongitude();
    double deltaLongitude = expectedLongitude / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedLongitude, resultLongitude, deltaLongitude);

    int expectedNSatellites = 4;
    int resultNSatellites = cParsing.getNSatellites();
    CPPUNIT_ASSERT_EQUAL(expectedNSatellites, resultNSatellites);

    double expectedSpeed = 300.0 * 1.852;
    double resultSpeed = cParsing.getSpeed();
    double deltaSpeed = expectedSpeed / 1000.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedSpeed, resultSpeed, deltaSpeed);

    std::tm time2013;
    time2013.tm_hour = 16;
    time2013.tm_min = 00;
    time2013.tm_sec = 26;
    time2013.tm_mday = 11;
    time2013.tm_mon = 2-1;
    time2013.tm_year = 113;
    int expectedTime = std::difftime( std::mktime(&time2013), 0 );
    int resultTime = cParsing.getTime();
    CPPUNIT_ASSERT_EQUAL(expectedTime, resultTime);
}

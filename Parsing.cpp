/* 
 * File:   CParsing.cpp
 * Author: Ivan
 * 
 * Created on May 29, 2013, 3:57 PM
 */

#include "Parsing.h"
#include <sstream>
#include <ctime>

CParsing::CParsing() {
    setValuesInZero();
}

void CParsing::setValuesInZero() {
    m_dLatitude = 0;
    m_dLongitude = 0;
    m_dAltitude = 0;
    m_iTime = 0;
    m_dSpeed = 0;
    m_iNSatellites = 0; // m_nSatellites
    m_bIsValidGPGGAData = 0;
    m_bIsValidGPRMCData = 0;
}

std::vector <std::string> & CParsing::split(
        std::vector <std::string> & result,
        const std::string& s,
        const std::string& delimiters,
        split::empties_t empties) {
    result.clear();
    size_t current;
    size_t next = -1;
    do {
        if (empties == split::no_empties) {
            next = s.find_first_not_of(delimiters, next + 1);
            if (next == std::string::npos) break;
            next -= 1;
        }
        current = next + 1;
        next = s.find_first_of(delimiters, current);
        result.push_back(s.substr(current, next - current));
    } while (next != std::string::npos);
    return result;
}

#include <iostream>

void printArr(std::vector<std::string> v) {
    for (std::size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
    std::cout << std::endl;
}

bool CParsing::parseData(std::string buffer) {
    //    buffer.replace("\r", "");
    //    m_buffer += buffer;
    //    var arr = m_buffer.split("\n");
    //    for (int i = 0; i < arr.length - 1; i++)
    //    {
    //        string packet = arr[i];
    //        if (packet.length == 0 || packet[0] !="$")
    //            return false;
    //        if (packet.indexOf("$GPRMC") == 0)    
    //        {
    //            
    //        }
    //        if (packet.indexOf("$GPGGA") == 0)    
    //        {
    //            
    //        }
    //    }
    //    m_buffer = arr[arr.length - 1];

    bool dataIsReady = false;
    static bool RMCDataIsReady = false;
    static bool GGADataIsReady = false;

    for (std::size_t i = 0; i < buffer.length(); i++) {
        if (buffer[i] == '\r') {
            buffer.erase(i, 1);
        }
    }
    m_buffer += buffer;
    std::vector<std::string> bufferArr;
    split(bufferArr, m_buffer, "\n");
    printArr(bufferArr);

    for (std::size_t i = 0; i < bufferArr.size() - 1; i++) {
        std::string packet = bufferArr[i];
        if ((packet.length() == 0) || (packet[0] != '$')) {
            setValuesInZero();
            return false;
        }
        std::string beginningOfPacket = packet.substr(0, 6);
        if (beginningOfPacket == "$GPRMC") {
            if (!setDataOfPacket_RMC(packet)) {
                setValuesInZero();
                RMCDataIsReady = false;
                GGADataIsReady = false;
                dataIsReady = false;
                m_buffer.clear();
                return false;
            } else {
                RMCDataIsReady = true;
            }
        } else if (beginningOfPacket == "$GPGGA") {
            if (!setDataOfPacket_GGA(packet)) {
                setValuesInZero();
                RMCDataIsReady = false;
                GGADataIsReady = false;
                dataIsReady = false;
                m_buffer.clear();
                return false;
            } else {
                GGADataIsReady = true;
            }
        }
    }

    dataIsReady = RMCDataIsReady && GGADataIsReady;

    if (dataIsReady) {
        RMCDataIsReady = false;
        GGADataIsReady = false;
        m_buffer.clear();
    }

    return dataIsReady;
}

bool CParsing::setDataOfPacket_RMC(std::string packet) {
    std::vector<std::string> RMCBuffer;

    split(RMCBuffer, packet, ",");

    // 2) Status, V = Navigation receiver warning
    if (RMCBuffer[2] != "A") {
        return false;
    } else {
        m_bIsValidGPRMCData = true;
    }

    // 1) Time (UTC)
    int utcTime;
    std::stringstream convertTime(RMCBuffer[1]);
    if (!(convertTime >> utcTime)) {
        return false;
    }

    std::string strUtcTime = RMCBuffer[1];

    // 9) Date, ddmmyy
    int date;
    std::stringstream convertDate(RMCBuffer[9]);
    if (!(convertDate >> date)) {
        return false;
    }

    std::string strUtcDate = RMCBuffer[9];

    if (!utcToMsecFrom1970(m_iTime, strUtcTime, strUtcDate)) {
        return false;
    }

    // 3) Latitude
    double latitudeDegreeWithMinutes;
    std::stringstream convertLatitude(RMCBuffer[3]);
    if (!(convertLatitude >> latitudeDegreeWithMinutes)) {
        return false;
    }
    if (!minutesToDegrees(latitudeDegreeWithMinutes, m_dLatitude)) {
        return false;
    }

    // 4) N or S
    if (RMCBuffer[4] == "S") {
        m_dLatitude *= -1;
    }

    // 5) Longitude
    double longitudeDegreeWithMinutes;
    std::stringstream convertLongitude(RMCBuffer[5]);
    if (!(convertLongitude >> longitudeDegreeWithMinutes)) {
        return false;
    }
    if (!minutesToDegrees(longitudeDegreeWithMinutes, m_dLongitude)) {
        setValuesInZero();
        return false;
    }

    // 6) E or W
    if (RMCBuffer[6] == "W") {
        m_dLongitude *= -1;
    }

    // 7) Speed over ground, knots
    std::stringstream convertSpeed(RMCBuffer[7]);
    if (!(convertSpeed >> m_dSpeed)) {
        setValuesInZero();
        return false;
    }
    m_dSpeed *= 1.852;

    return true;
}

bool CParsing::setDataOfPacket_GGA(std::string packet) {
    std::vector<std::string> GGABuffer;

    split(GGABuffer, packet, ",");

    // 6) GPS Quality Indicator,
    // 0 - fix not available,
    // 1 - GPS fix,
    // 2 - Differential GPS fix
    if (GGABuffer[6] != "2") {
        return false;
    } else {
        m_bIsValidGPGGAData = true;
    }

    // 7) Number of satellites in view, 00 - 12
    std::stringstream convertNSatellites(GGABuffer[7]);
    if (!(convertNSatellites >> m_iNSatellites)) {
        return false;
    }

    // 9) Antenna Altitude above/below mean-sea-level (geoid)
    std::stringstream convertAltitude(GGABuffer[9]);
    if (!(convertAltitude >> m_dAltitude)) {
        return false;
    }

    return true;
}

bool CParsing::utcToMsecFrom1970(int& msecFrom1970,
        const std::string& utcTime,
        const std::string& date) {

    bool successful = false;

    std::string strHours = utcTime.substr(0, 2);
    std::string strMinutes = utcTime.substr(2, 2);
    std::string strSeconds = utcTime.substr(4, 2);

    std::string strDays = date.substr(0, 2);
    std::string strMonths = date.substr(2, 2);
    std::string strYears = date.substr(4, 2);

    int hours;
    std::stringstream convertHour(strHours);
    if (!(convertHour >> hours)) {
        return false;
    }

    int minutes;
    std::stringstream convertMinute(strMinutes);
    if (!(convertMinute >> minutes)) {
        return false;
    }

    int seconds;
    std::stringstream convertSecond(strSeconds);
    if (!(convertSecond >> seconds)) {
        return false;
    }

    int days;
    std::stringstream convertDay(strDays);
    if (!(convertDay >> days)) {
        return false;
    }

    int months;
    std::stringstream convertMonth(strMonths);
    if (!(convertMonth >> months)) {
        return false;
    }

    int year;
    std::stringstream convertYear(strYears);
    if (!(convertYear >> year)) {
        return false;
    }

    std::tm tmTime;
    tmTime.tm_hour = hours;
    tmTime.tm_min = minutes;
    tmTime.tm_sec = seconds;
    tmTime.tm_mday = days;
    tmTime.tm_mon = months - 1;
    year += 100;
    tmTime.tm_year = year;

    msecFrom1970 = std::difftime(std::mktime(&tmTime), 0);

    if (msecFrom1970 != 0) {
        successful = true;
    }

    return successful;
}

bool CParsing::minutesToDegrees(double dDegreeWithMinutes, double& degrees) {
    // Conversion from double to string
    std::stringstream qDegreeWithMinutesToString;
    qDegreeWithMinutesToString << dDegreeWithMinutes;
    std::string qDegreeWithMinutesAsString(qDegreeWithMinutesToString.str());

    if (qDegreeWithMinutesAsString[4] != '.') {
        return false;
    }

    std::string degreesAsString = qDegreeWithMinutesAsString.substr(0, 2);
    std::string minutesAsString = qDegreeWithMinutesAsString.substr(2, qDegreeWithMinutesAsString.length() - 2);

    double minutes;
    std::stringstream qMinutesToString(minutesAsString);
    if (!(qMinutesToString >> minutes)) {
        return false;
    }
    double tenthsOfDegree = minutes / 60;

    std::stringstream qDegreesToString(degreesAsString);
    if (!(qDegreesToString >> degrees)) {
        return false;
    }

    degrees += tenthsOfDegree;

    return true;
}

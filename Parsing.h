/* 
 * File:   Parsing.h
 * Author: Ivan
 *
 * Created on May 29, 2013, 3:57 PM
 */

#ifndef PARSING_H
#define	PARSING_H

struct split {

    enum empties_t {
        empties_ok, no_empties
    };
};

#include <string>
#include <vector>

class CParsing {
public:
    /**
     * The constructor
     */
    CParsing();

    /**
     * Parses the data from the NMEA protocol
     * 
     * @param buffer The buffer for parsing
     * @return The analysis occurred or not
     */
    bool parseData(std::string buffer);

    /**
     * Gets the latitude
     * 
     * @return The latitude
     */
    double getLatitude() {
        return m_dLatitude;
    }

    /**
     * Gets the longitude
     * 
     * @return The longitude
     */
    double getLongitude() {
        return m_dLongitude;
    }

    /**
     * Gets the altitude
     * 
     * @return The altitude
     */
    double getAltitude() {
        return m_dAltitude;
    }

    /**
     * Gets the time
     * 
     * @return The time
     */
    int getTime() {
        return m_iTime;
    }

    /**
     * Gets the speed
     * 
     * @return The speed
     */
    double getSpeed() {
        return m_dSpeed;
    }

    /**
     * Gets the numbers of the satellites
     * 
     * @return The numbers of the satellites
     */
    int getNSatellites() {
        return m_iNSatellites;
    }

    /**
     * Whether the GPGGA data is correct
     * 
     * @return The GPGGA data is valid
     */
    bool isValidGPGGAData() {
        return m_bIsValidGPGGAData;
    }

    /**
     * Whether the GPGGA data is correct
     * 
     * @return The GPRMC data is valid
     */
    bool isValidGPRMCData() {
        return m_bIsValidGPRMCData;
    }

private:
    /**
     * Sets the data of the packet RMC
     * 
     * @param packet The packet
     */
    bool setDataOfPacket_RMC(std::string packet);

    /**
     * Sets the data of the packet GGA
     * 
     * @param packet The packet
     */
    bool setDataOfPacket_GGA(std::string packet);

    /**
     * Converts UTC time to msec from 1970
     * 
     * @param msecFrom1970 The output msec from 1970
     * @param utcTime The UTC time
     * @param date The date
     * @return The successful flag
     */
    bool utcToMsecFrom1970(int& msecFrom1970,
            const std::string& utcTime,
            const std::string& date);

    /**
     * Converts a minute record to a degree record
     * 
     * @param m The minute record
     * @return The degree record
     */
    bool minutesToDegrees(double m, double& result);

    /**
     * Splits this string
     * 
     * @param result The result
     * @param inputBuffer The input buffer
     * @param delimiters The delimiter
     * @param empties Include or not the empty strings
     * @return The result
     */
    std::vector <std::string> & split(
            std::vector <std::string> & result,
            const std::string& inputBuffer,
            const std::string& delimiters,
            split::empties_t empties = split::empties_ok);

    /**
     * Sets the values in zero
     */
    void setValuesInZero();

    /** The buffer */
    std::string m_buffer;

    /** The latitude */
    double m_dLatitude;

    /** The longitude */
    double m_dLongitude;

    /** The altitude */
    double m_dAltitude;

    /** The time */
    int m_iTime;

    /** The speed */
    double m_dSpeed;

    /** The number of the satellites */
    int m_iNSatellites;

    /** Whether the GPGGA data is correct */
    bool m_bIsValidGPGGAData;

    /** Whether the GPRMC data is correct */
    bool m_bIsValidGPRMCData;
};

#endif	/* PARSING_H */


#ifndef __ACS_UTIL
#define __ACS_UTIL

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <utility>

using namespace spacehauc_i2c;

const double PI = 3.1415926535;
const double Xmoi = .5; // kg/m^2
const double Ymoi = .5; // kg/m^2
const double Zmoi = 1; // kg/m^2
const double LIGHT_SIDE_THRESHOLD = 200; // LUX
const double SPIN_THRESHOLD = 3; // DEGREES
const double SUN_THRESHOLD = 3; // DEGREES

struct ACS_DATA_BLOCK {
        Triplet<double> mag_field;
        Triplet<double> gyro_data;
        Triplet<double> angular_momenta;

        bool sun_sensor_dark; // 1 if dark, 0 if sun
        std::pair<double, double> sun_angle;

        double light_sensor_1;
        double light_sensor_2;
        double light_sensor_3;
        double light_sensor_4;
        double light_sensor_5;
        double light_sensor_6;
        double light_sensor_7;
        double light_sensor_8;

        void print() {
                std::cout << "light_sensor_1 = " << light_sensor_1 \
                        << "\nlight_sensor_2 = " << light_sensor_2 \
                        << "\nlight_sensor_3 = " << light_sensor_3 \
                        << "\nlight_sensor_4 = " << light_sensor_4 \
                        << "\nlight_sensor_5 = " << light_sensor_5 \
                        << "\nlight_sensor_6 = " << light_sensor_6 \
                        << "\nlight_sensor_7 = " << light_sensor_7 \
                        << "\nlight_sensor_8 = " << light_sensor_8 \
                        << "\nsun_angle = " << sun_angle.first << sun_angle.second \
                        << "\nmag_field = " << mag_field.X << ", " << mag_field.Y << ", " << mag_field.Z \
                        << "\nmag_field = " << gyro_data.X << ", " << gyro_data.Y << ", " << gyro_data.Z;
        }
};

Triplet<double> getAngMomentum(ACS_DATA_BLOCK data);
bool isDarkSide(ACS_DATA_BLOCK data);
bool isSpinStab(ACS_DATA_BLOCK data);
bool isSunPointed(ACS_DATA_BLOCK data);

double mag(Triplet<double> trip) {
        return sqrt(pow(trip.X, 2) + pow(trip.Y, 2) + pow(trip.Z, 2));
}

class NSSOC_A60 {
        public:
                void init() {}
                std::pair<double, double> read() {
                        std::pair<double, double> return_val = std::make_pair(3, 5);
                        return return_val;
                }
                bool seeSun() {
                        return time(NULL) % 2;
                }
};

#endif  // __ACS_UTIL

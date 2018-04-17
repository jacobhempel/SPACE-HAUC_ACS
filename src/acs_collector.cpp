//#include "../include/spacehauc-i2c-dev.h"
#include "../include/spacehauc-i2c-mock.h"
#include "../include/acs_util.h"

//using namespace spacehauc_i2c;
using namespace spacehauc_i2c_mock;

#include "./spacehauc-i2c-dev.h"

int main(int argc, char const *argv[]) {
        ACS_DATA_BLOCK publisher_data;

        //publisher<bool> spin_stab_pub("SPIN_STAB", argv[1]);
        //publisher<bool> sun_point_pub("SUN_POINT", argv[1]);
        //publisher<bool> dark_side_pub("DARK_SIDE", argv[1]);
        //publisher<ACS_DATA_BLOCK> acs_pub("ACS_DATA", argv[1]);

        int bus = 1;
        spacehauc_i2c_mock::I2C_Bus::init(bus);

        spacehauc_i2c_mock::TSL2561 light_sensor_1(0x01);
        spacehauc_i2c_mock::TSL2561 light_sensor_2(0x02);
        spacehauc_i2c_mock::TSL2561 light_sensor_3(0x03);
        spacehauc_i2c_mock::TSL2561 light_sensor_4(0x04);
        spacehauc_i2c_mock::TSL2561 light_sensor_5(0x05);
        spacehauc_i2c_mock::TSL2561 light_sensor_6(0x06);
        spacehauc_i2c_mock::TSL2561 light_sensor_7(0x07);
        spacehauc_i2c_mock::TSL2561 light_sensor_8(0x08);

        light_sensor_1.init();
        light_sensor_2.init();
        light_sensor_3.init();
        light_sensor_4.init();
        light_sensor_5.init();
        light_sensor_6.init();
        light_sensor_7.init();
        light_sensor_8.init();

        Adafruit9DOF sensor;
        sensor.accelerometer.init();
        sensor.magnetometer.init();
        sensor.gyroscope.init();

        NSSOC_A60 sun_sensor;
        sun_sensor.init();

        while (true) {
                publisher_data.light_sensor_1 = light_sensor_1.read();
                publisher_data.light_sensor_2 = light_sensor_2.read();
                publisher_data.light_sensor_3 = light_sensor_3.read();
                publisher_data.light_sensor_4 = light_sensor_4.read();
                publisher_data.light_sensor_5 = light_sensor_5.read();
                publisher_data.light_sensor_6 = light_sensor_6.read();
                publisher_data.light_sensor_7 = light_sensor_7.read();
                publisher_data.light_sensor_8 = light_sensor_8.read();

                publisher_data.mag_field = sensor.magnetometer.readTriplet();
                publisher_data.gyro_data = sensor.gyroscope.readTriplet();

                publisher_data.sun_sensor_dark = !sun_sensor.seeSun();
                publisher_data.sun_angle = sun_sensor.read();

                publisher_data.angular_momenta = getAngMomentum(publisher_data);

                publisher_data.print();

                //acs_pub.publish(publisher_data);
                //dark_side_pub.publish(isDarkSide());
                //spin_stab_pub.publish(isSpinStab());
                //sun_point_pub.publish(isSunPointed());

        }
        return 0;
}

spacehauc_i2c::Triplet<double> getAngMomentum(ACS_DATA_BLOCK data) {
        spacehauc_i2c::Triplet<double> return_val;
        return_val.X = data.gyro_data.X * Xmoi;
        return_val.Y = data.gyro_data.Y * Ymoi;
        return_val.Z = data.gyro_data.Z * Zmoi;
        return return_val;
}

bool isDarkSide(ACS_DATA_BLOCK data) {
        if (data.light_sensor_1 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_2 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_3 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_4 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_5 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_6 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_7 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (data.light_sensor_8 > LIGHT_SIDE_THRESHOLD) {
                return false;
        }
        if (!data.sun_sensor_dark) {
                return false;
        }
        return true;
}

bool isSpinStab(ACS_DATA_BLOCK data) {
        return ((data.angular_momenta.Z / mag(data.angular_momenta)) > cos(SPIN_THRESHOLD * PI / 180.0));
}

bool isSunPointed(ACS_DATA_BLOCK data) {
        return (data.sun_angle.second < SUN_THRESHOLD);
}

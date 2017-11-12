

int main(int argc, char const *argv[]) {

  publisher<bool> power_request("ACS_POWER_REQUEST", argv[0]);
  subscriber<bool> power_reply("ACS_POWER_REPLY", argv[0]);

  subscriber<bool> spin_stab_sub("SPIN_STAB", argv[0]);
  subscriber<bool> sun_point_sub("SUN_POINT", argv[0]);
  subscriber<bool> dark_side_sub("DARK_SIDE", argv[0]);

  subscriber<ACS_DATA_BLOCK> acs_sub("ACS_DATA", argv[0]);

  ACS_DATA_BLOCK my_data;

  while (true)
    while (!dark_side_sub.getData()) { // LIGHT SIDE STUFF

      if (!spin_stab_sub.getData()) { // NOT SPIN STABBED
        my_data = acs_sub.getData();
        power_request.publish(true);
        if (power_reply.getData()) {
          stabilize_spin(my_data);
        }
      }

      else { // MEANS YOU ARE SPIN STABILIZED
        if (!sun_point_sub.getData()) {
          my_data = acs_sub.getData();
          power_request.publish(true);
          if (power_reply.getData()) {
            point_to_sun(my_data);
          }
        }
        else {
          power_request.publish(false);
          sleep(2);
        } // END OF SUN POINT

      } // END OF SPIN STAB LOOP

    } // END OF LIGHT SIDE LOOP

    power_request.publish(false);
    // SET TORQUERS TO 0

    while (dark_side_sub.getData()) { // DARK SIDE STUFF
      // J chill and wait for light side
      sleep(5);
    }
  }


  return 0;
}

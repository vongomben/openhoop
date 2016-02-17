
void printGyro()
{
  // To read from the gyroscope, you must first call the
  // readGyro() function. When this exits, it'll update the
  // gx, gy, and gz variables with the most current data.
  imu.readGyro();
  
  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
  SerialUSB.print("G: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
  SerialUSB.print(imu.calcGyro(imu.gx), 2);
  SerialUSB.print(", ");
  SerialUSB.print(imu.calcGyro(imu.gy), 2);
  SerialUSB.print(", ");
  SerialUSB.println(imu.calcGyro(imu.gz), 2);
#elif defined PRINT_RAW
  SerialUSB.print(imu.gx);
  SerialUSB.print(", ");
  SerialUSB.print(imu.gy);
  SerialUSB.print(", ");
  SerialUSB.println(imu.gz);
#endif
}

void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  imu.readAccel();
  
  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  SerialUSB.print("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  SerialUSB.print(imu.calcAccel(imu.ax), 2);
  SerialUSB.print(", ");
  SerialUSB.print(imu.calcAccel(imu.ay), 2);
  SerialUSB.print(", ");
  SerialUSB.println(imu.calcAccel(imu.az), 2);
#elif defined PRINT_RAW 
  SerialUSB.print(imu.ax);
  SerialUSB.print(", ");
  SerialUSB.print(imu.ay);
  SerialUSB.print(", ");
  SerialUSB.println(imu.az);
#endif

}

void printMag()
{
  // To read from the magnetometer, you must first call the
  // readMag() function. When this exits, it'll update the
  // mx, my, and mz variables with the most current data.
  imu.readMag();
  
  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
  SerialUSB.print("M: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
  SerialUSB.print(imu.calcMag(imu.mx), 2);
  SerialUSB.print(", ");
  SerialUSB.print(imu.calcMag(imu.my), 2);
  SerialUSB.print(", ");
  SerialUSB.println(imu.calcMag(imu.mz), 2);
#elif defined PRINT_RAW
  SerialUSB.print(imu.mx);
  SerialUSB.print(", ");
  SerialUSB.print(imu.my);
  SerialUSB.print(", ");
  SerialUSB.println(imu.mz);
#endif
}
// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  
  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
  SerialUSB.print("Pitch, Roll: ");
  SerialUSB.print(pitch, 2);
  SerialUSB.print(", ");
  SerialUSB.println(roll, 2);
  SerialUSB.print("Heading: "); SerialUSB.println(heading, 2);
}

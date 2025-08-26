#include <BleMouse.h>
#include <Adafruit_MPU6050.h>

#define SPEED 10

Adafruit_MPU6050 mpu;
BleMouse bleMouse;

bool sleepMPU = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting ESP32...");

  bleMouse.begin();
  Serial.println("BLE Mouse started, waiting for connection...");

  delay(1000);

  // Try to initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Put MPU to sleep until BLE is connected
  mpu.enableSleep(sleepMPU);
  Serial.println("MPU6050 is sleeping until BLE connects...");
}

void loop() {
  if (bleMouse.isConnected()) {
    if (sleepMPU) {
      delay(3000);
      Serial.println("BLE connected! Waking MPU6050...");
      sleepMPU = false;
      mpu.enableSleep(sleepMPU);
      delay(500);
    }

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Debug print gyro values
    Serial.print("Gyro X: ");
    Serial.print(g.gyro.x);
    Serial.print("  Gyro Z: ");
    Serial.println(g.gyro.z);

    // Move mouse based on gyro
    bleMouse.move(g.gyro.z * -SPEED, g.gyro.x * -SPEED);
  } else {
    Serial.println("Waiting for BLE connection...");
    delay(1000);
  }
}

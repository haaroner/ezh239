class MotorDRV8870 {
private:
    int pinIN1;
    int pinIN2;

public:
    MotorDRV8870(int in1, int in2) {
        pinIN1 = in1;
        pinIN2 = in2;
    }

    void init() {
        pinMode(pinIN1, OUTPUT);
        pinMode(pinIN2, OUTPUT);
        stop();
    }

    void move(int speed) {
        speed = constrain(speed, -255, 255);

        if (speed > 0) {
            analogWrite(pinIN1, speed);
            analogWrite(pinIN2, 0);
        } 
        else if (speed < 0) {
            analogWrite(pinIN1, 0);
            analogWrite(pinIN2, abs(speed));
        } 
        else {
            stop();
        }
    }

    void stop() {
        analogWrite(pinIN1, 0);
        analogWrite(pinIN2, 0);
    }
};

MotorDRV8870 motor(5, 6);

void setup() {
    motor.init();
}

void loop() {
    motor.move(255);
    delay(2000);

    motor.move(120);
    delay(2000);

    motor.stop();
    delay(1000);

    motor.move(-200);
    delay(2000);

    motor.stop();
    delay(3000);
}
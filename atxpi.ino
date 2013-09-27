#define DEBUG 0

#include <Bounce.h>

#define READY_LED 11
#define POWER_LED 0
#define HDD_LED 1
#define POWER_SWITCH 2
#define RESET_SWITCH 3
#define PI_GPIO_23 4 // used to tell rpi to shutdown
#define PI_GPIO_24 5 // used to tell teensy the rpi is up, and when to cut power
#define PI_GPIO_25 6 // used to tell the teensy the rpi is rebooting (don't cut power)
#define PS_ON 7


int power_state = 0;
int soft_shutdown_triggered = 0;
int rpi_is_up = 0;
int rpi_rebooting = 0;
Bounce power_button = Bounce(POWER_SWITCH, 5);
Bounce reset_button = Bounce(RESET_SWITCH, 5);
elapsedMillis power_button_timer;
elapsedMillis reset_button_timer;


int powerOn() {
	digitalWrite(PS_ON, LOW);
	digitalWrite(POWER_LED, HIGH);
	power_state = 1;
	delay(1000); // allow rpi gpio lines to stabilize
}


int powerOff() {
	digitalWrite(PS_ON, HIGH);
	digitalWrite(POWER_LED, LOW);
	digitalWrite(PI_GPIO_23, LOW);
	power_state = 0;
	rpi_is_up = 0;
}


void setup() {
	#if DEBUG
	Serial.begin(115200);
	#endif
	pinMode(READY_LED, OUTPUT);
	pinMode(POWER_LED, OUTPUT);
	pinMode(HDD_LED, OUTPUT);
	pinMode(PI_GPIO_23, OUTPUT);
	pinMode(PS_ON, OUTPUT);
	pinMode(PI_GPIO_23, OUTPUT);

	pinMode(PI_GPIO_24, INPUT);
	pinMode(PI_GPIO_25, INPUT);
	pinMode(POWER_SWITCH, INPUT_PULLUP);
	pinMode(RESET_SWITCH, INPUT_PULLUP);

	digitalWrite(PI_GPIO_23, LOW);
	digitalWrite(PS_ON, HIGH);
	digitalWrite(POWER_LED, LOW);
	digitalWrite(HDD_LED, LOW);

	digitalWrite(READY_LED, HIGH);
	#if DEBUG
	Serial.println("atxpi online");
	#endif
}


void loop() {
	power_button.update();
	if(power_button.fallingEdge()) {
		#if DEBUG
		Serial.println("power button pressed");
		#endif
		if(power_state == 0) { // activate psu
			#if DEBUG
			Serial.println("power_state: 0, activating psu");
			#endif
			powerOn();
		}
		else { // deactivate psu via power button
			#if DEBUG
			Serial.println("power_state: 1, shutdown");
			#endif
			power_button_timer = 0;
			for(; !power_button.risingEdge(); power_button.update()) {
				if(!soft_shutdown_triggered && power_button_timer >= 1000) {
					// power button held down for 1 second, tell rpi to shutdown
					soft_shutdown_triggered = 1; // set this so the soft shutdown will only be triggered once per power button press
					digitalWrite(PI_GPIO_23, HIGH);
					#if DEBUG
					Serial.println("soft shutdown triggered");
					#endif
				}
				if(power_button_timer >= 7000) {
					#if DEBUG
					Serial.println("forcing shutdown");
					#endif
					// power button held down for 7 seconds, force shutdown
					powerOff();
					break;
				}
			}
			soft_shutdown_triggered = 0;
		}
	}

	reset_button.update();
	if(power_state == 1 && reset_button.fallingEdge()) {
		#if DEBUG
		Serial.println("reset button pressed");
		#endif
		reset_button_timer = 0;
		for(; !reset_button.risingEdge(); reset_button.update()) {
			if(reset_button_timer >= 1000) {
				// reset button held down for 1 second, cycle power
				#if DEBUG
				Serial.println("cycling power");
				#endif
				powerOff();
				delay(1000);
				powerOn();
				break;
			}
		}
	}

	// set rpi_is_up to 1 when PI_GPIO_24 goes high
	if(power_state && !rpi_is_up && digitalRead(PI_GPIO_24)) {
		#if DEBUG
		Serial.println("rpi_is_up = 1");
		#endif
		rpi_is_up = 1;
	}

	// allow rpi to reboot without cutting power
	if(rpi_is_up && digitalRead(PI_GPIO_25)) {
		#if DEBUG
		Serial.println("rpi rebooting, waiting for GPIO 25 to go LOW");
		#endif
		rpi_is_up = 0; // stop power from being cut
		for(; digitalRead(PI_GPIO_25);) { // wait untill rpi has rebooted
		}
		#if DEBUG
		Serial.println("GPIO 25 has gone LOW");
		#endif
	}

	// cut power once rpi has shutdown
	if(rpi_is_up && !digitalRead(PI_GPIO_24)) {
		#if DEBUG
		Serial.println("rpi has shutdown, cutting power");
		#endif
		delay(5000);
		powerOff();
		#if DEBUG
		Serial.println("power cut");
		#endif
	}
}

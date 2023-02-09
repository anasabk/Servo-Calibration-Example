#include <iostream>
#include <cmath>

#include "lwip/sys.h"
#include "driver/ledc.h"
#include "MyServo.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (16) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 13 bits
#define MAX_LEDC_DUTY           (pow(2, (double) LEDC_DUTY_RES) - 1) // Set duty to 50%. ((2 ** 10) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (50) // Frequency in Hertz. Set frequency at 1 kHz


using namespace std;

static void init(void) {
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = LEDC_OUTPUT_IO,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

static void setPWM(int value) {
    uint32_t duty = MAX_LEDC_DUTY * value * LEDC_FREQUENCY / 1000000;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

static void servo_test(void) {
    uint8_t ch;
    int input = 0;
    int degree_list[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180};
    int pwm_list[] = {500, 570, 650, 750, 790, 800, 880, 1030, 1200, 1290, 1350, 1420, 1550, 1630, 1730, 1830, 1930, 2000, 2100};
    int data_len = 19;
    MyServo servoCal(16), servoNCal(16);
    MyServo *target = &servoNCal;

    servoCal.refreshFitter(pwm_list, degree_list, data_len);
    init();

    while (1) {
	    ch = fgetc(stdin);
	    if (ch!=0xFF) {
            if(ch == 'c') {
                target = &servoCal;
            }

            else if(ch == 'n') {
                target = &servoNCal;
            }

            else if(ch == 'z') {
                input = 0;
            }

            else if(ch != 'k'){
                input = input*10 + ch - 48;
                cout << "Recieved " << ch << ", input = " << input << "\n";
            }

            else {
                setPWM(target->getPWM(input));
                input = 0;
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void) {
    servo_test();
}
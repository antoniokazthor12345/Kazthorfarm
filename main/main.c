#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_crt_bundle.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "mqtt_client.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_adc/adc_oneshot.h"

// ============================================================
// PINES
// ============================================================
#define SOIL_SENSOR GPIO_NUM_4
#define WATER_SENSOR GPIO_NUM_5
#define RELAY GPIO_NUM_10

#define I2C_MASTER_SCL_IO GPIO_NUM_9
#define I2C_MASTER_SDA_IO GPIO_NUM_8
#define I2C_MASTER_FREQ_HZ 400000
#define SSD1306_ADDR 0x3C

#define LED_VERDE GPIO_NUM_15
#define LED_ROJO GPIO_NUM_11
#define BUZZER GPIO_NUM_12
#define FAN_PIN GPIO_NUM_16
#define MIST_PIN GPIO_NUM_17
#define LIGHT_PIN GPIO_NUM_18
#define TIMER_BTN_GPIO GPIO_NUM_19

#define DS18B20_GPIO GPIO_NUM_13

// ============================================================
// CREDENCIALES
// ============================================================
#define WIFI_SSID "**********************************"
#define WIFI_PASS "**********************************"

#define MQTT_BROKER_URI "**********************************"
#define MQTT_USERNAME "**********************************"
#define MQTT_PASSWORD "**********************************"

#define MQTT_TOPIC_PUB "kazthor/farm01/data"
#define MQTT_TOPIC_CMD "kazthor/farm01/cmd"
#define MQTT_TOPIC_STATUS "kazthor/farm01/status"
#define MQTT_TOPIC_ALERTS "kazthor/farm01/alerts"

// ============================================================
// PARAMETROS AJUSTABLES
// ============================================================
#define MAX_LEVEL 3
#define BOMBA_ON_TICKS 10
#define BOMBA_WAIT_TICKS 20
#define CRITICAL_ESCALATE_S 15

// ============================================================
// ESTADO GLOBAL
// ============================================================
static int fan_level = 0;
static int mist_level = 0;
static bool light_on = false;
static int timer_level = 0;

static uint32_t critical_seconds = 0;
static bool critical_phase2 = false;
static uint32_t bomba_wait_counter = 0;
static int critical_confirm_count = 0;
static bool critical_confirmed = false;

static const char *TAG = "KAZTHOR_FARM_RTOS";

static i2c_master_bus_handle_t bus_handle;
static i2c_master_dev_handle_t oled_handle;
static adc_oneshot_unit_handle_t adc_handle;
static SemaphoreHandle_t farm_mutex;

static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;

static bool modo_manual = false;
static bool modo_manual_clima = false;
static bool comando_bomba = false;

static int cmd_fan_target = -1;
static int cmd_mist_target = -1;
static bool cmd_light_pending = false;
static bool cmd_light_action = false;
static bool cmd_alarm_pending = false;
static bool cmd_alarm_action = false;
static int cmd_timer_target = -1;

static uint32_t timer_countdown_s = 0;
static const uint32_t TIMER_HORAS_S[] = {0, 3600, 7200, 10800};

// ============================================================
// ESTRUCTURA DE DATOS
// ============================================================
typedef struct
{
    float temp;
    int soil_percent;
    int water_percent;
    int soil_raw;
    int water_raw;
    bool temp_ok;
    char estado[20];
    bool bomba_activa;
    uint32_t tiempo_bomba;
    int fan_level;
    int mist_level;
    bool light_on;
    bool alarm_on;
    int timer_level;
} farm_data_t;

static farm_data_t farm_data = {
    .temp = 0,
    .soil_percent = 0,
    .water_percent = 0,
    .soil_raw = 0,
    .water_raw = 0,
    .temp_ok = true,
    .estado = "NORMAL",
    .bomba_activa = false,
    .tiempo_bomba = 0,
    .fan_level = 0,
    .mist_level = 0,
    .light_on = false,
    .alarm_on = false,
    .timer_level = 0,
};

// ============================================================
// FUENTE OLED 5x7
// ============================================================
static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x7E, 0x11, 0x11, 0x11, 0x7E},
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C},
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x49, 0x49, 0x7A},
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x04, 0x02, 0x7F},
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31},
    {0x01, 0x01, 0x7F, 0x01, 0x01},
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    {0x7F, 0x20, 0x18, 0x20, 0x7F},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x07, 0x08, 0x70, 0x08, 0x07},
    {0x61, 0x51, 0x49, 0x45, 0x43},
};

static const uint8_t digit5x7[][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4B, 0x31},
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x30},
    {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1E},
};

// ============================================================
// OLED DRIVER
// ============================================================
static void oled_cmd(uint8_t cmd)
{
    uint8_t data[2] = {0x00, cmd};
    i2c_master_transmit(oled_handle, data, sizeof(data), -1);
}

static void oled_data(uint8_t *data, size_t len)
{
    uint8_t buffer[129];
    buffer[0] = 0x40;
    if (len > 128)
        len = 128;
    memcpy(&buffer[1], data, len);
    i2c_master_transmit(oled_handle, buffer, len + 1, -1);
}

static void oled_set_cursor(uint8_t col, uint8_t page)
{
    oled_cmd(0xB0 + page);
    oled_cmd(0x00 + (col & 0x0F));
    oled_cmd(0x10 + ((col >> 4) & 0x0F));
}

static void oled_clear(void)
{
    uint8_t zeros[128];
    memset(zeros, 0x00, sizeof(zeros));
    for (int page = 0; page < 8; page++)
    {
        oled_set_cursor(0, page);
        oled_data(zeros, 128);
    }
}

static void oled_char(char c)
{
    uint8_t data[6];
    if (c == ' ')
        memcpy(data, font5x7[0], 5);
    else if (c >= 'A' && c <= 'Z')
        memcpy(data, font5x7[c - 'A' + 1], 5);
    else if (c >= '0' && c <= '9')
        memcpy(data, digit5x7[c - '0'], 5);
    else if (c == '.')
    {
        uint8_t d[5] = {0x00, 0x60, 0x60, 0x00, 0x00};
        memcpy(data, d, 5);
    }
    else if (c == '-')
    {
        uint8_t d[5] = {0x08, 0x08, 0x08, 0x08, 0x08};
        memcpy(data, d, 5);
    }
    else if (c == ':')
    {
        uint8_t d[5] = {0x00, 0x36, 0x36, 0x00, 0x00};
        memcpy(data, d, 5);
    }
    else
        memcpy(data, font5x7[0], 5);
    data[5] = 0x00;
    oled_data(data, 6);
}

static void oled_print(uint8_t col, uint8_t page, const char *text)
{
    oled_set_cursor(col, page);
    while (*text)
        oled_char(*text++);
}

static void oled_init(void)
{
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SSD1306_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &oled_handle));

    uint8_t init_cmds[] = {
        0xAE, 0x20, 0x00, 0xB0, 0xC8, 0x00, 0x10, 0x40,
        0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F, 0xA4,
        0xD3, 0x00, 0xD5, 0xF0, 0xD9, 0x22, 0xDA,
        0x12, 0xDB, 0x20, 0x8D, 0x14, 0xAF};
    for (int i = 0; i < (int)sizeof(init_cmds); i++)
        oled_cmd(init_cmds[i]);
}

// ============================================================
// ONE-WIRE / DS18B20
// ============================================================
static void ow_low(void)
{
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DS18B20_GPIO, 0);
}
static void ow_release(void) { gpio_set_direction(DS18B20_GPIO, GPIO_MODE_INPUT); }
static int ow_read_pin(void) { return gpio_get_level(DS18B20_GPIO); }

static bool ow_reset(void)
{
    ow_low();
    esp_rom_delay_us(480);
    ow_release();
    esp_rom_delay_us(70);
    bool presence = (ow_read_pin() == 0);
    esp_rom_delay_us(410);
    return presence;
}

static void ow_write_bit(int bit)
{
    if (bit)
    {
        ow_low();
        esp_rom_delay_us(6);
        ow_release();
        esp_rom_delay_us(64);
    }
    else
    {
        ow_low();
        esp_rom_delay_us(60);
        ow_release();
        esp_rom_delay_us(10);
    }
}

static int ow_read_bit(void)
{
    ow_low();
    esp_rom_delay_us(6);
    ow_release();
    esp_rom_delay_us(9);
    int bit = ow_read_pin();
    esp_rom_delay_us(55);
    return bit;
}

static void ow_write_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        ow_write_bit(byte & 0x01);
        byte >>= 1;
    }
}

static uint8_t ow_read_byte(void)
{
    uint8_t value = 0;
    for (int i = 0; i < 8; i++)
    {
        value >>= 1;
        if (ow_read_bit())
            value |= 0x80;
    }
    return value;
}

static bool ds18b20_read_temp(float *temp_c)
{
    if (!ow_reset())
        return false;
    ow_write_byte(0xCC);
    ow_write_byte(0x44);
    vTaskDelay(pdMS_TO_TICKS(750));
    if (!ow_reset())
        return false;
    ow_write_byte(0xCC);
    ow_write_byte(0xBE);
    uint8_t lsb = ow_read_byte();
    uint8_t msb = ow_read_byte();
    int16_t raw = (msb << 8) | lsb;
    *temp_c = raw / 16.0f;
    return true;
}

// ============================================================
// CONVERSION SENSORES
// ============================================================
static int soil_to_percent(int raw)
{
    const int DRY_RAW = 4095, WET_RAW = 1800;
    int p = (DRY_RAW - raw) * 100 / (DRY_RAW - WET_RAW);
    if (p < 0)
        p = 0;
    if (p > 100)
        p = 100;
    return p;
}

static int water_to_percent(int raw)
{
    const int DRY_RAW = 20, FULL_RAW = 2400;
    int p = (raw - DRY_RAW) * 100 / (FULL_RAW - DRY_RAW);
    if (p < 0)
        p = 0;
    if (p > 100)
        p = 100;
    return p;
}

static const char *get_estado(float temp, int soil, int water)
{
    if (temp >= 35.0f || soil < 20 || water < 20)
        return "CRITICAL";
    if (soil < 30 || water < 30)
        return "ALERTA";
    return "NORMAL";
}

// ============================================================
// CONTROL DE BOTONES
// ============================================================
static void press_button_ms(gpio_num_t pin, int ms)
{
    gpio_set_level(pin, 1);
    vTaskDelay(pdMS_TO_TICKS(ms));
    gpio_set_level(pin, 0);
    vTaskDelay(pdMS_TO_TICKS(2500));
    taskYIELD();
}

static void force_to(gpio_num_t pin, int *current, int target)
{
    if (*current == target)
        return;
    int pulse_ms = (pin == FAN_PIN) ? 600 : 300;
    int steps = (target - *current + (MAX_LEVEL + 1)) % (MAX_LEVEL + 1);
    ESP_LOGW(TAG, "[CLIMA] pin=%d %d->%d (%d pasos)", pin, *current, target, steps);
    for (int i = 0; i < steps; i++)
    {
        gpio_set_level(pin, 1);
        vTaskDelay(pdMS_TO_TICKS(pulse_ms));
        gpio_set_level(pin, 0);
        vTaskDelay(pdMS_TO_TICKS(2500));
        taskYIELD();
    }
    *current = target;
}

static void force_off(gpio_num_t pin, int *current)
{
    force_to(pin, current, 0);
}

static void set_timer_level(int target)
{
    if (target < 0)
        target = 0;
    if (target > MAX_LEVEL)
        target = MAX_LEVEL;

    timer_countdown_s = TIMER_HORAS_S[target];

    if (timer_level == target)
    {
        xSemaphoreTake(farm_mutex, portMAX_DELAY);
        farm_data.timer_level = timer_level;
        xSemaphoreGive(farm_mutex);
        ESP_LOGW(TAG, "[TIMER] Mismo nivel=%d, countdown reiniciado a %lu s",
                 timer_level, timer_countdown_s);
        return;
    }

    int steps = (target - timer_level + (MAX_LEVEL + 1)) % (MAX_LEVEL + 1);
    ESP_LOGW(TAG, "[TIMER] %d->%d (%d pulsos)", timer_level, target, steps);

    for (int i = 0; i < steps; i++)
    {
        press_button_ms(TIMER_BTN_GPIO, 300);
        timer_level = (timer_level + 1) % (MAX_LEVEL + 1);
        ESP_LOGI(TAG, "[TIMER] Nivel actual=%d", timer_level);
    }

    xSemaphoreTake(farm_mutex, portMAX_DELAY);
    farm_data.timer_level = timer_level;
    xSemaphoreGive(farm_mutex);
}

static void sync_climate_off_after_timer(void)
{
    fan_level = 0;
    mist_level = 0;
    light_on = false;
    timer_level = 0;
    timer_countdown_s = 0;
    modo_manual_clima = false;

    cmd_fan_target = -1;
    cmd_mist_target = -1;
    cmd_timer_target = -1;
    cmd_light_pending = false;

    gpio_set_level(BUZZER, 0);

    xSemaphoreTake(farm_mutex, portMAX_DELAY);
    farm_data.fan_level = 0;
    farm_data.mist_level = 0;
    farm_data.light_on = false;
    farm_data.alarm_on = false;
    farm_data.timer_level = 0;
    strcpy(farm_data.estado, "NORMAL");
    xSemaphoreGive(farm_mutex);

    ESP_LOGW(TAG, "[TIMER] Expirado: dashboard y OLED sincronizados en OFF");
}

// ============================================================
// MQTT EVENT HANDLER
// ============================================================
static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        mqtt_connected = true;
        ESP_LOGI(TAG, "[MQTT] Conectado");
        esp_mqtt_client_subscribe(mqtt_client, MQTT_TOPIC_CMD, 1);
        break;

    case MQTT_EVENT_DISCONNECTED:
        mqtt_connected = false;
        ESP_LOGW(TAG, "[MQTT] Desconectado");
        break;

    case MQTT_EVENT_DATA:
    {
        char msg[128] = {0};
        int len = event->data_len;
        if (len >= (int)sizeof(msg))
            len = sizeof(msg) - 1;
        memcpy(msg, event->data, len);
        msg[len] = '\0';
        ESP_LOGI(TAG, "[MQTT] CMD: %s", msg);

        if (strstr(msg, "\"mode\":\"AUTO\""))
        {
            modo_manual = false;
            modo_manual_clima = false;
            comando_bomba = false;
        }

        if (strstr(msg, "\"pump\":\"ON\""))
        {
            modo_manual_clima = false;
            modo_manual = true;
            comando_bomba = true;
        }
        else if (strstr(msg, "\"pump\":\"OFF\""))
        {
            modo_manual_clima = false;
            modo_manual = true;
            comando_bomba = false;
        }

        if (strstr(msg, "\"mode\":\"MANUAL_CLIMATE\""))
        {
            modo_manual_clima = true;
        }

        if (strstr(msg, "\"fan\":"))
        {
            int val = 0;
            sscanf(strstr(msg, "\"fan\":") + 6, "%d", &val);
            if (val >= 0 && val <= MAX_LEVEL)
            {
                modo_manual_clima = true;
                cmd_fan_target = val;
                ESP_LOGW(TAG, "[MQTT] CMD FAN -> %d", val);
            }
        }

        if (strstr(msg, "\"mist\":"))
        {
            int val = 0;
            sscanf(strstr(msg, "\"mist\":") + 7, "%d", &val);
            if (val >= 0 && val <= MAX_LEVEL)
            {
                modo_manual_clima = true;
                cmd_mist_target = val;
                ESP_LOGW(TAG, "[MQTT] CMD MIST -> %d", val);
            }
        }

        if (strstr(msg, "\"light\":\"on\""))
        {
            modo_manual_clima = true;
            cmd_light_action = true;
            cmd_light_pending = true;
            ESP_LOGW(TAG, "[MQTT] CMD LIGHT ON");
        }
        else if (strstr(msg, "\"light\":\"off\""))
        {
            modo_manual_clima = true;
            cmd_light_action = false;
            cmd_light_pending = true;
            ESP_LOGW(TAG, "[MQTT] CMD LIGHT OFF");
        }
        else if (strstr(msg, "\"light\":\"press\""))
        {
            modo_manual_clima = true;
            cmd_light_action = !light_on;
            cmd_light_pending = true;
            ESP_LOGW(TAG, "[MQTT] CMD LIGHT PRESS");
        }

        if (strstr(msg, "\"alarm\":\"on\""))
        {
            cmd_alarm_action = true;
            cmd_alarm_pending = true;
            ESP_LOGW(TAG, "[MQTT] CMD ALARM ON");
        }
        else if (strstr(msg, "\"alarm\":\"off\""))
        {
            cmd_alarm_action = false;
            cmd_alarm_pending = true;
            ESP_LOGW(TAG, "[MQTT] CMD ALARM OFF");
        }

        if (strstr(msg, "\"timer\":"))
        {
            int val = 0;
            sscanf(strstr(msg, "\"timer\":") + 8, "%d", &val);
            if (val >= 0 && val <= MAX_LEVEL)
            {
                modo_manual_clima = true;
                cmd_timer_target = val;
                ESP_LOGW(TAG, "[MQTT] CMD TIMER -> %d", val);
            }
        }

        break;
    }

    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "[MQTT] Error");
        break;

    default:
        break;
    }
}

static void mqtt_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
        .credentials.username = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

// ============================================================
// WIFI
// ============================================================
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
        esp_wifi_connect();
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGW(TAG, "[WIFI] Reconectando...");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "[WIFI] IP obtenida");
        mqtt_init();
    }
}

static void wifi_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL));
    wifi_config_t wifi_config = {.sta = {.ssid = WIFI_SSID, .password = WIFI_PASS}};
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "[WIFI] Iniciando...");
}

// ============================================================
// TAREA MQTT PUBLISH
// ============================================================
static void mqtt_publish_task(void *pv)
{
    while (1)
    {
        if (mqtt_connected && mqtt_client != NULL)
        {
            farm_data_t d;
            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            d = farm_data;
            xSemaphoreGive(farm_mutex);

            char payload[320];
            snprintf(payload, sizeof(payload),
                     "{\"temp\":%.2f,\"soil\":%d,\"water\":%d,"
                     "\"state\":\"%s\",\"pump\":%s,\"temp_ok\":%s,"
                     "\"fan\":%d,\"mist\":%d,\"light\":%s,\"alarm\":%s,\"timer\":%d}",
                     d.temp, d.soil_percent, d.water_percent,
                     d.estado,
                     d.bomba_activa ? "true" : "false",
                     d.temp_ok ? "true" : "false",
                     d.fan_level, d.mist_level,
                     d.light_on ? "true" : "false",
                     d.alarm_on ? "true" : "false",
                     d.timer_level);
            esp_mqtt_client_publish(mqtt_client, MQTT_TOPIC_PUB, payload, 0, 1, 0);
            ESP_LOGI(TAG, "[MQTT] %s", payload);

            char status[200];
            snprintf(status, sizeof(status),
                     "{\"state\":\"%s\",\"pump\":%s,\"fan\":%d,\"mist\":%d,"
                     "\"light\":%s,\"alarm\":%s,\"timer\":%d}",
                     d.estado,
                     d.bomba_activa ? "true" : "false",
                     d.fan_level, d.mist_level,
                     d.light_on ? "true" : "false",
                     d.alarm_on ? "true" : "false",
                     d.timer_level);
            esp_mqtt_client_publish(mqtt_client, MQTT_TOPIC_STATUS, status, 0, 1, 0);

            if (strcmp(d.estado, "NORMAL") != 0)
            {
                char alert[180];
                snprintf(alert, sizeof(alert),
                         "{\"alert\":\"%s\",\"soil\":%d,\"water\":%d,\"temp\":%.2f}",
                         d.estado, d.soil_percent, d.water_percent, d.temp);
                esp_mqtt_client_publish(mqtt_client, MQTT_TOPIC_ALERTS, alert, 0, 1, 0);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// ============================================================
// TAREA SENSOR
// ============================================================
static void sensor_task(void *pv)
{
    static float last_valid_temp = 23.0f;
    static int temp_fail_count = 0;

    while (1)
    {
        float temp = 0.0f;
        bool temp_ok = ds18b20_read_temp(&temp);

        if (temp_ok && (temp < -20.0f || temp > 80.0f))
            temp_ok = false;

        if (temp_ok)
        {
            last_valid_temp = temp;
            temp_fail_count = 0;
        }
        else
        {
            temp_fail_count++;
            temp = last_valid_temp;
            if (temp_fail_count < 5)
            {
                temp_ok = true;
                ESP_LOGW(TAG, "[SENSOR] DS18B20 fallo temporal");
            }
            else
            {
                temp_ok = false;
                ESP_LOGE(TAG, "[SENSOR] DS18B20 error persistente");
            }
        }

        int soil_raw = 0, water_raw = 0;
        adc_oneshot_read(adc_handle, ADC_CHANNEL_3, &soil_raw);
        adc_oneshot_read(adc_handle, ADC_CHANNEL_4, &water_raw);

        xSemaphoreTake(farm_mutex, portMAX_DELAY);
        farm_data.temp = temp;
        farm_data.temp_ok = temp_ok;
        farm_data.soil_raw = soil_raw;
        farm_data.water_raw = water_raw;
        farm_data.soil_percent = soil_to_percent(soil_raw);
        farm_data.water_percent = water_to_percent(water_raw);
        xSemaphoreGive(farm_mutex);

        ESP_LOGI(TAG, "[SENSOR] Temp=%.2f Soil=%d Water=%d",
                 temp, farm_data.soil_percent, farm_data.water_percent);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ============================================================
// HELPER
// ============================================================
static void process_climate_cmds(void)
{
    if (cmd_fan_target >= 0)
    {
        force_to(FAN_PIN, &fan_level, cmd_fan_target);
        cmd_fan_target = -1;
    }
    if (cmd_mist_target >= 0)
    {
        force_to(MIST_PIN, &mist_level, cmd_mist_target);
        cmd_mist_target = -1;
    }
    if (cmd_light_pending)
    {
        if (cmd_light_action && !light_on)
        {
            press_button_ms(LIGHT_PIN, 300);
            light_on = true;
            ESP_LOGW(TAG, "[CMD] LIGHT ON");
        }
        else if (!cmd_light_action && light_on)
        {
            press_button_ms(LIGHT_PIN, 300);
            light_on = false;
            ESP_LOGW(TAG, "[CMD] LIGHT OFF");
        }
        cmd_light_pending = false;
    }
    if (cmd_alarm_pending)
    {
        gpio_set_level(BUZZER, cmd_alarm_action ? 1 : 0);
        xSemaphoreTake(farm_mutex, portMAX_DELAY);
        farm_data.alarm_on = cmd_alarm_action;
        xSemaphoreGive(farm_mutex);
        cmd_alarm_pending = false;
        ESP_LOGW(TAG, "[CMD] ALARM %s", cmd_alarm_action ? "ON" : "OFF");
    }
    if (cmd_timer_target >= 0)
    {
        set_timer_level(cmd_timer_target);
        ESP_LOGW(TAG, "[TIMER] Countdown: %lu s", timer_countdown_s);
        cmd_timer_target = -1;
    }
}

// ============================================================
// TAREA CONTROL
// ============================================================
static void control_task(void *pv)
{
    vTaskDelay(pdMS_TO_TICKS(5000));

    char last_estado[20] = "INIT";

    while (1)
    {
        farm_data_t data;
        xSemaphoreTake(farm_mutex, portMAX_DELAY);
        data = farm_data;
        xSemaphoreGive(farm_mutex);

        if (!data.temp_ok)
        {
            gpio_set_level(LED_VERDE, 0);
            gpio_set_level(LED_ROJO, 1);
            gpio_set_level(BUZZER, 0);
            gpio_set_level(RELAY, 0);

            force_off(FAN_PIN, &fan_level);
            force_off(MIST_PIN, &mist_level);
            if (light_on)
            {
                press_button_ms(LIGHT_PIN, 300);
                light_on = false;
            }
            set_timer_level(0);

            critical_seconds = 0;
            critical_phase2 = false;
            bomba_wait_counter = 0;
            critical_confirm_count = 0;
            critical_confirmed = false;
            modo_manual_clima = false;

            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            strcpy(farm_data.estado, "TEMP ERROR");
            farm_data.bomba_activa = false;
            farm_data.tiempo_bomba = 0;
            farm_data.fan_level = fan_level;
            farm_data.mist_level = mist_level;
            farm_data.light_on = light_on;
            farm_data.alarm_on = false;
            farm_data.timer_level = timer_level;
            xSemaphoreGive(farm_mutex);

            strcpy(last_estado, "TEMP ERROR");
            ESP_LOGE(TAG, "[CONTROL] TEMP ERROR");
            vTaskDelay(pdMS_TO_TICKS(4000));
            continue;
        }

        const char *estado = get_estado(data.temp, data.soil_percent, data.water_percent);

        if (timer_countdown_s > 4)
        {
            timer_countdown_s -= 4;
        }
        else if (timer_countdown_s > 0)
        {
            sync_climate_off_after_timer();
            strcpy(last_estado, "TIMER OFF");
            vTaskDelay(pdMS_TO_TICKS(4000));
            continue;
        }

        if (strcmp(estado, "CRITICAL") == 0)
        {
            if (critical_confirm_count < 5)
                critical_confirm_count++;
            if (critical_confirm_count >= 5)
                critical_confirmed = true;
        }
        else
        {
            critical_confirm_count = 0;
            critical_confirmed = false;
        }

        if (modo_manual_clima)
        {
            process_climate_cmds();

            ESP_LOGI(TAG, "[TIMER] Restante: %lu s", timer_countdown_s);

            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            farm_data.fan_level = fan_level;
            farm_data.mist_level = mist_level;
            farm_data.light_on = light_on;
            farm_data.timer_level = timer_level;
            strcpy(farm_data.estado, "MANUAL");
            xSemaphoreGive(farm_mutex);

            vTaskDelay(pdMS_TO_TICKS(4000));
            continue;
        }

        if (modo_manual)
        {
            gpio_set_level(LED_VERDE, 0);
            gpio_set_level(LED_ROJO, 1);
            gpio_set_level(BUZZER, 0);
            gpio_set_level(RELAY, comando_bomba ? 1 : 0);

            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            strcpy(farm_data.estado, "MANUAL");
            farm_data.bomba_activa = comando_bomba;
            farm_data.tiempo_bomba = 0;
            farm_data.fan_level = fan_level;
            farm_data.mist_level = mist_level;
            farm_data.light_on = light_on;
            farm_data.timer_level = timer_level;
            xSemaphoreGive(farm_mutex);

            vTaskDelay(pdMS_TO_TICKS(4000));
            continue;
        }

        if (strcmp(estado, "NORMAL") == 0)
        {
            if (strcmp(last_estado, "NORMAL") != 0)
            {
                ESP_LOGI(TAG, "[CONTROL] -> NORMAL");
                critical_seconds = 0;
                critical_phase2 = false;
                bomba_wait_counter = 0;

                gpio_set_level(RELAY, 0);
                gpio_set_level(BUZZER, 0);

                xSemaphoreTake(farm_mutex, portMAX_DELAY);
                farm_data.bomba_activa = false;
                farm_data.tiempo_bomba = 0;
                farm_data.alarm_on = false;
                xSemaphoreGive(farm_mutex);

                force_off(FAN_PIN, &fan_level);
                force_off(MIST_PIN, &mist_level);
                if (light_on)
                {
                    press_button_ms(LIGHT_PIN, 300);
                    light_on = false;
                }
                set_timer_level(0);
                strcpy(last_estado, "NORMAL");
            }

            gpio_set_level(LED_VERDE, 1);
            gpio_set_level(LED_ROJO, 0);

            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            strcpy(farm_data.estado, "NORMAL");
            farm_data.fan_level = fan_level;
            farm_data.mist_level = mist_level;
            farm_data.light_on = light_on;
            farm_data.timer_level = timer_level;
            xSemaphoreGive(farm_mutex);

            ESP_LOGI(TAG, "[CONTROL] NORMAL FAN=%d MIST=%d", fan_level, mist_level);
        }

        else if (strcmp(estado, "ALERTA") == 0)
        {
            if (strcmp(last_estado, "ALERTA") != 0)
            {
                ESP_LOGI(TAG, "[CONTROL] -> ALERTA");
                critical_seconds = 0;
                critical_phase2 = false;
                bomba_wait_counter = 0;

                gpio_set_level(RELAY, 0);
                gpio_set_level(BUZZER, 0);

                xSemaphoreTake(farm_mutex, portMAX_DELAY);
                farm_data.bomba_activa = false;
                farm_data.tiempo_bomba = 0;
                farm_data.alarm_on = false;
                xSemaphoreGive(farm_mutex);

                force_to(FAN_PIN, &fan_level, 1);
                force_to(MIST_PIN, &mist_level, 1);
                if (!light_on)
                {
                    press_button_ms(LIGHT_PIN, 300);
                    light_on = true;
                    ESP_LOGW(TAG, "[ALERTA] LUZ ON");
                }
                set_timer_level(1);
                strcpy(last_estado, "ALERTA");
            }

            gpio_set_level(LED_VERDE, 0);
            gpio_set_level(LED_ROJO, 1);

            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            strcpy(farm_data.estado, "ALERTA");
            farm_data.fan_level = fan_level;
            farm_data.mist_level = mist_level;
            farm_data.light_on = light_on;
            farm_data.timer_level = timer_level;
            xSemaphoreGive(farm_mutex);

            ESP_LOGI(TAG, "[CONTROL] ALERTA FAN=%d MIST=%d LIGHT=%d",
                     fan_level, mist_level, light_on);
        }

        else
        {
            if (!critical_confirmed)
            {
                gpio_set_level(LED_VERDE, 0);
                gpio_set_level(LED_ROJO, 1);
                gpio_set_level(BUZZER, 0);
                gpio_set_level(RELAY, 0);

                xSemaphoreTake(farm_mutex, portMAX_DELAY);
                strcpy(farm_data.estado, "CRIT WAIT");
                farm_data.fan_level = fan_level;
                farm_data.mist_level = mist_level;
                farm_data.light_on = light_on;
                farm_data.timer_level = timer_level;
                xSemaphoreGive(farm_mutex);

                ESP_LOGW(TAG, "[CRITICAL WAIT] %d/5", critical_confirm_count);
                vTaskDelay(pdMS_TO_TICKS(4000));
                continue;
            }

            gpio_set_level(LED_VERDE, 0);
            gpio_set_level(LED_ROJO, 1);

            if (strcmp(last_estado, "CRITICAL") != 0)
            {
                ESP_LOGW(TAG, "[CONTROL] -> CRITICAL");
                critical_seconds = 0;
                critical_phase2 = false;
                bomba_wait_counter = 0;
                set_timer_level(3);
                strcpy(last_estado, "CRITICAL");
            }

            critical_seconds++;
            ESP_LOGW(TAG, "[CRITICAL] t=%lu s fase=%s",
                     critical_seconds, critical_phase2 ? "2(nv3)" : "1(nv2)");

            if (!critical_phase2)
            {
                gpio_set_level(BUZZER, 0);
                if (fan_level != 2)
                    force_to(FAN_PIN, &fan_level, 2);
                else if (mist_level != 2)
                    force_to(MIST_PIN, &mist_level, 2);

                if (critical_seconds >= CRITICAL_ESCALATE_S)
                {
                    critical_phase2 = true;
                    ESP_LOGW(TAG, "[CRITICAL] Escalando FASE 2 -> nivel 3");
                }
            }
            else
            {
                if (fan_level != 3)
                {
                    force_to(FAN_PIN, &fan_level, 3);
                }
                else if (mist_level != 3)
                {
                    force_to(MIST_PIN, &mist_level, 3);
                }
                else if (!light_on)
                {
                    press_button_ms(LIGHT_PIN, 300);
                    light_on = true;
                    ESP_LOGW(TAG, "[CRITICAL F2] LUZ ON");
                }
                else
                {
                    gpio_set_level(BUZZER, 1);

                    xSemaphoreTake(farm_mutex, portMAX_DELAY);
                    farm_data.alarm_on = true;

                    if (!farm_data.bomba_activa)
                    {
                        if (bomba_wait_counter == 0)
                        {
                            gpio_set_level(RELAY, 1);
                            farm_data.bomba_activa = true;
                            farm_data.tiempo_bomba = BOMBA_ON_TICKS;
                            ESP_LOGW(TAG, "[CRITICAL F2] BOMBA ON %ds", BOMBA_ON_TICKS);
                        }
                        else
                        {
                            bomba_wait_counter--;
                            ESP_LOGI(TAG, "[CRITICAL F2] Pausa bomba: %lu s", bomba_wait_counter);
                        }
                    }
                    else
                    {
                        if (farm_data.tiempo_bomba > 0)
                            farm_data.tiempo_bomba--;
                        if (farm_data.tiempo_bomba == 0)
                        {
                            gpio_set_level(RELAY, 0);
                            farm_data.bomba_activa = false;
                            bomba_wait_counter = BOMBA_WAIT_TICKS;
                            ESP_LOGW(TAG, "[CRITICAL F2] BOMBA OFF - pausa %ds", BOMBA_WAIT_TICKS);
                        }
                    }
                    xSemaphoreGive(farm_mutex);
                }
            }

            xSemaphoreTake(farm_mutex, portMAX_DELAY);
            strcpy(farm_data.estado, "CRITICAL");
            farm_data.fan_level = fan_level;
            farm_data.mist_level = mist_level;
            farm_data.light_on = light_on;
            farm_data.timer_level = timer_level;
            xSemaphoreGive(farm_mutex);

            ESP_LOGW(TAG, "[CONTROL] CRITICAL FAN=%d MIST=%d LIGHT=%d",
                     fan_level, mist_level, light_on);
        }

        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}

// ============================================================
// TAREA DISPLAY OLED
// ============================================================
static void display_task(void *pv)
{
    while (1)
    {
        farm_data_t data;
        xSemaphoreTake(farm_mutex, portMAX_DELAY);
        data = farm_data;
        xSemaphoreGive(farm_mutex);

        char line[32];
        oled_clear();
        oled_print(10, 0, "KAZTHOR FARM");

        if (data.temp_ok)
        {
            snprintf(line, sizeof(line), "TEMP: %.1f C", data.temp);
            oled_print(0, 2, line);
            snprintf(line, sizeof(line), "SUELO: %d", data.soil_percent);
            oled_print(0, 3, line);
            snprintf(line, sizeof(line), "AGUA: %d", data.water_percent);
            oled_print(0, 4, line);
            snprintf(line, sizeof(line), "F:%d M:%d L:%s T:%d",
                     data.fan_level, data.mist_level,
                     data.light_on ? "ON" : "OF",
                     data.timer_level);
            oled_print(0, 5, line);
            snprintf(line, sizeof(line), "EST:%s", data.estado);
            oled_print(0, 6, line);
        }
        else
        {
            oled_print(10, 2, "TEMP ERROR");
            oled_print(10, 4, "REVISAR SENSOR");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ============================================================
// APP MAIN
// ============================================================
void app_main(void)
{
    gpio_set_direction(LED_VERDE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_ROJO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY, GPIO_MODE_OUTPUT);
    gpio_set_direction(FAN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(MIST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LIGHT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(TIMER_BTN_GPIO, GPIO_MODE_OUTPUT);

    gpio_set_level(RELAY, 0);
    gpio_set_level(FAN_PIN, 0);
    gpio_set_level(MIST_PIN, 0);
    gpio_set_level(LIGHT_PIN, 0);
    gpio_set_level(TIMER_BTN_GPIO, 0);
    gpio_set_level(LED_VERDE, 0);
    gpio_set_level(LED_ROJO, 0);
    gpio_set_level(BUZZER, 0);

    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);
    ow_release();

    oled_init();
    oled_clear();

    adc_oneshot_unit_init_cfg_t init_config = {.unit_id = ADC_UNIT_1};
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    adc_oneshot_chan_cfg_t adc_cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_3, &adc_cfg));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_4, &adc_cfg));

    farm_mutex = xSemaphoreCreateMutex();
    if (farm_mutex == NULL)
    {
        ESP_LOGE(TAG, "Error creando mutex");
        return;
    }

    ESP_LOGI(TAG, "KAZTHOR FARM RTOS iniciado");

    wifi_init();

    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
    xTaskCreate(control_task, "control_task", 8192, NULL, 4, NULL);
    xTaskCreate(display_task, "display_task", 4096, NULL, 3, NULL);
    xTaskCreate(mqtt_publish_task, "mqtt_publish_task", 6144, NULL, 2, NULL);
}
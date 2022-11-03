/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/dns.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "driver/uart.h"
#include "user_config.h"
#include "mqtt_client.h"
#include "cJSON.h" 
#include "esp_task_wdt.h"

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define BUF_SIZE (1024)

static const char *TAG = "temperature_collection";

static int s_retry_num = 0;

esp_mqtt_client_handle_t client;

TaskHandle_t mqtt_task_handler;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, ALIYUN_MQTT_TOPIC_UPDATA, "device connect ok.", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, ALIYUN_MQTT_TOPIC_GET, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, ALIYUN_MQTT_TOPIC_GET, 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, ALIYUN_MQTT_TOPIC_UPDATA, "subcribe success.", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            //
            vTaskDelete(mqtt_task_handler);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS
        },
    };

    /* Setting a password implies station will connect to all security modes including WEP/WPA.
        * However these modes are deprecated and not advisable to be used. Incase your Access point
        * doesn't support WPA2, these mode can be enabled by commenting below line */

    if (strlen((char *)wifi_config.sta.password)) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    vEventGroupDelete(s_wifi_event_group);
}

/**
 * @brief 串口响应部分，函数内部完成串口参数设置、用户循环部分.
 *        - 从串口接收传来的温度数据，存入全局变量 temperature 内.
 *        - 从串口接收传来的电机状态数据，存入全局变量 switch_cool
 * 
 */
static void uart_transmit_temperature_handler(){
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0,BUF_SIZE * 2, 0 ,0 ,NULL, 0);
}

/**
  * @brief  采用Alink JSON的格式上报数据，该函数的用途在于合成 JSON 段，使用 string.h 中的strcat函数
  *         拼接。
  * 
  * @param temperature      温度值，用来补充 CurrentTemperature 键值对的 Value
  * @param swtich_toggle    开关状态，用来表示散热电机开关状态 同样补充JSON中对应的键值对。
  * @param publish_buf      接受 JSON 所使用的缓冲区
  */
void combine_publish_data(const char* temperature, bool swtich_toggle, char *publish_buf){
    
    char temp[1024] = {0};
    strcpy(temp, "{");
    strcat(temp,"\"params\":{");
    strcat(temp,"\"CurrentTemperature\":{");
    strcat(temp,"\"value\":");
    strcat(temp, temperature);
    strcat(temp,"},\"Switch\":{");
    strcat(temp,"\"value\":");
    if(swtich_toggle){
        strcat(temp,"1");
    }else{
        strcat(temp,"0");
    }
    strcat(temp,"}}}");
    strcpy(publish_buf, (const char *)temp);
}


/**
  * @brief  数据上报事件循环，通过xTaskCreate 创建到任务当中，死循环中需要喂狗(esp_task_wdt_reset)
  * 
  */
static void publish_temperature(){
    bool switch_cool = false;
    char temperature[48] = "0";
    uint8_t *dat = (uint8_t *)malloc(BUF_SIZE);

    while(1){ 
        char publish_buf[1024] = "initialized";
        
        int len = uart_read_bytes(UART_NUM_0, dat, BUF_SIZE, 20 / portTICK_RATE_MS);
        //这里用来做用户操作--------------
        if(strchr((const char*)dat, '\n') != NULL) {
            // 如果串口软件发送来的数据结尾自带换行符，通过该分支进行筛选
            dat[len] = '\0';    
        }

        if(*dat != 0 && (*dat == '-' || ((*dat >= (uint8_t)('0')) && (*dat <= (uint8_t)('9'))) )){ 
            memset(temperature, 0, 48);
            strncpy(temperature, (const char*)dat, len);
        }
        combine_publish_data((const char*)temperature, switch_cool, publish_buf);
        //uart_write_bytes(UART_NUM_0, (const char*)dat, len);
        /***
         *  esp_mqtt_client_publish 函数为 发布 函数。 若进行数据上报，则 topic 选用物模型相关的上报topic
         *  若只进行消息上报，可使用自定义 topic 中的用户 update
         */
        esp_mqtt_client_publish(client, ALIYUN_MQTT_TOPIC_ATTRIBUTE_PUBLISH, (const char*)publish_buf, 0, 0, 0);  
        /**
         * @brief printf用于调试，将发送到服务器的json数据打印到串口 UART0 
         * 
         */
        //printf(publish_buf);  
        //printf("\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        esp_task_wdt_reset();
    } 

}

/**
 * @brief mqtt 事件处理程序，用于响应mqtt通信过程中产生的事件来调度callback函数.
 *        参数列表一般固定。
 * @param handler_args 
 * @param base 
 * @param event_id 
 * @param event_data 
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

/**
 * @brief mqtt 的启动参数，函数内完成mqtt连接参数的设置、客户端创建、响应程序注册，
 *        以及将用户loop程序添加到任务列表。
 * 
 */
static void mqtt_app_start(void){
    esp_mqtt_client_config_t mcli_config = { 
        .client_id = CONFIG_MQTT_CLIENT_ID,
        .password = CONFIG_MQTT_PASSWORD,
        .username = CONFIG_MQTT_USERNAME,
        .host = CONFIG_MQTT_BORKER_HOST,
        .port = CONFIG_MQTT_BORKER_PORT
    };
    client = esp_mqtt_client_init(&mcli_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    xTaskCreate(publish_temperature,"publish_temperature",4096,NULL,10,mqtt_task_handler);
    //publish_temperature();
}
/**
 * @brief 新版本的 RTOS_SDK 使用 app_main 来作为程序入口.
 * 
 */
void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);
    wifi_init_sta();
    uart_transmit_temperature_handler();
    mqtt_app_start();
    while(1);
}

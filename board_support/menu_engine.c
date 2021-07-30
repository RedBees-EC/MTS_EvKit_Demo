#include "menu_engine.h"

#define CLEAR_SCREEN            "2J"
#define CURSOR_HOME             "4;0H"
#define RESTORE_TERM            "0m"
#define HIDE_CURSOR             "25l"

#define TEXT_BG_WHITE           "47m"
#define TEXT_COLOR_YELLOW       "33m"
#define TEXT_COLOR_GREEN        "32m"
#define TEXT_COLOR_BLACK        "30m"
#define TEXT_COLOR_WHITE        "37m"

#define TEXT_BG_BLUE            "44m"
#define TEXT_BG_YELLOW          "43m"
#define TEXT_BG_GREEN           "42m"
#define TEXT_COLOR_BOLD_RED     "91m"

void esc(const uint8_t *sequence)
{
    printf("\x1B[%s",sequence);
}

void set_cursor_XY(uint8_t X,uint8_t Y)
{
    uint8_t esc_cmd[20];

    snprintf(esc_cmd,20,"\x1B[%d;%dH",Y,X);
    printf("%s",esc_cmd);
}

uint16_t draw_menu_items(const menu_item_descriptor_t* menu_data,uint16_t highlighted_item_no,uint8_t redraw_only)
{
    uint16_t k;

    esc(HIDE_CURSOR);
    esc(CURSOR_HOME);
    esc(TEXT_COLOR_YELLOW);

    k=0;
    while ((menu_data[k].item_string_description[0] != '\0') || (menu_data[k].menu_item_handler != NULL))
    {
        if (k == highlighted_item_no)
        {
            esc(TEXT_BG_YELLOW);
            esc(TEXT_COLOR_BLACK);
        }
        else
        {
            esc(TEXT_BG_BLUE);
            esc(TEXT_COLOR_YELLOW);
        }

        if ((redraw_only == 0) || (k == highlighted_item_no) || (k == (highlighted_item_no-1)) || (k == (highlighted_item_no+1)))
        {
            printf("                                                                                          \r");
            printf(" %s",menu_data[k].item_string_description);
        }

        printf("\r\n");

        k++;
    }

    esc(RESTORE_TERM);

    return k;
}

void draw_menu_header(void)
{
    esc("0;0H");
    esc(CLEAR_SCREEN);
    esc(TEXT_COLOR_BLACK);
    esc(TEXT_BG_GREEN);
    printf("                *** Welcome to MTS NB-IoT Development Kit service menu ***                \r\n");
    printf("                                                                                          \r");
    printf("Firmware version: %s",FIRMWARE_VERSION);
}

/**
    @brief Функция, реализующая новое сервисное меню. См. описание сервисного меню на главной странице.
*/
void service_menu_modern(const menu_item_descriptor_t *menu_data)
{
    uint8_t ch;
    uint8_t esc_seq_counter=0;
    uint8_t exit_counter=0;
    uint16_t selected_item_no = 0;
    uint16_t menu_items_count;
    device_setup_data_t settings;

    recall_device_settings(&settings);

    printf("\x1B]2;MTS Development Kit Service Menu\x07"); //Setting the window title
    printf("\x1B[?3h"); //Set number of columns to 132

    draw_menu_header();
    menu_items_count = draw_menu_items(menu_data,0,0);

    while (1)
    {
        ch = UART_WaitRxByte(USART1);

        if (ch == 'r')
        {
            printf("\x1B]2;MTS Development Kit Service Menu\x07"); //Setting the window title
            printf("\x1B[?3h"); //Set number of columns to 132

            draw_menu_header();
            menu_items_count = draw_menu_items(menu_data,0,0);
        }

        if (ch == 0x0D)
        {
            esc(CLEAR_SCREEN);
            draw_menu_header();
            draw_menu_items(menu_data,selected_item_no,0);
            printf("\r\n");
            esc(TEXT_COLOR_YELLOW);
            menu_data[selected_item_no].menu_item_handler(&settings);
            esc(RESTORE_TERM);
            printf("\r\nFunction OK.\r\n");
        }

        if (ch == 0x1B)
        {
            esc_seq_counter = 1;
            exit_counter++;
        }

        if ((esc_seq_counter == 1) && (ch == '['))
        {
            esc_seq_counter++;
            exit_counter = 0;
        }

        if (exit_counter == 3)
        {
            break;
        }

        if (esc_seq_counter == 2)
        {
            if (ch == 'A')
            {
                esc_seq_counter = 0;

                if (selected_item_no > 0)
                {
                    selected_item_no--;
                }
            }

            if (ch == 'B')
            {
                esc_seq_counter = 0;

                if (selected_item_no < menu_items_count)
                {
                    selected_item_no++;
                }
            }

            draw_menu_items(menu_data,selected_item_no,1);
        }
    }

    esc(CLEAR_SCREEN);
    set_cursor_XY(0,0);
}

/**
    @brief Функция, реализующая классическое сервисное меню. См. описание сервисного меню на главной странице.
*/
void service_menu(void)
{
    uint8_t user_choice;
    device_setup_data_t settings;
    uint16_t k;
    uint8_t user_input[80];

    printf("\r\n*** Welcome to MTS NB-IoT Development Kit service menu ***\r\nFirmware version: %s\r\n\r\n",FIRMWARE_VERSION);

    printf("Current settings found in EEPROM:\r\n\r\n");

    recall_device_settings(&settings);

    if (settings.target_server_IP[0]=='\0')
    {
        printf("Warning: target server IP not set.\r\n");
    }
    else
    {
        printf("Target IP:\t\t%s\r\n",settings.target_server_IP);
    }

    printf("Target port:\t\t%d\r\n",settings.target_server_port);

    if ((!is_alphanumeric(settings.target_URL[1])) || (settings.target_URL[0]=='\0'))
    {
        printf("Warning: target URL not set.\r\n");
    }
    else
    {
        printf("Target URL:\t\t%s\r\n",settings.target_URL);
    }

    if ((!is_alphanumeric(settings.NIDD_APN_name[1])) || (settings.NIDD_APN_name[0]=='\0'))
    {
        printf("Warning: APN name not set.\r\n");
    }
    else
    {
        printf("NIDD APN:\t\t%s\r\n",settings.NIDD_APN_name);
    }

    printf("Use NIDD for telemetry:\t%d\r\n",settings.use_NIDD);

    printf("Board mode on startup:\t");

    if (settings.logger_mode_on_startup==LOGGER_MODE_VALUE)
    {
        printf("logger\r\n");
    }
    else
    {
        printf("service menu\r\n");
    }

    printf("Telemetry interval\r\n(in logger mode):\t%d ms\r\n",settings.telemetry_interval_ms);
    printf("GNSS privacy mode:\t%d\r\n",settings.gnss_privacy_mode);

    printf("\r\nType in a function number from a list below and press enter.\r\n\r\n");

    k=0;
    while ((menu_items[k].menu_item_handler!=NULL) && (menu_items[k].item_string_description[0]!='\0'))
    {
        if (k==0)
        {
            printf("Target server setup:\r\n");
        }
        if (k==4)
        {
            printf("System functions:\r\n");
        }
        printf("\t%d\t- %s\r\n",k+1,menu_items[k].item_string_description);
        k++;
    }

    printf("> ");
    scanf("%s",user_input);
    apply_backspace(user_input,80);
    user_choice = atoi(user_input);

    user_choice--; //Array numbering is zero-based, but menu items are numbered starting from one not to confuse the user

    printf("\r\n--------------------------------------------------------------------------------\r\n");

    if (user_choice>=k)
    {
        printf("Unknown function - %d",user_input+1);
    }
    else
    {
        menu_items[user_choice].menu_item_handler(&settings);
    }

    printf("\r\n--------------------------------------------------------------------------------\r\n");

    printf("Press Enter to return to service menu.\r\n");
    getchar();
    getchar();
    printf("\r\n");
}

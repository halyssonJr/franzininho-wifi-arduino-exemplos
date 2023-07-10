/*
  Autor: Equipe Embarcados
*/

//Inclusão das bibliotecas
#include <lvgl.h>
#include <TFT_eSPI.h>

#define BTN_PIN 3
// Declara objeto da classe TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

/*Declara os objetos da tela */
lv_obj_t* main_job_timer;
lv_obj_t* init_main_bg;
lv_obj_t* init_screen_btn;
lv_obj_t* init_board_bg;
lv_obj_t* franzininho_logo;
lv_obj_t* label_btn;

//Objeto responsável por chamar os outros objetos da tela
lv_obj_t* current_screen;

static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_disp_buf_t draw_buf;

//Declara o arquivo Franzininho_Logo.c como uma imagem
LV_IMG_DECLARE(Franzininho_Logo);

static uint32_t screen_width = 240;
bool change_screen = true;

/*Função de callback */
void disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}


// Função responsável por criar a tela inicial  
void init_screen(lv_obj_t* init_obj)
{
    
    init_board_bg = lv_obj_create(init_obj, NULL);
    lv_obj_set_size(init_board_bg, 240, 240);
    lv_obj_set_style_local_bg_color(init_board_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);

    init_main_bg = lv_obj_create(init_board_bg, NULL);
    lv_obj_set_size(init_main_bg, 220, 220);
    lv_obj_align(init_main_bg,NULL,LV_ALIGN_CENTER,0,0);

    franzininho_logo = lv_img_create(init_main_bg, NULL);
    lv_img_set_src(franzininho_logo, &Franzininho_Logo);
    lv_obj_align(franzininho_logo, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    init_screen_btn = lv_btn_create(init_main_bg,NULL);
    lv_obj_align(init_screen_btn,NULL,LV_ALIGN_IN_BOTTOM_MID,0,-10);
    lv_btn_set_fit2(init_screen_btn, LV_FIT_NONE, LV_FIT_TIGHT);

    label_btn = lv_label_create(init_screen_btn,NULL);
    lv_label_set_text(label_btn,"Iniciar");
    lv_obj_align(label_btn,NULL,LV_ALIGN_CENTER,0,0);
}
//Função responsável por criar a tela princial
void main_screen(lv_obj_t* main_obj)
{
    lv_obj_t* main_board = lv_obj_create(main_obj, NULL);
    lv_obj_set_size(main_board,240,240);

    
    lv_obj_t* main_title_bg  = lv_obj_create(main_board, NULL);
    lv_obj_set_size(main_title_bg, 100, 10);
    lv_obj_align(main_title_bg,NULL,LV_ALIGN_IN_TOP_MID,0,30);
    lv_obj_set_style_local_bg_color(main_title_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(43,197,29));

    lv_obj_t* main_title_label = lv_label_create(main_board,NULL);
    lv_label_set_text(main_title_label, "INFOS");
    lv_obj_align(main_title_label,NULL,LV_ALIGN_IN_TOP_MID,0,10);
    lv_obj_set_style_local_text_font(main_title_label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&lv_font_montserrat_16);

    
    lv_obj_t* main_bg  = lv_obj_create(main_board, NULL);
    lv_obj_set_size(main_bg, 220, 100);
    lv_obj_align(main_bg,NULL,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_local_bg_color(main_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(43,197,29));

    
    lv_obj_t* label_info = lv_label_create(main_bg,NULL);
    lv_label_set_recolor(label_info,true);
    lv_label_set_text(label_info,"#ffffff Board# #000000  : Franzininho WiFi#\n"
                                 "#ffffff Site#  #000000  : franzininho.com.br#\n"
                                 "#ffffff Author# #000000 : Diana and Fabio#\n"
                                 "#ffffff Chip#  #000000  : ESP32 S2#\n");
    lv_obj_align(label_info,NULL,LV_ALIGN_CENTER,5,10);
//    lv_obj_set_style_local_text_font(label_info,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&lv_font_montserrat_12);
    
    lv_obj_t* label_info_circular = lv_label_create(main_board,NULL);
    lv_label_set_long_mode(label_info_circular,LV_LABEL_LONG_SROLL_CIRC);
    lv_obj_set_width(label_info_circular, 200);
    lv_label_set_text(label_info_circular, "VENHA PARA A COMUNIDADE !!!");
    lv_obj_align(label_info_circular,NULL,LV_ALIGN_IN_BOTTOM_MID,0,-10);
    lv_obj_set_style_local_text_font(label_info_circular,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&lv_font_montserrat_16);
    
}

void setup() 
{
  Serial.begin(115200);// Inicializa o canal de comunicação Serial em 115200 bps
  delay(1000);
  Serial.println("Demo LVGL Franzininho"); 

  pinMode(BTN_PIN,INPUT_PULLUP);
  
  
  tft.begin(); // Inicializa o barramento SPI       
  tft.setRotation(4); // Orientação na tela (1) Landscape
  
  //Inicializa a lvgl
  lv_init();
  
  //Aloca espaço em memória para o buffer dinâmico de acordo com
  // a resolução da tela
  lv_disp_buf_init( &draw_buf, buf, NULL, screen_width * 10 );

  // Declara a estrutura para configurar o driver
  static lv_disp_drv_t disp_drv;
  
  // Inicializa a estrutura de dados, alocando espaço em memória
  lv_disp_drv_init(&disp_drv);
  
  // Caso deseje alterar a resolução manualmente
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;
  
  // Registra a função de callback para limpar a tela
  disp_drv.flush_cb = disp_flush;
  
  // Recebe o buffer responsável por armazenar os objetos na tela
  disp_drv.buffer = &draw_buf;

  // Registra a estrutura de dados,previamente configurada
  lv_disp_drv_register(&disp_drv);

  //Criar um objeto base para receber os componentes da tela  
  current_screen = lv_obj_create(NULL,NULL);
  
  Serial.println("Tela inicial");
  
  //Rebe os componentes da tela inicial
  init_screen(current_screen);
  
  // Carrega o objeto objeto base para mostrar na tela
  lv_scr_load(current_screen);
}

void loop() 
{
  // função responsável por manter a atualização dos frames
  lv_task_handler();

  // Se o botão for pressionado e ainda estiver na tela principal, então ...
  if(!digitalRead(BTN_PIN) && change_screen)
  {
    
    Serial.println("Tela principal");
    // Recebe os componentes da tela principal 
    main_screen(current_screen);
    
    // Carrega o objeto objeto base para mostrar na tela
    lv_scr_load(current_screen);
    change_screen = false;
  }
  delay(5);
}

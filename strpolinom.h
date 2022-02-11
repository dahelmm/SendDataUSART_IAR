//---------------------------------------------------------------------------
#include <stdbool.h>

#ifndef STRPOLINOM_H
#define STRPOLINOM_H

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif
/****** N_EL_MAX - обязательно кратно 8 *******/
#define N_EL_MAX  16
#define BIT_IN_BYTE 8
#define N_BIT_MAP (N_EL_MAX/BIT_IN_BYTE)
#define N_STR_MAX 1000
#define MAX_CNFG N_STR_MAX



//#define SMB_FOR_SIG 8
#define SD_MAX 32 // ??? load_polinom
/******************************* ????????? load_polinom ***********************************/
typedef struct
{
    unsigned short SW; //service word
    unsigned char DW[SD_MAX][2]; //data word

}PlnmToSend;



enum          {      Q = 0,           P = 1           }; // для p_or_q
enum          {  DISBL = 0,        ENBL = 1           };
enum          {   DOWN = 0,        NORM = 1,    UP = 2};
enum          {   ZERO = 0,        PLUS = 1, MINUS = 3};
enum type_ksi {NEGATIV = 0, POSITIV = 1,  FULL = 2};
enum type_choice {GRD_RND = 2, RND = 3, GRD_FST=4, GRD_LST=5, GRD_IND =6};

struct header_of_string // сервисная информация
{
  unsigned char leight: 6;
/*unsigned*/ char sig: 2; // от -2 до 1
};

struct bit_map //битовая карта char
{
  unsigned char b1: 1;
  unsigned char b2: 1;
  unsigned char b3: 1;
  unsigned char b4: 1;
  unsigned char b5: 1;
  unsigned char b6: 1;
  unsigned char b7: 1;
  unsigned char b8: 1;
};

struct smb_map //символьная карта short, для карты конфигурации
{
  unsigned short s1: 2; // UP,DOWN,NORM или P,Q
  unsigned short s2: 2;
  unsigned short s3: 2;
  unsigned short s4: 2;
  unsigned short s5: 2;
  unsigned short s6: 2;
  unsigned short s7: 2;
  unsigned short s8: 2;
};

//typedef struct bit_map bit_configuration [N_BIT_MAP];
typedef struct
{
  struct bit_map adr [N_BIT_MAP];
} bit_configuration;
//typedef struct smb_map smb_configuration [N_BIT_MAP];
typedef struct 
{
  struct smb_map adr [N_BIT_MAP];
} smb_configuration;

typedef unsigned char  byte;

typedef struct
{
   struct header_of_string head;
   struct bit_map  exist_pq[N_BIT_MAP]; //eight_bit = 8 bit
   struct bit_map   p_or_q [N_BIT_MAP]; //eight_smb = 16 bit ???
}string_of_polinom;

typedef struct
{
   unsigned short n_str; // колическтво строк
   unsigned char n_el; // колическтво элементов <= N_EL_MAX
   string_of_polinom strings[N_STR_MAX];

} polinom;

// --------------------------- обеспечивающие функции ------------------------------------------------------
  void null_bit_cnfg(bit_configuration * cnfg);
  void null_bit_map (struct bit_map    * cnfg); //для всех N_BIT_MAP
  void null_smb_map (struct smb_map    * cnfg); //для всех N_BIT_MAP
  unsigned char r_bit(const struct bit_map * str, unsigned char j); // j начинаем считать с первого, а не с 0-го |
  unsigned char r_smb(const struct smb_map * str, unsigned char j); // j начинаем считать с первого, а не с 0-го |
  void          w_bit(struct bit_map * str, unsigned char j, unsigned char bit); //                              |
  void          w_smb(struct smb_map * str, unsigned char j, unsigned char smb); //
  _Bool w_byte(struct bit_map * str, unsigned char byte);//                                              |
  void bit_to_smb (struct bit_map * bit_cnfg, struct smb_map * smb_cnfg);//
  void BitToSmb2 (struct bit_map * bit_cnfg, struct smb_map * smb_cnfg);
  void SetCnfg(struct bit_map * bit_cnfg, unsigned char EnOrDis);
  _Bool w_cnfg(struct bit_map * cnfg, const unsigned char * char_cnfg); //                                    |
  _Bool copy(struct bit_map * cnfg1, const struct bit_map * cnfg2);    //
  _Bool copy_smb(struct smb_map * cnfg1, const struct smb_map * cnfg2);
  _Bool equl (float f1,float f2);                                      //
  _Bool a_more_b (double a, double b);
  //------ Эта функция вызывается при загрузке полинома из txt------------------
  void polinom_init       (void * p_plnm, unsigned short in_n_str);
// -----------------------------------------------------------------------------

// --------------------------- основные функции ------------------------------------------------------------                                                                      |
// через work_conf можно задать вычисление положительного, отрицательного и нормального полинома           |
//относительно элемента j. j начинается с первого, а не с нулевого (!) элемента                            |
        float culc_p        (struct smb_map * work_conf);
        //double culc_pt      (double * pt_el);                                                  //	   |
        //float culc_ksi      (struct smb_map * work_conf, unsigned char j, enum type_ksi type);//           |
        void culc_all_ksi   (struct smb_map * work_conf, float * all_ksi, enum type_ksi type);//           |                                                                                  |
//unsigned char genrt_sing_cnfg(struct smb_map * work_cnfg,/* конфигурация разрешённых к включению элементов */
//                              float * index,             /* ранг элементов для GRD_RNG                     */
//                              struct bit_map * bit_cnfg, /* выбранная рабочая конфигурация                 */
//                              enum type_ksi ksi_type,         /* NEGATIVE или POSITIVE или FULL            */
//                              enum type_choice choice_type    /* GRD_FST или GRD_LST или GRD_IND           */
//                              );
unsigned char first_max_ksi(float * ksi, struct smb_map * cnfg); // начиная с 1

#endif

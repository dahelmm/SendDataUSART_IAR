//---------------------------------------------------------------------------

#include "strpolinom.h"

//------------------------- ?????????????? ??????? ---------------------------
void null_bit_cnfg(bit_configuration * cnfg)
{
   for(unsigned char i=0;i<N_BIT_MAP;i++)
       w_byte(&cnfg->adr[i],0x00);
}

void null_bit_map (struct bit_map    * cnfg)
{
   struct bit_map zero;
   w_byte(&zero,0x00); // 02.02.2022
   //for(unsigned char j=1;j<=BIT_IN_BYTE;j++) w_bit(&zero,j,0);

   for(unsigned char i=0;i<N_BIT_MAP;i++)
       cnfg[i] = zero;
}

void null_smb_map (struct smb_map    * cnfg)
{
   struct smb_map  zero;
   for(unsigned char j=1;j<=BIT_IN_BYTE;j++)
       w_smb(&zero,j,0);

   for(unsigned char i=0;i<N_BIT_MAP;i++)
       cnfg[i] = zero;
}

unsigned char r_smb(const struct smb_map * str, unsigned char j) //начинаем считать с первого, а не с 0-го
{
  if(j>BIT_IN_BYTE*N_BIT_MAP || j<1){
          printf("Err r_smb");
      return 0; // ???? ??????
  }

  for(unsigned char i=0;i<N_BIT_MAP;i++)
  {
     if(j == (1 + i*BIT_IN_BYTE))
        return str[i].s1;
     else if(j == (2 + i*BIT_IN_BYTE))
        return str[i].s2;
     else if(j == (3 + i*BIT_IN_BYTE))
        return str[i].s3;
     else if(j == (4 + i*BIT_IN_BYTE))
        return str[i].s4;
     else if(j == (5 + i*BIT_IN_BYTE))
        return str[i].s5;
     else if(j == (6 + i*BIT_IN_BYTE))
        return str[i].s6;
     else if(j == (7 + i*BIT_IN_BYTE))
        return str[i].s7;
     else if(j == (8 + i*BIT_IN_BYTE))
        return str[i].s8;
  }
}

unsigned char r_bit(const struct bit_map * str, unsigned char j) // ???????? ??????? ? ???????, ? ?? ? 0-??
{
  if(j>BIT_IN_BYTE*N_BIT_MAP || j<1){
          printf("Err r_bit\n");
      return 0; // ???? ??????
  }

  for(unsigned char i=0;i<N_BIT_MAP;i++)
  {
     if(j == (1 + i*BIT_IN_BYTE))
        return str[i].b1;
     else if(j == (2 + i*BIT_IN_BYTE))
        return str[i].b2;
     else if(j == (3 + i*BIT_IN_BYTE))
        return str[i].b3;
     else if(j == (4 + i*BIT_IN_BYTE))
        return str[i].b4;
     else if(j == (5 + i*BIT_IN_BYTE))
        return str[i].b5;
     else if(j == (6 + i*BIT_IN_BYTE))
        return str[i].b6;
     else if(j == (7 + i*BIT_IN_BYTE))
        return str[i].b7;
     else if(j == (8 + i*BIT_IN_BYTE))
        return str[i].b8;
  }
}

void          w_bit(struct bit_map * str, unsigned char j, unsigned char bit)
{
  if(j==0)
        printf("Err w_bit\n");

  if(j<=BIT_IN_BYTE*N_BIT_MAP && j>0)
    for(unsigned char i=0;i<N_BIT_MAP;i++)
    {
     if(j == (1 + i*BIT_IN_BYTE))
        {str[i].b1 = bit; break;}
     else if (j == (2 + i*BIT_IN_BYTE))
        {str[i].b2 = bit; break;}
     else if (j == (3 + i*BIT_IN_BYTE))
        {str[i].b3 = bit; break;}
     else if (j == (4 + i*BIT_IN_BYTE))
        {str[i].b4 = bit; break;}
     else if (j == (5 + i*BIT_IN_BYTE))
        {str[i].b5 = bit; break;}
     else if (j == (6 + i*BIT_IN_BYTE))
        {str[i].b6 = bit; break;}
     else if (j == (7 + i*BIT_IN_BYTE))
        {str[i].b7 = bit; break;}
     else if (j == (8 + i*BIT_IN_BYTE))
        {str[i].b8 = bit; break;}
    }
}
void          w_smb(struct smb_map * str, unsigned char j, unsigned char smb)
{
  if(j==0)
        printf("Err w_smb\n");

  if(j<=BIT_IN_BYTE*N_BIT_MAP && j>0)
    for(unsigned char i=0;i<N_BIT_MAP;i++)
    {
     if(j == (1 + i*BIT_IN_BYTE))
        {str[i].s1 = smb; break;}
     else if (j == (2 + i*BIT_IN_BYTE))
        {str[i].s2 = smb; break;}
     else if (j == (3 + i*BIT_IN_BYTE))
        {str[i].s3 = smb; break;}
     else if (j == (4 + i*BIT_IN_BYTE))
        {str[i].s4 = smb; break;}
     else if (j == (5 + i*BIT_IN_BYTE))
        {str[i].s5 = smb; break;}
     else if (j == (6 + i*BIT_IN_BYTE))
        {str[i].s6 = smb; break;}
     else if (j == (7 + i*BIT_IN_BYTE))
        {str[i].s7 = smb; break;}
     else if (j == (8 + i*BIT_IN_BYTE))
        {str[i].s8 = smb; break;}
    }
}

_Bool        w_byte(struct bit_map * str, unsigned char byte)
{
    struct bit_map * bit8 = (struct bit_map *) &byte;
    * str = * bit8;
}

_Bool     w_cnfg(struct bit_map * bit_cnfg, const unsigned char * char_cnfg)
{
  for(unsigned char i=0;i<N_BIT_MAP;i++)
        w_byte(&bit_cnfg[i], char_cnfg[i]);
  return 0;
}

_Bool     copy(struct bit_map * cnfg1, const struct bit_map * cnfg2)
{
  for(unsigned char i=0;i<N_BIT_MAP;i++)
        cnfg1[i] = cnfg2[i];
  return 0;
}

_Bool     copy_smb(struct smb_map * cnfg1, const struct smb_map * cnfg2)
{
  for(unsigned char i=0;i<N_BIT_MAP;i++)
        cnfg1[i] = cnfg2[i];
  return 0;
}

void bit_to_smb (struct bit_map * bit_cnfg, struct smb_map * smb_cnfg)
{
    for(unsigned char i=1; i<=N_EL_MAX;i++)
        if(r_bit(bit_cnfg,i) == 1)
           w_smb(smb_cnfg,i,NORM);
        else
           w_smb(smb_cnfg,i,DOWN);
}

void BitToSmb2 (struct bit_map * bit_cnfg, struct smb_map * smb_cnfg)
{
    for(unsigned char i=1; i<=N_EL_MAX;i++)
        if(r_bit(bit_cnfg,i) == P)
           w_smb(smb_cnfg,i,UP);
        else
           w_smb(smb_cnfg,i,DOWN);
}

void SetCnfg(struct bit_map * bit_cnfg, unsigned char EnOrDis)
{
    if(ENBL == EnOrDis){
        for(unsigned char i=0;i<N_BIT_MAP;i++)
              w_byte(&bit_cnfg[i], 0xff);
    }
    else if (DISBL == EnOrDis) {
        for(unsigned char i=0;i<N_BIT_MAP;i++)
              w_byte(&bit_cnfg[i], 0x00);
    }
}

polinom * plnm; //
//-------------------------------- ???????? ??????? -------------------------------

void polinom_init       (void * p_plnm, unsigned short in_n_str)
{
  plnm  = (polinom *) p_plnm;

  plnm->n_str = in_n_str; //?????????? ?????
  for(unsigned short i=0; i<plnm->n_str; i++)
  {
    plnm->strings[i].head.sig = 0;
    plnm->strings[i].head.leight = 0;
    for(unsigned short j=1; j<=N_EL_MAX; j++)
    {
      w_bit(plnm->strings[i].exist_pq,j,FALSE);
      w_bit(plnm->strings[i].p_or_q,j,Q);
    }
  }
}

//------------------------------- ?????. ??????? ---------------------------------
float culc_p_str(string_of_polinom* str, struct smb_map * work_conf)
{
  float p_str_res = 1.0;
  unsigned char k=0;

  for(unsigned char i=1;i<=N_EL_MAX;i++)
  {
     if(r_bit(str->exist_pq,i)) // ????? ???? ??? ???????? ? ??????????? ?? bit_work_conf[i] ? p_or_q
     {
       if(r_smb(work_conf, i) == NORM)
            p_str_res=p_str_res * 0.5f;
       else if(r_smb(work_conf, i) == DOWN && r_bit(str->p_or_q,i) == P)
           return 0.0;
       else if(r_smb(work_conf, i) == UP   && r_bit(str->p_or_q,i) == Q)
           return 0.0;
       k++;
     }
     if(k >= (str->head.leight) )
       return p_str_res;
  }
  return p_str_res;
}
//-----------------------------------------------------------------------------------
float culc_p    (struct smb_map * work_conf)
{
    float p_res = 0.0;
    for(unsigned short i=0; i<plnm->n_str; i++){
        if(plnm->strings[i].head.sig == 1)
            p_res = p_res + culc_p_str(&(plnm->strings[i]), work_conf);
        else if(plnm->strings[i].head.sig == -1)
            p_res = p_res - culc_p_str(&(plnm->strings[i]), work_conf);
    }
    return p_res;
}

//------------------------------- ?????. ??????? ---------------------------------
double culc_pt_str(string_of_polinom * str, double * pt_el)
{
   double p_str_res = 1.0;
   unsigned char k=0;

   for(unsigned char i=1;i<=plnm->n_el;i++){
      if(r_bit(str->exist_pq,i))
      {
        if(r_bit(str->p_or_q,i) == P)
                p_str_res=p_str_res*pt_el[i-1];
        else if(r_bit(str->p_or_q,i) == Q)
                p_str_res=p_str_res*(1.0-pt_el[i-1]);
        k++;
      }
      if(k >= (str->head.leight) )
        return p_str_res;
   }
   return p_str_res;
}

double culc_pt      (double * pt_el)
{
    double p_res = 0.0;

    for(unsigned short i=0; i<plnm->n_str; i++){
        if(plnm->strings[i].head.sig == 1)
                p_res = p_res + culc_pt_str(&(plnm->strings[i]), pt_el);
        else if(plnm->strings[i].head.sig == -1)
                p_res = p_res - culc_pt_str(&(plnm->strings[i]), pt_el);
    }
    return p_res;
}

float culc_ksi      (struct smb_map * work_cnfg, unsigned char j, enum type_ksi type)
{
      float ksi = 0.0;
      if(j>plnm->n_el)
         return -2.0; // ??????, j ?????? ???????????

      if(type == NEGATIV){
             ksi = culc_p(work_cnfg);
             w_smb(work_cnfg, j,DOWN);
             ksi = ksi - culc_p(work_cnfg);
             w_smb(work_cnfg, j,NORM);
             return ksi;
      }
      else if(type == POSITIV){
             w_smb(work_cnfg, j,UP);
             ksi = culc_p(work_cnfg);
             w_smb(work_cnfg, j,NORM);
             ksi = ksi - culc_p(work_cnfg);
             return ksi;
      }
      else if(type == FULL){
             w_smb(work_cnfg, j,UP);
             ksi = culc_p(work_cnfg);
             w_smb(work_cnfg, j,DOWN);
             ksi = ksi - culc_p(work_cnfg);
             w_smb(work_cnfg, j,NORM);
             return ksi;
      }
      return 0.0;
}
// ??????????? ??? ??? ??????? ?? ???? ????????? ????? ?????? ?????????? 2 ? -2 ? ??????????? ??
// ?????????????? ???? ??????? ??? ??? ??????? ? ????????????
void culc_all_ksi  (struct smb_map * work_cnfg, float * all_ksi, enum type_ksi type)
{
    for(unsigned char i=1;i<=plnm->n_el;i++) // i ?????? ?????????? ? 1 ? ?? n_el ????????????
    {
       if(r_smb(work_cnfg, i) == NORM)
         all_ksi[i-1] = culc_ksi(work_cnfg, i, type);
       else if(r_smb(work_cnfg, i) == DOWN)
         all_ksi[i-1] = -2.0;
       else if(r_smb(work_cnfg, i) == UP)
         all_ksi[i-1] = 2.0;
    }
}

#define EQUL_ZONE (5E-7f)

_Bool equl(float f1,float f2)
{
   float m_f = (f1+f2)/2.0f;
   if(f1>= (m_f - EQUL_ZONE) &&  f1<= (m_f + EQUL_ZONE) &&
      f2>= (m_f - EQUL_ZONE) &&  f2<= (m_f + EQUL_ZONE))
      return TRUE;
   else
      return FALSE;
}

_Bool a_more_b (double a, double b) //?.? b< ? <= b+eps
{
    double a_b = a - b;

    if(a_b>EQUL_ZONE)
        return TRUE;
    else
        return FALSE;
}

unsigned char find_max_ksi(float * vec, struct smb_map * cnfg, unsigned char * many_max)
{
  float ksi_max = -1.0;
  unsigned char n_max = 0;
  _Bool exist_max = FALSE;

  for(unsigned char i=1;i<=plnm->n_el;i++)
     if(r_smb(cnfg,i) == NORM)
        if(vec[i-1]>ksi_max)
        {
          ksi_max = vec[i-1];
          exist_max = TRUE;
        }

  if(!exist_max)
        return 0;

  for(unsigned char i=1;i<=plnm->n_el;i++)
     if(r_smb(cnfg,i) == NORM && equl(vec[i-1],ksi_max) )
     {
       many_max[n_max] = i;
       n_max++;
     }
  return n_max;
}

unsigned char first_max_ksi(float * ksi, struct smb_map * cnfg)
{
     float ksi_max = -1.0;
     unsigned char res = 0;
     for(unsigned char i=1;i<=plnm->n_el;i++)
        if(r_smb(cnfg,i) == NORM)
           if(ksi[i-1]>ksi_max){
                ksi_max = ksi[i-1];
                res = i;
           }
     //if(res==0)
      //  res = 0;

     return res;
}


_Bool check_copy_cnfg (const struct smb_map * smb_cnfg, struct smb_map * smb_cnfg_start)
{
   unsigned char smb_i = 0;
   for(unsigned char i=1;i<=plnm->n_el;i++)
   {
      smb_i = r_smb(smb_cnfg,i);
      if(smb_i == NORM)
        w_smb(smb_cnfg_start, i, NORM/*UP*/);
      else if (smb_i == DOWN)
        w_smb(smb_cnfg_start, i, DOWN);
      else
        return FALSE; // ?????? ???????????? smb_cnfg
   }
   return TRUE;
}

unsigned char genrt_sing_cnfg(struct smb_map * work_cnfg,/* ???????????? ??????????? ? ????????? ????????? */
                              float * index,             /* ???? ????????? ??? GRD_RNG                     */
                              struct bit_map * bit_cnfg, /* ????????? ??????? ????????????                 */
                              enum type_ksi ksi_type,         /* NEGATIV ??? POSITIV ??? FULL                 */
                              enum type_choice choice_type    /* GRD_FST ??? GRD_LST ??? GRD_IND                */
                              )
{
   struct smb_map smb_cnfg_start[N_BIT_MAP]; // ???????? ???????? ?? smb_cnfg ? smb_cnfg_start
   float vec_ksi[N_EL_MAX];

   if(!(check_copy_cnfg (work_cnfg, smb_cnfg_start))) // ???????? ? ?????? cnfg_start
        return 1;

   for(unsigned char j=0;j<N_BIT_MAP;j++) // ?? ?????? ?????? ??????? ????????????
        w_byte(&(bit_cnfg[j]), 0x00); // ????? ????? ?? ??????? ? ?????????? ???????

   if (culc_p(smb_cnfg_start)<EQUL_ZONE)  // ???????? ?????????????????, ???? ???????? ??? ???-??-? ????????
       return 0;                                // ??????? ???????????? ?? ??????????
// ------------------------- ??????? ???? ----------------------------------
 for(unsigned char step=0;step<plnm->n_el;step++)
 {
   culc_all_ksi(smb_cnfg_start, vec_ksi, ksi_type);
   //------------------- ???????? ??????? ---------------------------------
   unsigned char many_max[N_EL_MAX];
   //??????? ????? ???????? ? ???????? ???????????? ???
   unsigned char n_max = find_max_ksi(vec_ksi, smb_cnfg_start, many_max);
   unsigned char step_choice = 0;
   float max_index = -1.0;

   if(n_max == 1) // ???? ??????? ????, ?? ?? ??????????
      step_choice = many_max[0];
   // ???? ????????? ?????????, ?? ???????? ? ??????????? ?? choice_type
   else if (choice_type == GRD_FST || choice_type == GRD_LST)
   {
      step_choice = many_max[0];
      for(unsigned char loc_i=1;loc_i<n_max;loc_i++)
        if(choice_type == GRD_FST && many_max[loc_i] < step_choice)
           step_choice = many_max[loc_i];
        else if (choice_type == GRD_LST && many_max[loc_i] > step_choice)
           step_choice = many_max[loc_i];
   }
   else if (choice_type == GRD_IND)
   {
      step_choice = many_max[0];
      max_index = index[step_choice];
      for(unsigned char loc_i=1;loc_i<n_max;loc_i++)
          if(index[many_max[loc_i]] > max_index)
          {
             step_choice = many_max[loc_i];
             max_index = index[many_max[loc_i]];
          }
   }

   w_bit(bit_cnfg, step_choice, 1);
 //----------- ????????? ???? ??? UP ? ???????? ????? ???????
   w_smb(smb_cnfg_start, step_choice, UP);
 // ???? ????????? ??????? ???????? ???????? ???????, ?? ?????
   if(culc_p(smb_cnfg_start) + EQUL_ZONE > 1.0)
      return step; // ?????????? ????????? ??? ? ??????
 } //??????? ????

  return 0; // ???? ?????? ??? ? ?? ?????????? ?? ?????? ?????????? ???????
}

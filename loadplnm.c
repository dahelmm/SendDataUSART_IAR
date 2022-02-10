//#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "strpolinom.h"

#define SMB_FOR_SIG 8
#define SD_MAX 32 // ��� load_polinom
/******************************* ��������� load_polinom ***********************************/
typedef struct
{
    unsigned short SW; //service word
    unsigned char DW[SD_MAX][2]; //data word

}PlnmToSend;

/*******************************************************************************************/
unsigned short TransPlnmToMsg(polinom * plnm, PlnmToSend * mko_send)
{

//    printf("%d\n",sizeof(plnm->strings));
//    printf("%d\n",sizeof(plnm->strings[0].head));
//    printf("%d\n",sizeof(plnm->strings[0].p_or_q));
//    printf("%d\n",sizeof(plnm->strings[0].exist_pq));
  unsigned char * byte_for_copy;
  unsigned short no_ks = 0;
  unsigned short no_sd = 0;
  _Bool left_part = FALSE;
//----------------------------- Записываем хедеры всех строк ---------------------------------
  for (unsigned short str_i=0; str_i<plnm->n_str; str_i++)
  {
       byte_for_copy = (unsigned char*)(&(plnm->strings[str_i].head));
       if(str_i%2 == 0)
       {
//           printf("%c",plnm->strings[str_i].head);
          mko_send[no_ks].DW[no_sd][0] = *byte_for_copy;
          left_part = TRUE;
       }
       else
       {
          mko_send[no_ks].DW[no_sd][1] = *byte_for_copy;
          left_part = FALSE;
          no_sd++;
          if(no_sd>SD_MAX-1)
          { no_sd = 0; no_ks++;}
       }
  }
//-------------------------------------------------------------------------------------
  if(no_sd >0 || (no_sd == 0 && left_part))
      {no_ks++; no_sd=0;} // переходим к следующему КС, пропускаем нулевые СД
  left_part = FALSE;
//----------------------------- Пакуем битовые карты... ---------------------------------
  for (unsigned short str_i=0; str_i<plnm->n_str; str_i++)
  {
     for (unsigned short bm_j=0; bm_j<N_BIT_MAP; bm_j++)
     {
       byte_for_copy = (unsigned char*)(&(plnm->strings[str_i].exist_pq[bm_j]));
       if(!left_part)
       {
          mko_send[no_ks].DW[no_sd][0] = *byte_for_copy;
          left_part = TRUE;
       }
       else
       {
          mko_send[no_ks].DW[no_sd][1] = *byte_for_copy;
          left_part = FALSE;
          no_sd++;
          if(no_sd>SD_MAX-1)
          { no_sd = 0; no_ks++;}
       }
     }
  }
//-------------------------------------------------------------------------------------
  if(no_sd >0 || (no_sd == 0 && left_part))
      {no_ks++; no_sd=0;} // переходим к следующему КС, прпускаем нулевые СД
  left_part = FALSE;
//----------------------------- Пакуем символьные карты... ---------------------------------
  //unsigned short dbl_byte_for_copy;
  for (unsigned short str_i=0; str_i<plnm->n_str; str_i++)
  {
     for (unsigned short sm_j=0; sm_j<N_BIT_MAP; sm_j++)
     {
       byte_for_copy = (unsigned char*)(&(plnm->strings[str_i].p_or_q[sm_j]));
       //dbl_byte_for_copy >> 8;

       if(!left_part)
       {
          mko_send[no_ks].DW[no_sd][0] = *byte_for_copy;
          left_part = TRUE;
       }
       else
       {
          mko_send[no_ks].DW[no_sd][1] = *byte_for_copy;
          left_part = FALSE;
          no_sd++;
          if(no_sd>SD_MAX-1)
          { no_sd = 0; no_ks++;}
       }
     }
  }

  if(no_sd >0)
      {no_ks++; no_sd=0;}
//  no_ks++;
  unsigned short hSD = plnm->n_str;
                 hSD = hSD & 0x00ff;
  mko_send[no_ks].DW[0][0] = (unsigned char)hSD;
                 hSD = plnm->n_str;
                 hSD = hSD>>8;
  mko_send[no_ks].DW[0][1] = (unsigned char)hSD;

  mko_send[no_ks].DW[1][0] = plnm->n_el;

  return no_ks;
}

unsigned short LoadPlnmFromMsg (PlnmToSend * mko_send, polinom * plnm, unsigned short l_str)
{
    unsigned char err = 0;

   unsigned short n_ks = 0;
   unsigned short j_sd = 0;

   unsigned char byte = 0;
   struct header_of_string * p_head_byte;

   _Bool left_part = FALSE;
//1. Считываение всех хедеров l_str байт
   for(unsigned short i=0; i<l_str;)
   {
      byte = mko_send[n_ks].DW[j_sd][0]; // read_fr_left(const unsigned short * SD)
      p_head_byte = (struct header_of_string *)(&byte);
      plnm->strings[i].head = *p_head_byte;
      i++;
      left_part = TRUE;
      if(i>=l_str) break;

      byte = mko_send[n_ks].DW[j_sd][1];
      p_head_byte = (struct header_of_string *)(&byte);
      plnm->strings[i].head = *p_head_byte;
      i++;
      left_part = FALSE;
      j_sd++;
      if(j_sd>SD_MAX-1)
         {j_sd = 0; n_ks++; }
   }
// окончание считывания хедеров
   if(j_sd >0 || (j_sd == 0 && left_part))
      {n_ks++; j_sd=0;} // переходим к следующему КС, прпускаем нулевые СД
//2. Считываение всех битовых карт существоания элемента exist
   struct bit_map * p_bit_map_byte;
   left_part = FALSE;

   for(unsigned short i=0; i<l_str; i++)
   {
      for(unsigned short j=0; j<N_BIT_MAP; j++)
      {
         if(!left_part)
         {
                byte = mko_send[n_ks].DW[j_sd][0];
                p_bit_map_byte = (struct bit_map *) (&byte);
                plnm->strings[i].exist_pq[j] = *p_bit_map_byte;
                left_part = TRUE;
         }
         else
         {
                byte = mko_send[n_ks].DW[j_sd][1];
                p_bit_map_byte = (struct bit_map *) (&byte);
                plnm->strings[i].exist_pq[j] = *p_bit_map_byte;
                left_part = FALSE;
                j_sd++;
                if(j_sd>SD_MAX-1)
                {j_sd = 0; n_ks++;}
         }
      }
   } // конец for, окончание считывания битовых карт

   if(j_sd >0 || (j_sd == 0 && left_part))
      {n_ks++; j_sd=0;} // переходим к следующему КС, прпускаем нулевые СД
//3. Считываение всех символьные карт l_str*N_SMB_MAP байт
   //struct bit_map * p_smb_map_byte;
   left_part = FALSE;

   //unsigned short dbl_byte = 0;
   struct smb_map * p_smb_map;

   for(unsigned short i=0; i<l_str;i++)
   {
      for(unsigned short j=0; j<N_BIT_MAP; j++)
      {
          if(!left_part)
          {
                 byte = mko_send[n_ks].DW[j_sd][0];
                 p_bit_map_byte = (struct bit_map *) (&byte);
                 plnm->strings[i].p_or_q[j] = *p_bit_map_byte;
                 left_part = TRUE;
          }
          else
          {
                 byte = mko_send[n_ks].DW[j_sd][1];
                 p_bit_map_byte = (struct bit_map *) (&byte);
                 plnm->strings[i].p_or_q[j] = *p_bit_map_byte;
                 left_part = FALSE;
                 j_sd++;
                 if(j_sd>SD_MAX-1)
                 {j_sd = 0; n_ks++;}
          }
      }
   }

  return n_ks;
}

void LoadPlnmFromFile(char * PlnmFileName, polinom * Polinom)
{
    FILE * FilePolinom;
    FilePolinom=fopen(PlnmFileName,"r");
    if (FilePolinom!=NULL){
        unsigned short CntStr = 0; //number of string in Polinom
        char FailSmb;
        char StrRd[4*N_EL_MAX];
        long i;

        for(i=0;i<N_STR_MAX;i++){
            if(EOF == fscanf(FilePolinom,"%[^\n] %[\n]",StrRd,&FailSmb))
                break;
            CntStr++;
        }

        char Nomer[3]; //index of element, 000 to 999 or _00 to _99
        unsigned char nmb_symb_end = 0; //k текущий pq символ
        unsigned char nmb_symb_beg = 0; //h, предыдущий pq символ
        unsigned char pq_symb_cur = 0; //j_N
        unsigned char str_lng_cur = 0, last_smb =0; // str_lng_cur = a
        _Bool flag_frst = TRUE;  //n_bool
        unsigned short count_smb_in_str = 0;
        unsigned char index_pq = 0; //index_PQ
        unsigned char n_el = 0; //максимальный номер элемента
        long k,j;
        polinom_init((void*)/*&*/Polinom, CntStr);
        rewind(FilePolinom);

        for (unsigned short str_i=0; str_i<CntStr; str_i++){
            nmb_symb_end = 0;
            nmb_symb_beg = 0;
            pq_symb_cur = 0;
            flag_frst = TRUE;
            fscanf(FilePolinom,"%[^\n] %[\n]",StrRd,&FailSmb);
            str_lng_cur = 0;
            for (i=0;i<4*N_EL_MAX;i++) {
               if(StrRd[i] == 0)
                   break;
               str_lng_cur++;
            }
      //проверяем первые 6 символов на знак строки, пусть по умолчанию +
            Polinom->strings[str_i].head.sig = 1;
            for (k=0; k<=SMB_FOR_SIG; k++){
                if (StrRd[k] == '+'){
                    Polinom->strings[str_i].head.sig  = 1;
                    break;
                }

                if (StrRd[k] == '-'){
                    Polinom->strings[str_i].head.sig = -1;
                    break;
                }
            }

            for(j=0; j<str_lng_cur; j++){
                if (StrRd[j] == 'P')
                    pq_symb_cur = j;
                else if (StrRd[j] == 'Q')
                    pq_symb_cur = j;

                if (pq_symb_cur > nmb_symb_beg && flag_frst){
                           nmb_symb_beg = pq_symb_cur;
                           flag_frst = FALSE;
                }

                if (pq_symb_cur > nmb_symb_beg){
                    nmb_symb_end = pq_symb_cur;
                    nmb_symb_beg++;
                    Nomer[0]=0;Nomer[1]=0;Nomer[2]=0;
                    strncpy(Nomer, &StrRd[nmb_symb_beg],nmb_symb_end-nmb_symb_beg-1);
                    index_pq = atoi(Nomer);
                    nmb_symb_beg--;
                    if (StrRd[nmb_symb_beg] == 'P')
                        w_bit (Polinom->strings[str_i].p_or_q, index_pq, P);
                    else if (StrRd[nmb_symb_beg] == 'Q')
                        w_bit (Polinom->strings[str_i].p_or_q, index_pq, Q);
                    w_bit (Polinom->strings[str_i].exist_pq, index_pq, TRUE); // записываем биты существования
                    nmb_symb_beg = nmb_symb_end;
                    count_smb_in_str++;
                    if(index_pq > n_el)
                        n_el =  index_pq;
                }
           }
           nmb_symb_beg++;
           Nomer[0] = '0'; Nomer[1] = 0; Nomer[2] = 0;
           //---------------- ����� �������� � � ������� � ����� ������ � ��� ----
           if(StrRd[str_lng_cur-1]=='+' || StrRd[str_lng_cur-1]=='-')
              last_smb = 1;
           else last_smb = 0;
           //---------------------------------------------------------------------
           strncpy(Nomer, &StrRd[nmb_symb_beg],str_lng_cur-nmb_symb_beg-last_smb);
           index_pq = atoi(Nomer);
           if(index_pq == 0){
               printf("ER: read NULL el of pol in \n");
               printf("LoadPolinomFromFile \n");
           }

           nmb_symb_beg--;
           if (StrRd[nmb_symb_beg] == 'P')
               w_bit (Polinom->strings[str_i].p_or_q, index_pq, P);
           else if (StrRd[nmb_symb_beg] == 'Q')
               w_bit (Polinom->strings[str_i].p_or_q, index_pq, Q);
           w_bit (Polinom->strings[str_i].exist_pq, index_pq, TRUE); // записываем последний бит существования
           count_smb_in_str++;
           Polinom->strings[str_i].head.leight = count_smb_in_str;
           count_smb_in_str=0;
           if(index_pq > n_el)
              n_el =  index_pq;
     }
     Polinom->n_el = n_el;
   }
   fclose(FilePolinom);
}

#define MAX_MODE 10
polinom STRUCTURE;
#define N_BA 8
#define N_MODE 2
float KSI_EL[N_EL_MAX];
struct bit_map MANY_CONF[MAX_CNFG][N_BIT_MAP];

unsigned int FindConfiguration()
{

     struct bit_map    p_of_str[N_BIT_MAP];
     struct bit_map conf_of_str[MAX_MODE][N_BIT_MAP];
     struct smb_map  smb_of_str[N_BIT_MAP];
     struct smb_map smb_for_chc[N_BIT_MAP];

     null_bit_map(   p_of_str);
     for(char j=0;j<MAX_MODE;j++)
        null_bit_map(&conf_of_str[j][0]);
     null_smb_map(smb_of_str);
     null_smb_map(smb_for_chc);


     _Bool tmb_conf_same = TRUE;
     unsigned char num_max_ksi = 0; //������������ ����������
     unsigned int num_rec = 0; //������� ��������� �� �������� ������������

     unsigned char cur = 0;//������� ������������ � ������

     for(unsigned short i=0;i<STRUCTURE.n_str;i++){
        if(STRUCTURE.strings[i].head.sig == 1){

             for(unsigned char j=1;j<=N_BA;j++)
                 if(r_bit(STRUCTURE.strings[i].exist_pq,j) &&
                    r_smb(STRUCTURE.strings[i].p_or_q  ,j) == P){
                      w_bit(p_of_str,j,1);
                      w_smb(smb_of_str,j,NORM);
                 }
                 else{
                      w_bit(p_of_str,j,0);
                      w_smb(smb_of_str,j,DOWN);
                 }
            ///*
             for(unsigned char j=N_BA+1;j<=N_BA+N_MODE;j++){
                 w_smb(smb_of_str,j,UP);
                 if(r_bit(STRUCTURE.strings[i].exist_pq,j) &&
                    r_smb(STRUCTURE.strings[i].p_or_q  ,j) == P)
                      w_bit(p_of_str,j,1);
                 else
                      w_bit(p_of_str,j,0);
             }

             cur=0;

             if(N_MODE>0){
                for(unsigned char j=N_BA+1;j<=N_BA+N_MODE;j++){
                        if(r_bit(p_of_str,j) == 0){
                           w_smb(smb_of_str,j,DOWN);
                           copy(smb_for_chc,smb_of_str);
                           w_smb(smb_of_str,j,UP);
                           for(unsigned int j=1;j<=N_BA;j++){
                                culc_all_ksi(smb_for_chc,KSI_EL,NEGATIV);

                                num_max_ksi = first_max_ksi(KSI_EL, smb_for_chc);

                                if(num_max_ksi>0 && num_max_ksi<=N_BA)
                                        w_smb(smb_for_chc,num_max_ksi,UP);
                                else
                                        printf("Error choice max el in FindConf \n");

                                if(!(culc_p(smb_for_chc)<1.0)){
                                        for(unsigned int x=1;x<=N_BA;x++)
                                                if(r_smb(smb_for_chc,x) == UP)
                                                        w_bit(conf_of_str[cur],x,1);
                                                else
                                                        w_bit(conf_of_str[cur],x,0);

                                        for(unsigned int x=N_BA+1;x<=N_BA+N_MODE;x++)
                                                if(r_smb(smb_for_chc,x)==DOWN)
                                                        w_bit(conf_of_str[cur],x,1);
                                                else
                                                        w_bit(conf_of_str[cur],x,0);
                                        cur++;
                                        break;
                                }
                           }//for (1...N_BA)
                        }//p_of_str[j] == 0

                } //for(N_BA...N_MODE)
             }//N_MODE>0
             else{
                for(unsigned int j=1;j<=N_BA;j++){
                        culc_all_ksi(smb_of_str,KSI_EL,NEGATIV);
                        num_max_ksi = first_max_ksi(KSI_EL, smb_of_str);
                        if(num_max_ksi>0 && num_max_ksi<=N_BA)
                                w_smb(smb_of_str,num_max_ksi,UP);
                        else
                                printf("Error choice max el in FindConf \n");

                        if(!(culc_p(smb_of_str)<1.0)){
                                for(unsigned int x=1;x<=N_BA;x++)
                                        if(r_smb(smb_of_str,x) == UP)
                                                w_bit(conf_of_str[cur],x,1);
                                        else
                                                w_bit(conf_of_str[cur],x,0);
                                break;
                        }
                }//for (1...N_BA)
                cur++;
             }//

             null_bit_map(   p_of_str);
             null_smb_map(smb_of_str);
                //2. ��������� �� ��������

             for(char c=0;c<cur;c++){
                for(unsigned int k=0;k<num_rec;k++){
                        tmb_conf_same = TRUE;
                        for(unsigned int j=1;j<=STRUCTURE.n_el;j++){
                                if(r_bit(conf_of_str[c],j) != r_bit(MANY_CONF[k],j)){
                                        tmb_conf_same = FALSE;
                                        break;
                                }
                        }
                        if(tmb_conf_same)
                                break;
                }

                if(!tmb_conf_same ||  num_rec==0){
                //3. ���������� ������������ ��� ���������
                        copy(MANY_CONF[num_rec],conf_of_str[c]);
                               num_rec++;
                }
                null_bit_map(conf_of_str[c]);

             }//for(c...cur)
        }
     }//for(...i=0;i<...n_str;i++)

     return num_rec;
}

void TestingPlnmFromMsg(polinom * PlnmFromMsg)
{

}

void PrintPolinom(polinom *poli)
{
    for(int i = 0;i<poli->n_str;i++)
    {
        poli->strings[i].head.sig==1?printf("+ "):printf("- ");

        if(poli->strings[i].exist_pq[0].b1==1)
        {
            poli->strings[i].p_or_q[0].b1==1?printf("P1 "):printf("Q1 ");
        }

        if(poli->strings[i].exist_pq[0].b2==1)
        {
            poli->strings[i].p_or_q[0].b2==1?printf("P2 "):printf("Q2 ");
        }

        if(poli->strings[i].exist_pq[0].b3==1)
        {
            poli->strings[i].p_or_q[0].b3==1?printf("P3 "):printf("Q3 ");
        }

        if(poli->strings[i].exist_pq[0].b4==1)
        {
            poli->strings[i].p_or_q[0].b4==1?printf("P4 "):printf("Q4 ");
        }

        if(poli->strings[i].exist_pq[0].b5==1)
        {
            poli->strings[i].p_or_q[0].b5==1?printf("P5 "):printf("Q5 ");
        }

        if(poli->strings[i].exist_pq[0].b6==1)
        {
            poli->strings[i].p_or_q[0].b6==1?printf("P6 "):printf("Q6 ");
        }

        if(poli->strings[i].exist_pq[0].b7==1)
        {
            poli->strings[i].p_or_q[0].b7==1?printf("P7 "):printf("Q7 ");
        }

        if(poli->strings[i].exist_pq[0].b8==1)
        {
            poli->strings[i].p_or_q[0].b8==1?printf("P8 "):printf("Q8 ");
        }
        printf("\n");
    }

}

void printSendPolinom(PlnmToSend *poli,int size)
{
    for(int i = 0; i<size;i++)
    {
        printf("Message %d:\n",i+1);
        for(int j=0;j<SD_MAX;j++)
        {
            printf("Slovo %d:   ",j+1);
            printf("%.2x%.2x",poli[i].DW[j][0],poli[i].DW[j][1]);
            printf("\n");
        }
        printf("\n");
    }
}


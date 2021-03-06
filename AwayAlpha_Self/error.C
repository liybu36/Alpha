#include "TMath.h"
using namespace std;

#define Fermi

void error()
{
  const int NUM=2;

  /*  //Source Rotates away, 0.6g/L
#ifdef Fermi
  double NS[NUM] = {100266,904609};
  double NB[NUM] = {1240,2251};
  double NTotal[NUM] ={8492240,209429};
#else
  double NS[NUM] = {102091,908167};
  double NB[NUM] = {1252,2259};
  double NTotal[NUM] ={8492240,209429};
#endif
  */

  //Source Rotates away, 1.5g/L
#ifdef Fermi
  double NS[NUM] = {41345,388923};
  double NB[NUM] = {540,960};
  double NTotal[NUM] ={3590562,93239};
#else
  double NS[NUM] = {43725,404075};
  double NB[NUM] = {552,973};
  double NTotal[NUM] ={3715048,93239};
#endif

  /*Source close to cryostat, Fermi
  double NS[NUM] = {46051,222403};
  double NB[NUM] = {626,1023};
  double NTotal[NUM] ={2585373,81511};
  */
  /*  
  double NS[NUM] = {40257,217503};
  double NB[NUM] = {1068,1909};
  double NTotal[NUM] ={2594132,175700};
  */
  double NS_sigma[NUM];
  double NB_sigma[NUM];
  double NIMA[NUM];
  double NIMA_sigma[NUM];

  for(int i=0; i<NUM; i++)
    {
      NS_sigma[i] = TMath::Sqrt(NS[i]);
      NB_sigma[i] = TMath::Sqrt(NB[i]);
      NIMA[i] = NS[i]/NTotal[0]-NB[i]/NTotal[1];
      NIMA_sigma[i]=TMath::Sqrt((NS_sigma[i]/NTotal[0])**2 + (NB_sigma[i]/NTotal[1])**2);      
    }
  double BR = NIMA[0]/NIMA[1];
  double Error = BR*TMath::Sqrt((NIMA_sigma[0]/NIMA[0])**2 + (NIMA_sigma[1]/NIMA[1])**2);
  cout<<"BR= "<<BR<<"\t Error"<<Error<<endl;
}

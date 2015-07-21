#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h>
#include "TCut.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TChain.h"
#include "TRint.h"
#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"
#include "TNtuple.h"
#include "TColor.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPad.h"
#include "TStyle.h"
#include "TBox.h"

TRint *theApp;
using namespace std;

//#define Write_Plots
//#define new_data
//#define random_data

#ifdef random_data
string Time = "_sub_random_away_Mar28PM";
//string Time = "_sub_random_self_Mar26PM";
#else
string Time = "_away_Mar28PM";
//string Time = "_self_Mar26PM";
#endif

#ifdef new_data
string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTAwayData/";
//string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTAwayData/randomdata/";

//string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTSelfData/";
//string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTSelfData/randomdata/";
#else
string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/DSTAwayData/";
//string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/DSTAwayData/randomdata/";

//string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/DSTSelfData/";
//string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/DSTSelfData/randomdata/";
#endif

//#define new_mult
bool multiplicity_cut(Float_t height, Float_t multiplicity, Float_t charge){
  /*  double y0=2.563e7;
  double x0=14.40;
  double slope=1.390e12;
  double constant=1.574e14;
  */
  /*  double y0=2.75e7;
  double x0=43.25;
  double slope=0.15e7;
  double constant=0.0625e14;  
  return height/multiplicity < (y0 + TMath::Sqrt(constant+slope*TMath::Power(charge-x0,2)));
  //  return height/multiplicity < (2.563e7 + TMath::Sqrt(1.574e14+1.390e12*(charge-14.40)*(charge-14.40)));

  */

  //y=y0+Sqrt(a*(x-x0)^2+r)

  if(charge>0 && charge<=40)
    {
      double x0=40;
      double s=40.e+6;
      double x1=13;
      double y1=100.e+6;
      double x2=2;
      double y2=200.e+6;
      double r=(TMath::Power(y1-s,2)-(TMath::Power(x1-x0,2))*(TMath::Power(y2-s,2))/(TMath::Power(x2-x0,2)))/(2*(y2-s)*(TMath::Power(x1-x0,2))/(TMath::Power(x2-x0,2))-2*(y1-s));
      double a=((y1-s)*(y2-s)*(y1-y2))/((y2-s)*(TMath::Power(x1-x0,2))-(y1-s)*(TMath::Power(x2-x0,2)));
      double y0=s-r;
    return height/multiplicity<y0+TMath::Sqrt(a*(TMath::Power(charge-x0,2))+TMath::Power(r,2));
    }
  else{
    double x0=40;
    double s=40.e+6;
    double x1=300;
    double y1=75.e+6;
    double x2=600;
    double y2=130.e+6;
    double r=(TMath::Power(y1-s,2)-(TMath::Power(x1-x0,2))*(TMath::Power(y2-s,2))/(TMath::Power(x2-x0,2)))/(2*(y2-s)*(TMath::Power(x1-x0,2))/(TMath::Power(x2-x0,2))-2*(y1-s));
    double a=((y1-s)*(y2-s)*(y1-y2))/((y2-s)*(TMath::Power(x1-x0,2))-(y1-s)*(TMath::Power(x2-x0,2)));
    double y0=s-r;
    return height/multiplicity<y0+TMath::Sqrt(a*(TMath::Power(charge-x0,2))+TMath::Power(r,2));
  }
   
}

//double mult_fun(Double_t *x, Double_t *params){  
  /*  double y0=2.75e7;
  double x0=43.25;
  double slope=0.15e7;
  double constant=0.0625e14;
  return y0 + TMath::Sqrt(constant+slope*TMath::Power(x[0]-x0,2));
  */
/*
  double charge =x[0];
  if(charge>0 && charge<50)
    return charge*height/multiplicity < 12e+8;
  if(charge>200 && charge<500)
    return height/multiplicity <1.3e+6/6*charge+50.e+6/6;
}
*/
void Readdatafile(TChain *t, int i)
{
  string filename;
  string middle="DST_Run0";
  string last=".root";
  stringstream oss;
  oss<<i;
  filename=dirname+middle+oss.str()+last;
  t->Add(filename.c_str());
  if(t->IsZombie())
    cout<<"ERROR: Wrong Data"<<endl;
  cout<<"Processing Data file: "<<filename<<endl; 
  oss.str("");
}

void Readdatafile(TChain *t,int start, int end)
{
  string filename;
  string middle="DST_Run0";
  string last=".root";
  stringstream oss;
  ifstream NameCheck;
  for(int i=start; i<=end; i++)
    {
      oss.str("");
      oss<<i;
      filename=dirname+middle+oss.str()+last;
      NameCheck.open(filename.c_str());
      if(!NameCheck.good())
	{
	  oss.str("");
	  NameCheck.close();
	  continue;
	}
      else{
	/*	TFile *f1 = new TFile(filename.c_str());
	if(f1->IsZombie())
	  continue;
	else{
	*/
	  t->Add(filename.c_str());
	  cout<<"Processing Data file: "<<filename<<endl; 
	  //	}
	oss.str("");
      }
      NameCheck.close();
    }
}

bool SortFunction(double i, double j)
{
  return (TMath::Abs(i) < TMath::Abs(j));
}

vector<int> Colors()
{
  vector<int> color;
  color.push_back(TColor::GetColor("#FB78FF"));
  color.push_back(TColor::GetColor("#B24F18"));
  color.push_back(TColor::GetColor("#2FDAFF"));
  color.push_back(TColor::GetColor("#5A1DE8"));
  color.push_back(TColor::GetColor("#B26618"));
  color.push_back(TColor::GetColor("#AE3EFF"));
  color.push_back(TColor::GetColor("#FF0000"));
  color.push_back(TColor::GetColor("#5A1DE8"));
  color.push_back(TColor::GetColor("#1CFFDF"));
  color.push_back(TColor::GetColor("#1D537F"));
  color.push_back(TColor::GetColor("#000000"));
  color.push_back(TColor::GetColor("#4A067F"));
  color.push_back(TColor::GetColor("#FF379F"));
  color.push_back(TColor::GetColor("#25DCFF"));
  color.push_back(TColor::GetColor("#92B20B"));
  color.push_back(TColor::GetColor("#59FF49"));
  color.push_back(TColor::GetColor("#E8A60C"));
  color.push_back(TColor::GetColor("#79FFFF"));
  color.push_back(TColor::GetColor("#11FF8F"));
  color.push_back(TColor::GetColor("#09B23E"));
  color.push_back(TColor::GetColor("#62B21D"));

  return color;
}

//=============================Main Funtion===================================
int main(int argc, char **argv){
  theApp = new TRint("theApp",&argc,argv,NULL,0);
  TChain *DSTtree = new TChain("DSTtree");
  int startfile, endfile;
  TString series;
  if(theApp->Argc() == 2)
    {
      startfile = atoi(theApp->Argv(1));
      endfile = startfile;
      series = theApp->Argv(1);
      Readdatafile(DSTtree, startfile);
    }
  else if(theApp->Argc() == 3)
    {
      startfile = atoi(theApp->Argv(1));
      endfile = atoi(theApp->Argv(2));
      series = theApp->Argv(1);
      series += theApp->Argv(2);
      Readdatafile(DSTtree, startfile, endfile);
    }
  else{
    cout<<"Usage: ./DSTAway startfile endfile "<<endl;
    cout<<"Usage: ./DSTAway startfile "<<endl;
    return 0;
  }
  cout<<"startfile "<<startfile<<"\t endfile "<<endfile<<"  series "<<series<<endl;
  
  /*  Int_t    runID               = -1;
  Int_t    tpc_eventID         = -1;
  Int_t    tpc_event_type      = -1; // 0 gamma, 1 neutron, -1 else                                                             
  Double_t tpc_gps_fine        = -1.; //clock cycles                                                                            
  Double_t tpc_gps_coarse      = -1.; //[s]                                                                                     
  Double_t tpc_s1_start_time   = -1.; //[us]                                                                                    
  Double_t tpc_total_s1        = -1.; //[PE]                                                                                    
  Double_t tpc_total_f90       = -1.;
  Double_t tpc_t_drift         = -1.; //[us]                                                                                    
  Double_t tpc_s1_late         = -1.; //[PE]                                                                                    
  Int_t    tpc_npulses         = -1;
  Double_t tpc_timestamp       = -1;  //[us]    
  
  DSTtree->SetBranchAddress("runID",             &runID);
  DSTtree->SetBranchAddress("tpc_eventID",       &tpc_eventID);
  DSTtree->SetBranchAddress("tpc_event_type",    &tpc_event_type);
  DSTtree->SetBranchAddress("tpc_gps_fine",      &tpc_gps_fine);
  DSTtree->SetBranchAddress("tpc_gps_coarse",    &tpc_gps_coarse);
  DSTtree->SetBranchAddress("tpc_s1_start_time", &tpc_s1_start_time);
  DSTtree->SetBranchAddress("tpc_total_s1",      &tpc_total_s1);
  DSTtree->SetBranchAddress("tpc_total_f90",     &tpc_total_f90);
  DSTtree->SetBranchAddress("tpc_t_drift",       &tpc_t_drift);
  DSTtree->SetBranchAddress("tpc_s1_late",       &tpc_s1_late);
  DSTtree->SetBranchAddress("tpc_npulses",       &tpc_npulses);
  DSTtree->SetBranchAddress("tpc_timestamp",     &tpc_timestamp);
  */

  Int_t od_eventID       = -1;
  Int_t od_nclusters     = -1;
  Double_t od_gps_fine   = -1.;
  Double_t od_gps_coarse = -1.;
  Double_t od_timestamp  = -1.;
  Double_t od_wt_charge  = -1.;
  /*  std::vector<Double_t> od_cluster_charge;
  std::vector<Double_t> od_cluster_start;
  std::vector<Double_t> od_cluster_height;
  std::vector<Double_t> od_cluster_multiplicity;
  std::vector<Int_t> od_cluster_pass_multcut;
  std::vector<Double_t> od_cluster_dtprompt;
  */
  vector<double>  *od_cluster_charge;
  vector<double>  *od_cluster_start;
  vector<double>  *od_cluster_height;
  vector<double>  *od_cluster_multiplicity;
  vector<int>     *od_cluster_pass_multcut;
  vector<double>  *od_cluster_dtprompt;

  od_cluster_charge = 0;
  od_cluster_start = 0;
  od_cluster_height = 0;
  od_cluster_multiplicity = 0;
  od_cluster_pass_multcut = 0;
  od_cluster_dtprompt = 0;

  DSTtree->SetBranchAddress("od_eventID",&od_eventID);
  DSTtree->SetBranchAddress("od_gps_fine",&od_gps_fine);
  DSTtree->SetBranchAddress("od_gps_coarse",&od_gps_coarse);
  DSTtree->SetBranchAddress("od_timestamp", &od_timestamp);
  DSTtree->SetBranchAddress("od_nclusters", &od_nclusters);
  DSTtree->SetBranchAddress("od_wt_charge", &od_wt_charge);
  DSTtree->SetBranchAddress("od_cluster_charge", &od_cluster_charge);
  DSTtree->SetBranchAddress("od_cluster_start", &od_cluster_start); // time in ns                                                  
  DSTtree->SetBranchAddress("od_cluster_height", &od_cluster_height);
  DSTtree->SetBranchAddress("od_cluster_multiplicity", &od_cluster_multiplicity);
  DSTtree->SetBranchAddress("od_cluster_pass_multcut", &od_cluster_pass_multcut);
  DSTtree->SetBranchAddress("od_cluster_dtprompt", &od_cluster_dtprompt);

  int Entries = DSTtree->GetEntries();
  cout<<"Entries= "<<Entries<<endl;

  //===================Create New Histograms========================  
  //  double prompt_time = 2.e+4; 
  double prompt_time = 10.e+3; 
  double middle_time = 0;
  //  double delay_time = 6.e+4; 
  double delay_time =5.e+4;
  double first_time = 2.e+4;
  double after_time = 2.e+4;
  double late_time = 1.e+5;
  double end_time = 1.3e+5;
  /*  double alpha_time = 1.e+3;
  double alpha_delay_time = 2.e+4; 
  */
  double window1start = 3.e+4;
  double window1end = 6.e+4;   //7.e+4;
  double window2start = 4.e+4;
  double window2end = 7.e+4;   //8.e+4;  
  
  int gamma_bins = 1000;
  int gamma_gps_bins = 5000;
  double gamma_coin_ene = 5.e+3;
  int neutron_bins = 1000;
  int neutron_gps_bins = 5000;
  double neutron_coin_ene = 5.e+3;  


  double pos1 = 200.;
  double pos2 = 350.;
  double pos4 = 50.;
#ifdef new_data
  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTAwayResults/";
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTAwayResults/Plots/";
  double pos3 = 10.;

  /*
  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTSelfResults/";
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTSelfResults/Plots/";
  double pos3 = 5.;
  pos1=150;
  pos2=330;
  */
#else
  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/";
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/Plots/";
  double pos3 = 0.;

  /*  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/DSTSelfResults/";
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/DSTSelfResults/Plots/";
  double pos3 = 5.;
  pos1=150;
  pos2=330;
  */
#endif
  string ext = "png";

  /*
  TH1F* tpc_total_s1_hist = new TH1F("tpc_total_s1_hist","tpc total s1;Energy [PE]",200,0,40000);
  TH1F* tpc_s1_late_hist = new TH1F("tpc_s1_late_hist","tpc s1 late;Energy [PE]",200,0,25000);
  TH1F* tpc_s1_starttime_hist = new TH1F("tpc_s1_starttime_hist", "s1 start Time", 5000, -10., 10.);
  TH1F* tpc_total_f90_hist = new TH1F("tpc_total_f90_hist", "F90 Distribution", 110, 0, 1.3);
  TH2F* tpc_total_s1_f90_hist = new TH2F("tpc_total_s1_f90_hist", "F90 vs S1; S1 [PE]; F90", 10000, 0, 10000, 130, 0, 1.3);

  TH1F* tpc_gamma_total_s1_hist = new TH1F("tpc_gamma_total_s1_hist","tpc gamma total s1;Energy [PE]",200,0,40000);
  TH1F* tpc_gamma_s1_late_hist = new TH1F("tpc_gamma_s1_late_hist","tpc gamma s1 late;Energy [PE]",200,0,25000);

  TH1F* tpc_neutron_total_s1_hist = new TH1F("tpc_neutron_total_s1_hist","tpc neutron total s1;Energy [PE]",200,0,1000);
  TH1F* tpc_neutron_s1_late_hist = new TH1F("tpc_neutron_s1_late_hist","tpc neutron s1 late;Energy [PE]",100,0,500);

  TH1F* tpc_gamma_gps_hist = new TH1F("tpc_gamma_gps_hist","TPC Gamma Time;Time [ns]",1000,0,1.e+4); 
  TH1F* tpc_gamma_ene_hist = new TH1F("tpc_gamma_ene_hist","TPC Gamma Energy;Energy [PE]",1000,0,1.e+4);
  TH2F* tpc_gamma_gps_ene_hist = new TH2F("tpc_gamma_gps_ene_hist","TPC Gamma Time Vs Energy;Time [ns];Energy [PE]",1000,0,1.e+4,1000,0,1.e+4);
  TNtuple* tpc_gamma_ntuple = new TNtuple("tpc_gamma_ntuple","TPC Gamma Data","time:ene");
  */

  /////===============All OD clusters================================================
  double fail_ene = 500; //pe
  int fail_bins = 500;  //125;
  int gamma_first_bins =300;   // 200;

  //  TH2F* nv_all_ene_s1_hist = new TH2F("nv_all_ene_s1_hist","All Clusters Veto Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",500,0,1.e+4,200,0,40000);  
  TH1F* nv_all_gps_hist = new TH1F("nv_all_gps_hist","All Clusters Veto Time;Time [ns]",1000,middle_time,end_time);
  TH1F* nv_all_ene_hist = new TH1F("nv_all_ene_hist","All Clusters Veto Energy;Energy [PE]",500,0,1.e+4);
  TH1F* nv_all_small_ene_hist = new TH1F("nv_all_small_ene_hist","All Clusters Veto Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_all_gps_ene_hist = new TH2F("nv_all_gps_ene_hist","All ClustersVeto Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,end_time,500,0,1.e+4);
  //  TNtuple* nv_all_ntuple = new TNtuple("nv_all_ntuple","All Clusters Veto Data","total_s1:time:ene");
  TNtuple* nv_all_ntuple = new TNtuple("nv_all_ntuple","All Clusters Veto Data","time:ene:height:mult");

  TH2F* nv_all_charge_height_multiplicity_hist = new TH2F("nv_all_charge_height_multiplicity_hist","Veto: height/multiplicity Vs Charge;od_cluster_charge[PE];height/multicut",1000,0,1000,10000,0,2.e+8);
  TH2F* nv_all_mult_height_multiplicity_hist = new TH2F("nv_all_mult_height_multiplicity_hist","Veto: height/multiplicity Vs Multiplicity;multiplicity;height/multicut",100,0,100,1000,0,4.e+8);

  TH1F* nv_all_first_gps_hist = new TH1F("nv_all_first_gps_hist","Veto All First Time;Time [ns]",gamma_first_bins,first_time,delay_time);
  TH1F* nv_all_first_ene_hist = new TH1F("nv_all_first_ene_hist","Veto All First Energy;Energy [PE]",gamma_first_bins,0,gamma_coin_ene);
  TH1F* nv_all_first_small_ene_hist = new TH1F("nv_all_first_small_ene_hist","Veto First Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_all_first_gps_ene_hist = new TH2F("nv_all_first_gps_ene_hist","Veto All First Time Vs Energy;Time [ns];Energy[PE]",gamma_first_bins,first_time,delay_time,gamma_first_bins,0,gamma_coin_ene);
  //  TNtuple* nv_all_first_ntuple = new TNtuple("nv_all_first_ntuple","Veto All First Data","total_s1:time:ene");
  TNtuple* nv_all_first_ntuple = new TNtuple("nv_all_first_ntuple","Veto All First Data","time:ene:height:mult");
  
  /////===============Failing The Multiplicity cut events================================================
  //  TH2F* nv_fail_ene_s1_hist = new TH2F("nv_fail_ene_s1_hist","Fail Multicut Veto Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",500,0,fail_ene,200,0,40000);  
  TH1F* nv_fail_gps_hist = new TH1F("nv_fail_gps_hist","Fail Multicut Veto Time;Time [ns]",1000,middle_time,end_time);
  TH1F* nv_fail_ene_hist = new TH1F("nv_fail_ene_hist","Fail Multicut Veto Energy;Energy [PE]",500,0,fail_ene);
  TH1F* nv_fail_small_ene_hist = new TH1F("nv_fail_small_ene_hist","Fail Multicut Veto Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_fail_gps_ene_hist = new TH2F("nv_fail_gps_ene_hist","Fail MulticutVeto Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,end_time,500,0,fail_ene);
  //  TNtuple* nv_fail_ntuple = new TNtuple("nv_fail_ntuple","Fail Multicut Veto Data","total_s1:time:ene");
  TNtuple* nv_fail_ntuple = new TNtuple("nv_fail_ntuple","Fail Multicut Veto Data","time:ene:height:mult");
  
  /////===============Passing The Multiplicity cut events================================================

  //  TH2F* nv_ene_s1_hist = new TH2F("nv_ene_s1_hist","Veto Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",500,0,1.e+4,200,0,40000);  
  TH1F* nv_gps_hist = new TH1F("nv_gps_hist","Veto Time;Time [ns]",1000,middle_time,end_time);
  TH1F* nv_ene_hist = new TH1F("nv_ene_hist","Veto Energy;Energy [PE]",500,0,1.e+4);
  TH1F* nv_small_ene_hist = new TH1F("nv_small_ene_hist","Veto Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gps_ene_hist = new TH2F("nv_gps_ene_hist","Veto Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,end_time,500,0,1.e+4);
  //  TNtuple* nv_ntuple = new TNtuple("nv_ntuple","Veto Data","total_s1:time:ene");
  TNtuple* nv_ntuple = new TNtuple("nv_ntuple","Veto Data","time:ene:height:mult");

  /* TH2F* nv_late_ene_s1_hist = new TH2F("nv_late_ene_s1_hist","Veto Late Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",500,0,1.e+4,200,0,40000);  
  TH1F* nv_late_gps_hist = new TH1F("nv_late_gps_hist","Veto Late Time;Time [ns]",gamma_bins,late_time,end_time);
  TH1F* nv_late_ene_hist = new TH1F("nv_late_ene_hist","Veto Late Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_late_small_ene_hist = new TH1F("nv_late_small_ene_hist","Veto Late Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_late_gps_ene_hist = new TH2F("nv_late_gps_ene_hist","Veto Late Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,late_time,end_time,gamma_bins,0,gamma_coin_ene);
  //  TNtuple* nv_late_ntuple = new TNtuple("nv_late_ntuple","Veto Late Data","total_s1:time:ene");
  TNtuple* nv_late_ntuple = new TNtuple("nv_late_ntuple","Veto Late Data","time:ene");
  */
  TH2F* nv_charge_height_multiplicity_hist = new TH2F("nv_charge_height_multiplicity_hist","Veto: height/multiplicity Vs Charge;od_cluster_charge[PE];height/multicut",1000,0,1000,10000,0,2.e+8);
  TH2F* nv_mult_height_multiplicity_hist = new TH2F("nv_mult_height_multiplicity_hist","Veto: height/multiplicity Vs Multiplicity;multiplicity;height/multicut",100,0,100,1000,0,4.e+8);
  //  TF1 *mult_hist = new TF1("mult_hist",mult_fun,0,1000,0);
  //===============Define Gamma Coincidence Plots in Veto==============================================
 
  TH1F* nv_gamma_full_ene_hist = new TH1F("nv_gamma_full_ene_hist","Veto Gamma Full Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_full_small_ene_hist = new TH1F("nv_gamma_full_small_ene_hist","Veto Gamma Full Energy(small range);Energy [PE]",fail_bins,0,fail_ene);

  TNtuple* nv_gamma_0_ntuple = new TNtuple("nv_gamma_0_ntuple","Veto Gamma 0 Data","time:ene:height:mult");  
  TNtuple* nv_gamma_1_ntuple = new TNtuple("nv_gamma_1_ntuple","Veto Gamma 1 Data","time:ene:height:mult");  
  TNtuple* nv_gamma_2_ntuple = new TNtuple("nv_gamma_2_ntuple","Veto Gamma 2 Data","time:ene:height:mult");  
  TNtuple* nv_gamma_3_ntuple = new TNtuple("nv_gamma_3_ntuple","Veto Gamma 3 Data","time:ene:height:mult");  

  //  TH2F* nv_gamma_ene_s1_hist = new TH2F("nv_gamma_ene_s1_hist","Veto Gamma Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  

  // TH2F* nv_gamma_coincidence_ene_s1_hist = new TH2F("nv_gamma_coincidence_ene_s1_hist","Veto Gamma Coincidence Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_coincidence_gps_hist = new TH1F("nv_gamma_coincidence_gps_hist","Veto Gamma Coincidence Time;Time [ns]",gamma_bins,middle_time,end_time);
  TH1F* nv_gamma_coincidence_ene_hist = new TH1F("nv_gamma_coincidence_ene_hist","Veto Gamma Coincidence Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_coincidence_small_ene_hist = new TH1F("nv_gamma_coincidence_small_ene_hist","Veto Gamma Coincidence Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_coincidence_gps_ene_hist = new TH2F("nv_gamma_coincidence_gps_ene_hist","Veto Gamma Coincidence Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,middle_time,end_time,gamma_bins,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_coincidence_ntuple = new TNtuple("nv_gamma_coincidence_ntuple","Veto Gamma Coincidence Data","total_s1:time:ene");  
  TNtuple* nv_gamma_coincidence_ntuple = new TNtuple("nv_gamma_coincidence_ntuple","Veto Gamma Coincidence Data","time:ene:height:mult");  

  //  TH2F* nv_gamma_prompt_ene_s1_hist = new TH2F("nv_gamma_prompt_ene_s1_hist","Veto Gamma Prompt Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_prompt_gps_hist = new TH1F("nv_gamma_prompt_gps_hist","Veto Gamma Prompt Time;Time [ns]",100,middle_time,prompt_time);
  TH1F* nv_gamma_prompt_ene_hist = new TH1F("nv_gamma_prompt_ene_hist","Veto Gamma Prompt Energy;Energy [PE]",100,0,gamma_coin_ene);
  TH1F* nv_gamma_prompt_small_ene_hist = new TH1F("nv_gamma_prompt_small_ene_hist","Veto Gamma Prompt Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_prompt_gps_ene_hist = new TH2F("nv_gamma_prompt_gps_ene_hist","Veto Gamma Prompt Time Vs Energy;Time [ns];Energy[PE]",100,middle_time,prompt_time,100,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_prompt_ntuple = new TNtuple("nv_gamma_prompt_ntuple","Veto Gamma Prompt Data","total_s1:time:ene");
  TNtuple* nv_gamma_prompt_ntuple = new TNtuple("nv_gamma_prompt_ntuple","Veto Gamma Prompt Data","time:ene:height:mult:g");

  TH1F* nv_gamma_second_gps_hist = new TH1F("nv_gamma_second_gps_hist","Veto Gamma Second Time;Time [ns]",100,prompt_time,first_time);
  TH1F* nv_gamma_second_ene_hist = new TH1F("nv_gamma_second_ene_hist","Veto Gamma Second Energy;Energy [PE]",100,0,gamma_coin_ene);
  TH1F* nv_gamma_second_small_ene_hist = new TH1F("nv_gamma_second_small_ene_hist","Veto Gamma Second Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_second_gps_ene_hist = new TH2F("nv_gamma_second_gps_ene_hist","Veto Gamma Second Time Vs Energy;Time [ns];Energy[PE]",100,prompt_time,first_time,100,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_second_ntuple = new TNtuple("nv_gamma_second_ntuple","Veto Gamma Second Data","total_s1:time:ene");
  TNtuple* nv_gamma_second_ntuple = new TNtuple("nv_gamma_second_ntuple","Veto Gamma Second Data","time:ene:height:mult");
  
  //  TH2F* nv_gamma_delay_ene_s1_hist = new TH2F("nv_gamma_delay_ene_s1_hist","Veto Gamma Delay Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_delay_gps_hist = new TH1F("nv_gamma_delay_gps_hist","Veto Gamma Delay Time;Time [ns]",gamma_bins,delay_time,late_time);
  TH1F* nv_gamma_delay_ene_hist = new TH1F("nv_gamma_delay_ene_hist","Veto Gamma Delay Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_delay_small_ene_hist = new TH1F("nv_gamma_delay_small_ene_hist","Veto Gamma Delay Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_gamma_delay_gps_ene_hist = new TH2F("nv_gamma_delay_gps_ene_hist","Veto Gamma Delay Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,delay_time,late_time,gamma_bins,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_delay_ntuple = new TNtuple("nv_gamma_delay_ntuple","Veto Gamma Delay Data","total_s1:time:ene");
  TNtuple* nv_gamma_delay_ntuple = new TNtuple("nv_gamma_delay_ntuple","Veto Gamma Delay Data","time:ene:height:mult");

  //  TH2F* nv_gamma_after_ene_s1_hist = new TH2F("nv_gamma_after_ene_s1_hist","Veto Gamma After Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_after_gps_hist = new TH1F("nv_gamma_after_gps_hist","Veto Gamma After Time;Time [ns]",gamma_bins,after_time,delay_time);
  TH1F* nv_gamma_after_ene_hist = new TH1F("nv_gamma_after_ene_hist","Veto Gamma After Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_after_small_ene_hist = new TH1F("nv_gamma_after_small_ene_hist","Veto Gamma After Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_gamma_after_gps_ene_hist = new TH2F("nv_gamma_after_gps_ene_hist","Veto Gamma After Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,after_time,delay_time,gamma_bins,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_after_ntuple = new TNtuple("nv_gamma_after_ntuple","Veto Gamma After Data","total_s1:time:ene");
  TNtuple* nv_gamma_after_ntuple = new TNtuple("nv_gamma_after_ntuple","Veto Gamma After Data","time:ene:height:mult");

  //  TH2F* nv_gamma_first_ene_s1_hist = new TH2F("nv_gamma_first_ene_s1_hist","Veto Gamma First Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_first_gps_hist = new TH1F("nv_gamma_first_gps_hist","Veto Gamma First Time;Time [ns]",gamma_first_bins,first_time,delay_time);
  TH1F* nv_gamma_first_ene_hist = new TH1F("nv_gamma_first_ene_hist","Veto Gamma First Energy;Energy [PE]",gamma_first_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_first_small_ene_hist = new TH1F("nv_gamma_first_small_ene_hist","Veto First Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_first_gps_ene_hist = new TH2F("nv_gamma_first_gps_ene_hist","Veto Gamma First Time Vs Energy;Time [ns];Energy[PE]",gamma_first_bins,first_time,delay_time,gamma_first_bins,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_first_ntuple = new TNtuple("nv_gamma_first_ntuple","Veto Gamma First Data","total_s1:time:ene");
  TH1F* nv_gamma_first_alpha_gps_hist = new TH1F("nv_gamma_first_alpha_gps_hist","Veto Gamma First Alpha Only Time;Time [ns]",500,first_time,window2end);//delay_time);
  TH1F* nv_gamma_first_r_gps_hist = new TH1F("nv_gamma_first_r_gps_hist","Veto Gamma First Alpha+gamma Time;Time [ns]",500,first_time,window2end);//delay_time);  //300
  TH1F* nv_gamma_first_delta_gps_hist = new TH1F("nv_gamma_first_delta_gps_hist","Veto Gamma First delta Time;Time [ns]",500,first_time,window2end);//delay_time);  
  TNtuple* nv_gamma_first_delta_ntuple = new TNtuple("nv_gamma_first_delta_ntuple","Veto Gamma First Delta Data","time");
  TNtuple* nv_gamma_first_ntuple = new TNtuple("nv_gamma_first_ntuple","Veto Gamma First Data","time:ene:height:mult");

  TH1F* nv_gamma_window1_first_gps_hist = new TH1F("nv_gamma_window1_first_gps_hist","Veto Gamma Window1 First Time;Time [ns]",gamma_first_bins,window1start,window1end);
  TH1F* nv_gamma_window1_first_ene_hist = new TH1F("nv_gamma_window1_first_ene_hist","Veto Gamma Window1 First Energy;Energy [PE]",gamma_first_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_window1_first_small_ene_hist = new TH1F("nv_gamma_window1_first_small_ene_hist","Veto Window1 First Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_window1_first_gps_ene_hist = new TH2F("nv_gamma_window1_first_gps_ene_hist","Veto Gamma Window1 First Time Vs Energy;Time [ns];Energy[PE]",gamma_first_bins,window1start,window1end,gamma_first_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_window1_first_alpha_gps_hist = new TH1F("nv_gamma_window1_first_alpha_gps_hist","Veto Gamma window1 First Alpha Only Time;Time [ns]",gamma_first_bins,window1start,window1end);
  TH1F* nv_gamma_window1_first_r_gps_hist = new TH1F("nv_gamma_window1_first_r_gps_hist","Veto Gamma window1 First Alpha+gamma Time;Time [ns]",gamma_first_bins,window1start,window1end);  
  //  TNtuple* nv_gamma_window_first_ntuple = new TNtuple("nv_gamma_window_first_ntuple","Veto Gamma Window_First Data","total_s1:time:ene");
  TNtuple* nv_gamma_window1_first_ntuple = new TNtuple("nv_gamma_window1_first_ntuple","Veto Gamma Window1 First Data","time:ene:height:mult");

  TH1F* nv_gamma_window2_first_gps_hist = new TH1F("nv_gamma_window2_first_gps_hist","Veto Gamma Window2 First Time;Time [ns]",gamma_first_bins,window2start,window2end);
  TH1F* nv_gamma_window2_first_ene_hist = new TH1F("nv_gamma_window2_first_ene_hist","Veto Gamma Window2 First Energy;Energy [PE]",gamma_first_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_window2_first_small_ene_hist = new TH1F("nv_gamma_window2_first_small_ene_hist","Veto Window2 First Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_window2_first_gps_ene_hist = new TH2F("nv_gamma_window2_first_gps_ene_hist","Veto Gamma Window2 First Time Vs Energy;Time [ns];Energy[PE]",gamma_first_bins,window2start,window2end,gamma_first_bins,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_window_first_ntuple = new TNtuple("nv_gamma_window_first_ntuple","Veto Gamma Window_First Data","total_s1:time:ene");
  TH1F* nv_gamma_window2_first_alpha_gps_hist = new TH1F("nv_gamma_window2_first_alpha_gps_hist","Veto Gamma window2 First Alpha Only Time;Time [ns]",gamma_first_bins,window2start,window2end);
  TH1F* nv_gamma_window2_first_r_gps_hist = new TH1F("nv_gamma_window2_first_r_gps_hist","Veto Gamma window2First Alpha+gamma Time;Time [ns]",gamma_first_bins,window2start,window2end);  
  TNtuple* nv_gamma_window2_first_ntuple = new TNtuple("nv_gamma_window2_first_ntuple","Veto Gamma Window2 First Data","time:ene:height:mult");

  //  TH2F* nv_gamma_late_ene_s1_hist = new TH2F("nv_gamma_late_ene_s1_hist","Veto Gamma Late Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_late_gps_hist = new TH1F("nv_gamma_late_gps_hist","Veto Gamma Late Time;Time [ns]",gamma_bins,late_time,end_time);
  TH1F* nv_gamma_late_ene_hist = new TH1F("nv_gamma_late_ene_hist","Veto Gamma Late Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_late_small_ene_hist = new TH1F("nv_gamma_late_small_ene_hist","Veto Gamma Late Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_gamma_late_gps_ene_hist = new TH2F("nv_gamma_late_gps_ene_hist","Veto Gamma Late Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,late_time,end_time,gamma_bins,0,gamma_coin_ene);
  //  TNtuple* nv_gamma_late_ntuple = new TNtuple("nv_gamma_late_ntuple","Veto Gamma Late Data","total_s1:time:ene");
  TNtuple* nv_gamma_late_ntuple = new TNtuple("nv_gamma_late_ntuple","Veto Gamma Late Data","time:ene:height:mult");

  int pos1bin = nv_gamma_first_small_ene_hist->FindBin(pos1);
  int pos2bin = nv_gamma_first_small_ene_hist->FindBin(pos2);
  int pos3bin = nv_gamma_first_small_ene_hist->FindBin(pos3);
  int pos4bin = nv_gamma_first_small_ene_hist->FindBin(pos4);
  cout<<"pos1="<<pos1<<"\t pos1bin"<<pos1bin<<endl;
  cout<<"pos2="<<pos2<<"\t pos2bin"<<pos2bin<<endl;
  cout<<"pos3="<<pos3<<"\t pos3bin"<<pos3bin<<endl;
  cout<<"pos4="<<pos4<<"\t pos4bin"<<pos4bin<<endl;

  ////**************end of Histograms definitions*******************************************

  double gamma_s1_cut = 20000; //pe
  double gamma_gps_cut = 20000; //ns
  double gamma_od_charge_cut = 500.;
  double neutron_gps_cut = 20000; //ns
  double neutron_od_charge_cut = 500;
  double gamma_ene_cut = 500.;
  int tpc_neutron = 0;
  int lsv_neutron = 0;
  double afterpulse_cut=50.;
  int alpha_counts=0;
  int neutron_counts=0;
  //=================Start Analysis Coincidence Events===========================
  for(int i=0; i<Entries; i++)
    {
      DSTtree->GetEntry(i);
      /*      tpc_total_s1_hist->Fill(tpc_total_s1);
      tpc_s1_late_hist->Fill(tpc_s1_late);
      tpc_s1_starttime_hist->Fill(tpc_s1_start_time);
      tpc_total_f90_hist->Fill(tpc_total_f90);
      tpc_total_s1_f90_hist->Fill(tpc_total_s1,tpc_total_f90);
      if(tpc_event_type==0)  //select gamma
	{
	  tpc_gamma_total_s1_hist->Fill(tpc_total_s1);
	  tpc_gamma_s1_late_hist->Fill(tpc_s1_late);
	}
      else if(tpc_event_type==1)  //select neutron
	{
	  tpc_neutron_total_s1_hist->Fill(tpc_total_s1);
	  tpc_neutron_s1_late_hist->Fill(tpc_s1_late);
	  tpc_neutron++;
	}
      double gps_diff = (od_timestamp - tpc_timestamp)*1.e+3; //ns
      */
      //      vector<double> lsv_gamma_ene,lsv_gamma_s1;
      //      vector<double> lsv_neutron_gps, lsv_gamma_gps;
      //      vector<double> lsv_gamma_alpha_ene, lsv_gamma_alpha_gps, lsv_gamma_alpha_s1;
      //      vector<double> lsv_neutron_alpha_ene, lsv_neutron_alpha_gps, lsv_neutron_alpha_s1; 
      //      bool lsv_gamma_tag = false;
      //      bool lsv_neutron_tag = false;
      //      int lsv_neutron_capture = false;
      //**************************Start Loop Over the OD clusters**********************************************
      //      for(int ncluster=0; ncluster<od_nclusters; ncluster++)
      int g=0;
      int gp=0;
      int g1=0;
      int g2=0;
      int glate=0;
      vector<double> prompt_gps;
      double first_gps=0.;
      bool bigpulse_tag = false;
      bool afterpulse_tag=false;
      bool afterpulse_window1_tag=false;
      bool afterpulse_window2_tag=false;
      for(int ncluster=0; ncluster<od_cluster_charge->size(); ncluster++)
	{ 
	  if(od_cluster_charge->at(ncluster)>0 && od_wt_charge<500) 
	    {
	  //*********************All OD cluster******************************************************************
	      double gps_all_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;  //ns	  
	      nv_all_gps_hist->Fill(gps_all_diff_temp);
	      nv_all_ene_hist->Fill(od_cluster_charge->at(ncluster));
	      nv_all_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
	      nv_all_gps_ene_hist->Fill(gps_all_diff_temp,od_cluster_charge->at(ncluster));
	      nv_all_ntuple->Fill(gps_all_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
	      //      nv_all_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
	      nv_all_charge_height_multiplicity_hist->Fill(od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster)/od_cluster_multiplicity->at(ncluster));
	      nv_all_mult_height_multiplicity_hist->Fill(od_cluster_multiplicity->at(ncluster),od_cluster_height->at(ncluster)/od_cluster_multiplicity->at(ncluster));

	      if(gps_all_diff_temp>first_time && gps_all_diff_temp<delay_time)
		{ //++g;
		  if(ncluster==1)
		    {
		      nv_all_first_gps_hist->Fill(gps_all_diff_temp);
		      nv_all_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_all_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_all_first_gps_ene_hist->Fill(gps_all_diff_temp,od_cluster_charge->at(ncluster));
		      nv_all_first_ntuple->Fill(gps_all_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	     
		    }
		}
	      
	 //*********************Failing MultiCut cluster******************************************************************
	      if(od_cluster_pass_multcut->at(ncluster)==0)
		{
		  double gps_fail_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;  //ns	  
		  nv_fail_gps_hist->Fill(gps_fail_diff_temp);
		  nv_fail_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_fail_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_fail_gps_ene_hist->Fill(gps_fail_diff_temp,od_cluster_charge->at(ncluster));
		  nv_fail_ntuple->Fill(gps_fail_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
		}//end of failing multi cut 
		  
	  //*********************Passing MultiCut cluster******************************************************************
#ifdef new_mult
	      if(multiplicity_cut(od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster),od_cluster_charge->at(ncluster)))
#else	      
		if(od_cluster_pass_multcut->at(ncluster)==1 || true)
#endif
		{	
		  // double gps_diff_temp = gps_diff + od_cluster_start->at(ncluster)*1.e+3;  //ns	  
		  double gps_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;  //ns	  
		  nv_gps_hist->Fill(gps_diff_temp);
		  nv_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_gps_ene_hist->Fill(gps_diff_temp,od_cluster_charge->at(ncluster));
		  nv_ntuple->Fill(gps_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
		  //  nv_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		  nv_charge_height_multiplicity_hist->Fill(od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster)/od_cluster_multiplicity->at(ncluster));
		  nv_mult_height_multiplicity_hist->Fill(od_cluster_multiplicity->at(ncluster),od_cluster_height->at(ncluster)/od_cluster_multiplicity->at(ncluster));
  
		  double gps_gamma_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;	 
		  //    lsv_gamma_gps.push_back(gps_gamma_diff_temp);
		  nv_gamma_full_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_gamma_full_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  //if(gps_gamma_diff_temp<gamma_gps_cut && od_cluster_charge->at(ncluster)>gamma_od_charge_cut)
		  //	lsv_gamma_tag = true;
		  //		  int g=0;
		  if(ncluster==0)
		    nv_gamma_0_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	    
		  if(ncluster==1)
		    nv_gamma_1_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	    
		  if(ncluster==2)
		    nv_gamma_2_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	    
		  if(ncluster==3)
		    nv_gamma_3_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	    	   		  

		  if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<prompt_time)
		    {	       
		      bigpulse_tag=true;
		    }
		  if(gps_gamma_diff_temp>prompt_time && gps_gamma_diff_temp<first_time && od_cluster_charge->at(ncluster)>afterpulse_cut)
		    {
		      afterpulse_tag=true;
		      nv_gamma_second_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_second_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_second_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_second_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_second_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));		  
		    }
		  if(gps_gamma_diff_temp<window1start && gps_gamma_diff_temp>first_time && od_cluster_charge->at(ncluster)>afterpulse_cut)
		    {
		      afterpulse_window1_tag=true;
		    }
		  if(gps_gamma_diff_temp<window2start && gps_gamma_diff_temp>window1start && od_cluster_charge->at(ncluster)>afterpulse_cut)
		    {
		      afterpulse_window2_tag=true;
		    }
		}}}
      //      if(!afterpulse_tag){
      for(int ncluster=0; ncluster<od_cluster_charge->size(); ncluster++)
	{ 
	  if(od_cluster_charge->at(ncluster)>0 && od_wt_charge<500) 
	    {
	      if(od_cluster_pass_multcut->at(ncluster)==1)
		{
		  double gps_gamma_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;	 
		  if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<end_time)
		    {
		      //  nv_gamma_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_coincidence_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_coincidence_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_coincidence_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	    
		    }		    
		  if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<prompt_time)
		    {	       
		      ++g;
		      //    cout<<g<<endl;
		      //     nv_gamma_prompt_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_prompt_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_prompt_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_prompt_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_prompt_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_prompt_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster),g);
		      prompt_gps.push_back(gps_gamma_diff_temp);
		    }
		  if(gps_gamma_diff_temp>delay_time && gps_gamma_diff_temp<late_time)
		    {
		      //  nv_gamma_delay_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_delay_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_delay_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_delay_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_delay_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_delay_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));		
		    }
		  if(gps_gamma_diff_temp>after_time && gps_gamma_diff_temp<delay_time)
		    {
		      //  nv_gamma_after_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_after_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_after_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_after_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_after_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_after_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));		     
		    }
		  if(gps_gamma_diff_temp>first_time && gps_gamma_diff_temp<delay_time && !afterpulse_tag)
		    { ++gp;
		      if(gp==1)
			{
			  //  nv_gamma_first_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_gamma_first_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_first_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_first_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
			  if(od_cluster_charge->at(ncluster)>pos3 && od_cluster_charge->at(ncluster)<pos4)
			    {
			      nv_gamma_first_alpha_gps_hist->Fill(gps_gamma_diff_temp);
			      alpha_counts++;
			    }
			  if(od_cluster_charge->at(ncluster)>pos3 && od_cluster_charge->at(ncluster)<pos2)
			    {			    
			      neutron_counts++;
			    }			  
			  if(od_cluster_charge->at(ncluster)>pos1 && od_cluster_charge->at(ncluster)<pos2)
			    nv_gamma_first_r_gps_hist->Fill(gps_gamma_diff_temp);
			  first_gps=gps_gamma_diff_temp;
			}
		    }
		  if(gps_gamma_diff_temp>window1start && gps_gamma_diff_temp<window1end && !afterpulse_window1_tag)
		    { ++g1;
		      if(g1==1)
			{		      
			  nv_gamma_window1_first_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_window1_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_window1_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_window1_first_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_window1_first_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	     
			  if(od_cluster_charge->at(ncluster)>pos3 && od_cluster_charge->at(ncluster)<pos4)
			    nv_gamma_window1_first_alpha_gps_hist->Fill(gps_gamma_diff_temp);
			  if(od_cluster_charge->at(ncluster)>pos1 && od_cluster_charge->at(ncluster)<pos2)
			    nv_gamma_window1_first_r_gps_hist->Fill(gps_gamma_diff_temp);
			}
		    }
		  if(gps_gamma_diff_temp>window2start && gps_gamma_diff_temp<window2end && !afterpulse_window2_tag)
		    { ++g2;
		      if(g2==1)
			{		      
			  nv_gamma_window2_first_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_window2_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_window2_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_window2_first_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_window2_first_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	     
			  if(od_cluster_charge->at(ncluster)>pos3 && od_cluster_charge->at(ncluster)<pos4)
			    nv_gamma_window2_first_alpha_gps_hist->Fill(gps_gamma_diff_temp);
			  if(od_cluster_charge->at(ncluster)>pos1 && od_cluster_charge->at(ncluster)<pos2)
			    nv_gamma_window2_first_r_gps_hist->Fill(gps_gamma_diff_temp);
			}
		    }
		  if(gps_gamma_diff_temp>late_time && gps_gamma_diff_temp<end_time)
		    { ++glate;
		      if(glate==1){
		      //     nv_gamma_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_late_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_late_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_late_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));	 
		      }
		    }
		  //}
		}//end of passing multicut 
	    }//end of od_cluster_charge>0 cut	  	  
	} //end of veto clusters       
      if(prompt_gps.size()){	
	nv_gamma_first_delta_gps_hist->Fill(first_gps-prompt_gps.back());
	nv_gamma_first_delta_ntuple->Fill(first_gps-prompt_gps.back());	
      }
    } //end events 
  cout<<"alphacounts="<<alpha_counts<<"\t neutroncounts="<<neutron_counts<<endl;
  /*  nv_gamma_after_small_ene_hist->Sumw2();
  nv_gamma_first_small_ene_hist->Sumw2();
  nv_gamma_coincidence_small_ene_hist->Sumw2();
  nv_gamma_prompt_small_ene_hist->Sumw2();
  nv_gamma_delay_small_ene_hist->Sumw2();
  */
  nv_gamma_first_small_ene_hist->Sumw2();
  nv_gamma_window1_first_small_ene_hist->Sumw2();
  nv_gamma_window2_first_small_ene_hist->Sumw2();
  nv_gamma_late_small_ene_hist->Sumw2();

  nv_gamma_after_small_ene_hist->Scale(1./Entries);
  nv_gamma_first_small_ene_hist->Scale(1./Entries);
  nv_gamma_coincidence_small_ene_hist->Scale(1./Entries);
  nv_gamma_prompt_small_ene_hist->Scale(1./Entries);
  nv_gamma_late_small_ene_hist->Scale(1./Entries);
  nv_gamma_delay_small_ene_hist->Scale(1./Entries);
  nv_gamma_window1_first_small_ene_hist->Scale(1./Entries);
  nv_gamma_window2_first_small_ene_hist->Scale(1./Entries);
  
  //  cout<<"tpc_neutron= "<<tpc_neutron<<"\t lsv_neutron= "<<lsv_neutron<<"\t neutron capture efficiency= "<<(1.0*lsv_neutron)/tpc_neutron<<endl;
  vector<int> linecolor = Colors();
  nv_gamma_delay_ene_hist->SetLineColor(linecolor.at(0));
  nv_gamma_late_ene_hist->SetLineColor(linecolor.at(1));
  //  nv_neutron_late_ene_hist->SetLineColor(linecolor.at(2));
  nv_gamma_late_small_ene_hist->SetLineColor(linecolor.at(3));
  //  nv_neutron_late_small_ene_hist->SetLineColor(linecolor.at(4));
  /*
#ifdef new_data
  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTAwayResults/";
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/FermiDSTAwayResults/Plots/";
  double pos3 = 10.;
#else
  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/";
  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/Plots/";
  double pos3 = 0.;
#endif
  string ext = "png";

  double pos1 = 200.;
  double pos2 = 350.;
  //  double pos3 = 0.;
  double pos4 = 50.;
  */
  double height = 0.008;
  TBox *box1 = new TBox(pos3,0,pos4,height);
  box1->SetLineColor(2);
  box1->SetLineWidth(1);

  TBox *box2 = new TBox(pos1,0,pos2,height);
  box2->SetLineColor(3);
  box2->SetLineWidth(1);

#ifdef random_data
  string bgfile = outdirname+"AmBe10Hz_1125611257_away_Mar28PM.root";  
  //  string bgfile = outdirname+"AmBe10Hz_1107011071_away_Mar28PM.root"; 
  //  string bgfile = outdirname+"AmBe10Hz_1107211073_self_Mar26PM.root"; 
  cout<<"bg data "<<bgfile<<endl;
  TFile *bg = new TFile(bgfile.c_str());
  TH1F *bg_gamma_prompt_small_ene_hist = (TH1F*) bg->Get("nv_gamma_prompt_small_ene_hist");
  TH1F *bg_gamma_after_small_ene_hist = (TH1F*) bg->Get("nv_gamma_after_small_ene_hist");
  TH1F *bg_gamma_first_small_ene_hist = (TH1F*) bg->Get("nv_gamma_first_small_ene_hist");
  TH1F *bg_gamma_delay_small_ene_hist = (TH1F*) bg->Get("nv_gamma_delay_small_ene_hist");
  TH1F *bg_gamma_late_small_ene_hist = (TH1F*) bg->Get("nv_gamma_late_small_ene_hist");
  TH1F *bg_gamma_coincidence_small_ene_hist = (TH1F*) bg->Get("nv_gamma_coincidence_small_ene_hist");
  TH1F *bg_gamma_window1_first_small_ene_hist = (TH1F*) bg->Get("nv_gamma_window1_first_small_ene_hist");
  TH1F *bg_gamma_window2_first_small_ene_hist = (TH1F*) bg->Get("nv_gamma_window2_first_small_ene_hist");

  bg_gamma_prompt_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_after_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_first_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_delay_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_late_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_coincidence_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_window1_first_small_ene_hist->SetLineColor(linecolor.at(3));
  bg_gamma_window2_first_small_ene_hist->SetLineColor(linecolor.at(3));
#endif  

  TH1F* nv_gamma_delay_small_ene_subtracted_hist = new TH1F("nv_gamma_delay_small_ene_subtracted_hist","Veto Gamma Subtracted Delay Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_delay_small_ene_subtracted_hist->Add(nv_gamma_delay_small_ene_hist,bg_gamma_delay_small_ene_hist,1,-1);
#else
  nv_gamma_delay_small_ene_subtracted_hist->Add(nv_gamma_delay_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c11 = new TCanvas("c11","Veto Gamma Delay Small Energy Subtracting BG",1000,400);
  c11->Divide(2,1);
  c11->cd(1);
  gPad->SetLogy();
  nv_gamma_delay_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_delay_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c11->cd(2);
  gPad->SetLogy();
  nv_gamma_delay_small_ene_subtracted_hist->Draw();
  //  c11->SaveAs(Form("%snv_gamma_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));  
  box1->Draw();
  box2->Draw();

  TH1F* nv_gamma_coincidence_small_ene_subtracted_hist = new TH1F("nv_gamma_coincidence_small_ene_subtracted_hist","Veto Gamma Subtracted Coincidence Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_coincidence_small_ene_subtracted_hist->Add(nv_gamma_coincidence_small_ene_hist,bg_gamma_coincidence_small_ene_hist,1,-1);
#else
  nv_gamma_coincidence_small_ene_subtracted_hist->Add(nv_gamma_coincidence_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c12 = new TCanvas("c12","Veto Gamma Coincidence Small Energy Subtracting BG",1000,400);
  c12->Divide(2,1);
  c12->cd(1);
  gPad->SetLogy();
  nv_gamma_coincidence_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_coincidence_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c12->cd(2);
  gPad->SetLogy();
  nv_gamma_coincidence_small_ene_subtracted_hist->Draw();
  //c12->SaveAs(Form("%snv_gamma_coincidence_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  box1->Draw();
  box2->Draw();

  TH1F* nv_gamma_after_small_ene_subtracted_hist = new TH1F("nv_gamma_after_small_ene_subtracted_hist","Veto Gamma Subtracted After Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_after_small_ene_subtracted_hist->Add(nv_gamma_after_small_ene_hist,bg_gamma_after_small_ene_hist,1,-1);
#else
  nv_gamma_after_small_ene_subtracted_hist->Add(nv_gamma_after_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c13 = new TCanvas("c13","Veto Gamma AFter Small Energy Subtracting BG",1000,400);
  c13->Divide(2,1);
  c13->cd(1);
  //  gPad->SetLogy();
  nv_gamma_after_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_after_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c13->cd(2);
  // gPad->SetLogy();
  nv_gamma_after_small_ene_subtracted_hist->Draw();
  //c13->SaveAs(Form("%snv_gamma_after_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  box1->Draw();
  box2->Draw();
  double br1 = nv_gamma_after_small_ene_subtracted_hist->Integral(nv_gamma_after_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_after_small_ene_subtracted_hist->FindBin(pos4));
  double br2 = nv_gamma_after_small_ene_subtracted_hist->Integral(nv_gamma_after_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_after_small_ene_subtracted_hist->FindBin(pos2));
  cout<<"after "<<1.*br1/br2<<endl;


  TH1F* nv_gamma_first_small_ene_subtracted_hist = new TH1F("nv_gamma_first_small_ene_subtracted_hist","Veto Gamma Subtracted First Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_first_small_ene_subtracted_hist->Add(nv_gamma_first_small_ene_hist,bg_gamma_first_small_ene_hist,1,-1);
#else
  nv_gamma_first_small_ene_subtracted_hist->Add(nv_gamma_first_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c14 = new TCanvas("c14","Veto Gamma First Small Energy Subtracting BG",1000,400);
  c14->Divide(2,1);
  c14->cd(1);
  //  gPad->SetLogy();
  nv_gamma_first_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_first_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c14->cd(2);
  // gPad->SetLogy();
  nv_gamma_first_small_ene_subtracted_hist->Draw();
  //c14->SaveAs(Form("%snv_gamma_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  box1->Draw();
  box2->Draw();
  double br3 = nv_gamma_first_small_ene_subtracted_hist->Integral(nv_gamma_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_first_small_ene_subtracted_hist->FindBin(pos4));
  double br4 = nv_gamma_first_small_ene_subtracted_hist->Integral(nv_gamma_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_first_small_ene_subtracted_hist->FindBin(pos2));
  cout<<"first "<<1.*br3/br4<<endl;


  TH1F* nv_gamma_prompt_small_ene_subtracted_hist = new TH1F("nv_gamma_prompt_small_ene_subtracted_hist","Veto Gamma Subtracted Prompt Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_prompt_small_ene_subtracted_hist->Add(nv_gamma_prompt_small_ene_hist,bg_gamma_prompt_small_ene_hist,1,-1);
#else
  nv_gamma_prompt_small_ene_subtracted_hist->Add(nv_gamma_prompt_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c15 = new TCanvas("c15","Veto Gamma Prompt Small Energy Subtracting BG",1000,400);
  c15->Divide(2,1);
  c15->cd(1);
  gPad->SetLogy();
  nv_gamma_prompt_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_prompt_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c15->cd(2);
  gPad->SetLogy();
  nv_gamma_prompt_small_ene_subtracted_hist->Draw();
  //c15->SaveAs(Form("%snv_gamma_prompt_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  box1->Draw();
  box2->Draw();

  TCanvas *c16 = new TCanvas("c16","Veto Gamma Coincidence Energy",1000,400);
  c16->Divide(2,1);
  c16->cd(1);
  gPad->SetLogy();
  nv_gamma_coincidence_ene_hist->Draw();
  c16->cd(2);
  gPad->SetLogy();
  nv_gamma_coincidence_small_ene_hist->Draw();

  TCanvas *c10 = new TCanvas("c10","Veto height/multiplicity Vs Charge",600,400);
  //  nv_charge_height_multiplicity_hist->SetLineColor(linecolor.at(1));
  nv_charge_height_multiplicity_hist->SetMarkerColor(linecolor.at(6));
  nv_all_charge_height_multiplicity_hist->Draw("colz");
  nv_charge_height_multiplicity_hist->Draw("same");
  //  mult_hist->Draw("same");

  TLegend *leg = new TLegend(0.47,0.7,0.87,0.9);
  string entryname[3] = {"all clusters","passing multicut","failing multicut"};
  
  TCanvas *c2 = new TCanvas("c2","Low Energy Spectrum",600,400);
  gPad->SetLogy();
  //  gStyle->SetOptStat(0);
  nv_all_small_ene_hist->SetLineColor(linecolor.at(0));
  nv_small_ene_hist->SetLineColor(linecolor.at(1));
  nv_fail_small_ene_hist->SetLineColor(linecolor.at(2));
  leg->AddEntry(nv_all_small_ene_hist,entryname[0].c_str(),"l");
  leg->AddEntry(nv_small_ene_hist,entryname[1].c_str(),"l");
  leg->AddEntry(nv_fail_small_ene_hist,entryname[2].c_str(),"l");
  nv_all_small_ene_hist->Draw();
  nv_small_ene_hist->Draw("same");
  nv_fail_small_ene_hist->Draw("same");
  leg->Draw();
  //c2->SaveAs(Form("%smulticut%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c21 = new TCanvas("c21","Veto Gamma Coincidence",1000,600);
  c21->Divide(2,2);
  c21->cd(1);
  gPad->SetLogy();
  nv_gamma_coincidence_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_coincidence_gps_hist->Draw();
  c21->cd(2);
  gPad->SetLogy();
  nv_gamma_coincidence_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_coincidence_ene_hist->Draw();
  c21->cd(3);
  gPad->SetLogy();
  //  nv_gamma_coincidence_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_coincidence_gps_ene_hist->Draw("colz");
  // c21->cd(4);
  // gPad->SetLogy();
  //  nv_gamma_coincidence_ene_s1_hist->SetLineColor(linecolor.at(6));
  // nv_gamma_coincidence_ene_s1_hist->Draw("colz");
  //c21->SaveAs(Form("%snv_gamma_coincidence%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c22 = new TCanvas("c22","Veto Gamma Prompt",1000,600);
  c22->Divide(2,2);
  c22->cd(1);
  gPad->SetLogy();
  nv_gamma_prompt_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_prompt_gps_hist->Draw();
  c22->cd(2);
  gPad->SetLogy();
  nv_gamma_prompt_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_prompt_ene_hist->Draw();
  c22->cd(3);
  gPad->SetLogy();
  //  nv_gamma_prompt_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_prompt_gps_ene_hist->Draw("colz");
  //  c22->cd(4);
  //  gPad->SetLogy();
  //  nv_gamma_prompt_ene_s1_hist->SetLineColor(linecolor.at(6));
  //  nv_gamma_prompt_ene_s1_hist->Draw("colz");
  //c22->SaveAs(Form("%snv_gamma_prompt%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c23 = new TCanvas("c23","Veto Gamma Delay",1000,600);
  c23->Divide(2,2);
  c23->cd(1);
  gPad->SetLogy();
  nv_gamma_delay_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_delay_gps_hist->Draw();
  c23->cd(2);
  gPad->SetLogy();
  nv_gamma_delay_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_delay_ene_hist->Draw();
  c23->cd(3);
  gPad->SetLogy();
  //  nv_gamma_delay_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_delay_gps_ene_hist->Draw("colz");
  //  c23->cd(4);
  //  gPad->SetLogy();
  //  nv_gamma_delay_ene_s1_hist->SetLineColor(linecolor.at(6));
  //  nv_gamma_delay_ene_s1_hist->Draw("colz");
  //  c23->SaveAs(Form("%snv_gamma_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c24 = new TCanvas("c24","Veto Gamma After",1000,600);
  c24->Divide(2,2);
  c24->cd(1);
  gPad->SetLogy();
  nv_gamma_after_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_after_gps_hist->Draw();
  c24->cd(2);
  gPad->SetLogy();
  nv_gamma_after_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_after_ene_hist->Draw();
  c24->cd(3);
  gPad->SetLogy();
  //  nv_gamma_after_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_after_gps_ene_hist->Draw("colz");
  //  c24->cd(4);
  //  gPad->SetLogy();
  //  nv_gamma_after_ene_s1_hist->SetLineColor(linecolor.at(6));
  //  nv_gamma_after_ene_s1_hist->Draw("colz");
  //  c24->SaveAs(Form("%snv_gamma_after%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c25 = new TCanvas("c25","Veto Gamma Late",1000,600);
  c25->Divide(2,2);
  c25->cd(1);
  gPad->SetLogy();
  //  nv_gamma_late_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_late_gps_hist->Draw();
  c25->cd(2);
  gPad->SetLogy();
  // nv_gamma_late_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_late_ene_hist->Draw();
  c25->cd(3);
  gPad->SetLogy();
  //  nv_gamma_late_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_late_gps_ene_hist->Draw("colz");
  //  c25->cd(4);
  //  gPad->SetLogy();
  //  nv_gamma_late_ene_s1_hist->SetLineColor(linecolor.at(6));
  //  nv_gamma_late_ene_s1_hist->Draw("colz");
  //  c25->SaveAs(Form("%snv_gamma_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c26 = new TCanvas("c26","Veto Gamma First",1000,600);
  c26->Divide(2,2);
  c26->cd(1);
  gPad->SetLogy();
  nv_gamma_first_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_first_gps_hist->Draw();
  c26->cd(2);
  gPad->SetLogy();
  nv_gamma_first_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_first_ene_hist->Draw();
  c26->cd(3);
  gPad->SetLogy();
  //  nv_gamma_first_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_first_gps_ene_hist->Draw("colz");
  //  c26->cd(4);
  //  gPad->SetLogy();
  //  nv_gamma_first_ene_s1_hist->SetLineColor(linecolor.at(6));
  //  nv_gamma_first_ene_s1_hist->Draw("colz");
  //  c26->SaveAs(Form("%snv_gamma_first%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  nv_gamma_prompt_small_ene_hist->SetLineColor(linecolor.at(8));
  nv_gamma_after_small_ene_hist->SetLineColor(linecolor.at(9));
  nv_gamma_delay_small_ene_hist->SetLineColor(linecolor.at(11));

  TLegend *leg27 = new TLegend(0.47,0.7,0.87,0.9);
  TLegend *leg28 = new TLegend(0.47,0.7,0.87,0.9);
  string entry27[4] = {"[0,20us]","[20us,60us]","[60us,100us]","[100us,140us]"};
  string entry28[3] = {"[20us,60us]","[60us,100us]","[100us,140us]"};

  TCanvas *c27 = new TCanvas("c27","Veto Gamma All sorts",600,400);
  c27->cd();
  gPad->SetLogy();
  leg27->AddEntry(nv_gamma_prompt_small_ene_hist,entry27[0].c_str(),"l");
  leg27->AddEntry(nv_gamma_after_small_ene_hist,entry27[1].c_str(),"l");
  leg27->AddEntry(nv_gamma_delay_small_ene_hist,entry27[2].c_str(),"l");
  leg27->AddEntry(nv_gamma_late_small_ene_hist,entry27[3].c_str(),"l");
  nv_gamma_prompt_small_ene_hist->Draw();
  nv_gamma_after_small_ene_hist->Draw("same");
  nv_gamma_delay_small_ene_hist->Draw("same");
  nv_gamma_late_small_ene_hist->Draw("same");
  leg27->Draw();
  
  TCanvas *c28 = new TCanvas("c28","Veto Gamma sorts",600,400);
  c28->cd();
  gPad->SetLogy();
  leg28->AddEntry(nv_gamma_after_small_ene_hist,entry28[0].c_str(),"l");
  leg28->AddEntry(nv_gamma_delay_small_ene_hist,entry28[1].c_str(),"l");
  leg28->AddEntry(nv_gamma_late_small_ene_hist,entry28[2].c_str(),"l");
  nv_gamma_after_small_ene_hist->Draw();
  nv_gamma_delay_small_ene_hist->Draw("same");
  nv_gamma_late_small_ene_hist->Draw("same");
  leg28->Draw();

  TH1F* nv_gamma_window1_first_small_ene_subtracted_hist = new TH1F("nv_gamma_window1_first_small_ene_subtracted_hist","Veto Gamma window1 Subtracted First Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_window1_first_small_ene_subtracted_hist->Add(nv_gamma_window1_first_small_ene_hist,bg_gamma_window1_first_small_ene_hist,1,-1);
#else
  nv_gamma_window1_first_small_ene_subtracted_hist->Add(nv_gamma_window1_first_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c51 = new TCanvas("c51","Veto Gamma window1 First Small Energy Subtracting BG",1000,400);
  c51->Divide(2,1);
  c51->cd(1);
  //  gPad->SetLogy();
  nv_gamma_window1_first_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_window1_first_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c51->cd(2);
  // gPad->SetLogy();
  nv_gamma_window1_first_small_ene_subtracted_hist->SetLineColor(linecolor.at(8));;
  nv_gamma_window1_first_small_ene_subtracted_hist->Draw();
  //c14->SaveAs(Form("%snv_gamma_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  box1->Draw();
  box2->Draw();
  double br51 = nv_gamma_window1_first_small_ene_subtracted_hist->Integral(nv_gamma_window1_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_window1_first_small_ene_subtracted_hist->FindBin(pos4));
  double br52 = nv_gamma_window1_first_small_ene_subtracted_hist->Integral(nv_gamma_window1_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_window1_first_small_ene_subtracted_hist->FindBin(pos2));
  cout<<"window1 first "<<1.*br51/br52<<endl;

  TH1F* nv_gamma_window2_first_small_ene_subtracted_hist = new TH1F("nv_gamma_window2_first_small_ene_subtracted_hist","Veto Gamma window2 Subtracted First Small Energy;Energy [PE]",fail_bins,0,fail_ene);
#ifdef random_data
  nv_gamma_window2_first_small_ene_subtracted_hist->Add(nv_gamma_window2_first_small_ene_hist,bg_gamma_window2_first_small_ene_hist,1,-1);
#else
  nv_gamma_window2_first_small_ene_subtracted_hist->Add(nv_gamma_window2_first_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
#endif
  TCanvas *c52 = new TCanvas("c52","Veto Gamma window2 First Small Energy Subtracting BG",1000,400);
  c52->Divide(2,1);
  c52->cd(1);
  //  gPad->SetLogy();
  nv_gamma_window2_first_small_ene_hist->Draw();
#ifdef random_data
  bg_gamma_window2_first_small_ene_hist->Draw("same");
#else
  nv_gamma_late_small_ene_hist->Draw("same");
#endif
  box1->Draw();
  box2->Draw();
  c52->cd(2);
  // gPad->SetLogy();
  nv_gamma_window2_first_small_ene_subtracted_hist->SetLineColor(linecolor.at(7));;
  nv_gamma_window2_first_small_ene_subtracted_hist->Draw();
  //c14->SaveAs(Form("%snv_gamma_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  box1->Draw();
  box2->Draw();
  double br53 = nv_gamma_window2_first_small_ene_subtracted_hist->Integral(nv_gamma_window2_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_window2_first_small_ene_subtracted_hist->FindBin(pos4));
  double br54 = nv_gamma_window2_first_small_ene_subtracted_hist->Integral(nv_gamma_window2_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_window2_first_small_ene_subtracted_hist->FindBin(pos2));
  cout<<"window2 first "<<1.*br53/br54<<endl;

  TLegend *leg53 = new TLegend(0.47,0.7,0.87,0.9);
  //  string entry53[3] = {"First cluster in [20us,60us]","First cluster in [30us,70us]","First cluster in [40us,80us]"};
  string entry53[3] = {"First cluster in [20us,50us]","First cluster in [30us,60us]","First cluster in [40us,70us]"};
  leg53->AddEntry(nv_gamma_first_small_ene_subtracted_hist,entry53[0].c_str(),"l");
  leg53->AddEntry(nv_gamma_window1_first_small_ene_subtracted_hist,entry53[1].c_str(),"l");
  leg53->AddEntry(nv_gamma_window2_first_small_ene_subtracted_hist,entry53[2].c_str(),"l");
  TCanvas *c53 = new TCanvas("c53","Veto Gamma First Small Energy Sub sorts",600,400);
  nv_gamma_first_small_ene_subtracted_hist->Draw();
  nv_gamma_window1_first_small_ene_subtracted_hist->Draw("same");
  nv_gamma_window2_first_small_ene_subtracted_hist->Draw("same");
  leg53->Draw();

  TCanvas *c54 = new TCanvas("c54","Veto First Alpha Time sorts",600,400);
  nv_gamma_window1_first_alpha_gps_hist->SetLineColor(linecolor.at(8));
  nv_gamma_window2_first_alpha_gps_hist->SetLineColor(linecolor.at(7));
  nv_gamma_first_alpha_gps_hist->Draw();
  nv_gamma_window1_first_alpha_gps_hist->Draw("same");
  nv_gamma_window2_first_alpha_gps_hist->Draw("same");
  leg53->Draw();

  TCanvas *c55 = new TCanvas("c55","Veto First Alpha+Gamma Time sorts",600,400);
  nv_gamma_window1_first_r_gps_hist->SetLineColor(linecolor.at(8));
  nv_gamma_window2_first_r_gps_hist->SetLineColor(linecolor.at(7));
  nv_gamma_first_r_gps_hist->Draw();
  nv_gamma_window1_first_r_gps_hist->Draw("same");
  nv_gamma_window2_first_r_gps_hist->Draw("same");
  leg53->Draw();


#ifdef Write_Plots
  //  c1->SaveAs(Form("%snv_gamma_delay_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));  
  c2->SaveAs(Form("%smulticut%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  /*  c3->SaveAs(Form("%snv_gamma_after_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c4->SaveAs(Form("%snv_gamma_alpha_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c5->SaveAs(Form("%snv_neutron_alpha_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c6->SaveAs(Form("%snv_gamma_alpha_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c7->SaveAs(Form("%snv_neutron_alpha_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c8->SaveAs(Form("%snv_gamma_alpha_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c9->SaveAs(Form("%snv_neutron_alpha_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */
  c10->SaveAs(Form("%snv_charge_height_multiplicity%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  c11->SaveAs(Form("%snv_gamma_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));  
  c12->SaveAs(Form("%snv_gamma_coincidence_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c13->SaveAs(Form("%snv_gamma_after_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c14->SaveAs(Form("%snv_gamma_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c15->SaveAs(Form("%snv_gamma_prompt_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c16->SaveAs(Form("%snv_gamma_coincidence_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  //  c17->SaveAs(Form("%snv_gamma_coincidence_small_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  c21->SaveAs(Form("%snv_gamma_coincidence%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c22->SaveAs(Form("%snv_gamma_prompt%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c23->SaveAs(Form("%snv_gamma_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c24->SaveAs(Form("%snv_gamma_after%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c25->SaveAs(Form("%snv_gamma_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c26->SaveAs(Form("%snv_gamma_first%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c27->SaveAs(Form("%snv_gamma_all_sorts%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c28->SaveAs(Form("%snv_gamma_sorts%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c51->SaveAs(Form("%snv_gamma_window1_first_small_ene_sub%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c52->SaveAs(Form("%snv_gamma_window2_first_small_ene_sub%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c53->SaveAs(Form("%snv_gamma_first_small_ene_sub_sorts%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c54->SaveAs(Form("%snv_gamma_first_alpha_time_sorts%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c55->SaveAs(Form("%snv_gamma_first_r_time_sorts%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  //  c29->SaveAs(Form("%snv_gamma_alpha_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  /*
  c31->SaveAs(Form("%snv_neutron_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));  
  c32->SaveAs(Form("%snv_neutron_coincidence_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c33->SaveAs(Form("%snv_neutron_after_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c34->SaveAs(Form("%snv_neutron_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c35->SaveAs(Form("%snv_neutron_prompt_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  c41->SaveAs(Form("%snv_neutron_coincidence%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c42->SaveAs(Form("%snv_neutron_prompt%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c43->SaveAs(Form("%snv_neutron_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c44->SaveAs(Form("%snv_neutron_after%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c45->SaveAs(Form("%snv_neutron_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c46->SaveAs(Form("%snv_neutron_first%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */ //  c47->SaveAs(Form("%snv_neutron_alpha%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  //  c48->SaveAs(Form("%snv_neutron_alpha_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  //  c49->SaveAs(Form("%snv_neutron_alpha_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
#endif
  //  c10->SaveAs(Form("%snv_charge_height_multiplicity%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  //===============Save The Plots=============================================  
  //  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/";
  string outputname=outdirname+"AmBe10Hz_"+series.Data()+Time+".root";
  TFile f2D(outputname.c_str(), "RECREATE"); 
  /*  tpc_total_s1_hist->Write();
  tpc_s1_late_hist->Write();
  tpc_s1_starttime_hist->Write();
  tpc_total_f90_hist->Write();
  tpc_total_s1_f90_hist->Write();
  tpc_gamma_total_s1_hist->Write();
  tpc_gamma_s1_late_hist->Write();
  tpc_neutron_total_s1_hist->Write();
  tpc_neutron_s1_late_hist->Write();
  */
  /*  tpc_gamma_gps_hist->Write();
  tpc_gamma_ene_hist->Write();
  tpc_gamma_gps_ene_hist->Write();
  tpc_gamma_ntuple->Write();
  */
  //********Save All OD clusters************************
  nv_all_gps_hist->Write();
  nv_all_ene_hist->Write();
  nv_all_small_ene_hist->Write();
  nv_all_gps_ene_hist->Write();
  nv_all_ntuple->Write();  

  nv_all_first_gps_hist->Write();
  nv_all_first_ene_hist->Write();
  nv_all_first_small_ene_hist->Write();
  nv_all_first_gps_ene_hist->Write();
  nv_all_first_ntuple->Write();  

  /*  nv_all_gamma_coincidence_gps_hist->Write();
  nv_all_gamma_coincidence_ene_hist->Write();
  nv_all_gamma_coincidence_small_ene_hist->Write();
  nv_all_gamma_coincidence_gps_ene_hist->Write();
  nv_all_gamma_coincidence_ene_s1_hist->Write();
  nv_all_gamma_coincidence_ntuple->Write();  
  nv_all_neutron_coincidence_gps_hist->Write();
  nv_all_neutron_coincidence_ene_hist->Write();
  nv_all_neutron_coincidence_small_ene_hist->Write();
  nv_all_neutron_coincidence_gps_ene_hist->Write();
  nv_all_neutron_coincidence_ene_s1_hist->Write();
  nv_all_neutron_coincidence_ntuple->Write();  
  */
  //*********Save Passing MultiCut cluster**************
  nv_gps_hist->Write();
  nv_ene_hist->Write();
  nv_small_ene_hist->Write();
  nv_gps_ene_hist->Write();
  //  nv_ene_s1_hist->Write();
  nv_ntuple->Write();  
  /*  nv_late_gps_hist->Write();
  nv_late_ene_hist->Write();
  nv_late_small_ene_hist->Write();
  nv_late_gps_ene_hist->Write();
  nv_late_ene_s1_hist->Write();
  nv_late_ntuple->Write();  
  */
  nv_gamma_full_ene_hist->Write();
  nv_gamma_full_small_ene_hist->Write();
  nv_gamma_0_ntuple->Write();
  nv_gamma_1_ntuple->Write();
  nv_gamma_2_ntuple->Write();
  nv_gamma_3_ntuple->Write();
  
  //  nv_gamma_ene_s1_hist->Write();
  //  nv_gamma_coincidence_ene_s1_hist->Write();
  nv_gamma_coincidence_gps_hist->Write();
  nv_gamma_coincidence_ene_hist->Write();
  nv_gamma_coincidence_small_ene_hist->Write();
  nv_gamma_coincidence_gps_ene_hist->Write();
  nv_gamma_coincidence_ntuple->Write();
  //  nv_gamma_prompt_ene_s1_hist->Write();
  nv_gamma_prompt_gps_hist->Write();
  nv_gamma_prompt_ene_hist->Write();
  nv_gamma_prompt_small_ene_hist->Write();
  nv_gamma_prompt_gps_ene_hist->Write();
  nv_gamma_prompt_ntuple->Write();

  nv_gamma_second_gps_hist->Write();
  nv_gamma_second_ene_hist->Write();
  nv_gamma_second_small_ene_hist->Write();
  nv_gamma_second_gps_ene_hist->Write();
  nv_gamma_second_ntuple->Write();

  //  nv_gamma_delay_ene_s1_hist->Write();
  nv_gamma_delay_gps_hist->Write();
  nv_gamma_delay_ene_hist->Write();
  nv_gamma_delay_small_ene_hist->Write();
  nv_gamma_delay_gps_ene_hist->Write();
  nv_gamma_delay_ntuple->Write();
  //  nv_gamma_after_ene_s1_hist->Write();
  nv_gamma_after_gps_hist->Write();
  nv_gamma_after_ene_hist->Write();
  nv_gamma_after_small_ene_hist->Write();
  nv_gamma_after_gps_ene_hist->Write();
  nv_gamma_after_ntuple->Write();
  //  nv_gamma_first_ene_s1_hist->Write();
  nv_gamma_first_gps_hist->Write();
  nv_gamma_first_ene_hist->Write();
  nv_gamma_first_small_ene_hist->Write();
  nv_gamma_first_gps_ene_hist->Write();
  nv_gamma_first_alpha_gps_hist->Write();
  nv_gamma_first_r_gps_hist->Write();
  nv_gamma_first_delta_gps_hist->Write();
  nv_gamma_first_delta_ntuple->Write();
  nv_gamma_first_ntuple->Write();
  //  nv_gamma_late_ene_s1_hist->Write();
  nv_gamma_late_gps_hist->Write();
  nv_gamma_late_ene_hist->Write();
  nv_gamma_late_small_ene_hist->Write();
  nv_gamma_late_gps_ene_hist->Write();
  nv_gamma_late_ntuple->Write();

  nv_gamma_window1_first_gps_hist->Write();
  nv_gamma_window1_first_ene_hist->Write();
  nv_gamma_window1_first_small_ene_hist->Write();
  nv_gamma_window1_first_gps_ene_hist->Write();
  nv_gamma_window1_first_alpha_gps_hist->Write();
  nv_gamma_window1_first_r_gps_hist->Write();
  nv_gamma_window1_first_ntuple->Write();

  nv_gamma_window2_first_gps_hist->Write();
  nv_gamma_window2_first_ene_hist->Write();
  nv_gamma_window2_first_small_ene_hist->Write();
  nv_gamma_window2_first_alpha_gps_hist->Write();
  nv_gamma_window2_first_r_gps_hist->Write();
  nv_gamma_window2_first_gps_ene_hist->Write();
  nv_gamma_window2_first_ntuple->Write();

  /*  nv_neutron_ene_s1_hist->Write();
  nv_neutron_full_ene_hist->Write();
  nv_neutron_full_small_ene_hist->Write();
  nv_neutron_coincidence_ene_s1_hist->Write();
  nv_neutron_coincidence_gps_hist->Write();
  nv_neutron_coincidence_ene_hist->Write();
  nv_neutron_coincidence_small_ene_hist->Write();
  nv_neutron_coincidence_gps_ene_hist->Write();
  nv_neutron_coincidence_ntuple->Write();
  nv_neutron_prompt_ene_s1_hist->Write();
  nv_neutron_prompt_gps_hist->Write();
  nv_neutron_prompt_ene_hist->Write();
  nv_neutron_prompt_small_ene_hist->Write();
  nv_neutron_prompt_gps_ene_hist->Write();
  nv_neutron_prompt_ntuple->Write();
  nv_neutron_delay_ene_s1_hist->Write();
  nv_neutron_delay_gps_hist->Write();
  nv_neutron_delay_ene_hist->Write();
  nv_neutron_delay_small_ene_hist->Write();
  nv_neutron_delay_gps_ene_hist->Write();
  nv_neutron_delay_ntuple->Write();
  nv_neutron_after_ene_s1_hist->Write();
  nv_neutron_after_gps_hist->Write();
  nv_neutron_after_ene_hist->Write();
  nv_neutron_after_small_ene_hist->Write();
  nv_neutron_after_gps_ene_hist->Write();
  nv_neutron_after_ntuple->Write();
  nv_neutron_first_ene_s1_hist->Write();
  nv_neutron_first_gps_hist->Write();
  nv_neutron_first_ene_hist->Write();
  nv_neutron_first_small_ene_hist->Write();
  nv_neutron_first_gps_ene_hist->Write();
  nv_neutron_first_ntuple->Write();
  nv_neutron_late_ene_s1_hist->Write();
  nv_neutron_late_gps_hist->Write();
  nv_neutron_late_ene_hist->Write();
  nv_neutron_late_small_ene_hist->Write();
  nv_neutron_late_gps_ene_hist->Write();
  nv_neutron_late_ntuple->Write();
  */
  //********Save Fail OD clusters************************
  nv_fail_gps_hist->Write();
  nv_fail_ene_hist->Write();
  nv_fail_small_ene_hist->Write();
  nv_fail_gps_ene_hist->Write();
  //  nv_fail_ene_s1_hist->Write();
  nv_fail_ntuple->Write();  
  /*  nv_fail_gamma_coincidence_gps_hist->Write();
  nv_fail_gamma_coincidence_ene_hist->Write();
  nv_fail_gamma_coincidence_small_ene_hist->Write();
  nv_fail_gamma_coincidence_gps_ene_hist->Write();
  nv_fail_gamma_coincidence_ene_s1_hist->Write();
  nv_fail_gamma_coincidence_ntuple->Write();  
  nv_fail_neutron_coincidence_gps_hist->Write();
  nv_fail_neutron_coincidence_ene_hist->Write();
  nv_fail_neutron_coincidence_small_ene_hist->Write();
  nv_fail_neutron_coincidence_gps_ene_hist->Write();
  nv_fail_neutron_coincidence_ene_s1_hist->Write();
  nv_fail_neutron_coincidence_ntuple->Write();  
  */
  //**********Save Canvas**************************
  nv_gamma_delay_small_ene_subtracted_hist->Write();
  nv_gamma_after_small_ene_subtracted_hist->Write();
  nv_gamma_coincidence_small_ene_subtracted_hist->Write();
  nv_gamma_first_small_ene_subtracted_hist->Write();
  nv_gamma_prompt_small_ene_subtracted_hist->Write();
  nv_gamma_window1_first_small_ene_subtracted_hist->Write();
  nv_gamma_window2_first_small_ene_subtracted_hist->Write();

  /*  nv_neutron_delay_small_ene_subtracted_hist->Write();
  nv_neutron_after_small_ene_subtracted_hist->Write();
  nv_neutron_coincidence_small_ene_subtracted_hist->Write();
  nv_neutron_first_small_ene_subtracted_hist->Write();
  nv_neutron_prompt_small_ene_subtracted_hist->Write();
  */
  nv_charge_height_multiplicity_hist->Write();
  nv_all_charge_height_multiplicity_hist->Write();
  nv_mult_height_multiplicity_hist->Write();
  nv_all_mult_height_multiplicity_hist->Write();
  //  mult_hist->Write();

  //  c1->Write();
  c2->Write();
  /* c3->Write();
  c4->Write();
  c5->Write();
  c6->Write();
  c7->Write();
  c8->Write();
  c9->Write();
  */ 
  c10->Write();

  c11->Write();
  c12->Write();
  c13->Write();
  c14->Write();
  c15->Write();
  c16->Write();


  c21->Write();
  c22->Write();
  c23->Write();
  c24->Write();
  c25->Write();
  c26->Write();
  c27->Write();
  c28->Write();
  //  c29->Write();
  /*
  c31->Write();
  c32->Write();
  c33->Write();
  c34->Write();
  c35->Write();

  c41->Write();
  c42->Write();
  c43->Write();
  c44->Write();
  c45->Write();
  c46->Write();
  //  c47->Write();
  //  c48->Write();
  //  c49->Write();
  */
  c51->Write();
  c52->Write();
  c53->Write();
  c54->Write();
  c55->Write();

  f2D.Write();
  f2D.Close();

  cout<<"===>> Application Finished"<<endl;
  return 1;
}



  /*  TH2F* nv_gamma_alpha_ene_s1_hist = new TH2F("nv_gamma_alpha_ene_s1_hist","Veto Gamma Alpha Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_alpha_gps_hist = new TH1F("nv_gamma_alpha_gps_hist","Veto Gamma Alpha Time;Time [ns]",gamma_bins,alpha_time,delay_time);
  TH1F* nv_gamma_alpha_ene_hist = new TH1F("nv_gamma_alpha_ene_hist","Veto Gamma Alpha Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_alpha_small_ene_hist = new TH1F("nv_gamma_alpha_small_ene_hist","Veto Gamma Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_gamma_alpha_gps_ene_hist = new TH2F("nv_gamma_alpha_gps_ene_hist","Veto Gamma Alpha Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,alpha_time,delay_time,gamma_bins,0,gamma_coin_ene);
  TNtuple* nv_gamma_alpha_ntuple = new TNtuple("nv_gamma_alpha_ntuple","Veto Gamma Alpha Data","total_s1:time:ene");

  TH2F* nv_gamma_alpha_late_ene_s1_hist = new TH2F("nv_gamma_alpha_late_ene_s1_hist","Veto Gamma Alpha Late Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_alpha_late_gps_hist = new TH1F("nv_gamma_alpha_late_gps_hist","Veto Gamma Alpha Late Time;Time [ns]",gamma_bins,late_time,end_time);
  TH1F* nv_gamma_alpha_late_ene_hist = new TH1F("nv_gamma_alpha_late_ene_hist","Veto Gamma Alpha Late Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_alpha_late_small_ene_hist = new TH1F("nv_gamma_alpha_late_small_ene_hist","Veto Gamma Alpha Late Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_gamma_alpha_late_gps_ene_hist = new TH2F("nv_gamma_alpha_late_gps_ene_hist","Veto Gamma Alpha Late Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,late_time,end_time,gamma_bins,0,gamma_coin_ene);
  TNtuple* nv_gamma_alpha_late_ntuple = new TNtuple("nv_gamma_alpha_late_ntuple","Veto Gamma Alpha Late Data","total_s1:time:ene");
  
  TH2F* nv_gamma_alpha_delay_ene_s1_hist = new TH2F("nv_gamma_alpha_delay_ene_s1_hist","Veto Gamma Alpha Delay Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_gamma_alpha_delay_gps_hist = new TH1F("nv_gamma_alpha_delay_gps_hist","Veto Gamma Alpha Delay Time;Time [ns]",gamma_bins,alpha_delay_time,delay_time);
  TH1F* nv_gamma_alpha_delay_ene_hist = new TH1F("nv_gamma_alpha_delay_ene_hist","Veto Gamma Alpha Delay Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_gamma_alpha_delay_small_ene_hist = new TH1F("nv_gamma_alpha_delay_small_ene_hist","Veto Gamma Alpha Delay Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_gamma_alpha_delay_gps_ene_hist = new TH2F("nv_gamma_alpha_delay_gps_ene_hist","Veto Gamma Alpha Delay Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,alpha_delay_time,delay_time,gamma_bins,0,gamma_coin_ene);
  TNtuple* nv_gamma_alpha_delay_ntuple = new TNtuple("nv_gamma_alpha_delay_ntuple","Veto Gamma Alpha Delay Data","total_s1:time:ene");

  */		    // lsv_gamma_gps.push_back(gps_gamma_diff_temp);		  
		  //  lsv_gamma_ene.push_back(od_cluster_charge->at(ncluster));
		  //	  lsv_gamma_s1.push_back(tpc_total_s1);
		  // if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<delay_time)
		  //  if(lsv_gamma_gps<gamma_gps_cut && od_cluster_charge->at(ncluster)<gamma_ene_cut)

		  /*		  if(lsv_gamma_gps.at(0)>gamma_gps_cut)
		    {
		      if(gps_gamma_diff_temp>alpha_time && gps_gamma_diff_temp<delay_time)
			{
			  nv_gamma_alpha_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_gamma_alpha_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_alpha_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));	    
			}
		      if(gps_gamma_diff_temp>alpha_delay_time && gps_gamma_diff_temp<delay_time)
			{
			  lsv_gamma_alpha_ene.push_back(od_cluster_charge->at(ncluster));
			  lsv_gamma_alpha_gps.push_back(gps_gamma_diff_temp);
			  lsv_gamma_alpha_s1.push_back(tpc_total_s1); 
			 
			  nv_gamma_alpha_delay_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_gamma_alpha_delay_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_alpha_delay_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_delay_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_delay_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_delay_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));	    
			}
		      if(gps_gamma_diff_temp>late_time && gps_gamma_diff_temp<end_time)
			{
			  nv_gamma_alpha_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_gamma_alpha_late_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_alpha_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_late_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_alpha_late_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));	    
			}

		      
#ifndef after_pulse		      
		    }
#endif
		  */
		  



  /*  TH2F* nv_neutron_alpha_ene_s1_hist = new TH2F("nv_neutron_alpha_ene_s1_hist","Veto Neutron Alpha Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,40000);  
  TH1F* nv_neutron_alpha_gps_hist = new TH1F("nv_neutron_alpha_gps_hist","Veto Neutron Alpha Time;Time [ns]",neutron_bins,alpha_time,delay_time);
  TH1F* nv_neutron_alpha_ene_hist = new TH1F("nv_neutron_alpha_ene_hist","Veto Neutron Alpha Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_alpha_small_ene_hist = new TH1F("nv_neutron_alpha_small_ene_hist","Veto Neutron Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_neutron_alpha_gps_ene_hist = new TH2F("nv_neutron_alpha_gps_ene_hist","Veto Neutron Alpha Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,alpha_time,delay_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_alpha_ntuple = new TNtuple("nv_neutron_alpha_ntuple","Veto Neutron Alpha Data","total_s1:time:ene");

  TH2F* nv_neutron_alpha_late_ene_s1_hist = new TH2F("nv_neutron_alpha_late_ene_s1_hist","Veto Neutron Alpha Late Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,40000);  
  TH1F* nv_neutron_alpha_late_gps_hist = new TH1F("nv_neutron_alpha_late_gps_hist","Veto Neutron Alpha Late Time;Time [ns]",neutron_bins,late_time,end_time);
  TH1F* nv_neutron_alpha_late_ene_hist = new TH1F("nv_neutron_alpha_late_ene_hist","Veto Neutron Alpha Late Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_alpha_late_small_ene_hist = new TH1F("nv_neutron_alpha_late_small_ene_hist","Veto Neutron Alpha Late Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_neutron_alpha_late_gps_ene_hist = new TH2F("nv_neutron_alpha_late_gps_ene_hist","Veto Neutron Alpha Late Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,late_time,end_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_alpha_late_ntuple = new TNtuple("nv_neutron_alpha_late_ntuple","Veto Neutron Alpha Late Data","total_s1:time:ene");

  TH2F* nv_neutron_alpha_delay_ene_s1_hist = new TH2F("nv_neutron_alpha_delay_ene_s1_hist","Veto Neutron Alpha Delay Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,40000);  
  TH1F* nv_neutron_alpha_delay_gps_hist = new TH1F("nv_neutron_alpha_delay_gps_hist","Veto Neutron Alpha Delay Time;Time [ns]",neutron_bins,alpha_delay_time,delay_time);
  TH1F* nv_neutron_alpha_delay_ene_hist = new TH1F("nv_neutron_alpha_delay_ene_hist","Veto Neutron Alpha Delay Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_alpha_delay_small_ene_hist = new TH1F("nv_neutron_alpha_delay_small_ene_hist","Veto Neutron Alpha Delay Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_neutron_alpha_delay_gps_ene_hist = new TH2F("nv_neutron_alpha_delay_gps_ene_hist","Veto Neutron Alpha Delay Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,alpha_delay_time,delay_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_alpha_delay_ntuple = new TNtuple("nv_neutron_alpha_delay_ntuple","Veto Neutron Alpha Delay Data","total_s1:time:ene");
  */
		  /*	  if(lsv_neutron_gps.at(0)>gamma_gps_cut)
		    {
		      if(gps_neutron_diff_temp>alpha_time && gps_neutron_diff_temp<delay_time)
			{
			  nv_neutron_alpha_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_neutron_alpha_gps_hist->Fill(gps_neutron_diff_temp);
			  nv_neutron_alpha_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
			}
		      if(gps_neutron_diff_temp>alpha_delay_time && gps_neutron_diff_temp<delay_time)
			{
			  lsv_neutron_alpha_ene.push_back(od_cluster_charge->at(ncluster));
			  lsv_neutron_alpha_gps.push_back(gps_gamma_diff_temp);
			  lsv_neutron_alpha_s1.push_back(tpc_total_s1); 
			 
			  nv_neutron_alpha_delay_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_neutron_alpha_delay_gps_hist->Fill(gps_neutron_diff_temp);
			  nv_neutron_alpha_delay_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_delay_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_delay_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_delay_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));  
			}
		      if(gps_neutron_diff_temp>late_time && gps_neutron_diff_temp<end_time)
			{
			  nv_neutron_alpha_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_neutron_alpha_late_gps_hist->Fill(gps_neutron_diff_temp);
			  nv_neutron_alpha_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_late_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
			  nv_neutron_alpha_late_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));   
			}
		
#ifndef after_pulse		      
		    }
#endif		  
      
		  */

  /*  TH1F* nv_gamma_alpha_ene_subtracted_hist = new TH1F("nv_gamma_alpha_ene_subtracted_hist","Veto Gamma Subtracted Alpha Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  nv_gamma_alpha_ene_subtracted_hist->Add(nv_gamma_alpha_ene_hist,nv_gamma_late_ene_hist,1,-1);
  TCanvas *c4 = new TCanvas("c4","Veto Gamma Alpha Energy Subtracting BG",1000,400);
  c4->Divide(2,1);
  c4->cd(1);
  //  gPad->SetLogy();
  nv_gamma_alpha_ene_hist->Draw();
  nv_gamma_late_ene_hist->Draw("same");
  c4->cd(2);
  //  gPad->SetLogy();
  //  nv_gamma_alpha_ene_hist->Add(nv_gamma_late_ene_hist,-1);
  nv_gamma_alpha_ene_subtracted_hist->Draw();
  //c4->SaveAs(Form("%snv_gamma_alpha_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_alpha_ene_subtracted_hist = new TH1F("nv_neutron_alpha_ene_subtracted_hist","Veto Neutron Subtracted Alpha Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  nv_neutron_alpha_ene_subtracted_hist->Add(nv_neutron_alpha_ene_hist,nv_neutron_late_ene_hist,1,-1);
  TCanvas *c5 = new TCanvas("c5","Veto Neutron Alpha Energy Subtracting BG",1000,400);
  c5->Divide(2,1);
  c5->cd(1);
  //  gPad->SetLogy();
  nv_neutron_alpha_ene_hist->Draw();
  nv_neutron_late_ene_hist->Draw("same");
  c5->cd(2);
  //  gPad->SetLogy();
  //  nv_neutron_alpha_ene_hist->Add(nv_neutron_late_ene_hist,-1);
  nv_neutron_alpha_ene_subtracted_hist->Draw();
  //c5->SaveAs(Form("%snv_neutron_alpha_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_gamma_alpha_small_ene_subtracted_hist = new TH1F("nv_gamma_alpha_small_ene_subtracted_hist","Veto Gamma Subtracted Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_alpha_small_ene_subtracted_hist->Add(nv_gamma_alpha_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
  TCanvas *c6 = new TCanvas("c6","Veto Gamma Alpha Small Energy Subtracting BG",1000,400);
  c6->Divide(2,1);
  c6->cd(1);
  //  gPad->SetLogy();
  nv_gamma_alpha_small_ene_hist->Draw();
  nv_gamma_late_small_ene_hist->Draw("same");
  c6->cd(2);
  //  gPad->SetLogy();
  //  nv_gamma_alpha_small_ene_hist->Add(nv_gamma_late_ene_hist,-1);
  nv_gamma_alpha_small_ene_subtracted_hist->Draw();
  //c6->SaveAs(Form("%snv_gamma_alpha_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_alpha_small_ene_subtracted_hist = new TH1F("nv_neutron_alpha_small_ene_subtracted_hist","Veto Neutron Subtracted Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_alpha_small_ene_subtracted_hist->Add(nv_neutron_alpha_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c7 = new TCanvas("c7","Veto Neutron Alpha Small Energy Subtracting BG",1000,400);
  c7->Divide(2,1);
  c7->cd(1);
  //  gPad->SetLogy();
  nv_neutron_alpha_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c7->cd(2);
  //  gPad->SetLogy();
  //  nv_neutron_alpha_small_ene_hist->Add(nv_neutron_late_ene_hist,-1);
  nv_neutron_alpha_small_ene_subtracted_hist->Draw();
  //c7->SaveAs(Form("%snv_neutron_alpha_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_gamma_alpha_delay_small_ene_subtracted_hist = new TH1F("nv_gamma_alpha_delay_small_ene_subtracted_hist","Veto Gamma Subtracted Alpha Delay Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_alpha_delay_small_ene_subtracted_hist->Add(nv_gamma_alpha_delay_small_ene_hist,nv_gamma_alpha_late_small_ene_hist,1,-1);
  TCanvas *c8 = new TCanvas("c8","Veto Gamma Alpha Delay Small Energy Subtracting BG",1000,400);
  c8->Divide(2,1);
  c8->cd(1);
  //  gPad->SetLogy();
  nv_gamma_alpha_delay_small_ene_hist->Draw();
  nv_gamma_alpha_late_small_ene_hist->Draw("same");
  c8->cd(2);
  //  gPad->SetLogy();
  //  nv_gamma_alpha_delay_small_ene_hist->Add(nv_gamma_late_ene_hist,-1);
  nv_gamma_alpha_delay_small_ene_subtracted_hist->Draw();
  //c8->SaveAs(Form("%snv_gamma_alpha_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_alpha_delay_small_ene_subtracted_hist = new TH1F("nv_neutron_alpha_delay_small_ene_subtracted_hist","Veto Neutron Subtracted Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_alpha_delay_small_ene_subtracted_hist->Add(nv_neutron_alpha_delay_small_ene_hist,nv_neutron_alpha_late_small_ene_hist,1,-1);
  TCanvas *c9 = new TCanvas("c9","Veto Neutron Alpha Delay Small Energy Subtracting BG",1000,400);
  c9->Divide(2,1);
  c9->cd(1);
  //  gPad->SetLogy();
  nv_neutron_alpha_delay_small_ene_hist->Draw();
  nv_neutron_alpha_late_small_ene_hist->Draw("same");
  c9->cd(2);
  //  gPad->SetLogy();
  //  nv_neutron_alpha_delay_small_ene_hist->Add(nv_neutron_late_ene_hist,-1);
  nv_neutron_alpha_delay_small_ene_subtracted_hist->Draw();
  //c9->SaveAs(Form("%snv_neutron_alpha_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  //  nv_gamma_alpha_late_small_ene_hist->SetLineColor(linecolor.at(3));
  //  nv_neutron_alpha_late_small_ene_hist->SetLineColor(linecolor.at(4));

  nv_gamma_alpha_ene_subtracted_hist->Write();
  nv_neutron_alpha_ene_subtracted_hist->Write();
  nv_gamma_alpha_small_ene_subtracted_hist->Write();
  nv_neutron_alpha_small_ene_subtracted_hist->Write();
  nv_gamma_alpha_delay_small_ene_subtracted_hist->Write();
  nv_neutron_alpha_delay_small_ene_subtracted_hist->Write();
  

  */  

  /*  TCanvas *c27 = new TCanvas("c27","Veto Gamma Alpha",1000,600);
  c27->Divide(2,2);
  c27->cd(1);
  gPad->SetLogy();
  nv_gamma_alpha_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_alpha_gps_hist->Draw();
  c27->cd(2);
  gPad->SetLogy();
  nv_gamma_alpha_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_alpha_ene_hist->Draw();
  c27->cd(3);
  gPad->SetLogy();
  //  nv_gamma_alpha_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_alpha_gps_ene_hist->Draw("colz");
  c27->cd(4);
  gPad->SetLogy();
  //  nv_gamma_alpha_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_gamma_alpha_ene_s1_hist->Draw("colz");
  //  c27->SaveAs(Form("%snv_gamma_alpha%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c28 = new TCanvas("c28","Veto Gamma Alpha Delay",1000,600);
  c28->Divide(2,2);
  c28->cd(1);
  gPad->SetLogy();
  nv_gamma_alpha_delay_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_alpha_delay_gps_hist->Draw();
  c28->cd(2);
  gPad->SetLogy();
  nv_gamma_alpha_delay_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_alpha_delay_ene_hist->Draw();
  c28->cd(3);
  gPad->SetLogy();
  //  nv_gamma_alpha_delay_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_alpha_delay_gps_ene_hist->Draw("colz");
  c28->cd(4);
  gPad->SetLogy();
  //  nv_gamma_alpha_delay_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_gamma_alpha_delay_ene_s1_hist->Draw("colz");
  //  c28->SaveAs(Form("%snv_gamma_alpha_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c29 = new TCanvas("c29","Veto Gamma Alpha Late",1000,600);
  c29->Divide(2,2);
  c29->cd(1);
  gPad->SetLogy();
  nv_gamma_alpha_late_gps_hist->SetLineColor(linecolor.at(5));
  nv_gamma_alpha_late_gps_hist->Draw();
  c29->cd(2);
  gPad->SetLogy();
  nv_gamma_alpha_late_ene_hist->SetLineColor(linecolor.at(6));
  nv_gamma_alpha_late_ene_hist->Draw();
  c29->cd(3);
  gPad->SetLogy();
  //  nv_gamma_alpha_late_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_gamma_alpha_late_gps_ene_hist->Draw("colz");
  c29->cd(4);
  gPad->SetLogy();
  //  nv_gamma_alpha_late_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_gamma_alpha_late_ene_s1_hist->Draw("colz");
  //  c29->SaveAs(Form("%snv_gamma_alpha_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */

  /*  TCanvas *c47 = new TCanvas("c47","Veto Neutron Alpha",1000,600);
  c47->Divide(2,2);
  c47->cd(1);
  gPad->SetLogy();
  nv_neutron_alpha_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_alpha_gps_hist->Draw();
  c47->cd(2);
  gPad->SetLogy();
  nv_neutron_alpha_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_alpha_ene_hist->Draw();
  c47->cd(3);
  gPad->SetLogy();
  //  nv_neutron_alpha_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_alpha_gps_ene_hist->Draw("colz");
  c47->cd(4);
  gPad->SetLogy();
  //  nv_neutron_alpha_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_alpha_ene_s1_hist->Draw("colz");
  //  c47->SaveAs(Form("%snv_neutron_alpha%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c48 = new TCanvas("c48","Veto Neutron Alpha Delay",1000,600);
  c48->Divide(2,2);
  c48->cd(1);
  gPad->SetLogy();
  nv_neutron_alpha_delay_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_alpha_delay_gps_hist->Draw();
  c48->cd(2);
  gPad->SetLogy();
  nv_neutron_alpha_delay_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_alpha_delay_ene_hist->Draw();
  c48->cd(3);
  gPad->SetLogy();
  //  nv_neutron_alpha_delay_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_alpha_delay_gps_ene_hist->Draw("colz");
  c48->cd(4);
  gPad->SetLogy();
  //  nv_neutron_alpha_delay_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_alpha_delay_ene_s1_hist->Draw("colz");
  //  c48->SaveAs(Form("%snv_neutron_alpha_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c49 = new TCanvas("c49","Veto Neutron Alpha Late",1000,600);
  c49->Divide(2,2);
  c49->cd(1);
  gPad->SetLogy();
  nv_neutron_alpha_late_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_alpha_late_gps_hist->Draw();
  c49->cd(2);
  gPad->SetLogy();
  nv_neutron_alpha_late_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_alpha_late_ene_hist->Draw();
  c49->cd(3);
  gPad->SetLogy();
  //  nv_neutron_alpha_late_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_alpha_late_gps_ene_hist->Draw("colz");
  c49->cd(4);
  gPad->SetLogy();
  //  nv_neutron_alpha_late_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_alpha_late_ene_s1_hist->Draw("colz");
  //  c49->SaveAs(Form("%snv_neutron_alpha_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */

  /*  TH1F* nv_neutron_alpha_ene_subtracted_hist = new TH1F("nv_neutron_alpha_ene_subtracted_hist","Veto Neutron Subtracted Alpha Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  nv_neutron_alpha_ene_subtracted_hist->Add(nv_neutron_alpha_ene_hist,nv_neutron_late_ene_hist,1,-1);
  TCanvas *c5 = new TCanvas("c5","Veto Neutron Alpha Energy Subtracting BG",1000,400);
  c5->Divide(2,1);
  c5->cd(1);
  //  gPad->SetLogy();
  nv_neutron_alpha_ene_hist->Draw();
  nv_neutron_late_ene_hist->Draw("same");
  c5->cd(2);
  //  gPad->SetLogy();
  //  nv_neutron_alpha_ene_hist->Add(nv_neutron_late_ene_hist,-1);
  nv_neutron_alpha_ene_subtracted_hist->Draw();
  //c5->SaveAs(Form("%snv_neutron_alpha_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
 
  TH1F* nv_neutron_alpha_small_ene_subtracted_hist = new TH1F("nv_neutron_alpha_small_ene_subtracted_hist","Veto Neutron Subtracted Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_alpha_small_ene_subtracted_hist->Add(nv_neutron_alpha_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c7 = new TCanvas("c7","Veto Neutron Alpha Small Energy Subtracting BG",1000,400);
  c7->Divide(2,1);
  c7->cd(1);
  //  gPad->SetLogy();
  nv_neutron_alpha_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c7->cd(2);
  //  gPad->SetLogy();
  //  nv_neutron_alpha_small_ene_hist->Add(nv_neutron_late_ene_hist,-1);
  nv_neutron_alpha_small_ene_subtracted_hist->Draw();
  //c7->SaveAs(Form("%snv_neutron_alpha_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_alpha_delay_small_ene_subtracted_hist = new TH1F("nv_neutron_alpha_delay_small_ene_subtracted_hist","Veto Neutron Subtracted Alpha Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_alpha_delay_small_ene_subtracted_hist->Add(nv_neutron_alpha_delay_small_ene_hist,nv_neutron_alpha_late_small_ene_hist,1,-1);
  TCanvas *c9 = new TCanvas("c9","Veto Neutron Alpha Delay Small Energy Subtracting BG",1000,400);
  c9->Divide(2,1);
  c9->cd(1);
  //  gPad->SetLogy();
  nv_neutron_alpha_delay_small_ene_hist->Draw();
  nv_neutron_alpha_late_small_ene_hist->Draw("same");
  c9->cd(2);
  //  gPad->SetLogy();
  //  nv_neutron_alpha_delay_small_ene_hist->Add(nv_neutron_late_ene_hist,-1);
  nv_neutron_alpha_delay_small_ene_subtracted_hist->Draw();
  //c9->SaveAs(Form("%snv_neutron_alpha_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */



  /*  nv_gamma_alpha_gps_hist->Write();
  nv_gamma_alpha_ene_hist->Write();
  nv_gamma_alpha_small_ene_hist->Write();
  nv_gamma_alpha_gps_ene_hist->Write();
  nv_gamma_alpha_ene_s1_hist->Write();
  nv_gamma_alpha_ntuple->Write();
  nv_gamma_alpha_delay_gps_hist->Write();
  nv_gamma_alpha_delay_ene_hist->Write();
  nv_gamma_alpha_delay_small_ene_hist->Write();
  nv_gamma_alpha_delay_gps_ene_hist->Write();
  nv_gamma_alpha_delay_ene_s1_hist->Write();
  nv_gamma_alpha_delay_ntuple->Write();
  nv_gamma_alpha_late_gps_hist->Write();
  nv_gamma_alpha_late_ene_hist->Write();
  nv_gamma_alpha_late_small_ene_hist->Write();
  nv_gamma_alpha_late_gps_ene_hist->Write();
  nv_gamma_alpha_late_ene_s1_hist->Write();
  nv_gamma_alpha_late_ntuple->Write();
  */
  /*  nv_neutron_alpha_gps_hist->Write();
  nv_neutron_alpha_ene_hist->Write();
  nv_neutron_alpha_small_ene_hist->Write();
  nv_neutron_alpha_gps_ene_hist->Write();
  nv_neutron_alpha_ene_s1_hist->Write();
  nv_neutron_alpha_ntuple->Write();
  nv_neutron_alpha_delay_gps_hist->Write();
  nv_neutron_alpha_delay_ene_hist->Write();
  nv_neutron_alpha_delay_small_ene_hist->Write();
  nv_neutron_alpha_delay_gps_ene_hist->Write();
  nv_neutron_alpha_delay_ene_s1_hist->Write();
  nv_neutron_alpha_delay_ntuple->Write();
  nv_neutron_alpha_late_gps_hist->Write();
  nv_neutron_alpha_late_ene_hist->Write();
  nv_neutron_alpha_late_small_ene_hist->Write();
  nv_neutron_alpha_late_gps_ene_hist->Write();
  nv_neutron_alpha_late_ene_s1_hist->Write();
  nv_neutron_alpha_late_ntuple->Write();
  */
  /*  TH2F* nv_all_gamma_coincidence_ene_s1_hist = new TH2F("nv_all_gamma_coincidence_ene_s1_hist","All Clusters Veto Gamma Coincidence Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,gamma_coin_ene,200,0,40000);  
  TH1F* nv_all_gamma_coincidence_gps_hist = new TH1F("nv_all_gamma_coincidence_gps_hist","All Clusters Veto Gamma Coincidence Time;Time [ns]",gamma_bins,middle_time,end_time);
  TH1F* nv_all_gamma_coincidence_ene_hist = new TH1F("nv_all_gamma_coincidence_ene_hist","All Clusters Veto Gamma Coincidence Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_all_gamma_coincidence_small_ene_hist = new TH1F("nv_all_gamma_coincidence_small_ene_hist","All Clusters Veto Gamma Coincidence Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_all_gamma_coincidence_gps_ene_hist = new TH2F("nv_all_gamma_coincidence_gps_ene_hist","All Clusters Veto Gamma Coincidence Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,middle_time,end_time,gamma_bins,0,gamma_coin_ene);
  TNtuple* nv_all_gamma_coincidence_ntuple = new TNtuple("nv_all_gamma_coincidence_ntuple","All Clusters Veto Gamma Coincidence Data","total_s1:time:ene");

  TH2F* nv_all_neutron_coincidence_ene_s1_hist = new TH2F("nv_all_neutron_coincidence_ene_s1_hist","All Clusters Veto Neutron Coincidence Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,40000);  
  TH1F* nv_all_neutron_coincidence_gps_hist = new TH1F("nv_all_neutron_coincidence_gps_hist","All Clusters Veto Neutron Coincidence Time;Time [ns]",neutron_bins,middle_time,end_time);
  TH1F* nv_all_neutron_coincidence_ene_hist = new TH1F("nv_all_neutron_coincidence_ene_hist","All Clusters Veto Neutron Coincidence Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_all_neutron_coincidence_small_ene_hist = new TH1F("nv_all_neutron_coincidence_small_ene_hist","All Clusters Veto Neutron Coincidence Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_all_neutron_coincidence_gps_ene_hist = new TH2F("nv_all_neutron_coincidence_gps_ene_hist","All Clusters Veto Neutron Coincidence Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,middle_time,end_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_all_neutron_coincidence_ntuple = new TNtuple("nv_all_neutron_coincidence_ntuple","All Clusters Veto Neutron Coincidence Data","total_s1:time:ene");
  */
  /*
  TH2F* nv_fail_gamma_coincidence_ene_s1_hist = new TH2F("nv_fail_gamma_coincidence_ene_s1_hist","Fail Multicut Veto Gamma Coincidence Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",gamma_bins,0,fail_ene,200,0,40000);  
  TH1F* nv_fail_gamma_coincidence_gps_hist = new TH1F("nv_fail_gamma_coincidence_gps_hist","Fail Multicut Veto Gamma Coincidence Time;Time [ns]",gamma_bins,prompt_time,delay_time);
  TH1F* nv_fail_gamma_coincidence_ene_hist = new TH1F("nv_fail_gamma_coincidence_ene_hist","Fail Multicut Veto Gamma Coincidence Energy;Energy [PE]",gamma_bins,0,fail_ene);
  TH1F* nv_fail_gamma_coincidence_small_ene_hist = new TH1F("nv_fail_gamma_coincidence_small_ene_hist","Fail Multicut Veto Gamma Coincidence Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_fail_gamma_coincidence_gps_ene_hist = new TH2F("nv_fail_gamma_coincidence_gps_ene_hist","Fail Multicut Veto Gamma Coincidence Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,prompt_time,delay_time,gamma_bins,0,fail_ene);
  TNtuple* nv_fail_gamma_coincidence_ntuple = new TNtuple("nv_fail_gamma_coincidence_ntuple","Fail Multicut Veto Gamma Coincidence Data","total_s1:time:ene");

  TH2F* nv_fail_neutron_coincidence_ene_s1_hist = new TH2F("nv_fail_neutron_coincidence_ene_s1_hist","Fail Multicut Veto Neutron Coincidence Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,fail_ene,200,0,40000);  
  TH1F* nv_fail_neutron_coincidence_gps_hist = new TH1F("nv_fail_neutron_coincidence_gps_hist","Fail Multicut Veto Neutron Coincidence Time;Time [ns]",neutron_bins,prompt_time,delay_time);
  TH1F* nv_fail_neutron_coincidence_ene_hist = new TH1F("nv_fail_neutron_coincidence_ene_hist","Fail Multicut Veto Neutron Coincidence Energy;Energy [PE]",neutron_bins,0,fail_ene);
  TH1F* nv_fail_neutron_coincidence_small_ene_hist = new TH1F("nv_fail_neutron_coincidence_small_ene_hist","Fail Multicut Veto Neutron_Coincidence Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_fail_neutron_coincidence_gps_ene_hist = new TH2F("nv_fail_neutron_coincidence_gps_ene_hist","Fail Multicut Veto Neutron Coincidence Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,prompt_time,delay_time,neutron_bins,0,fail_ene);
  TNtuple* nv_fail_neutron_coincidence_ntuple = new TNtuple("nv_fail_neutron_coincidence_ntuple","Fail Multicut Veto Neutron Coincidence Data","total_s1:time:ene");
  */
  //=====================Define Neutron coincidence Events in Veto==============================================
  /*
  TH1F* nv_neutron_full_ene_hist = new TH1F("nv_neutron_full_ene_hist","Veto Neutron Full Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  TH1F* nv_neutron_full_small_ene_hist = new TH1F("nv_neutron_full_small_ene_hist","Veto Neutron Full Energy(small range);Energy [PE]",fail_bins,0,fail_ene);

  TH2F* nv_neutron_ene_s1_hist = new TH2F("nv_neutron_ene_s1_hist","Veto Neutron Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  

  TH2F* nv_neutron_coincidence_ene_s1_hist = new TH2F("nv_neutron_coincidence_ene_s1_hist","Veto Neutron Coincidence Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  
  TH1F* nv_neutron_coincidence_gps_hist = new TH1F("nv_neutron_coincidence_gps_hist","Veto Neutron Coincidence Time;Time [ns]",neutron_bins,prompt_time,delay_time);
  TH1F* nv_neutron_coincidence_ene_hist = new TH1F("nv_neutron_coincidence_ene_hist","Veto Neutron Coincidence Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_coincidence_small_ene_hist = new TH1F("nv_neutron_coincidence_small_ene_hist","Veto Neutron Coincidence Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_neutron_coincidence_gps_ene_hist = new TH2F("nv_neutron_coincidence_gps_ene_hist","Veto Neutron Coincidence Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,prompt_time,delay_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_coincidence_ntuple = new TNtuple("nv_neutron_coincidence_ntuple","Veto Neutron Coincidence Data","total_s1:time:ene");  

  TH2F* nv_neutron_prompt_ene_s1_hist = new TH2F("nv_neutron_prompt_ene_s1_hist","Veto Neutron Prompt Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  
  TH1F* nv_neutron_prompt_gps_hist = new TH1F("nv_neutron_prompt_gps_hist","Veto Neutron Prompt Time;Time [ns]",100,prompt_time,middle_time);
  TH1F* nv_neutron_prompt_ene_hist = new TH1F("nv_neutron_prompt_ene_hist","Veto Neutron Prompt Energy;Energy [PE]",100,0,neutron_coin_ene);
  TH1F* nv_neutron_prompt_small_ene_hist = new TH1F("nv_neutron_prompt_small_ene_hist","Veto Neutron Prompt Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_neutron_prompt_gps_ene_hist = new TH2F("nv_neutron_prompt_gps_ene_hist","Veto Neutron Prompt Time Vs Energy;Time [ns];Energy[PE]",100,prompt_time,middle_time,100,0,neutron_coin_ene);
  TNtuple* nv_neutron_prompt_ntuple = new TNtuple("nv_neutron_prompt_ntuple","Veto Neutron Prompt Data","total_s1:time:ene");
  
  TH2F* nv_neutron_delay_ene_s1_hist = new TH2F("nv_neutron_delay_ene_s1_hist","Veto Neutron Delay Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  
 TH1F* nv_neutron_delay_gps_hist = new TH1F("nv_neutron_delay_gps_hist","Veto Neutron Delay Time;Time [ns]",neutron_bins,middle_time,delay_time);
  TH1F* nv_neutron_delay_ene_hist = new TH1F("nv_neutron_delay_ene_hist","Veto Neutron Delay Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_delay_small_ene_hist = new TH1F("nv_neutron_delay_small_ene_hist","Veto Neutron Delay Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_neutron_delay_gps_ene_hist = new TH2F("nv_neutron_delay_gps_ene_hist","Veto Neutron Delay Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,middle_time,delay_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_delay_ntuple = new TNtuple("nv_neutron_delay_ntuple","Veto Neutron Delay Data","total_s1:time:ene");
  
  TH2F* nv_neutron_after_ene_s1_hist = new TH2F("nv_neutron_after_ene_s1_hist","Veto Neutron After Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  
  TH1F* nv_neutron_after_gps_hist = new TH1F("nv_neutron_after_gps_hist","Veto Neutron After Time;Time [ns]",neutron_bins,after_time,delay_time);
  TH1F* nv_neutron_after_ene_hist = new TH1F("nv_neutron_after_ene_hist","Veto Neutron After Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_after_small_ene_hist = new TH1F("nv_neutron_after_small_ene_hist","Veto Neutron After Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_neutron_after_gps_ene_hist = new TH2F("nv_neutron_after_gps_ene_hist","Veto Neutron After Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,after_time,delay_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_after_ntuple = new TNtuple("nv_neutron_after_ntuple","Veto Neutron After Data","total_s1:time:ene");

  int neutron_first_bins = 200;
  TH2F* nv_neutron_first_ene_s1_hist = new TH2F("nv_neutron_first_ene_s1_hist","Veto Neutron First Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  
  TH1F* nv_neutron_first_gps_hist = new TH1F("nv_neutron_first_gps_hist","Veto Neutron First Time;Time [ns]",neutron_first_bins,middle_time,first_time);
  TH1F* nv_neutron_first_ene_hist = new TH1F("nv_neutron_first_ene_hist","Veto Neutron First Energy;Energy [PE]",neutron_first_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_first_small_ene_hist = new TH1F("nv_neutron_first_small_ene_hist","Veto Neutron first Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  TH2F* nv_neutron_first_gps_ene_hist = new TH2F("nv_neutron_first_gps_ene_hist","Veto Neutron First Time Vs Energy;Time [ns];Energy[PE]",neutron_first_bins,middle_time,first_time,neutron_first_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_first_ntuple = new TNtuple("nv_neutron_first_ntuple","Veto Neutron First Data","total_s1:time:ene");

  TH2F* nv_neutron_late_ene_s1_hist = new TH2F("nv_neutron_late_ene_s1_hist","Veto Neutron Late Charge Vs S1 Total;od_cluster_charge [PE];tpc_total_s1[PE]",neutron_bins,0,neutron_coin_ene,200,0,25000);  
  TH1F* nv_neutron_late_gps_hist = new TH1F("nv_neutron_late_gps_hist","Veto Neutron Late Time;Time [ns]",neutron_bins,late_time,end_time);
  TH1F* nv_neutron_late_ene_hist = new TH1F("nv_neutron_late_ene_hist","Veto Neutron Late Energy;Energy [PE]",neutron_bins,0,neutron_coin_ene);
  TH1F* nv_neutron_late_small_ene_hist = new TH1F("nv_neutron_late_small_ene_hist","Veto Neutron Late Energy(small range);Energy [PE]",fail_bins,0,fail_ene);  
  TH2F* nv_neutron_late_gps_ene_hist = new TH2F("nv_neutron_late_gps_ene_hist","Veto Neutron Late Time Vs Energy;Time [ns];Energy[PE]",neutron_bins,late_time,end_time,neutron_bins,0,neutron_coin_ene);
  TNtuple* nv_neutron_late_ntuple = new TNtuple("nv_neutron_late_ntuple","Veto Neutron Late Data","total_s1:time:ene");
  */

	      /*if(tpc_event_type==0)
		{
		  nv_all_gamma_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		  double gps_all_gamma_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;	 
		  if(gps_all_gamma_diff_temp>prompt_time && gps_all_gamma_diff_temp<delay_time)
		    {
		      nv_all_gamma_coincidence_gps_hist->Fill(gps_all_gamma_diff_temp);
		      nv_all_gamma_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_all_gamma_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_all_gamma_coincidence_gps_ene_hist->Fill(gps_all_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_all_gamma_coincidence_ntuple->Fill(tpc_total_s1,gps_all_gamma_diff_temp,od_cluster_charge->at(ncluster));  
		    }		    
		}
	      if(tpc_event_type==1)
		{
		  double gps_all_neutron_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;      
		  nv_all_neutron_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		  if(gps_all_neutron_diff_temp>prompt_time && gps_all_neutron_diff_temp<delay_time)
		    {
		      nv_all_neutron_coincidence_gps_hist->Fill(gps_all_neutron_diff_temp);
		      nv_all_neutron_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_all_neutron_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_all_neutron_coincidence_gps_ene_hist->Fill(gps_all_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_all_neutron_coincidence_ntuple->Fill(tpc_total_s1,gps_all_neutron_diff_temp,od_cluster_charge->at(ncluster));
		    }		    
		} //end of all clusters
	      */
		  //  nv_fail_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		  /*  if(tpc_event_type==0)
		    {
		      nv_fail_gamma_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      double gps_fail_gamma_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;	    
		      if(gps_fail_gamma_diff_temp>prompt_time && gps_fail_gamma_diff_temp<delay_time)
			{
			  nv_fail_gamma_coincidence_gps_hist->Fill(gps_fail_gamma_diff_temp);
			  nv_fail_gamma_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_fail_gamma_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_fail_gamma_coincidence_gps_ene_hist->Fill(gps_fail_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_fail_gamma_coincidence_ntuple->Fill(tpc_total_s1,gps_fail_gamma_diff_temp,od_cluster_charge->at(ncluster)); 
			}		    
		    }
		  if(tpc_event_type==1)
		    {
		      double gps_fail_neutron_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;    
		      nv_fail_neutron_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      if(gps_fail_neutron_diff_temp>prompt_time && gps_fail_neutron_diff_temp<delay_time)
			{
			  nv_fail_neutron_coincidence_gps_hist->Fill(gps_fail_neutron_diff_temp);
			  nv_fail_neutron_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_fail_neutron_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_fail_neutron_coincidence_gps_ene_hist->Fill(gps_fail_neutron_diff_temp,od_cluster_charge->at(ncluster));
			  nv_fail_neutron_coincidence_ntuple->Fill(tpc_total_s1,gps_fail_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
			}		    
			}*/
		  /*	  if(gps_diff_temp<end_time && gps_diff_temp>late_time)
		    {
		      nv_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_late_gps_hist->Fill(gps_diff_temp);
		      nv_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_late_gps_ene_hist->Fill(gps_diff_temp,od_cluster_charge->at(ncluster));
		      nv_late_ntuple->Fill(tpc_total_s1,gps_diff_temp,od_cluster_charge->at(ncluster));		     
		    }
		  */	      
		  //  if(tpc_event_type==0)
		  //  {
		  //      nv_gamma_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		  /*    }	      
			if(tpc_event_type==1)
		    {
		      nv_neutron_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      double gps_neutron_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;      
		      lsv_neutron_gps.push_back(gps_neutron_diff_temp);
		      nv_neutron_full_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_full_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      //  if(gps_neutron_diff_temp<neutron_gps_cut && od_cluster_charge->at(ncluster)>neutron_od_charge_cut)
		      //	lsv_neutron_tag = true;	  
		      // if(od_cluster_charge->at(ncluster)>200 && od_cluster_charge->at(ncluster)<300)
		      //lsv_neutron_capture = true;
		        if(gps_neutron_diff_temp>middle_time && gps_neutron_diff_temp<end_time)
		    {
		      nv_neutron_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_coincidence_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_coincidence_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_coincidence_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }		    
		  if(gps_neutron_diff_temp>middle_time && gps_neutron_diff_temp<prompt_time)
		    {
		      nv_neutron_prompt_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_prompt_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_prompt_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_prompt_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_prompt_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_prompt_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }
		  if(gps_neutron_diff_temp>middle_time && gps_neutron_diff_temp<delay_time)
		    {
		      nv_neutron_delay_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_delay_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_delay_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_delay_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_delay_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_delay_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }
		  if(gps_neutron_diff_temp>after_time && gps_neutron_diff_temp<delay_time)
		    {
		      nv_neutron_after_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_after_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_after_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_after_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_after_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_after_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }
		  if(gps_neutron_diff_temp>first_time && gps_neutron_diff_temp<delay_time)
		    { ++n;
		      if(n==1)
			{
			  nv_neutron_first_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_neutron_first_gps_hist->Fill(gps_neutron_diff_temp);
			  nv_neutron_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_first_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
			  nv_neutron_first_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	     
			}
		    }
		  if(gps_neutron_diff_temp>late_time && gps_neutron_diff_temp<end_time)
		    {
		      nv_neutron_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_late_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_late_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_late_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));		    
		    }
		      
		    }*/
      /*      if(!lsv_gamma_tag && lsv_gamma_gps.size())	  
	{ int g=0; 
	  for(int ncluster=0; ncluster<od_nclusters; ncluster++)
	    { 	 
	      if(od_cluster_charge->at(ncluster)>0 && od_cluster_pass_multcut->at(ncluster)==1 && tpc_event_type==0)
		{
		  double gps_gamma_diff_temp = od_timestamp - tpc_timestamp + od_cluster_dtprompt->at(ncluster)*1.e+3;	 
		  // if(gps_gamma_diff_temp>prompt_time && gps_gamma_diff_temp<delay_time)
		  if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<end_time)
		    {
		      nv_gamma_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_coincidence_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_coincidence_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_coincidence_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));	    
		    }		    
		  if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<prompt_time)
		    {
		      nv_gamma_prompt_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_prompt_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_prompt_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_prompt_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_prompt_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_prompt_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));		
		    }
		  if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<delay_time)
		    {
		      nv_gamma_delay_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_delay_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_delay_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_delay_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_delay_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_delay_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));		
		    }
		  if(gps_gamma_diff_temp>after_time && gps_gamma_diff_temp<delay_time)
		    {
		      nv_gamma_after_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_after_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_after_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_after_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_after_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_after_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));		     
		    }
		  if(gps_gamma_diff_temp>first_time && gps_gamma_diff_temp<delay_time)
		    { ++g;
		      if(g==1)
			{
			  nv_gamma_first_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_gamma_first_gps_hist->Fill(gps_gamma_diff_temp);
			  nv_gamma_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_gamma_first_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
			  nv_gamma_first_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));		   
			}
		    }
		  if(gps_gamma_diff_temp>late_time && gps_gamma_diff_temp<end_time)
		    {
		      nv_gamma_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_gamma_late_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_late_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_late_ntuple->Fill(tpc_total_s1,gps_gamma_diff_temp,od_cluster_charge->at(ncluster));		     
		    }
		}//end of multicuts
	    } //end of loop
	} //end of tpc gamma coincidence 
      */
      /*      	      
      if(!lsv_neutron_tag && lsv_neutron_gps.size())	  
	{ int n=0;
	  for(int ncluster=0; ncluster<od_nclusters; ncluster++)
	    { 	 
	      if(od_cluster_charge->at(ncluster)>0 && od_cluster_pass_multcut->at(ncluster)==1 && tpc_event_type==1)
		{
		  double gps_neutron_diff_temp = od_timestamp - tpc_timestamp + od_cluster_dtprompt->at(ncluster)*1.e+3;      
		  if(gps_neutron_diff_temp>middle_time && gps_neutron_diff_temp<end_time)
		    {
		      nv_neutron_coincidence_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_coincidence_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_coincidence_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_coincidence_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_coincidence_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_coincidence_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }		    
		  if(gps_neutron_diff_temp>middle_time && gps_neutron_diff_temp<prompt_time)
		    {
		      nv_neutron_prompt_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_prompt_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_prompt_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_prompt_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_prompt_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_prompt_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }
		  if(gps_neutron_diff_temp>middle_time && gps_neutron_diff_temp<delay_time)
		    {
		      nv_neutron_delay_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_delay_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_delay_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_delay_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_delay_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_delay_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }
		  if(gps_neutron_diff_temp>after_time && gps_neutron_diff_temp<delay_time)
		    {
		      nv_neutron_after_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_after_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_after_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_after_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_after_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_after_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	    
		    }
		  if(gps_neutron_diff_temp>first_time && gps_neutron_diff_temp<delay_time)
		    { ++n;
		      if(n==1)
			{
			  nv_neutron_first_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
			  nv_neutron_first_gps_hist->Fill(gps_neutron_diff_temp);
			  nv_neutron_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
			  nv_neutron_first_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
			  nv_neutron_first_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));	     
			}
		    }
		  if(gps_neutron_diff_temp>late_time && gps_neutron_diff_temp<end_time)
		    {
		      nv_neutron_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);
		      nv_neutron_late_gps_hist->Fill(gps_neutron_diff_temp);
		      nv_neutron_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_neutron_late_gps_ene_hist->Fill(gps_neutron_diff_temp,od_cluster_charge->at(ncluster));
		      nv_neutron_late_ntuple->Fill(tpc_total_s1,gps_neutron_diff_temp,od_cluster_charge->at(ncluster));		    
		    }
		} //end of multicuts
	    }//end of loop
	}//end of tpc neutron coincidence
    
	if(lsv_neutron_capture)
	lsv_neutron++;
      */
  /*  TH1F* nv_neutron_delay_small_ene_subtracted_hist = new TH1F("nv_neutron_delay_small_ene_subtracted_hist","Veto Neutron Subtracted Delay Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_delay_small_ene_subtracted_hist->Add(nv_neutron_delay_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c31 = new TCanvas("c31","Veto Neutron Delay Small Energy Subtracting BG",1000,400);
  c31->Divide(2,1);
  c31->cd(1);
  gPad->SetLogy();
  nv_neutron_delay_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c31->cd(2);
  gPad->SetLogy();
  nv_neutron_delay_small_ene_subtracted_hist->Draw();
  //  c31->SaveAs(Form("%snv_neutron_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));  

  TH1F* nv_neutron_coincidence_small_ene_subtracted_hist = new TH1F("nv_neutron_coincidence_small_ene_subtracted_hist","Veto Neutron Subtracted After Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_coincidence_small_ene_subtracted_hist->Add(nv_neutron_coincidence_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c32 = new TCanvas("c32","Veto Neutron Coincidence Small Energy Subtracting BG",1000,400);
  c32->Divide(2,1);
  c32->cd(1);
  gPad->SetLogy();
  nv_neutron_coincidence_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c32->cd(2);
  gPad->SetLogy();
  nv_neutron_coincidence_small_ene_subtracted_hist->Draw();
  //c32->SaveAs(Form("%snv_neutron_coincidence_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_after_small_ene_subtracted_hist = new TH1F("nv_neutron_after_small_ene_subtracted_hist","Veto Neutron Subtracted After Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_after_small_ene_subtracted_hist->Add(nv_neutron_after_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c33 = new TCanvas("c33","Veto Neutron AFter Small Energy Subtracting BG",1000,400);
  c33->Divide(2,1);
  c33->cd(1);
  //  gPad->SetLogy();
  nv_neutron_after_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c33->cd(2);
  //  gPad->SetLogy();
  nv_neutron_after_small_ene_subtracted_hist->Draw();
  //c33->SaveAs(Form("%snv_neutron_after_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_first_small_ene_subtracted_hist = new TH1F("nv_neutron_first_small_ene_subtracted_hist","Veto Neutron Subtracted First Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_first_small_ene_subtracted_hist->Add(nv_neutron_first_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c34 = new TCanvas("c34","Veto Neutron First Small Energy Subtracting BG",1000,400);
  c34->Divide(2,1);
  c34->cd(1);
  // gPad->SetLogy();
  nv_neutron_first_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c34->cd(2);
  // gPad->SetLogy();
  nv_neutron_first_small_ene_subtracted_hist->Draw();
  //c34->SaveAs(Form("%snv_neutron_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TH1F* nv_neutron_prompt_small_ene_subtracted_hist = new TH1F("nv_neutron_prompt_small_ene_subtracted_hist","Veto Neutron Subtracted Prompt Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_neutron_prompt_small_ene_subtracted_hist->Add(nv_neutron_prompt_small_ene_hist,nv_neutron_late_small_ene_hist,1,-1);
  TCanvas *c35 = new TCanvas("c35","Veto Neutron Prompt Small Energy Subtracting BG",1000,400);
  c35->Divide(2,1);
  c35->cd(1);
  gPad->SetLogy();
  nv_neutron_prompt_small_ene_hist->Draw();
  nv_neutron_late_small_ene_hist->Draw("same");
  c35->cd(2);
  gPad->SetLogy();
  nv_neutron_prompt_small_ene_subtracted_hist->Draw();
  //c35->SaveAs(Form("%snv_neutron_prompt_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */
  /*
  TCanvas *c41 = new TCanvas("c41","Veto Neutron Coincidence",1000,600);
  c41->Divide(2,2);
  c41->cd(1);
  gPad->SetLogy();
  nv_neutron_coincidence_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_coincidence_gps_hist->Draw();
  c41->cd(2);
  gPad->SetLogy();
  nv_neutron_coincidence_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_coincidence_ene_hist->Draw();
  c41->cd(3);
  gPad->SetLogy();
  //  nv_neutron_coincidence_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_coincidence_gps_ene_hist->Draw("colz");
  c41->cd(4);
  gPad->SetLogy();
  //  nv_neutron_coincidence_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_coincidence_ene_s1_hist->Draw("colz");
  //  c41->SaveAs(Form("%snv_neutron_coincidence%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c42 = new TCanvas("c42","Veto Neutron Prompt",1000,600);
  c42->Divide(2,2);
  c42->cd(1);
  gPad->SetLogy();
  nv_neutron_prompt_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_prompt_gps_hist->Draw();
  c42->cd(2);
  gPad->SetLogy();
  nv_neutron_prompt_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_prompt_ene_hist->Draw();
  c42->cd(3);
  gPad->SetLogy();
  //  nv_neutron_prompt_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_prompt_gps_ene_hist->Draw("colz");
  c42->cd(4);
  gPad->SetLogy();
  //  nv_neutron_prompt_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_prompt_ene_s1_hist->Draw("colz");
  //  c42->SaveAs(Form("%snv_neutron_prompt%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c43 = new TCanvas("c43","Veto Neutron Delay",1000,600);
  c43->Divide(2,2);
  c43->cd(1);
  gPad->SetLogy();
  nv_neutron_delay_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_delay_gps_hist->Draw();
  c43->cd(2);
  gPad->SetLogy();
  nv_neutron_delay_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_delay_ene_hist->Draw();
  c43->cd(3);
  gPad->SetLogy();
  //  nv_neutron_delay_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_delay_gps_ene_hist->Draw("colz");
  c43->cd(4);
  gPad->SetLogy();
  //  nv_neutron_delay_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_delay_ene_s1_hist->Draw("colz");
  //  c43->SaveAs(Form("%snv_neutron_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c44 = new TCanvas("c44","Veto Neutron After",1000,600);
  c44->Divide(2,2);
  c44->cd(1);
  gPad->SetLogy();
  nv_neutron_after_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_after_gps_hist->Draw();
  c44->cd(2);
  gPad->SetLogy();
  nv_neutron_after_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_after_ene_hist->Draw();
  c44->cd(3);
  gPad->SetLogy();
  //  nv_neutron_after_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_after_gps_ene_hist->Draw("colz");
  c44->cd(4);
  gPad->SetLogy();
  //  nv_neutron_after_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_after_ene_s1_hist->Draw("colz");
  //  c44->SaveAs(Form("%snv_neutron_after%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c45 = new TCanvas("c45","Veto Neutron Late",1000,600);
  c45->Divide(2,2);
  c45->cd(1);
  gPad->SetLogy();
  //  nv_neutron_late_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_late_gps_hist->Draw();
  c45->cd(2);
  gPad->SetLogy();
  // nv_neutron_late_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_late_ene_hist->Draw();
  c45->cd(3);
  gPad->SetLogy();
  //  nv_neutron_late_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_late_gps_ene_hist->Draw("colz");
  c45->cd(4);
  gPad->SetLogy();
  //  nv_neutron_late_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_late_ene_s1_hist->Draw("colz");
  //  c45->SaveAs(Form("%snv_neutron_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));

  TCanvas *c46 = new TCanvas("c46","Veto Neutron First",1000,600);
  c46->Divide(2,2);
  c46->cd(1);
  gPad->SetLogy();
  nv_neutron_first_gps_hist->SetLineColor(linecolor.at(5));
  nv_neutron_first_gps_hist->Draw();
  c46->cd(2);
  gPad->SetLogy();
  nv_neutron_first_ene_hist->SetLineColor(linecolor.at(6));
  nv_neutron_first_ene_hist->Draw();
  c46->cd(3);
  gPad->SetLogy();
  //  nv_neutron_first_gps_ene_hist->SetLineColor(linecolor.at(5));
  nv_neutron_first_gps_ene_hist->Draw("colz");
  c46->cd(4);
  gPad->SetLogy();
  //  nv_neutron_first_ene_s1_hist->SetLineColor(linecolor.at(6));
  nv_neutron_first_ene_s1_hist->Draw("colz");
  // c46->SaveAs(Form("%snv_neutron_first%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  */

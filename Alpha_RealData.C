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

TRint *theApp;
using namespace std;
string Time = "Jan22AM";
string dirname = "/darkside/users/hqian/NeutronCalibration/";
 
int check_digits(int number)
{
  int digits = 0;
  do{
    number /= 10;
    digits++;
  }  
  while(number);
  return digits; 
}

string add_digits(int number, int total_digits)
{
  stringstream oss;
  string temp;
  int digits = check_digits(number);
  //  cout<<digits<<endl;
  int diff_digits = total_digits - digits;
  string new_number;
  if(!diff_digits)
    {
      oss<<number;
      return oss.str();
    }
  else{
    for(int i=0; i<diff_digits; i++)
      temp += "0";
    oss<<number;
    new_number = temp+oss.str();       
    //  cout<<new_number<<endl;
  }
  return new_number;
}

void Readdatafile(TChain *t, int startfile, int endfile)
{
  //  string dirname = "/darkside/users/hqian/NeutronCalibration/";
  string middle="ODRun";
  string last=".root";
  int total_digits = 6;
  ifstream NameCheck;
  for(int i=startfile; i<=endfile; i++)
    {
      string temp =  add_digits(i,total_digits);
      cout<<temp<<endl;
      string filename = dirname+middle+temp+last;
      NameCheck.open(filename.c_str());
      //      cout<<"Process Data file: "<<filename<<endl;
      if(!NameCheck.good())
	{
	  NameCheck.close();
	  continue;
	}
      else{
	t->Add(filename.c_str());
	cout<<"Process Data file: "<<filename<<endl;
      }
      NameCheck.close();
    }
}

bool SortFunction(double i, double j)
{
  return (TMath::Abs(i) < TMath::Abs(j));
}

int main(int argc, char **argv){
  theApp = new TRint("theApp",&argc,argv,NULL,0);
  //  string inputdir = "/darkside/data/lsv/v6/";
  //  string inputfile = inputdir+"ODRun010176.root";
  // TFile *file = new TFile(inputfile.c_str());
  // TTree *odtree = (TTree*) file->Get("odtree");
  int startfile, endfile;
  TString series;
  if(theApp->Argc() == 2)
    {
      startfile = atoi(theApp->Argv(1));
      endfile = startfile;
      series = theApp->Argv(1);
    }
  else if(theApp->Argc() == 3)
    {
      startfile = atoi(theApp->Argv(1));
      endfile = atoi(theApp->Argv(2));
      series = theApp->Argv(1);
      series += theApp->Argv(2);
    }
  else{
    cout<<"Usage: ./real startfile endfile "<<endl;
    cout<<"Usage: ./real startfile "<<endl;
    return 0;
  }
  cout<<"startfile "<<startfile<<"\t endfile "<<endfile<<"  series "<<series<<endl;
  cout<<"======= Start Matching TPC and OD data ====="<<endl;      
  TChain *odtree = new TChain("odtree");
  Readdatafile(odtree, startfile, endfile);
  int odEntries = odtree->GetEntries();
  cout<<"odEntries= "<<odEntries<<endl;
  Float_t livetime = odEntries*45./1.e+6;

  //define loop_over_old_branches
#ifdef loop_over_old_branches
  Double_t        absolute_t[64];
  Double_t        relative_t[64];
  UShort_t        pps_counter;
  UInt_t          gps_fine_time_counter;
  Float_t         lsv_cluster_fixed_width_charge[200];
  Int_t           lsv_n_clusters;
  Float_t         lsv_cluster_start_ns[200];
  Float_t         lsv_cluster_end_ns[200];
  odtree->SetBranchAddress("absolute_t", absolute_t);
  odtree->SetBranchAddress("relative_t", relative_t);
  odtree->SetBranchAddress("pps_counter", &pps_counter);
  odtree->SetBranchAddress("gps_fine_time_counter", &gps_fine_time_counter);  
  odtree->SetBranchAddress("lsv_cluster_fixed_width_charge", lsv_cluster_fixed_width_charge);
  odtree->SetBranchAddress("lsv_n_clusters", &lsv_n_clusters);
  odtree->SetBranchAddress("lsv_cluster_start_ns", lsv_cluster_start_ns);
  odtree->SetBranchAddress("lsv_cluster_end_ns", lsv_cluster_end_ns);
  
  odtree->SetBranchStatus("*",0);
  odtree->SetBranchStatus("relative_t",kTRUE);
  odtree->SetBranchStatus("absolute_t",kTRUE);
  odtree->SetBranchStatus("pps_counter",kTRUE);
  odtree->SetBranchStatus("gps_fine_time_counter",kTRUE);  
  odtree->SetBranchStatus("lsv_cluster_fixed_width_charge",kTRUE);
  odtree->SetBranchStatus("lsv_n_clusters",kTRUE);
  odtree->SetBranchStatus("lsv_cluster_start_ns",kTRUE);
  odtree->SetBranchStatus("lsv_cluster_end_ns",kTRUE);  
#else
  //=================Read odtree Data==========================
  Int_t           run;
  UShort_t        pps_counter;
  UInt_t          gps_fine_time_counter;
  Int_t           lsv_n_clusters;
  TArrayF         *lsv_cluster_fixed_width_charge;
  TArrayF         *lsv_cluster_start_ns;
  TArrayF         *lsv_cluster_end_ns;
  lsv_cluster_fixed_width_charge = 0;
  lsv_cluster_start_ns = 0;
  lsv_cluster_end_ns = 0;

  odtree->SetBranchAddress("run", &run);
  odtree->SetBranchAddress("pps_counter", &pps_counter);
  odtree->SetBranchAddress("gps_fine_time_counter", &gps_fine_time_counter);
  odtree->SetBranchAddress("lsv_cluster_fixed_width_charge", &lsv_cluster_fixed_width_charge);
  odtree->SetBranchAddress("lsv_n_clusters", &lsv_n_clusters);
  odtree->SetBranchAddress("lsv_cluster_start_ns", &lsv_cluster_start_ns);
  odtree->SetBranchAddress("lsv_cluster_end_ns", &lsv_cluster_end_ns);  

  odtree->SetBranchStatus("*",0);
  odtree->SetBranchStatus("run",kTRUE);
  odtree->SetBranchStatus("pps_counter",kTRUE);
  odtree->SetBranchStatus("gps_fine_time_counter",kTRUE);  
  odtree->SetBranchStatus("lsv_cluster_fixed_width_charge",kTRUE);
  odtree->SetBranchStatus("lsv_n_clusters",kTRUE);
  odtree->SetBranchStatus("lsv_cluster_start_ns",kTRUE);
  odtree->SetBranchStatus("lsv_cluster_end_ns",kTRUE);  
#endif
  
#define start_tpc
#ifdef start_tpc
  //==========================Start Read TPC Data================================
  TFile *file = new TFile("/home/hqian/montecarlo/g4ds10/Linux-g++/GammaCalibration/CalRun10176-10198_proc.root");
  TTree *tpc = (TTree*) file->Get("timestamps_etc");
  int tpcEntries = tpc->GetEntries();
  cout<<"tpcEntries= "<<tpcEntries<<endl;  
  Int_t           runID;
  Int_t           eventID;
  Double_t        gps_coarse;
  Double_t        gps_fine;
  Double_t        s1_start_time;
  Double_t        total_s1;
  Double_t        total_f90;
  Int_t           no_of_pulses;
  tpc->SetBranchAddress("runID",&runID);
  tpc->SetBranchAddress("eventID",&eventID);
  tpc->SetBranchAddress("gps_coarse",&gps_coarse); //s
  tpc->SetBranchAddress("gps_fine",&gps_fine);
  tpc->SetBranchAddress("s1_start_time",&s1_start_time); //us
  tpc->SetBranchAddress("total_s1",&total_s1);
  tpc->SetBranchAddress("total_f90",&total_f90);
  tpc->SetBranchAddress("no_of_pulses",&no_of_pulses);
#endif

  //===================Create New Histograms========================  
  double prompt_time = -500.; 
  double middle_time = 0;
  double delay_time = 7.e+4; 
  double first_time = 500.; //ns
  TH1F* tpc_gamma_gps_hist = new TH1F("tpc_gamma_gps_hist","TPC Gamma Time;Time [ns]",1000,0,1.e+4); 
  TH1F* tpc_gamma_ene_hist = new TH1F("tpc_gamma_ene_hist","TPC Gamma Energy;Energy [PE]",1000,0,1.e+4);
  TH2F* tpc_gamma_gps_ene_hist = new TH2F("tpc_gamma_gps_ene_hist","TPC Gamma Time Vs Energy;Time [ns];Energy [PE]",1000,0,1.e+4,1000,0,1.e+4);
  TNtuple* tpc_gamma_ntuple = new TNtuple("tpc_gamma_ntuple","TPC Gamma Data","time:ene");

  TH1F* nv_gps_hist = new TH1F("nv_gps_hist","Veto Time;Time [ns]",10000,0,7.e+4);
  TH1F* nv_ene_hist = new TH1F("nv_ene_hist","Veto Energy;Energy [PE]",1000,0,1.e+4);
  TH2F* nv_gps_ene_hist = new TH2F("nv_gps_ene_hist","Veto Time Vs Energy;Time [ns];Energy[PE]",10000,0,7.e+4,1000,0,1.e+4);
  TNtuple* nv_ntuple = new TNtuple("nv_ntuple","Veto Data","time:ene");

  TH1F* nv_coincidence_gps_hist = new TH1F("nv_coincidence_gps_hist","Veto Coincidence Time;Time [ns]",10000,prompt_time,delay_time);
  TH1F* nv_coincidence_ene_hist = new TH1F("nv_coincidence_ene_hist","Veto Coincidence Energy;Energy [PE]",1000,0,1.e+4);
  TH1F* nv_coincidence_small_ene_hist = new TH1F("nv_coincidence_small_ene_hist","Veto Coincidence Energy(small range);Energy [PE]",1000,0,600);  
  TH2F* nv_coincidence_gps_ene_hist = new TH2F("nv_coincidence_gps_ene_hist","Veto Coincidence Time Vs Energy;Time [ns];Energy[PE]",10000,prompt_time,delay_time,1000,0,1.e+4);
  TNtuple* nv_coincidence_ntuple = new TNtuple("nv_coincidence_ntuple","Veto Coincidence Data","time:ene");  

  TH1F* nv_prompt_gps_hist = new TH1F("nv_prompt_gps_hist","Veto Prompt Time;Time [ns]",1000,prompt_time,middle_time);
  TH1F* nv_prompt_ene_hist = new TH1F("nv_prompt_ene_hist","Veto Prompt Energy;Energy [PE]",1000,0,1.e+4);
  TH2F* nv_prompt_gps_ene_hist = new TH2F("nv_prompt_gps_ene_hist","Veto Prompt Time Vs Energy;Time [ns];Energy[PE]",1000,prompt_time,middle_time,1000,0,1.e+4);
  TNtuple* nv_prompt_ntuple = new TNtuple("nv_prompt_ntuple","Veto Prompt Data","time:ene");
  
  TH1F* nv_delay_gps_hist = new TH1F("nv_delay_gps_hist","Veto Delay Time;Time [ns]",10000,middle_time,delay_time);
  TH1F* nv_delay_ene_hist = new TH1F("nv_delay_ene_hist","Veto Delay Energy;Energy [PE]",1000,0,1.e+4);
  TH1F* nv_delay_small_ene_hist = new TH1F("nv_delay_small_ene_hist","Veto Delay Energy(small range);Energy [PE]",1000,0,600);  
  TH2F* nv_delay_gps_ene_hist = new TH2F("nv_delay_gps_ene_hist","Veto Delay Time Vs Energy;Time [ns];Energy[PE]",10000,middle_time,delay_time,1000,0,1.e+4);
  TNtuple* nv_delay_ntuple = new TNtuple("nv_delay_ntuple","Veto Delay Data","time:ene");
  
  TH1F* nv_first_gps_hist = new TH1F("nv_first_gps_hist","Veto First Time;Time [ns]",1000,middle_time,first_time);
  TH1F* nv_first_ene_hist = new TH1F("nv_first_ene_hist","Veto First Energy;Energy [PE]",1000,0,1.e+4);
  TH2F* nv_first_gps_ene_hist = new TH2F("nv_first_gps_ene_hist","Veto First Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,first_time,1000,0,1.e+4);
  TNtuple* nv_first_ntuple = new TNtuple("nv_first_ntuple","Veto First Data","time:ene");
  

  //=================Start Analysis Coincidence Events===========================
  //  double gamma_time = 1.5e+6; //ns
  int od_event_start_tag = 0;
  int od_event_end_tag = 0;
  vector<int> tpc_event_tag;
  //  tpcEntries = 900;
  for(int j=0; j<tpcEntries; j++)
    { 
 //===============Loop Over TPC Events=========================================    
      tpc->GetEntry(j);
#define single_run
#ifdef single_run
      if(runID != startfile) continue;
      else{ 
#endif
	if(!(j%10)) cout<<"Processing TPC Event "<<j<<endl;
	tpc_event_tag.push_back(runID);
      	double tpc_time = (gps_coarse + gps_fine*20.e-9 + s1_start_time*1.e-6)*1.e+9;
	vector<int> od_event_tag;
	tpc_gamma_gps_hist->Fill(tpc_time);
	/*	tpc_gamma_ene_hist->Fill(tpc_ene);
	tpc_gamma_gps_ene_hist->Fill(tpc_time,tpc_ene);
	tpc_gamma_ntuple->Fill(tpc_time,tpc_ene);
	*/
 //===============Loop Over Veto Events=========================================    
	for(int i=0; i<odEntries; i++)
	  { odtree->GetEntry(i);
#ifndef single_run
	    if(runID < run) break;
	    else if(runID == run) {
	      od_event_tag.push_back(i);
#endif
	      vector<double> gps_diff;
	      vector<double> energy;
	      if((int)pps_counter == 0)
		continue;
	      else {
		for(int ncluster=0; ncluster<lsv_n_clusters; ncluster++)
		  { double od_time = ((double)pps_counter+((double)gps_fine_time_counter)*20.e-9)*1.e+9 + lsv_cluster_start_ns->At(ncluster);
		    double gps_diff_temp = od_time - tpc_time;

		    nv_gps_hist->Fill(od_time);
		    nv_ene_hist->Fill(lsv_cluster_fixed_width_charge->At(ncluster));
		    nv_gps_ene_hist->Fill(od_time,lsv_cluster_fixed_width_charge->At(ncluster));
		    nv_ntuple->Fill(od_time,lsv_cluster_fixed_width_charge->At(ncluster));
		    /* double prompt_time = -500.; 
		    double middle_time = 0;
		    double delay_time = 7.e+4; 
		    double first_time = 500; //ns
		    */
		    if(gps_diff_temp>prompt_time && gps_diff_temp<middle_time)
		      {
			nv_prompt_gps_hist->Fill(gps_diff_temp);
			nv_prompt_ene_hist->Fill(lsv_cluster_fixed_width_chage->At(ncluster));
			nv_prompt_gps_ene_hist->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));
			nv_prompt_ntuple->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));			
		      }
		    if(gps_diff_temp>middle_time && gps_diff_temp<delay_time)
		      {
			nv_delay_gps_hist->Fill(gps_diff_temp);
			nv_delay_ene_hist->Fill(lsv_cluster_fixed_width_chage->At(ncluster));
			nv_delay_gps_ene_hist->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));
			nv_delay_ntuple->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));			
		      }
		    if(gps_diff_temp>prompt_time && gps_diff_temp<delay_time)
		      {
			nv_coincidence_gps_hist->Fill(gps_diff_temp);
			nv_coincidence_ene_hist->Fill(lsv_cluster_fixed_width_chage->At(ncluster));
			nv_coincidence_gps_ene_hist->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));
			nv_coincidence_ntuple->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));			
		      }		    
		    if(gps_diff_temp<first_time && gps_diff_temp>middle_time)
		      {
			nv_first_gps_hist->Fill(gps_diff_temp);
			nv_first_ene_hist->Fill(lsv_cluster_fixed_width_chage->At(ncluster));
			nv_first_gps_ene_hist->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));
			nv_first_ntuple->Fill(gps_diff_temp,lsv_cluster_fixed_width_chage->At(ncluster));			
		      }
		  } //end of looping over veto cluster per veto event   	       
		/*		if(gps_diff.size())	
		  {
		    sort(gps_diff.begin(),gps_diff.end(),SortFunction);
		    gps_window_closest_hist->Fill(gps_diff.front());
		    gps_window_closest_zoom_hist->Fill(gps_diff.front());
		  }
		*/
	      } //end of else cloeset to veto cluster loop
#ifndef single_run  
	      if(od_event_tag.size())
		{
		  od_event_start_tag = od_event_tag.front();
		  od_event_end_tag = od_event_tag.back();
		}
	    } //end of runID
	    else
	      { 
		if(j>0 && (tpc_event_tag.at(j) == tpc_event_tag.at(j-1)))
		  i=od_event_start_tag;
		else if(j>0 && (tpc_event_tag.at(j) > tpc_event_tag.at(j-1)))
		  i=od_event_end_tag;			       
		else 
	      }	    
#endif
	  }  //end of loop over veto events
#ifdef single_run
      }
#endif	
    } //end of TPC events 
  
  //  string outdirname="/home/hqian/montecarlo/g4ds10/Linux-g++/GammaCalibration/rawfiles/";
  string outputname=dirname+"Alpha_RealData_"+series.Data()+Time+".root";
  TFile f2D(outputname.c_str(), "RECREATE"); 
  tpc_gamma_gps_hist->Write();
  tpc_gamma_ene_hist->Write();
  tpc_gamma_gps_ene_hist->Write();
  tpc_gamma_ntuple->Write();
  nv_gps_hist->Write();
  nv_ene_hist->Write();
  nv_gps_ene_hist->Write();
  nv_ntuple->Write();
  nv_coincidence_gps_hist->Write();
  nv_coincidence_ene_hist->Write();
  nv_coincidence_gps_ene_hist->Write();
  nv_coincidence_ntuple->Write();
  nv_prompt_gps_hist->Write();
  nv_prompt_ene_hist->Write();
  nv_prompt_gps_ene_hist->Write();
  nv_prompt_ntuple->Write();
  nv_delay_gps_hist->Write();
  nv_delay_ene_hist->Write();
  nv_delay_gps_ene_hist->Write();
  nv_delay_ntuple->Write();
  nv_first_gps_hist->Write();
  nv_first_ene_hist->Write();
  nv_first_gps_ene_hist->Write();
  nv_first_ntuple->Write();
  f2D.Write();
  f2D.Close();

#endif
  cout<<"===>> Application Finished"<<endl;
  return 1;
}

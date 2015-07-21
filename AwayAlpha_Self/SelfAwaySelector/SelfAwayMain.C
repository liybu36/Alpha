#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TRint.h"
#include "TProof.h"
#include "TDSet.h"

using namespace std;
#include "SelfAwaySelector.h"

const string Time = "_away_Jan11AM";
//string dirname="/darkside/data/DST/";
string dirname="/darkside/users/hqian/AmBe10Hz_Calibration/DSTAwayData/";

TRint* theApp;

/*
void Readdatafile(TChain *t, int start, int end)
{ 
  string dirname="/darkside/data/DST/";
  string filename;
  string middle="DST_Run0";
  string last=".root";
  stringstream oss;
  for(int i=start; i<=end; i++)
    {
      if(i==0)
	filename=dirname+middle+last;
      else
	{
	  oss<<i;
	  filename=dirname+middle+"_v"+oss.str()+last;
	}
      t->Add(filename.c_str());
      cout<<"Processing Data file: "<<filename<<endl;
      oss.str("");
    }
}

string Readdatafile(int i)
{ 
  //  string dirname="/cache/shared/darkside/hqian/TMBfraction/coincidence_data/";
  string dirname="/darkside/users/hqian/TMBfraction/cluster_data/";
  string filename;
#ifdef neutron_data
  //  string middle="outneutron";
  string middle="outAm241";
#else
  string middle="outtmb";
#endif
  string last="_clustered.root";
  stringstream oss;
  if(i==0)
    filename=dirname+middle+last;
  else
    {
      oss<<i;
      filename=dirname+middle+"_v"+oss.str()+last;
    }
  cout<<"Processing Data file: "<<filename<<endl;
  //  TFile *file = new TFile(filename.c_str());
  // t = (TTree*)file->Get("dstree");
  oss.str("");
  return filename;
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
	//        TFile *f1 = new TFile(filename.c_str());
	//        if(f1->IsZombie())
	//          continue;
	//        else{
          t->Add(filename.c_str());
          cout<<"Processing Data file: "<<filename<<endl;
	  //        }
        oss.str("");
      }
      NameCheck.close();
    }
}



void Process(TChain* chain, TString label){
  TString option = label;
  chain->SetProof();
  TProof* pr = TProof::Open("workers=2");
  //Print more information
  //  pr->SetLogLevel(2,TProofDebug::kPacketizer);
  pr->SetParameter("PROOF_Packetizer","TPacketizer");
  pr->SetParameter("PROOF_MaxSlavesPerNode",8);

  //  ReconSelector *selector = new ReconSelector();
  //  Bool_t withfriends = kFALSE;
  //  TDSet *dataset = new TDSet(*chain, withfriends);
  // dataset->Add("/darkside/users/hqian/neutron0G163tilt/cluster_data/outnvetoambe9L_cylinder_clustered.root");
  // pr->ShowDataSets();
  // dataset->Process("./ReconSelector/ReconSelector.C++",option.Data(),100,100);  
  chain->Process("./SelfAwaySelector.C+",option.Data(),-1,0);  
  //  gSystem->Exit(0);
  //  chain->Process(pr,option.Data(),100,0);    
}

#ifndef __CINT__
//main function
int main(int argc, char** argv){
  theApp = new TRint("App",&argc,argv,NULL,0);
  int start, end;
  //read the input number from terminal
  if(theApp->Argc() == 2)
    {
      start = atoi(theApp->Argv(1));
      end = start;
    }
  else if(theApp->Argc() == 3)
    {
      start = atoi(theApp->Argv(1));
      end = atoi(theApp->Argv(2));
    }
  else{
    cout<<"Usage: ./SelfAwayMain startfile endfile "<<endl;
    cout<<"Usage: ./SelfAwayMain startfile "<<endl;
    return 0;
  }
  
  cout<<"Start Using the Selector..."<<endl;
  //read the dstree root file

  /*  for(int i=start; i<=end; i++)
    {
      string filename = Readdatafile(i);
      TChain *t = new TChain("Recon");
      t->Add(filename.c_str());
      string label = "tmb_alpha";
      stringstream oss;
      if(i==0)
	label += "";
      else
	{
	  oss<<i;
	  label +="_v"+oss.str();
	}
      label += Time;
      Process(t,label);
    }
  */

  TChain *t = new TChain("DSTtree");                                                                
  string label = "Self_alpha";               
  stringstream oss;                                                                                                                  
  Readdatafile(t,start,end);                                                                                                         
  oss<<start<<end;                 
  label +=oss.str()+Time;           
  Process(t,label);                                                                                                                  
  oss.str("");  
  
  cout<<"Successfully finished the app."<<endl;
  return 1;

}
#endif /*__CINT__*/

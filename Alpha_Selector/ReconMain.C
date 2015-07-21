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
#include "./ReconSelector/ReconSelector.h"

const string Time = "_Apr15PM";
TRint* theApp;

//#define neutron_data
void Readdatafile(TChain *t, int start, int end)
{ 
  //  string dirname="/cache/shared/darkside/hqian/TMBfraction/cluster_data/";
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
  chain->Process("./ReconSelector/ReconSelector.C+",option.Data(),-1,0);  
  //  gSystem->Exit(0);
  //  chain->Process(pr,option.Data(),100,0);    
}

#ifndef __CINT__
//main function
int main(int argc, char** argv){
  theApp = new TRint("App",&argc,argv,NULL,0);
  //  theApp->Connect("keypressed(Int_t)","TSystem",gSystem,"ExitLoop()");
  //  TChain *t=new TChain("dstree");
  //  Readdatafile(t,Volume);
  // t->Add("/cache/shared/darkside/hqian/TMBfraction/outtmb_v8.root");
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
    cout<<"Usage: ./ReconMain startfile endfile "<<endl;
    cout<<"Usage: ./ReconMain startfile "<<endl;
    return 0;
  }
  
  cout<<"Start Using the Selector..."<<endl;
  //read the dstree root file

  for(int i=start; i<=end; i++)
    {
      string filename = Readdatafile(i);
      //   TFile* file = new TFile(filename.c_str());
      //   TTree* t = (TTree*) file->Get("dstree");
      TChain *t = new TChain("Recon");
      t->Add(filename.c_str());
#ifdef neutron_data
      //      string label = "tmb_neutron";
      string label="tmb_Am241";
#else
      string label = "tmb_alpha";
#endif
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
  

  /*  TChain *t = new TChain("Recon");                                                                
  string label = "tmb_alpha";                                                                                
  stringstream oss;                                                                                                                  
  Readdatafile(t,start,end);                                                                                                         
  oss<<start<<end;                                                                                                                   
  label +="_v"+oss.str()+Time;               
  Process(t,label);                                                                                                                  
  oss.str("");  
  */
  cout<<"Successfully finished the app."<<endl;
  return 1;

}
#endif /*__CINT__*/

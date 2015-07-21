#include "TMath.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TRint.h"
#include "TChain.h"
#include "TFile.h"

using namespace std;
TRint *theApp;
const string Time = "_Jan8PM";

string Readdatafile(int i)
{
  //  string dirname="/cache/shared/darkside/hqian/TMBfraction/coincidence_data/";
  string dirname="/darkside/users/hqian/TMBfraction/cluster_data/";
  string filename;
#ifdef neutron_data
  string middle="outneutron";
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
  oss.str("");
  return filename;
}

void MakeCounts(TChain *t, string label){    
    int nEntries = t->GetEntries();
    cout << "Entries= " << nEntries << endl;
    vector<double> *ex=0;
    vector<double> *ey=0;
    vector<double> *ez=0;
    vector<double> *et=0;
    vector<double> *edep=0;
    vector<double> *eqch=0;
    vector<double> *quenchingfactor=0;
    vector<TString>* volume=0;
    
    t->SetBranchAddress("volume", &volume);
    t->SetBranchAddress("et", &et);
    t->SetBranchAddress("ex", &ex);
    t->SetBranchAddress("ey", &ey);
    t->SetBranchAddress("ez", &ez);
    t->SetBranchAddress("edep", &edep);
    t->SetBranchAddress("eqch", &eqch);
    t->SetBranchAddress("quenchingfactor", &quenchingfactor);
    
    double quenchingcut = 0.43;
    double dep_enecut= 23.0; //keV
    double dep_enecut2=269.0; //keV
    double dep_enecut3 = 600.;//keV 
    double dep_enecut4 = 90.9;

    int eff=0;   
    int ndeps;
    bool InTPC,IsNR,IsER,IsG2,IsG2electron; 
    int insideTPCevents, nuclearevents, purenuclearevents;
    int electronevents,G2events, singleevents, singlenuclearevents;
    int G2electronevents, pureelectronevents;
    insideTPCevents=0;
    nuclearevents=0;
    purenuclearevents=0;
    electronevents=0;
    G2events=0;
    G2electronevents=0;
    singleevents=0;
    singlenuclearevents=0;
    pureelectronevents = 0;

    for(int j=0; j<nEntries; j++)
      { 
	if(!(j%100000)) std::cout<<"Processing Event "<<j<<", " <<Int_t(100.*j/nEntries)<<"% Completed"<<std::endl;
	ndeps=0;
	t->GetEntry(j);
	InTPC = false;
	IsNR = false;
	IsER=false;
	IsG2=false;
	IsG2electron=false;
	for(int i=0; i< et->size(); i++)
	  {
	    if(volume->at(i) =="p_active")
	      { ndeps++;
		InTPC=true;
		if(ndeps==1) eff++; 
		if(quenchingfactor->at(i) < quenchingcut )
		  {
		    IsNR = true;
		    //    if(edep->at(i)>dep_enecut && edep->at(i)<dep_enecut2)
		    if(edep->at(i)<dep_enecut4)
		      IsG2 = true;
		  }
		if(quenchingfactor->at(i) > quenchingcut )
		  {
		    IsER = true;
		    //    if(edep->at(i)>dep_enecut && edep->at(i)<dep_enecut2)
		    if(edep->at(i)>dep_enecut3)
		      IsG2electron = true;
		  }
	      }
	  }
	if(InTPC)               insideTPCevents++;
	if(IsNR)                nuclearevents++;
	if(IsNR && !IsER)       purenuclearevents++;
	if(IsER)	        electronevents++;
	if(IsER && !IsNR)       pureelectronevents++;
	if(IsG2 && !IsER)                G2events++;
	if(IsG2electron && !IsNR)        G2electronevents++;	
	if(ndeps==1)            singleevents++;	
	if(ndeps==1 && IsNR && !IsER )   singlenuclearevents++;
      }
    cout<<label<<Time<<"  number of events: "<<nEntries<<endl;
    cout<<"InTPC= " <<insideTPCevents<<" IsNR= "<<nuclearevents<<" IsPureNR=  "<<purenuclearevents<<" IsER= "<<electronevents<<endl;
    cout<<"IsPureelectronevents= "<<pureelectronevents<<endl;
    cout<<"IsG2electronevents= "<<G2electronevents<<"  "<<"IsG2= "<<G2events<<endl;
    cout<<"IsSingle= " <<singleevents <<"     "<<"IsSinglenuclear= "<<singlenuclearevents<<endl;
    cout<<"NR_without_G2: "<<nuclearevents<<" ("<<1.*nuclearevents/nEntries<<")"<<endl;
    cout<<"with_G2_ER: "<<G2electronevents<<" ("<<1.*G2electronevents/nEntries<<")"<<endl;
    cout<<"eff= "<<eff<<endl;

    string dirname="/darkside/users/hqian/TMBfraction/results/";
    string outdataname = dirname+label+".txt";
    ofstream outdata (outdataname.c_str());
    if(outdata.is_open())
      {
	outdata<<label<<Time<<"  number of events: "<<nEntries<<"\n";
	outdata<<"InTPC= " <<insideTPCevents<<" IsNR= "<<nuclearevents<<" IsPureNR=  "<<purenuclearevents<<" IsER= "<<electronevents<<"\n";
	outdata<<"IsPureelectronevents= "<<pureelectronevents<<"\n";
	outdata<<"IsG2electronevents= "<<G2electronevents<<"  "<<"IsG2= "<<G2events<<"\n";
	outdata<<"IsSingle= " <<singleevents <<"     "<<"IsSinglenuclear= "<<singlenuclearevents<<"\n";
	outdata<<"NR_without_G2: "<<nuclearevents<<" ("<<1.*nuclearevents/nEntries<<")"<<"\n";
	outdata<<"with_G2_ER: "<<G2electronevents<<" ("<<1.*G2electronevents/nEntries<<")"<<"\n";  
      }
    else cout<<"Cannot open the file"<<endl;
    outdata.close();
}

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
    cout<<"Usage: ./Alpha_Count startfile endfile "<<endl;
    cout<<"Usage: ./Alpha_Count startfile "<<endl;
    return 0;
  }
  for(int i=start; i<=end; i++)
    {
      string filename = Readdatafile(i);
      TChain *t = new TChain("Recon");
      t->Add(filename.c_str());
#ifdef neutron_data
      string label = "tmb_neutron_count";
#else
      string label = "tmb_alpha_count";
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
      MakeCounts(t,label);
    }

  cout<<"Successfully finished the app."<<endl;
  return 1;
}

/*
void Alpha_counting(){
  string label;
  TChain *t=new TChain("Recon");
  //  Readdatafile(t,Volume);
  //  MakePlots(t,label);
  MakeCounts(t,label);
  //  Coincidence(t,label);
}

void Coincidence(TChain *t, string label)
{
  int nEntries = t->GetEntries();
  cout << "Entries= " << nEntries << endl;
  vector<double> *ex=0;
  vector<double> *ey=0;
  vector<double> *ez=0;
  vector<double> *et=0;
  vector<double> *edep=0;
  vector<double> *edep_nuclear=0;
  vector<double> *edep_electron=0;
  vector<double> *eqch=0;
  vector<double> *quenchingfactor=0;
  vector<TString>* volume=0;

  t->SetBranchAddress("volume", &volume);
  t->SetBranchAddress("et", &et); //s
  t->SetBranchAddress("ex", &ex);
  t->SetBranchAddress("ey", &ey);
  t->SetBranchAddress("ez", &ez);
  t->SetBranchAddress("edep", &edep); //keV
  t->SetBranchAddress("edep_nuclear", &edep_nuclear);
  t->SetBranchAddress("edep_electron", &edep_electron);
  t->SetBranchAddress("eqch", &eqch);
  t->SetBranchAddress("quenchingfactor", &quenchingfactor);
  
  TH1F* first_tpc_NR_time_hist = new TH1F("first_tpc_NR_time_hist","Time distribution of the first NR in TPC;time [s]",125,0,1.e-6);
  TH1F* tpc_NR_time_hist = new TH1F("tpc_NR_time_hist","Time distribution of the NR in TPC;time [s]",125,0,1.e-6);
  TH1F* tpc_quenching_factor_hist = new TH1F("tpc_quenching_factor_hist","Quenching Factor in TPC",125,0,1);
  TH1F* nv_NR_time_hist = new TH1F("nv_NR_time_hist","Time distribution of the NR in NV;time [s]",125,-1.e-6,1.e-6);
  TH1F* nv_NR_energy_hist = new TH1F("nv_NR_energy_hist","Energy distribution of the NR in NV;energy [keV]",125,0,5000); 
  TH1F* nv_time_hist = new TH1F("nv_time_hist","Time distribution in NV;time [s]",125,-1.e-6,1.e-6);
  TH1F* nv_energy_hist = new TH1F("nv_energy_hist","Energy distribution in NV;energy [keV]",1000,0,15000); 
  TH1F* nv_quenching_factor_hist = new TH1F("nv_quenching_factor_hist","Quenching Factor in NV",125,0,1);
  TH1F* nv_pdf_hist = new TH1F("nv_pdf_hist","pdf;Energy [keV];Probability",1000,0,15000);

  double quenchingcut = 0.43;
  double dep_enecut = 23.0; //keV
  double dep_enecut2 = 269.0; //keV
  double prompt_window = -100.e-9;
  double delay_window = 500.e-9;
  for(int i=0; i<nEntries; i++)
    {
      t->GetEntry(i);
      vector<double> TPCNR_Time;
      vector<double> NVNR_Time, NVNR_Edep;
      vector<double> NV_Time, NV_Edep;
      for(size_t j=0; j<et->size(); j++)
	{
	  if(volume->at(j)=="p_active")
	    {
	      tpc_quenching_factor_hist->Fill(quenchingfactor->at(j));
	      if(quenchingfactor->at(j) < quenchingcut && edep->at(j)>dep_enecut && edep->at(j)<dep_enecut2 )
		{ TPCNR_Time.push_back(et->at(j));
		  tpc_NR_time_hist->Fill(et->at(j));
		}
	    }
	  if(volume->at(j)=="p_scint")
	    {
	      nv_quenching_factor_hist->Fill(quenchingfactor->at(j));
	      NV_Time.push_back(et->at(j));
	      NV_Edep.push_back(eqch->at(j));
	      if(quenchingfactor->at(j) < quenchingcut )
		{
		  NVNR_Time.push_back(et->at(j));
		  NVNR_Edep.push_back(eqch->at(j));
		}
	    }
	} //end of loop over edeps for each event
      if(TPCNR_Time.size() && NVNR_Time.size())
	{
	  double first_tpc_NR_time = TPCNR_Time.front();
	  first_tpc_NR_time_hist->Fill(first_tpc_NR_time);
	  double total_nv_NR_energy = 0;
	  double total_nv_energy = 0;
	  for(size_t h=0; h<NVNR_Time.size(); h++)
	    {
	      double NR_time_diff = NVNR_Time.at(h) - first_tpc_NR_time;
	      if(NR_time_diff>prompt_window && NR_time_diff<delay_window)
		{ nv_NR_time_hist->Fill(NR_time_diff);
		  total_nv_NR_energy += NVNR_Edep.at(h);
		}
	    }
	  nv_NR_energy_hist->Fill(total_nv_NR_energy);
	  for(size_t h=0; h<NV_Time.size(); h++)
	    {
	      double time_diff = NV_Time.at(h) - first_tpc_NR_time;
	      if(time_diff>prompt_window && time_diff<delay_window)
		{ nv_time_hist->Fill(time_diff);
		  total_nv_energy += NV_Edep.at(h);
		}
	    }
	  nv_energy_hist->Fill(total_nv_energy);
	}
    }
  
  int nbins = nv_energy_hist->GetNbinsX();
  double sum = nv_energy_hist->Integral();
  cout<<nbins<<" "<<sum<<endl;
  for(int i=1; i<=nbins; i++)
    {
      double energy = nv_energy_hist->GetBinCenter(i);
      double density = nv_energy_hist->Integral(1,i)/sum;
      nv_pdf_hist->SetBinContent(i,density);
    }

  string outdirname="/home/hqian/montecarlo/g4ds10/Linux-g++/";       
  string outputname=outdirname+"ambe_coincidence_"+label+Time+".root";  
  TFile f2D(outputname.c_str(), "RECREATE");
  first_tpc_NR_time_hist->Write();
  tpc_NR_time_hist->Write();
  tpc_quenching_factor_hist->Write();
  nv_NR_time_hist->Write();
  nv_NR_energy_hist->Write();
  nv_quenching_factor_hist->Write();
  nv_time_hist->Write();
  nv_energy_hist->Write();
  nv_pdf_hist->Write();
  f2D.Write();
  f2D.Close();

}

void Readdatafile(TChain *t, int volume)
{   string dirname="/darkside/users/hqian/neutron0G163tiltlow/cluster_data/";
  //   string dirname="/ds50/data/user/hqian36/Collimator/neutron0G163tilt/cluster_data/";
    string filename;
    string middle="outnvetoambe";
    string last="_cylinder_clustered.root";
    stringstream oss;
    stringstream oss2;
    for(int i=0; i<datafiles; i++)
    {
        if(i==0)
        {
	  oss<<volume;
	  filename=dirname+middle+oss.str()+"L"+last;
	  //	  filename<<dirname<<middle<<volume<<"L"<<last;
	  //sprintf(filename,"%s%s%iL%s",dirname.c_str(),middle.c_str(),volume,last.c_str());
	  t->Add(filename.c_str());
	  cout<<"Processing Data file: "<<filename<<endl;
	  oss.str("");
        }
        else
	  { 
	    oss<<volume;
	    oss2<<i;
	    filename=dirname+middle+oss.str()+"L_v"+oss2.str()+last;
            t->Add(filename.c_str());	
	    cout<<"Processing Data file: "<<filename<<endl;
	    oss.str("");
	    oss2.str("");
	  }
    }
}

void MakePlots(TChain *t, string label){

  TCut nuclearenergy="edep_nuclear>0.025";
  //  TCut tpcactive="ez<14.6 && ez>-22.2 && sqrt(ex**2+ey**2)<17.77";
  TCut tpcactive="volume==\"p_active\"";
  TCut yslice="ey>-0.5 && ey<0.5";
  TCut gap="ex>-100 && ex<0";
  TCut quchcutn="quenchingfactor<0.43";
  TCut quchcute="quenchingfactor>0.43";
  
  string quchf = " quenchingfactor<0.43 ";
  string quche = " quenchingfactor>0.43 ";
  
  string zslice1 = " 2.2<dep_z<4.6 ";
  string zslice2 = " 0<dep_z<2.2 ";
  string zslice3 = " -2.2<dep_z<0 ";

  //without quenching cuts
  string Hist2DTitle = "dep_y vs dep_x in TPC active volume "+ label;
  TH2F* Hist2D = new TH2F("Hist2D",Hist2DTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2D->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2D->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYslice1Title = "dep_y vs dep_x in TPC active volume"+zslice1+ label;
  TH2F* Hist2DXYslice1 = new TH2F("Hist2DXYslice1",Hist2DXYslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYslice1->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYslice2Title = "dep_y vs dep_x in TPC active volume"+zslice2+ label;
  TH2F* Hist2DXYslice2 = new TH2F("Hist2DXYslice2",Hist2DXYslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYslice2->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYslice3Title = "dep_y vs dep_x in TPC active volume"+zslice3+ label;
  TH2F* Hist2DXYslice3 = new TH2F("Hist2DXYslice3",Hist2DXYslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYslice3->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DGapTitle = "dep_z vs dep_x with y slice "+ label;
  TH2F* Hist2DGap = new TH2F("Hist2DGap",Hist2DGapTitle.c_str(), 150,-100,0,150,-50,100);
  Hist2DGap->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DGap->GetYaxis()->SetTitle("dep_z [cm]");

  string QuenchingFactorTitle = "quenchingfactor in TPC active volume "+ label;
  TH1F* QuenchingFactor = new TH1F("QuenchingFactor",QuenchingFactorTitle.c_str(),100,0,1);
  QuenchingFactor->GetXaxis()->SetTitle("quenching factor");
  QuenchingFactor->GetYaxis()->SetTitle("counts");

  //quecnhing cuts for nuclear-like recoil
  string Hist2DnuclearTitle = "dep_y vs dep_x in TPC active volume"+quchf+ label;
  TH2F* Hist2Dnuclear = new TH2F("Hist2Dnuclear",Hist2DnuclearTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2Dnuclear->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2Dnuclear->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYnuclearslice1Title = "dep_y vs dep_x in TPC active volume"+zslice1+quchf+ label;
  TH2F* Hist2DXYnuclearslice1 = new TH2F("Hist2DXYnuclearslice1",Hist2DXYnuclearslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYnuclearslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYnuclearslice1->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYnuclearslice2Title = "dep_y vs dep_x in TPC active volume"+zslice2+quchf+ label;
  TH2F* Hist2DXYnuclearslice2 = new TH2F("Hist2DXYnuclearslice2",Hist2DXYnuclearslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYnuclearslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYnuclearslice2->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYnuclearslice3Title = "dep_y vs dep_x in TPC active volume"+zslice3+quchf+ label;
  TH2F* Hist2DXYnuclearslice3 = new TH2F("Hist2DXYnuclearslice3",Hist2DXYnuclearslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYnuclearslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYnuclearslice3->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist1DNRcountsslice1Title = "NR counts in slice1 in TPC active volume "+ label;
  TH1F* Hist1DNRcountsslice1 = new TH1F("Hist1DNRcountsslice1",Hist1DNRcountsslice1Title.c_str(),100,0,20);
  Hist1DNRcountsslice1->GetXaxis()->SetTitle("NR counts");

  string Hist1DNRcountsslice2Title = "NR counts in slice2 in TPC active volume "+ label;
  TH1F* Hist1DNRcountsslice2 = new TH1F("Hist1DNRcountsslice2",Hist1DNRcountsslice2Title.c_str(),100,0,20);
  Hist1DNRcountsslice2->GetXaxis()->SetTitle("NR counts");

  string Hist1DNRcountsslice3Title = "NR counts in slice3 in TPC active volume "+ label;
  TH1F* Hist1DNRcountsslice3 = new TH1F("Hist1DNRcountsslice3",Hist1DNRcountsslice3Title.c_str(),100,0,20);
  Hist1DNRcountsslice3->GetXaxis()->SetTitle("NR counts");

  string Hist1DNRcountsslice4Title = "NR counts in slice4 in TPC active volume "+ label;
  TH1F* Hist1DNRcountsslice4 = new TH1F("Hist1DNRcountsslice4",Hist1DNRcountsslice4Title.c_str(),100,0,20);
  Hist1DNRcountsslice4->GetXaxis()->SetTitle("NR counts");

  //single nuclear sacttering in TPC active volume
  string Hist2DsinglenuclearTitle = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+quchf+ label;
  TH2F* Hist2Dsinglenuclear = new TH2F("Hist2Dsinglenuclear",Hist2DsinglenuclearTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2Dsinglenuclear->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2Dsinglenuclear->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DYZsinglenuclearTitle = "dep_z vs dep_y for single nuclear scattering in TPC active volume"+quchf+ label;
  TH2F* Hist2DYZsinglenuclear = new TH2F("Hist2DYZsinglenuclear",Hist2DYZsinglenuclearTitle.c_str(),100,-20,20,100,-25,20);
  Hist2DYZsinglenuclear->GetXaxis()->SetTitle("dep_y [cm]");
  Hist2DYZsinglenuclear->GetYaxis()->SetTitle("dep_z [cm]");
  
  string Hist2DXYsinglenuclearslice1Title = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+zslice1+quchf+ label;
  TH2F* Hist2DXYsinglenuclearslice1 = new TH2F("Hist2DXYsinglenuclearslice1",Hist2DXYsinglenuclearslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsinglenuclearslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsinglenuclearslice1->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYsinglenuclearslice2Title = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+zslice2+quchf+ label;
  TH2F* Hist2DXYsinglenuclearslice2 = new TH2F("Hist2DXYsinglenuclearslice2",Hist2DXYsinglenuclearslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsinglenuclearslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsinglenuclearslice2->GetYaxis()->SetTitle("dep_y [cm]");
    
  string Hist2DXYsinglenuclearslice3Title = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+zslice3+quchf+ label;
  TH2F* Hist2DXYsinglenuclearslice3 = new TH2F("Hist2DXYsinglenuclearslice3",Hist2DXYsinglenuclearslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsinglenuclearslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsinglenuclearslice3->GetYaxis()->SetTitle("dep_y [cm]");

  //single sacttering in TPC active volume
  string Hist2DsingleTitle = "dep_y vs dep_x for single scattering in TPC active volume "+ label;
  TH2F* Hist2Dsingle = new TH2F("Hist2Dsingle",Hist2DsingleTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2Dsingle->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2Dsingle->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DYZsingleTitle = "dep_z vs dep_y for single scattering in TPC active volume "+ label;
  TH2F* Hist2DYZsingle = new TH2F("Hist2DYZsingle",Hist2DYZsingleTitle.c_str(),100,-20,20,100,-25,20);
  Hist2DYZsingle->GetXaxis()->SetTitle("dep_y [cm]");
  Hist2DYZsingle->GetYaxis()->SetTitle("dep_z [cm]");

  string Hist2DXYsingleslice1Title = "dep_y vs dep_x for single scattering in TPC active volume"+zslice1+ label;
  TH2F* Hist2DXYsingleslice1 = new TH2F("Hist2DXYsingleslice1",Hist2DXYsingleslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsingleslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsingleslice1->GetYaxis()->SetTitle("dep_y [cm]");

  string Hist2DXYsingleslice2Title = "dep_y vs dep_x for single scattering in TPC active volume"+zslice2+ label;
  TH2F* Hist2DXYsingleslice2 = new TH2F("Hist2DXYsingleslice2",Hist2DXYsingleslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsingleslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsingleslice2->GetYaxis()->SetTitle("dep_y [cm]");
    
  string Hist2DXYsingleslice3Title = "dep_y vs dep_x for single scattering in TPC active volume"+zslice3+ label;
  TH2F* Hist2DXYsingleslice3 = new TH2F("Hist2DXYsingleslice3",Hist2DXYsingleslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsingleslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsingleslice3->GetYaxis()->SetTitle("dep_y [cm]");

  int nEntries = t->GetEntries();
  cout << "Entries= " << nEntries << endl;
  vector<double> *ex=0;
  vector<double> *ey=0;
  vector<double> *ez=0;
  vector<double> *et=0;
  vector<double> *edep=0;
  vector<double> *edep_nuclear=0;
  vector<double> *edep_electron=0;
  vector<double> *eqch=0;
  vector<double> *quenchingfactor=0;
  vector<TString>* volume=0;
  vector<double> nrx=0;
  vector<double> nry=0;
  vector<double> nrz=0;
  vector<double> rx=0;
  vector<double> ry=0;
  vector<double> rz=0;
  
  t->SetBranchAddress("volume", &volume);
  t->SetBranchAddress("et", &et);
  t->SetBranchAddress("ex", &ex);
  t->SetBranchAddress("ey", &ey);
  t->SetBranchAddress("ez", &ez);
  t->SetBranchAddress("edep", &edep);
  t->SetBranchAddress("edep_nuclear", &edep_nuclear);
  t->SetBranchAddress("edep_electron", &edep_electron);
  t->SetBranchAddress("eqch", &eqch);
  t->SetBranchAddress("quenchingfactor", &quenchingfactor);
  
  double quenchingcut = 0.43;
  double dep_enecut= 23.0; //keV
  double dep_enecut2=269.0; //keV

  int eff=0;   
  bool IsNR,IsER;
  int NNR, NER, ndeps, nsing, NRcounts;;
  NNR=NER=0;
  nsing=0;
  for(int j=0; j<nEntries; j++)
    { 
      if(!(j%100000)) std::cout<<"Processing Event "<<j<<", " <<Int_t(100.*j/nEntries)<<"% Completed"<<std::endl;
      ndeps=0;
      NRcounts=0;
      t->GetEntry(j);
      IsNR = false;
      IsER = false;
      nrx.clear();
      nry.clear();
      nrz.clear();
      rx.clear();
      ry.clear();
      rz.clear();
      for(int i=0; i< et->size(); i++)
        {
	  if(volume->at(i) =="p_active")
            { ndeps++;
	      IsER=true;
	      Hist2D->Fill(ex->at(i),ey->at(i));
	      QuenchingFactor->Fill(quenchingfactor->at(i));
	      if(ndeps==1)
		{ eff++;
		  rx.push_back(ex->at(i));
		  ry.push_back(ey->at(i));
		  rz.push_back(ez->at(i));
		}           
	      if(ez->at(i)<4.6 && ez->at(i)>2.2)
		Hist2DXYslice1->Fill(ex->at(i),ey->at(i));
	      if(ez->at(i)<2.2 && ez->at(i)>0)
		Hist2DXYslice2->Fill(ex->at(i),ey->at(i));
	      if(ez->at(i)>-2.2 && ez->at(i)<0)
		Hist2DXYslice3->Fill(ex->at(i),ey->at(i));
	      if(quenchingfactor->at(i) < quenchingcut && edep->at(i)<dep_enecut2 && edep->at(i)>dep_enecut )
                {
		  //  NRcounts++;
		  IsNR = true;
		  Hist2Dnuclear->Fill(ex->at(i),ey->at(i));
		  nrx.push_back(ex->at(i));
		  nry.push_back(ey->at(i));
		  nrz.push_back(ez->at(i));
		  if(ez->at(i)>4.6)
		  {
		    NRcounts++;
		    Hist1DNRcountsslice4->Fill(NRcounts);		    		   
		  } 
		 else if(ez->at(i)<4.6 && ez->at(i)>2.2)
		    {  Hist2DXYnuclearslice1->Fill(ex->at(i),ey->at(i));
		      NRcounts++;
		      Hist1DNRcountsslice1->Fill(NRcounts);
		    }
		 else if(ez->at(i)<2.2 && ez->at(i)>0)
		    { Hist2DXYnuclearslice2->Fill(ex->at(i),ey->at(i));	
		      NRcounts++;
		      Hist1DNRcountsslice2->Fill(NRcounts);
		    }
		 else if(ez->at(i)>-2.2 && ez->at(i)<0)
		    {  Hist2DXYnuclearslice3->Fill(ex->at(i),ey->at(i));	
		      NRcounts++;
		      Hist1DNRcountsslice3->Fill(NRcounts);		    
		    }
		}
	    }

	  if(ex->at(i)>-100 && ex->at(i)<0 && ey->at(i)<0.5 && ey->at(i)>-0.5)
            {
	      Hist2DGap->Fill(ex->at(i),ez->at(i));
	    }
        }

      if(IsNR)
	NNR++;
      if(IsER)
	NER++;
      if(ndeps==1)
	{ nsing++;
	  for(int a=0; a<ndeps; a++)
	    {
	      Hist2Dsingle->Fill(rx.at(a),ry.at(a));
	      Hist2DYZsingle->Fill(ry.at(a),rz.at(a));
	      if(rz.at(a)<4.6 && rz.at(a)>2.2)
		Hist2DXYsingleslice1->Fill(rx.at(a),ry.at(a));
	      if(rz.at(a)<2.2 && rz.at(a)>0)
		Hist2DXYsingleslice2->Fill(rx.at(a),ry.at(a));	  
	      if(rz.at(a)>-2.2 && rz.at(a)<0)
		Hist2DXYsingleslice3->Fill(rx.at(a),ry.at(a));	  
	    }
	}

      if(ndeps==1 && IsNR )
	{ 
	  for(int h=0; h<ndeps; h++)
	    {
	      Hist2Dsinglenuclear->Fill(nrx.at(h),nry.at(h));
	      Hist2DYZsinglenuclear->Fill(nry.at(h),nrz.at(h));
	      if(nrz.at(h)<4.6 && nrz.at(h)>2.2)
		Hist2DXYsinglenuclearslice1->Fill(nrx.at(h),nry.at(h));
	      if(nrz.at(h)<2.2 && nrz.at(h)>0)
		Hist2DXYsinglenuclearslice2->Fill(nrx.at(h),nry.at(h));	  
	      if(nrz.at(h)>-2.2 && nrz.at(h)<0)
		Hist2DXYsinglenuclearslice3->Fill(nrx.at(h),nry.at(h));	  
	    }
	}
    }
    
  cout<<"NNR= " <<NNR <<"     "<<"NER= "<<NER<<"  "<<"nsing=  "<<nsing<<endl;
  cout<<"eff= "<<eff<<endl;
  float leftend = -33.5;
  int leftendbin = Hist2DGap->GetXaxis()->FindBin(leftend);
  //    int leftendbin = 102;
  TH1D* py1 = Hist2DGap->ProjectionY("py1",leftendbin,leftendbin);
  
  int slicebin = Hist2DXYslice1->GetNbinsX();
  TH1D* Hist2DXYslice1ProjectionY = Hist2DXYslice1->ProjectionY("Hist2DXYslice1ProjectionY",0,slicebin);
  TH1D* Hist2DXYnuclearslice1ProjectionY = Hist2DXYnuclearslice1->ProjectionY("Hist2DXYnuclearslice1ProjectionY",0,slicebin);
  TH1D* Hist2DXYsingleslice1ProjectionY = Hist2DXYsingleslice1->ProjectionY("Hist2DXYsingleslice1ProjectionY",0,slicebin);
  TH1D* Hist2DXYsinglenuclearslice1ProjectionY = Hist2DXYsinglenuclearslice1->ProjectionY("Hist2DXYsinglenuclearslice1ProjectionY",0,slicebin);
    
  TH1D* Hist2DXYslice2ProjectionY = Hist2DXYslice2->ProjectionY("Hist2DXYslice2ProjectionY",0,slicebin);
  TH1D* Hist2DXYnuclearslice2ProjectionY = Hist2DXYnuclearslice2->ProjectionY("Hist2DXYnuclearslice2ProjectionY",0,slicebin);
  TH1D* Hist2DXYsingleslice2ProjectionY = Hist2DXYsingleslice2->ProjectionY("Hist2DXYsingleslice2ProjectionY",0,slicebin);
  TH1D* Hist2DXYsinglenuclearslice2ProjectionY = Hist2DXYsinglenuclearslice2->ProjectionY("Hist2DXYsinglenuclearslice2ProjectionY",0,slicebin);

  TH1D* Hist2DXYslice3ProjectionY = Hist2DXYslice3->ProjectionY("Hist2DXYslice3ProjectionY",0,slicebin);
  TH1D* Hist2DXYnuclearslice3ProjectionY = Hist2DXYnuclearslice3->ProjectionY("Hist2DXYnuclearslice3ProjectionY",0,slicebin);
  TH1D* Hist2DXYsingleslice3ProjectionY = Hist2DXYsingleslice3->ProjectionY("Hist2DXYsingleslice3ProjectionY",0,slicebin);
  TH1D* Hist2DXYsinglenuclearslice3ProjectionY = Hist2DXYsinglenuclearslice3->ProjectionY("Hist2DXYsinglenuclearslice3ProjectionY",0,slicebin);

  int slicebinX1 = Hist2DXYslice1->GetYaxis()->FindBin(-2);
  int slicebinX2 = Hist2DXYslice1->GetYaxis()->FindBin(2);
  TH1D* Hist2DXYslice1ProjectionX = Hist2DXYslice1->ProjectionX("Hist2DXYslice1ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYnuclearslice1ProjectionX = Hist2DXYnuclearslice1->ProjectionX("Hist2DXYnuclearslice1ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYsingleslice1ProjectionX = Hist2DXYsingleslice1->ProjectionX("Hist2DXYsingleslice1ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYsinglenuclearslice1ProjectionX = Hist2DXYsinglenuclearslice1->ProjectionX("Hist2DXYsinglenuclearslice1ProjectionX",slicebinX1,slicebinX2);
    
  TH1D* Hist2DXYslice2ProjectionX = Hist2DXYslice2->ProjectionX("Hist2DXYslice2ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYnuclearslice2ProjectionX = Hist2DXYnuclearslice2->ProjectionX("Hist2DXYnuclearslice2ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYsingleslice2ProjectionX = Hist2DXYsingleslice2->ProjectionX("Hist2DXYsingleslice2ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYsinglenuclearslice2ProjectionX = Hist2DXYsinglenuclearslice2->ProjectionX("Hist2DXYsinglenuclearslice2ProjectionX",slicebinX1,slicebinX2);

  TH1D* Hist2DXYslice3ProjectionX = Hist2DXYslice3->ProjectionX("Hist2DXYslice3ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYnuclearslice3ProjectionX = Hist2DXYnuclearslice3->ProjectionX("Hist2DXYnuclearslice3ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYsingleslice3ProjectionX = Hist2DXYsingleslice3->ProjectionX("Hist2DXYsingleslice3ProjectionX",slicebinX1,slicebinX2);
  TH1D* Hist2DXYsinglenuclearslice3ProjectionX = Hist2DXYsinglenuclearslice3->ProjectionX("Hist2DXYsinglenuclearslice3ProjectionX",slicebinX1,slicebinX2);

#ifdef Start_Plot    
  TCanvas *c1=new TCanvas("c1",label.c_str(),1000,600);
  Hist2D->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2D_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice1->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXYslice1_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice2->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXYslice2_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice3->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXYslice3_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice1ProjectionY->Draw();
  c1->SaveAs(Form("ambe_Hist2DXYslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice2ProjectionY->Draw();
  c1->SaveAs(Form("ambe_Hist2DXYslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice3ProjectionY->Draw();
  c1->SaveAs(Form("ambe_Hist2DXYslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice1ProjectionX->Draw();
  c1->SaveAs(Form("ambe_Hist2DXYslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice2ProjectionX->Draw();
  c1->SaveAs(Form("ambe_Hist2DXYslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice3ProjectionX->Draw();
  c1->SaveAs(Form("ambe_Hist2DXYslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  py1->Draw();
  c1->SaveAs(Form("ambe_Hist2DGapProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  c1->SetLogz();
  Hist2DGap->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DGap_%s%s.png",label.c_str(),Time.c_str()));
    
  TCanvas *c2=new TCanvas("c2",label.c_str(),1000,600);
  Hist2Dnuclear->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2Dnuclear_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice1->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice1_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice2->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice2_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice3->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice3_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice1ProjectionY->Draw();
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice2ProjectionY->Draw();
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice3ProjectionY->Draw();
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice1ProjectionX->Draw();
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice2ProjectionX->Draw();
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice3ProjectionX->Draw();
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
    
  TCanvas *c4=new TCanvas("c4",label.c_str(),1000,600);
  Hist2Dsinglenuclear->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2Dsinglenuclear_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice1->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice1_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice2->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice2_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice3->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice3_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice1ProjectionY->Draw();
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice2ProjectionY->Draw();
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice3ProjectionY->Draw();
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice1ProjectionX->Draw();
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice2ProjectionX->Draw();
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice3ProjectionX->Draw();
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DYZsinglenuclear->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DYZsinglenuclear_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c5=new TCanvas("c5",label.c_str(),1000,600);
  Hist2Dsingle->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2Dsingle_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice1->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice1_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice2->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice2_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice3->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice3_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice1ProjectionY->Draw();
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice2ProjectionY->Draw();
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice3ProjectionY->Draw();
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice1ProjectionX->Draw();
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice2ProjectionX->Draw();
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice3ProjectionX->Draw();
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DYZsingle->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DYZsingle_%s%s.png",label.c_str(),Time.c_str()));
#endif
*/
  /*  TCanvas *c1=new TCanvas("c1",label.c_str(),1000,600);
      c1->SetLogz();
  //  c1->SaveAs(Form("AmBe_yz_%s.png",label.c_str()));
  //without quenching cuts
  t->Draw("ey:ex>>Hist2D",tpcactive,"colz");
  //  c1->SaveAs(Form("ambe_Hist2D_%s%s.png",label.c_str(),Time.c_str()));
  t->Draw("ez:ey>>Hist2DYZ",tpcactive, "colz");
  //  c1->SaveAs(Form("ambe_Hist2DYZ_%s%s.png",label.c_str(),Time.c_str()));
  t->Draw("ez:ex>>Hist2DXZ",tpcactive, "colz");
  t->Draw("ey:ex>>Hist2DLAr",tpcactive && zslice, "colz");
  t->Draw("ey:ex>>Hist2DGap",gap && zslice, "colz");
  t->Draw("quenchingfactor>>QuenchingFactor",tpcactive);
  //quenching cuts for nuclear-like recoil
  t->Draw("ey:ex>>Hist2Dnuclear",tpcactive && quchcutn,"colz");
  t->Draw("ez:ey>>Hist2DYZnuclear",tpcactive && quchcutn, "colz");
  t->Draw("ez:ex>>Hist2DXZnuclear",tpcactive && quchcutn, "colz");
  t->Draw("ey:ex>>Hist2DLArnuclear",tpcactive && zslice && quchcutn, "colz");
  t->Draw("ey:ex>>Hist2DGapnuclear",quchcutn && gap && zslice, "colz");
  //quenching cuts for electron-like recoil
  t->Draw("ey:ex>>Hist2Delectron",tpcactive && quchcute,"colz");
  t->Draw("ez:ey>>Hist2DYZelectron",tpcactive && quchcute, "colz");
  t->Draw("ez:ex>>Hist2DXZelectron",tpcactive && quchcute, "colz");
  t->Draw("ey:ex>>Hist2DLArelectron",tpcactive && zslice && quchcute, "colz");
  t->Draw("ey:ex>>Hist2DGapelectron",quchcute && gap && zslice, "colz");
 */

    /*  string outdirname="/darkside/users/hqian/neutron0G163tiltlow/cluster_data/";
  //  string outdirname="/ds50/data/user/hqian36/Collimator/neutron0G163tilt/cluster_data/";
       
  string outputname=outdirname+"nuclear0G163tilt_recoil_"+label+Time+"_clustered"+".root";  
  TFile f2D(outputname.c_str(), "RECREATE");
  Hist2D->Write(); 
  Hist2DXYslice1->Write();
  Hist2DXYslice2->Write();
  Hist2DXYslice3->Write();
  Hist2DXYslice1ProjectionY->Write();
  Hist2DXYslice2ProjectionY->Write();
  Hist2DXYslice3ProjectionY->Write();
  Hist2DXYslice1ProjectionX->Write();
  Hist2DXYslice2ProjectionX->Write();
  Hist2DXYslice3ProjectionX->Write();
  Hist2DGap->Write();
  py1->Write();
  QuenchingFactor->Write();
  Hist2Dnuclear->Write(); 
  Hist2DXYnuclearslice1->Write();
  Hist2DXYnuclearslice2->Write();
  Hist2DXYnuclearslice3->Write();
  Hist2DXYnuclearslice1ProjectionY->Write();
  Hist2DXYnuclearslice2ProjectionY->Write();
  Hist2DXYnuclearslice3ProjectionY->Write();
  Hist2DXYnuclearslice1ProjectionX->Write();
  Hist2DXYnuclearslice2ProjectionX->Write();
  Hist2DXYnuclearslice3ProjectionX->Write();
  Hist1DNRcountsslice1->Write();
  Hist1DNRcountsslice2->Write();
  Hist1DNRcountsslice3->Write();
  Hist1DNRcountsslice4->Write();
  Hist2Dsinglenuclear->Write();
  Hist2DYZsinglenuclear->Write();
  Hist2DXYsinglenuclearslice1->Write();
  Hist2DXYsinglenuclearslice2->Write();
  Hist2DXYsinglenuclearslice3->Write();
  Hist2DXYsinglenuclearslice1ProjectionY->Write();
  Hist2DXYsinglenuclearslice2ProjectionY->Write();
  Hist2DXYsinglenuclearslice3ProjectionY->Write();
  Hist2DXYsinglenuclearslice1ProjectionX->Write();
  Hist2DXYsinglenuclearslice2ProjectionX->Write();
  Hist2DXYsinglenuclearslice3ProjectionX->Write();
  Hist2Dsingle->Write();
  Hist2DYZsingle->Write();
  Hist2DXYsingleslice1->Write();
  Hist2DXYsingleslice2->Write();
  Hist2DXYsingleslice3->Write();
  Hist2DXYsingleslice1ProjectionY->Write();
  Hist2DXYsingleslice2ProjectionY->Write();
  Hist2DXYsingleslice3ProjectionY->Write();
  Hist2DXYsingleslice1ProjectionX->Write();
  Hist2DXYsingleslice2ProjectionX->Write();
  Hist2DXYsingleslice3ProjectionX->Write();
  f2D.Write();
  f2D.Close();
}
*/



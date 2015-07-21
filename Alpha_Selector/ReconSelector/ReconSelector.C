#define ReconSelector_cxx
// The class definition in ReconSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("ReconSelector.C")
// Root > T->Process("ReconSelector.C","some options")
// Root > T->Process("ReconSelector.C+")
//
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "ReconSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TBox.h>
using namespace std;

const string Time = "_Jan1AM";
//const int datafiles=9;
//int Volume = 9;

void ReconSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void ReconSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
  Info("SlaveBegin()","beginning .....");
  TString option = GetOption();
  BookHistograms();
  
}

Bool_t ReconSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ReconSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
  Int_t chainentry = fChain->GetChainEntryNumber(entry);
  if(chainentry%(fChain->GetEntries()/10)==0) printf("Processing Entry number %ld [%ld %% of %lld]\n", (long int)chainentry, (long int)(chainentry/(fChain->GetEntries()/100)), fChain->GetEntries());
  fChain->GetEntry(entry);
  ++fNumberOfEvents;
  FillHistograms();
  return kTRUE;
}

void ReconSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ReconSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  string label = GetOption();
  TList* list = GetOutputList();

  first_tpc_ER_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("first_tpc_ER_time_hist")));
  tpc_NR_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("tpc_NR_time_hist")));
  tpc_gamma_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("tpc_gamma_energy_hist")));
  tpc_quenching_factor_hist = dynamic_cast<TH1F*>(list->FindObject(Form("tpc_quenching_factor_hist")));
  nv_prompt_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_prompt_energy_hist")));
  nv_prompt_eof_energy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_prompt_eof_energy_hist")));
  nv_prompt_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_prompt_time_hist")));
  nv_prompt_radius_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_prompt_radius_hist")));
  energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("energy_hist")));
  contain_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("contain_energy_hist")));
  contain_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("contain_time_hist")));
  nv_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_quchenergy_hist")));
  nv_eof_quchenergy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_eof_quchenergy_hist")));
  nv_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_time_hist")));
  nv_residual_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_residual_energy_hist")));
  nv_residual_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_residual_quchenergy_hist")));
  nv_residual_eof_quchenergy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_residual_eof_quchenergy_hist")));
  nv_residual_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_residual_time_hist")));
  nv_residual_XY_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_residual_XY_hist")));
  nv_small1residual_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_small1residual_energy_hist")));
  nv_small1residual_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_small1residual_quchenergy_hist")));
  nv_small1residual_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_small1residual_time_hist")));
  tpc_puregamma_qenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("tpc_puregamma_qenergy_hist")));
  tpc_puregamma_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("tpc_puregamma_time_hist")));
  tpc_puregamma_eof_qenergy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("tpc_puregamma_eof_qenergy_hist")));
  tpc_puregamma_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("tpc_puregamma_ntuple")));
  nv_first_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_first_quchenergy_hist")));
  nv_first_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_first_energy_hist")));
  nv_first_eof_quchenergy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_first_eof_quchenergy_hist")));
  nv_first_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_first_time_hist")));
  nv_first_XY_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_first_XY_hist")));
  nv_thermal_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_energy_hist")));
  nv_thermal_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_quchenergy_hist")));
  nv_thermal_eof_quchenergy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_thermal_eof_quchenergy_hist")));
  nv_thermal_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_time_hist")));
  nv_thermal_XY_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_thermal_XY_hist")));

  nv_thermal_long_eqch_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_long_eqch_hist")));
  nv_thermal_long_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_thermal_long_ntuple")));
  nv_thermal_long_eof_eqch_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_thermal_long_eof_eqch_hist")));
  nv_thermal_long_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_long_time_hist")));
  nv_thermal_long_XY_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_thermal_long_XY_hist")));
  nv_thermal_long_alpha_radius_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_long_alpha_radius_hist")));
  nv_thermal_long_gamma_radius_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_thermal_long_gamma_radius_hist")));

  nv_remain_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_remain_energy_hist")));
  nv_remain_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_remain_quchenergy_hist")));
  nv_remain_eof_quchenergy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_remain_eof_quchenergy_hist")));
  nv_remain_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_remain_time_hist")));
  nv_remain_XY_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_remain_XY_hist")));
  nv_long_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_long_time_hist")));
  nv_long_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_long_energy_hist")));
  nv_long_eof_energy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_long_eof_energy_hist")));
  nv_alpha_capture_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_alpha_capture_time_hist")));
  nv_alpha_eof_energy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_alpha_eof_energy_hist")));
  nv_alpha_energy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_alpha_energy_hist")));
  nv_alpha_qenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_alpha_qenergy_hist")));
  nv_alpha_prompt_eof_energy_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_alpha_prompt_eof_energy_hist")));
  nv_alpha_prompt_quchenergy_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_alpha_prompt_quchenergy_hist")));
  nv_alpha_prompt_time_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_alpha_prompt_time_hist")));
  id = dynamic_cast<TH1F*>(list->FindObject(Form("id")));
  tpc_gamma_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("tpc_gamma_ntuple")));
  tpc_n_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("tpc_n_ntuple")));
  nv_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_ntuple")));
  all_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("all_ntuple")));
  nv_alpha_capture_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_alpha_capture_ntuple")));
  nv_thermal_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_thermal_ntuple")));
  nv_first_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_first_ntuple")));

  /*  TH1F* nv_pdf_hist = new TH1F("nv_pdf_hist","pdf;Energy [keV];Probability",15000,0,15000);
  TH1F* nv_pdf_fine_hist = new TH1F("nv_pdf_fine_hist","pdf;Energy [keV];Probability",250,0,250);
  TH1F* nv_ER_pdf_hist = new TH1F("nv_ER_pdf_hist","Gamma pdf;Energy [keV];Probability",25000,0,25000);
  TH1F* nv_ER_pdf_fine_hist = new TH1F("nv_ER_pdf_fine_hist","Gamma pdf;Energy [keV];Probability",250,0,250);
  */
  first_tpc_ER_time_hist->Sumw2();
  tpc_NR_time_hist->Sumw2();
  tpc_gamma_energy_hist->Sumw2();
  tpc_quenching_factor_hist->Sumw2();
  nv_prompt_time_hist->Sumw2();
  nv_prompt_energy_hist->Sumw2();
  nv_prompt_radius_hist->Sumw2();
  energy_hist->Sumw2();
  contain_energy_hist->Sumw2();
  contain_time_hist->Sumw2();
  nv_alpha_capture_time_hist->Sumw2();
  nv_alpha_energy_hist->Sumw2();                                                                                           
  nv_alpha_qenergy_hist->Sumw2();                                                                                           
  nv_quchenergy_hist->Sumw2();                                                                                        
  nv_time_hist->Sumw2();
  nv_residual_energy_hist->Sumw2();                                                                                        
  nv_residual_quchenergy_hist->Sumw2();                                                                                        
  nv_residual_time_hist->Sumw2();
  tpc_puregamma_time_hist->Sumw2();
  tpc_puregamma_qenergy_hist->Sumw2();
  nv_small1residual_energy_hist->Sumw2();                                                                                        
  nv_small1residual_quchenergy_hist->Sumw2();                                                                                        
  nv_small1residual_time_hist->Sumw2();
  nv_thermal_energy_hist->Sumw2();                                                                                        
  nv_thermal_quchenergy_hist->Sumw2();                                                                                        
  nv_thermal_time_hist->Sumw2();
  //  nv_thermal_long_eqch_hist->Sumw2();                                                                                        
  nv_thermal_long_time_hist->Sumw2();
  nv_first_energy_hist->Sumw2();                                                                                        
  nv_first_quchenergy_hist->Sumw2();                                                                                        
  nv_first_time_hist->Sumw2();
  nv_remain_energy_hist->Sumw2();                                                                                        
  nv_remain_quchenergy_hist->Sumw2();                                                                                        
  nv_remain_time_hist->Sumw2();
  nv_long_time_hist->Sumw2();
  nv_long_energy_hist->Sumw2();
  nv_alpha_prompt_quchenergy_hist->Sumw2();
  nv_alpha_prompt_time_hist->Sumw2();
  
  //  nv_thermal_long_eqch_hist->Rebin(4);

  //Normalization
  //  double events =1.; 
  double events = id->Integral();
    //fNumberOfEvents;
    //fChain->GetEntries();
  if(events>0)
    {
      first_tpc_ER_time_hist->Scale(1./events);
      tpc_NR_time_hist->Scale(1./events);
      tpc_gamma_energy_hist->Scale(1./events);
      tpc_quenching_factor_hist->Scale(1./events);
      nv_prompt_time_hist->Scale(1./events);
      nv_prompt_energy_hist->Scale(1./events);
      nv_prompt_radius_hist->Scale(1./events);
      energy_hist->Scale(1./events);
      contain_energy_hist->Scale(1./events);
      contain_time_hist->Scale(1./events);
      nv_alpha_capture_time_hist->Scale(1./events);
      nv_alpha_energy_hist->Scale(1./events);                                                                                    
      nv_alpha_qenergy_hist->Scale(1./events);                                                                                    
      nv_quchenergy_hist->Scale(1./events);                                                                                        
      nv_time_hist->Scale(1./events);
      nv_residual_energy_hist->Scale(1./events);                                                                                    
      nv_residual_quchenergy_hist->Scale(1./events);                                                                                
      nv_residual_time_hist->Scale(1./events);
      nv_small1residual_energy_hist->Scale(1./events);                                                                               
      nv_small1residual_quchenergy_hist->Scale(1./events);                                
      nv_small1residual_time_hist->Scale(1./events);
      nv_thermal_energy_hist->Scale(1./events);                                                                                     
      nv_thermal_quchenergy_hist->Scale(1./events);                                                                                 
      nv_thermal_time_hist->Scale(1./events);
      nv_thermal_long_eqch_hist->Scale(1./events);                                                                                   
      nv_thermal_long_time_hist->Scale(1./events);                                                                                 
      nv_first_energy_hist->Scale(1./events);                                                                                        
      nv_first_quchenergy_hist->Scale(1./events);                                                                                    
      nv_first_time_hist->Scale(1./events);
      nv_remain_energy_hist->Scale(1./events);                                                                                      
      nv_remain_quchenergy_hist->Scale(1./events);                                                                                   
      nv_remain_time_hist->Scale(1./events);
      nv_long_time_hist->Scale(1./events);
      nv_long_energy_hist->Scale(1./events);
      nv_alpha_prompt_quchenergy_hist->Scale(1./events);
      nv_alpha_prompt_time_hist->Scale(1./events);
    }
  //  Calculate_pdf(nv_energy_hist,nv_pdf_hist,nv_pdf_fine_hist);
  // Calculate_pdf(nv_ER_energy_hist,nv_ER_pdf_hist,nv_ER_pdf_fine_hist);
  //  TCanvas *c1 = new TCanvas("c1","Alpha Quenching Energy",600,400);

  double pos1 = 350.;
  double pos2 = 470.;
  double pos3 = 30.;
  double pos4 = 60.;

  double height = 0.04;
  TBox *box1 = new TBox(pos3,0,pos4,height);
  box1->SetLineColor(2);
  box1->SetLineWidth(1);

  TBox *box2 = new TBox(pos1,0,pos2,height);
  box2->SetLineColor(3);
  box2->SetLineWidth(1);
  
  TCanvas *c2 = new TCanvas("c2","Thermal long small ene",600,400);
  gPad->SetLogy();
  nv_thermal_long_eqch_hist->Draw();
  box1->Draw();
  box2->Draw();
  double br1 = nv_thermal_long_eqch_hist->Integral(nv_thermal_long_eqch_hist->FindBin(pos3),nv_thermal_long_eqch_hist->FindBin(pos4));
  double br2 = nv_thermal_long_eqch_hist->Integral(nv_thermal_long_eqch_hist->FindBin(pos3),nv_thermal_long_eqch_hist->FindBin(pos2));
  cout<<"Thermal Long Branch Ratio= "<<1.0*br1/br2<<endl;

  TCanvas *c3 = new TCanvas("c3","First small ene",600,400);
  gPad->SetLogy();
  nv_first_quchenergy_hist->Draw();
  box1->Draw();
  box2->Draw();
  double br3 = nv_first_quchenergy_hist->Integral(nv_first_quchenergy_hist->FindBin(pos3),nv_first_quchenergy_hist->FindBin(pos4));
  double br4 = nv_first_quchenergy_hist->Integral(nv_first_quchenergy_hist->FindBin(pos3),nv_first_quchenergy_hist->FindBin(pos2));
  cout<<"First Branch Ratio= "<<1.0*br3/br4<<endl;
  
  //  string outdirname="/cache/shared/darkside/hqian/TMBfraction/coincidence_data/";
  string outdirname="/darkside/users/hqian/TMBfraction/results/";
  //  string outputname=outdirname+"tmb_alpha_"+label+Time+".root";
  string outputname=outdirname+label+".root";
  //  cout<<"Output Data: "<<outputname<<endl;
  TFile f2D(outputname.c_str(), "RECREATE");
  id->Write();
  tpc_gamma_ntuple->Write();
  tpc_n_ntuple->Write();
  nv_ntuple->Write();
  all_ntuple->Write();
  nv_alpha_capture_ntuple->Write();
  first_tpc_ER_time_hist->Write();
  tpc_NR_time_hist->Write();
  tpc_gamma_energy_hist->Write();
  tpc_quenching_factor_hist->Write();
  energy_hist->Write();
  nv_time_hist->Write();
  nv_quchenergy_hist->Write();
  nv_eof_quchenergy_hist->Write();
  nv_residual_time_hist->Write();
  nv_residual_energy_hist->Write();
  nv_residual_quchenergy_hist->Write();
  nv_residual_eof_quchenergy_hist->Write();
  nv_residual_XY_hist->Write();
  nv_small1residual_time_hist->Write();
  nv_small1residual_energy_hist->Write();
  nv_small1residual_quchenergy_hist->Write();
  tpc_puregamma_qenergy_hist->Write();
  tpc_puregamma_time_hist->Write();
  tpc_puregamma_eof_qenergy_hist->Write();
  tpc_puregamma_ntuple->Write();
  nv_first_time_hist->Write();
  nv_first_quchenergy_hist->Write();
  nv_first_energy_hist->Write();
  nv_first_eof_quchenergy_hist->Write();
  nv_first_XY_hist->Write();
  nv_first_ntuple->Write();
  nv_thermal_time_hist->Write();
  nv_thermal_energy_hist->Write();
  nv_thermal_quchenergy_hist->Write();
  nv_thermal_eof_quchenergy_hist->Write();
  nv_thermal_XY_hist->Write();
  nv_thermal_ntuple->Write();
  nv_thermal_long_time_hist->Write();
  nv_thermal_long_ntuple->Write();
  nv_thermal_long_eqch_hist->Write();
  nv_thermal_long_eof_eqch_hist->Write();
  nv_thermal_long_XY_hist->Write();
  nv_thermal_long_alpha_radius_hist->Write();
  nv_thermal_long_gamma_radius_hist->Write();

  nv_remain_time_hist->Write();
  nv_remain_energy_hist->Write();
  nv_remain_quchenergy_hist->Write();
  nv_remain_eof_quchenergy_hist->Write();
  nv_remain_XY_hist->Write();
  contain_energy_hist->Write();
  contain_time_hist->Write();
  //  nv_pdf_hist->Write();
  // nv_pdf_fine_hist->Write();
  // nv_ER_pdf_hist->Write();
  // nv_ER_pdf_fine_hist->Write();
  nv_prompt_time_hist->Write();
  nv_prompt_energy_hist->Write();
  nv_prompt_radius_hist->Write();
  nv_prompt_eof_energy_hist->Write();
  nv_long_time_hist->Write();
  nv_long_energy_hist->Write();
  nv_long_eof_energy_hist->Write();
  nv_alpha_capture_time_hist->Write();
  nv_alpha_eof_energy_hist->Write();
  nv_alpha_energy_hist->Write();
  nv_alpha_qenergy_hist->Write();
  nv_alpha_prompt_eof_energy_hist->Write();
  nv_alpha_prompt_quchenergy_hist->Write();
  nv_alpha_prompt_time_hist->Write();
  c2->Write();
  c3->Write();
  f2D.Write();
  f2D.Close();

  Info("Terminate()","terminating ...%s....%f",outputname.c_str(),events);

}

void ReconSelector::FillHistograms()
{
  double quenchingcut = 0.43;
  double dep_enecut = 23.0; //keV                                                                                              
  double dep_enecut2 = 269.0; //keV                                                                                             
  double dep_enecut3 = 600.;//keV
  double prompt_window = -100.; //ns
  double delay_window = 500.;
  double long_start = -1.e+3;
  double gamma_window = 7.e+4;
  double thermal_window = 2.e+2;
  double remain_start = -500;
  double remain_end = 500;
  vector<double> TPCNR_Time, TPCER_Time, TPC_Time, TPC_Eqch;
  vector<double> NVA_Time, NVA_Edep, NVA_Eqch;
  vector<double> NV_Time, NV_Edep,NV_QEdep, NV_Radius;
  vector<double> NV_X, NV_Y,NV_Z, TPCER_Eqch;
  vector<double> TPCER_X, TPCER_Y,TPCER_Z, TPCER_Edep;
  
  id->Fill(0);
  bool veto_gamma_trigger=false;

  for(size_t j=0; j<et->size(); j++)
    { energy_hist->Fill(eqch->at(j));
      if(volume->at(j)=="p_active")
	all_ntuple->Fill(edep->at(j),eqch->at(j),et->at(j)*1.e+9,ex->at(j),ey->at(j),ez->at(j),quenchingfactor->at(j),1);
      else if(volume->at(j)=="p_scint")
	all_ntuple->Fill(edep->at(j),eqch->at(j),et->at(j)*1.e+9,ex->at(j),ey->at(j),ez->at(j),quenchingfactor->at(j),2);

      if(volume->at(j)=="p_active")
	{ tpc_quenching_factor_hist->Fill(quenchingfactor->at(j));
	  TPC_Time.push_back(et->at(j)*1.e+9);
	  TPC_Eqch.push_back(eqch->at(j));      
	  tpc_n_ntuple->Fill(edep->at(j),eqch->at(j),et->at(j)*1.e+9);
	  //	  if(quenchingfactor->at(j) < quenchingcut && eqch->at(j)>dep_enecut && eqch->at(j)<dep_enecut2 )
	  if(quenchingfactor->at(j) < quenchingcut)
	    { TPCNR_Time.push_back(et->at(j)*1.e+9);
	      tpc_NR_time_hist->Fill(et->at(j)*1.e+9);
	    }
	  if(quenchingfactor->at(j) > quenchingcut)
	    {
	      tpc_gamma_ntuple->Fill(edep->at(j),eqch->at(j),et->at(j)*1.e+9);
	      tpc_gamma_energy_hist->Fill(eqch->at(j));
	      //      if(eqch->at(j)>dep_enecut3)
	      //     if(edep->at(j)>0)
	      if(edep->at(j)>dep_enecut3)
		{
		  TPCER_Time.push_back(et->at(j)*1.e+9);
		  TPCER_Eqch.push_back(eqch->at(j));
		  TPCER_Edep.push_back(edep->at(j));
		  TPCER_X.push_back(ex->at(j));
		  TPCER_Y.push_back(ey->at(j));
		  TPCER_Z.push_back(ez->at(j));		  
		}
	    }
	}
      if(volume->at(j)=="p_scint")
	{ NV_Time.push_back(et->at(j)*1.e+9);
	  NV_QEdep.push_back(eqch->at(j));
	  NV_Edep.push_back(edep->at(j));
	  double radius = TMath::Sqrt(TMath::Power(ex->at(j),2)+TMath::Power(ey->at(j),2)+TMath::Power(ez->at(j),2));
	  NV_Radius.push_back(radius);
	  NV_X.push_back(ex->at(j));
	  NV_Y.push_back(ey->at(j)); 
	  NV_Z.push_back(ez->at(j)); 
	  /*  if((et->at(j)*1.e+9)<thermal_window)
	    { nv_thermal_quchenergy_hist->Fill(eqch->at(j)); 
	      nv_thermal_time_hist->Fill(et->at(j)*1.e+9);                                              
	      nv_thermal_eof_quchenergy_hist->Fill(et->at(j)*1.e+9,eqch->at(j));                                              
	    }
	  */	  
	  if(eqch->at(j)>3500. && eqch->at(j)<5000. && et->at(j)*1.e+9<20.e+3)
	    veto_gamma_trigger = true;

	  nv_ntuple->Fill(edep->at(j),eqch->at(j),et->at(j)*1.e+9);
	  nv_quchenergy_hist->Fill(eqch->at(j)); 
	  nv_time_hist->Fill(et->at(j)*1.e+9);                                              
	  nv_eof_quchenergy_hist->Fill(et->at(j)*1.e+9,eqch->at(j));                                              
	  
	  if(contain_alpha->at(j))
	    { contain_energy_hist->Fill(eqch->at(j));
	      contain_time_hist->Fill(et->at(j)*1.e+9);
	      NVA_Time.push_back(et->at(j)*1.e+9);
	      NVA_Edep.push_back(edep->at(j));
	      NVA_Eqch.push_back(eqch->at(j));
	    }
	}
    } //end of loop over edeps for each event       
  if(NV_Time.size())
    { nv_alpha_capture_time_hist->Fill(NV_Time.back());                       
      nv_alpha_energy_hist->Fill(NV_Edep.back());
      nv_alpha_qenergy_hist->Fill(NV_QEdep.back());
      nv_alpha_eof_energy_hist->Fill(NV_Time.back(),NV_QEdep.back());
      nv_alpha_capture_ntuple->Fill(NV_Edep.back(),NV_QEdep.back(),NV_Time.back(),NV_X.back(),NV_Y.back(),NV_Z.back());
    }
  if(NV_Time.size() && veto_gamma_trigger)
    {
      for(size_t h=0; h<NV_Time.size(); h++)
	{
	  if(NV_Time.at(h)>20.e+3 && NV_Time.at(h)<60.e+3)
	    {
	      nv_thermal_long_eqch_hist->Fill(NV_QEdep.at(h));
	      nv_thermal_long_time_hist->Fill(NV_Time.at(h));
	      nv_thermal_long_XY_hist->Fill(NV_X.at(h),NV_Y.at(h));
	      nv_thermal_long_eof_eqch_hist->Fill(NV_Time.at(h),NV_QEdep.at(h));
	      nv_thermal_long_ntuple->Fill(NV_Edep.at(h),NV_QEdep.at(h),NV_Time.at(h),NV_X.at(h),NV_Y.at(h),NV_Z.at(h));
	      //  double radius = TMath::Sqrt(NV_X.at(h)*NV_X.at(h) +NV_Y.at(h)*NV_Y.at(h) + NV_Z.at(h)*NV_Z.at(h));
	      if(NV_Z.at(h)<60. && NV_Z.at(h)>-50.){
		double radius = TMath::Sqrt(NV_X.at(h)*NV_X.at(h) +NV_Y.at(h)*NV_Y.at(h))-32.1;
		if(NV_QEdep.at(h)>30. && NV_QEdep.at(h)<60.)
		  nv_thermal_long_alpha_radius_hist->Fill(radius);
		if(NV_QEdep.at(h)>350. && NV_QEdep.at(h)<470.)
		  nv_thermal_long_gamma_radius_hist->Fill(radius);
	      }
	    }
	}
    } 

  if(NV_Time.size() && TPCER_Time.size())
    { double first_tpc_ER_time = TPCER_Time.front();
      first_tpc_ER_time_hist->Fill(first_tpc_ER_time);
      for(size_t h=0; h<NV_Time.size(); h++)
	{ double time_diff = NV_Time.at(h) - first_tpc_ER_time;
	  if(time_diff>prompt_window && time_diff<delay_window)
	    {
	      nv_prompt_radius_hist->Fill(NV_Radius.at(h));
	      nv_prompt_energy_hist->Fill(NV_QEdep.at(h));
	      nv_prompt_time_hist->Fill(time_diff);
	      nv_prompt_eof_energy_hist->Fill(time_diff,NV_QEdep.at(h));
	    }
	  /*	    {
	      nv_prompt_radius_hist->Fill(NV_Radius.front());
	      nv_prompt_energy_hist->Fill(NV_QEdep.front());
	      nv_prompt_time_hist->Fill(time_diff);
	      nv_prompt_eof_energy_hist->Fill(time_diff,NV_QEdep.front());
	    }
	  */
	  double ER_time_diff = NV_Time.at(h) - TPCER_Time.front();
	  if(ER_time_diff>long_start && ER_time_diff<gamma_window)
	    { nv_long_energy_hist->Fill(NV_QEdep.at(h));
	      nv_long_time_hist->Fill(ER_time_diff);	
	      nv_long_eof_energy_hist->Fill(ER_time_diff,NV_QEdep.at(h));
	    }

	}
    }
  
  if(!TPCNR_Time.size() && TPCER_Time.size() && NV_Time.size())
    {  for(size_t h=0; h<NV_Time.size(); h++)
	{
	  double time_diff = NV_Time.at(h)-TPCER_Time.front();
	  //    double long_start = -1.e+3;
	  //    double gamma_window = 7.e+4;
	  if(time_diff>0 && time_diff<gamma_window)
	    //	  if(time_diff>long_start && time_diff<gamma_window)
	  //	  if(NV_Time.at(h)<long_start)
	    {
	      nv_residual_energy_hist->Fill(NV_Edep.at(h));
	      nv_residual_quchenergy_hist->Fill(NV_QEdep.at(h));
	      nv_residual_time_hist->Fill(time_diff);     
	      //	      nv_residual_time_hist->Fill(NV_Time.at(h));     
	      nv_residual_eof_quchenergy_hist->Fill(time_diff,NV_QEdep.at(h));
	      nv_residual_XY_hist->Fill(NV_X.at(h),NV_Y.at(h));
	    }
	  double small1 = 200;
	  if(time_diff>0 && time_diff<small1)
	  //	  if(NV_Time.at(h)<long_start)
	    {
	      nv_small1residual_energy_hist->Fill(NV_Edep.at(h));
	      nv_small1residual_quchenergy_hist->Fill(NV_QEdep.at(h));
	      nv_small1residual_time_hist->Fill(time_diff);     
	    }	  	  	  
	}
      for(size_t h=0; h<TPCER_Time.size();h++)
	{
	  tpc_puregamma_qenergy_hist->Fill(TPCER_Eqch.at(h));
	  tpc_puregamma_time_hist->Fill(TPCER_Time.at(h));
	  tpc_puregamma_eof_qenergy_hist->Fill(TPCER_Time.at(h),TPCER_Eqch.at(h));
	  tpc_puregamma_ntuple->Fill(TPCER_Time.at(h),TPCER_Eqch.at(h),TPCER_X.at(h),TPCER_Y.at(h),TPCER_Z.at(h));
	}

      double time_diff = NV_Time.front()-TPCER_Time.front();
      //      if(time_diff>long_start && time_diff<gamma_window)
      double small2 = 200;
      if(time_diff>0 && time_diff<small2)
	{	
	  nv_first_quchenergy_hist->Fill(NV_QEdep.front()); 
	  nv_first_energy_hist->Fill(NV_Edep.front()); 
	  nv_first_time_hist->Fill(time_diff);                                              
	  //	      nv_first_time_hist->Fill(NV_Time.front());                                              
	  nv_first_eof_quchenergy_hist->Fill(time_diff,NV_QEdep.front());
	  //  nv_first_eof_quchenergy_hist->Fill(time_diff,NV_Edep.front());
	  nv_first_XY_hist->Fill(NV_X.front(),NV_Y.front());
	  nv_first_ntuple->Fill(NV_Edep.front(),NV_QEdep.front(),NV_Time.front(),NV_X.front(),NV_Y.front(),NV_Z.front());
	}
    } 

  if(!TPC_Time.size() && NV_Time.size())
    {  for(size_t h=0; h<NV_Time.size(); h++)
	{
	  //    double thermal_window = 2.e+2;
	  if(NV_Time.at(h)<thermal_window)
	    // double time_diff = NV_Time.at(h)-TPCNR_Time.front();
	    //  if(time_diff < remain_end && time_diff > remain_start)
	    {
	      nv_thermal_energy_hist->Fill(NV_Edep.at(h)); 
	      nv_thermal_quchenergy_hist->Fill(NV_QEdep.at(h)); 
	      nv_thermal_time_hist->Fill(NV_Time.at(h));                                              
	      nv_thermal_eof_quchenergy_hist->Fill(NV_Time.at(h),NV_QEdep.at(h));
	      nv_thermal_XY_hist->Fill(NV_X.at(h),NV_Y.at(h));
	      nv_thermal_ntuple->Fill(NV_Edep.at(h),NV_QEdep.at(h),NV_Time.at(h),NV_X.at(h),NV_Y.at(h),NV_Z.at(h));
	    }
	}
    }

  if(TPCNR_Time.size() && !TPCER_Time.size() && NV_Time.size())
    {  for(size_t h=0; h<NV_Time.size(); h++)
	{
	  double time_diff = NV_Time.at(h)-TPCNR_Time.front();
	  //    double remain_start = -500;
	  //    double remain_end = 500;
	  //	  if(time_diff < remain_end && time_diff > remain_start && NV_Edep.at(h)<90.9)
	  if(time_diff < remain_end && time_diff > 0 && NV_Edep.at(h)<90.9)
	    {
	      nv_remain_energy_hist->Fill(NV_Edep.at(h)); 
	      nv_remain_quchenergy_hist->Fill(NV_QEdep.at(h)); 
	      nv_remain_time_hist->Fill(time_diff);                                              
	      nv_remain_eof_quchenergy_hist->Fill(time_diff,NV_QEdep.at(h));
	      nv_remain_XY_hist->Fill(NV_X.at(h),NV_Y.at(h));
	    }
	}
      /*
	for()
      { TPC_gamma_energy_hist->Fill(TPCER_Eqch.at(h));
	TPC_gamma_time_hist->Fill(TPCER_Time.at(h));	
	TPC_gamma_eof_energy_hist->Fill(ER_time_diff,NV_QEdep.at(h));
      }
      */
    }

  if(NVA_Time.size() && TPCER_Time.size())
    {  for(size_t h=0; h<NVA_Time.size(); h++)
	{ double NVA_time_diff = NVA_Time.at(h) - TPCER_Time.front();
	  if(NVA_time_diff>prompt_window && NVA_time_diff<delay_window)
	    { nv_alpha_prompt_quchenergy_hist->Fill(NVA_Eqch.at(h));
	      nv_alpha_prompt_time_hist->Fill(NVA_time_diff);     
	      nv_alpha_prompt_eof_energy_hist->Fill(NVA_time_diff,NVA_Eqch.at(h));
  	    }
	}
    } 
  /* if(TPC_Time.size() && NV_Time.size())
    {  for(size_t h=0; h<NV_Time.size(); h++)
	{ double NV_time_diff = NV_Time.at(h) - TPC_Time.front();
	  if(NV_time_diff>long_start && NV_time_diff<gamma_window)
	    { nv_alpha_prompt_quchenergy_hist->Fill(NV_QEdep.at(h));
	      nv_alpha_prompt_time_hist->Fill(NV_time_diff);     
	      nv_alpha_prompt_eof_energy_hist->Fill(NV_time_diff,NV_QEdep.at(h));
  	    }
	}
    } 
*/
}

void ReconSelector::BookHistograms()
{
  Info("ReconSelector::BookHistograms()","creating histograms...");  
  string label = GetOption();
  TList* list = GetOutputList();
  
  first_tpc_ER_time_hist = new TH1F("first_tpc_ER_time_hist","Time of the first ER in TPC;time [ns]",12500,0,1.e+6);
  tpc_gamma_energy_hist = new TH1F("tpc_gamma_energy_hist","Gamma Energy in TPC; Energy [keV]",1000,0,4000);
  //  tpc_gamma_energy_hist = new TH1F("tpc_gamma_energy_hist","Gamma Energy in TPC; Energy [keV]",200,0,200);
  tpc_NR_time_hist = new TH1F("tpc_NR_time_hist","Time of the NR in TPC;time [ns]",12500,0,1.e+6);
  tpc_quenching_factor_hist = new TH1F("tpc_quenching_factor_hist","Quenching Factor in TPC",1000,0,1);
  energy_hist = new TH1F("energy_hist","Quenching Energy in all volumes;Energy [KeV]",1000,0,10000);
  contain_energy_hist = new TH1F("contain_energy_hist","Quenching Energy (contain alpha) in NV;Energy [KeV]",1000,0,10000);
  contain_time_hist = new TH1F("contain_time_hist","Time (contain alpha) in NV;Time [ns]",1000,0,1.e+6);
  nv_prompt_energy_hist = new TH1F("nv_prompt_energy_hist","Prompt Energy in NV;energy [keV]",15000,0,15000);
  nv_prompt_time_hist = new TH1F("nv_prompt_time_hist","Prompt Time in NV;time [ns]",15000,-100,500);//-1.e+3,1.e+6);
  nv_prompt_radius_hist = new TH1F("nv_prompt_radius_hist","Prompt Radius in NV;radius [cm]",1500,0,200);
  nv_prompt_eof_energy_hist = new TH2F("nv_prompt_eof_energy_hist","Prompt Time vs Energy in NV;Time[ns];energy [keV]",1500,-100,500,15000,0,15000);
  nv_long_time_hist = new TH1F("nv_long_time_hist","Time long in NV;time [ns]",15000,-1.e+3,7.e+4); //-1.e+3,1.e+6);
  nv_long_energy_hist = new TH1F("nv_long_energy_hist","Energy long in NV;energy [keV]",10000,0,10000);
  nv_long_eof_energy_hist = new TH2F("nv_long_eof_energy_hist","Time vs Energy long in NV;Time[ns];energy [keV]",15000,-1.e+3,7.e+4,10000,0,10000);
  nv_alpha_energy_hist = new TH1F("nv_alpha_energy_hist","Energy in NV;energy [keV]",10000,0,10000);
  nv_alpha_qenergy_hist = new TH1F("nv_alpha_qenergy_hist","Quench Energy in NV;energy [KeV]",10000,0,10000);
  nv_alpha_capture_time_hist = new TH1F("nv_alpha_capture_time_hist","Capture Time in NV;Time [ns]",10000,0,80000);
  nv_alpha_eof_energy_hist = new TH2F("nv_alpha_eof_energy_hist","Time vs Energy in NV;Time [ns];energy [KeV]",10000,0,80000,10000,0,10000);
  nv_eof_quchenergy_hist = new TH2F("nv_eof_quchenergy_hist","Time vs Quenching Energy in NV;Time [ns];energy [KeV]",10000,0,80000,10000,0,10000);
  nv_quchenergy_hist = new TH1F("nv_quchenergy_hist","Quenching Energy in NV;energy [KeV]",10000,0,10000);
  //  nv_time_hist = new TH1F("nv_time_hist","Time in NV;Time [ns]",1000,0,500);
  nv_time_hist = new TH1F("nv_time_hist","Time in NV;Time [ns]",10000,0,80000);

  nv_first_eof_quchenergy_hist = new TH2F("nv_first_eof_quchenergy_hist","First Time vs Quenching Energy in NV;Time [ns];energy [keV]",1000,0,200,1000,0,10000);
  nv_first_quchenergy_hist = new TH1F("nv_first_quchenergy_hist","First Quenching Energy in NV;energy [KeV]",500,0,500);
  nv_first_energy_hist = new TH1F("nv_first_energy_hist","First Energy in NV;energy [KeV]",250,0,10000);
  nv_first_time_hist = new TH1F("nv_first_time_hist","First Time in NV;Time [ns]",1000,0,200);
  nv_first_XY_hist = new TH2F("nv_first_XY_hist","First XY in NV;X [cm];Y[cm]",1000,-100,100,1000,-100,100);
  nv_first_ntuple = new TNtuple("nv_first_ntuple","Veto First data","edep:eqch:time:x:y:z");

  nv_residual_eof_quchenergy_hist = new TH2F("nv_residual_eof_quchenergy_hist","Residual Time vs Quenching Energy in NV;Time [ns];energy [keV]",1000,0,7.e+4,1000,0,10000);
  nv_residual_energy_hist = new TH1F("nv_residual_energy_hist","Residual Energy in NV;energy [KeV]",1000,0,10000);
  nv_residual_quchenergy_hist = new TH1F("nv_residual_quchenergy_hist","Residual Quenching Energy in NV;energy [KeV]",1000,0,10000);
  nv_residual_time_hist = new TH1F("nv_residual_time_hist","Residual Time in NV;Time [ns]",1000,0,7.e+4);
  nv_residual_XY_hist = new TH2F("nv_residual_XY_hist","residual XY in NV;X[cm];Y[cm]",1000,-100,100,1000,-100,100);

  tpc_puregamma_qenergy_hist = new TH1F("tpc_puregamma_qenergy_hist","Pure Gamma Quench Energy in TPC;Energy [keV]",1000,0,5000);
  tpc_puregamma_time_hist = new TH1F("tpc_puregamma_time_hist","Pure Gamma Time in TPC;Time [ns]",1000,0,1.e+4);
  tpc_puregamma_eof_qenergy_hist = new TH2F("tpc_puregamma_eof_qenergy_hist","Pure Gamma Time Vs Quench Energy in TPC;Time[ns];Energy [keV]",1000,0,1.e+4,1000,0,5000);
  tpc_puregamma_ntuple = new TNtuple("tpc_puregamma_ntuple","TPC pure gamma data","qene:time:x:y:z");

  nv_small1residual_energy_hist = new TH1F("nv_small1residual_energy_hist","small1 Residual Energy in NV;energy [KeV]",1000,0,10000);
  nv_small1residual_quchenergy_hist = new TH1F("nv_small1residual_quchenergy_hist","small1 Residual Quenching Energy in NV;energy [KeV]",1000,0,10000);
  nv_small1residual_time_hist = new TH1F("nv_small1residual_time_hist","small1 Residual Time in NV;Time [ns]",100,0,20);

  nv_thermal_eof_quchenergy_hist = new TH2F("nv_thermal_eof_quchenergy_hist","Thermal Time vs Quenching Energy in NV;Time [ns];energy [keV]",1000,0,200,1000,0,10000);
  nv_thermal_energy_hist = new TH1F("nv_thermal_energy_hist","Thermal Energy in NV;energy [KeV]",1000,0,10000);
  nv_thermal_quchenergy_hist = new TH1F("nv_thermal_quchenergy_hist","Thermal Quenching Energy in NV;energy [KeV]",1000,0,10000);
  nv_thermal_time_hist = new TH1F("nv_thermal_time_hist","Thermal Time in NV;Time [ns]",1000,0,200);
  nv_thermal_XY_hist = new TH2F("nv_thermal_XY_hist","thermal XY in NV;X[cm];Y[cm]",1000,-100,100,1000,-100,100);
  nv_thermal_ntuple = new TNtuple("nv_thermal_ntuple","Veto Thermal data","edep:eqch:time:x:y:z");

  nv_thermal_long_eof_eqch_hist = new TH2F("nv_thermal_long_eof_eqch_hist","Long Thermal Time vs Quenching Energy in NV;Time [ns];energy [keV]",1000,20.e+3,60.e+3,500,0,500);
  nv_thermal_long_eqch_hist = new TH1F("nv_thermal_long_eqch_hist","Long Thermal Energy in NV;energy [KeV]",500,0,500);
  //  nv_thermal_quchenergy_hist = new TH1F("nv_thermal_quchenergy_hist","Thermal Quenching Energy in NV;energy [KeV]",1000,0,10000);
  nv_thermal_long_time_hist = new TH1F("nv_thermal_long_time_hist","Long Thermal Time in NV;Time [ns]",1000,20.e+3,60.e+3);
  nv_thermal_long_XY_hist = new TH2F("nv_thermal_long_XY_hist","Long thermal XY in NV;X[cm];Y[cm]",1000,-100,100,1000,-100,100);
  nv_thermal_long_ntuple = new TNtuple("nv_thermal_long_ntuple","Veto Long Thermal data","edep:eqch:time:x:y:z");
  nv_thermal_long_alpha_radius_hist = new TH1F("nv_thermal_long_alpha_radius_hist","Long Thermal Alpha Center;distance [cm]",200,0,200);
  nv_thermal_long_gamma_radius_hist = new TH1F("nv_thermal_long_gamma_radius_hist","Long Thermal Gamma Center;distance [cm]",200,0,200);
  
  nv_remain_eof_quchenergy_hist = new TH2F("nv_remain_eof_quchenergy_hist","Remain Time vs Quenching Energy in NV;Time [ns];energy [keV]",1000,0,500,100,0,100);
  nv_remain_energy_hist = new TH1F("nv_remain_energy_hist","Remain Energy in NV;energy [KeV]",100,0,100);
  nv_remain_quchenergy_hist = new TH1F("nv_remain_quchenergy_hist","Remain Quenching Energy in NV;energy [KeV]",100,0,100);
  nv_remain_time_hist = new TH1F("nv_remain_time_hist","Remain Time in NV;Time [ns]",1000,0,500);
  nv_remain_XY_hist = new TH2F("nv_remain_XY_hist","remain XY in NV;X[cm];Y[cm]",1000,-100,100,1000,-100,100);

  nv_alpha_prompt_eof_energy_hist = new TH2F("nv_alpha_prompt_eof_energy_hist","Alpha Prompt Time vs Energy in NV;Time[ns];energy [keV]",1000,-100,500,10000,0,10000);
  nv_alpha_prompt_quchenergy_hist = new TH1F("nv_alpha_prompt_quchenergy_hist","Alpha Prompt Quenching Energy in NV;energy [KeV]",10000,0,10000);
  nv_alpha_prompt_time_hist = new TH1F("nv_alpha_prompt_time_hist","Alpha Prompt Time in NV;Time [ns]",1000,-100,500);//-1.e+3,1.e+6);
  id = new TH1F("id","event ID",1,0,1);
  tpc_gamma_ntuple = new TNtuple("tpc_gamma_ntuple","TPC Gamma Data","ene:qene:time");
  tpc_n_ntuple = new TNtuple("tpc_n_ntuple","TPC Neutron Data","ene:qene:time");
  nv_ntuple = new TNtuple("nv_ntuple","veto Data","ene:qene:time");
  all_ntuple = new TNtuple("all_ntuple","Data in all volumes","ene:qene:time:x:y:z:quenchfactor:volume");
  nv_alpha_capture_ntuple = new TNtuple("nv_alpha_capture_ntuple","veto neutron capture Data(last cluster)","ene:qene:time:x:y:z");

  list->Add(id);
  list->Add(tpc_gamma_ntuple);
  list->Add(tpc_n_ntuple);
  list->Add(nv_ntuple);
  list->Add(all_ntuple);
  list->Add(nv_alpha_capture_ntuple);
  list->Add(nv_thermal_ntuple);
  list->Add(nv_first_ntuple);

  list->Add(first_tpc_ER_time_hist);
  list->Add(tpc_NR_time_hist);
  list->Add(tpc_gamma_energy_hist);
  list->Add(tpc_quenching_factor_hist);
  list->Add(nv_prompt_time_hist);
  list->Add(nv_prompt_radius_hist);
  list->Add(nv_prompt_energy_hist);
  list->Add(nv_prompt_eof_energy_hist);
  list->Add(energy_hist);
  list->Add(nv_time_hist);
  list->Add(nv_quchenergy_hist);
  list->Add(nv_eof_quchenergy_hist);
  list->Add(nv_residual_time_hist);
  list->Add(nv_residual_energy_hist);
  list->Add(nv_residual_quchenergy_hist);
  list->Add(nv_residual_eof_quchenergy_hist);
  list->Add(nv_residual_XY_hist);
  list->Add(nv_small1residual_time_hist);
  list->Add(nv_small1residual_energy_hist);
  list->Add(nv_small1residual_quchenergy_hist);
  list->Add(tpc_puregamma_qenergy_hist);
  list->Add(tpc_puregamma_time_hist);
  list->Add(tpc_puregamma_eof_qenergy_hist);
  list->Add(tpc_puregamma_ntuple);  
  list->Add(nv_first_time_hist);
  list->Add(nv_first_energy_hist);
  list->Add(nv_first_quchenergy_hist);
  list->Add(nv_first_energy_hist);
  list->Add(nv_first_eof_quchenergy_hist);
  list->Add(nv_first_XY_hist);
  list->Add(nv_thermal_time_hist);
  list->Add(nv_thermal_energy_hist);
  list->Add(nv_thermal_quchenergy_hist);
  list->Add(nv_thermal_eof_quchenergy_hist);
  list->Add(nv_thermal_XY_hist);
  list->Add(nv_thermal_long_time_hist);
  list->Add(nv_thermal_long_eqch_hist);
  list->Add(nv_thermal_long_ntuple);
  list->Add(nv_thermal_long_eof_eqch_hist);
  list->Add(nv_thermal_long_XY_hist);
  list->Add(nv_thermal_long_alpha_radius_hist);
  list->Add(nv_thermal_long_gamma_radius_hist);

  list->Add(nv_remain_time_hist);
  list->Add(nv_remain_quchenergy_hist);
  list->Add(nv_remain_energy_hist);
  list->Add(nv_remain_eof_quchenergy_hist);
  list->Add(nv_remain_XY_hist);
  list->Add(contain_energy_hist);
  list->Add(contain_time_hist);
  list->Add(nv_alpha_eof_energy_hist);
  list->Add(nv_alpha_capture_time_hist);
  list->Add(nv_alpha_energy_hist);
  list->Add(nv_alpha_qenergy_hist);
  list->Add(nv_long_time_hist);
  list->Add(nv_long_energy_hist);
  list->Add(nv_long_eof_energy_hist);
  list->Add(nv_alpha_prompt_eof_energy_hist);
  list->Add(nv_alpha_prompt_quchenergy_hist);
  list->Add(nv_alpha_prompt_time_hist);

}

void ReconSelector::Calculate_pdf(TH1F* nv_energy_hist,TH1F* nv_pdf_hist, TH1F* nv_pdf_fine_hist)
{
  int nbins = nv_energy_hist->GetNbinsX();
  double binw = nv_energy_hist->GetXaxis()->GetBinWidth(1);
  double sum = nv_energy_hist->Integral();
  cout<<nbins<<" "<<sum<<" binw="<<binw<<endl;
  for(int i=1; i<=nbins; i++)
    {
      double energy = nv_energy_hist->GetBinCenter(i);
      double density = nv_energy_hist->Integral(i,nbins)/sum;
      //      nv_pdf_hist->SetBinContent(i,density);                                                                               
      nv_pdf_hist->Fill(i*binw, density);
      nv_pdf_fine_hist->Fill(i*binw, density);
    }

}



/*
void ReconSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  string label = GetOption();
  cout<<"fNumberOfEvents= "<<fNumberOfEvents<<endl;
  Info("ReconSelector::Terminate()","Closing ....");
    string outdirname="/darkside/users/hqian/neutron0G163tiltlow/cluster_data/";
  //  string outdirname="/ds50/data/user/hqian36/Collimator/neutron0G163tilt/cluster_data/";
  string outputname=outdirname+"nuclear0G163tilt_recoil_"+label+Time+"_clustered"+".root";

  TList *list = GetOutputList();

  TCanvas *c6=new TCanvas("c6",label.c_str(),1000,600);
  c6->SetLogy();
  QuenchingFactor=dynamic_cast<TH1F*>(list->FindObject(Form("QuenchingFactor")));
  QuenchingFactor->Draw();
  c6->SaveAs(Form("ambe_QuenchingFactor_%s%s.png",label.c_str(),Time.c_str()));

  Hist1DTPCedep=dynamic_cast<TH1F*>(list->FindObject(Form("Hist1DTPCedep")));
  Hist1DTPCedep->Draw();
  c6->SaveAs(Form("ambe_Hist1DTPCedep_%s%s.png",label.c_str(),Time.c_str()));

  Hist1DTPCedep_nuclear=dynamic_cast<TH1F*>(list->FindObject(Form("Hist1DTPCedep_nuclear")));
  Hist1DTPCedep_nuclear->Draw();
  c6->SaveAs(Form("ambe_Hist1DTPCedep_nuclear_%s%s.png",label.c_str(),Time.c_str()));

  Hist1DTPCedep_electron=dynamic_cast<TH1F*>(list->FindObject(Form("Hist1DTPCedep_electron")));
  Hist1DTPCedep_electron->Draw();
  c6->SaveAs(Form("ambe_Hist1DTPCedep_electron_%s%s.png",label.c_str(),Time.c_str()));

  Hist1DNVedep=dynamic_cast<TH1F*>(list->FindObject(Form("Hist1DNVedep")));
  Hist1DNVedep->Draw();
  c6->SaveAs(Form("ambe_Hist1DNVedep_%s%s.png",label.c_str(),Time.c_str()));

  Hist1DNVedep_nuclear=dynamic_cast<TH1F*>(list->FindObject(Form("Hist1DNVedep_nuclear")));
  Hist1DNVedep_nuclear->Draw();
  c6->SaveAs(Form("ambe_Hist1DNVedep_nuclear_%s%s.png",label.c_str(),Time.c_str()));

  Hist1DNVedep_electron=dynamic_cast<TH1F*>(list->FindObject(Form("Hist1DNVedep_electron")));
  Hist1DNVedep_electron->Draw();
  c6->SaveAs(Form("ambe_Hist1DNVedep_electron_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c1=new TCanvas("c1",label.c_str(),1000,600);
  Hist2D=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2D")));
  Hist2D->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2D_%s%s.png",label.c_str(),Time.c_str()));
 
  Hist2DYZ=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DYZ")));
  Hist2DYZ->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DYZ_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXZ=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXZ")));
  Hist2DXZ->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXZ_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYslice1=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYslice1")));
  Hist2DXYslice1->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXYslice1_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYslice2=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYslice2")));
  Hist2DXYslice2->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXYslice2_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYslice3=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYslice3")));
  Hist2DXYslice3->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DXYslice3_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DGap=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DGap")));
  c1->SetLogz();
  Hist2DGap->Draw("colz");
  c1->SaveAs(Form("ambe_Hist2DGap_%s%s.png",label.c_str(),Time.c_str()));
  
  TCanvas *c2=new TCanvas("c2",label.c_str(),1000,600);
  Hist2Dnuclear=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2Dnuclear")));
  Hist2Dnuclear->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2Dnuclear_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DYZnuclear=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DYZnuclear")));
  Hist2DYZnuclear->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DYZnuclear_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXZnuclear=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXZnuclear")));
  Hist2DXZnuclear->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXZnuclear_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYnuclearslice1=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYnuclearslice1")));
  Hist2DXYnuclearslice1->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice1_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYnuclearslice2=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYnuclearslice2")));
  Hist2DXYnuclearslice2->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice2_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYnuclearslice3=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYnuclearslice3")));
  Hist2DXYnuclearslice3->Draw("colz");
  c2->SaveAs(Form("ambe_Hist2DXYnuclearslice3_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c4=new TCanvas("c4",label.c_str(),1000,600);
  Hist2Dsinglenuclear=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2Dsinglenuclear")));
  Hist2Dsinglenuclear->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2Dsinglenuclear_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYsinglenuclearslice1=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYsinglenuclearslice1")));
  Hist2DXYsinglenuclearslice1->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice1_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYsinglenuclearslice2=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYsinglenuclearslice2")));
  Hist2DXYsinglenuclearslice2->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice2_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYsinglenuclearslice3=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYsinglenuclearslice3")));
  Hist2DXYsinglenuclearslice3->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice3_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DYZsinglenuclear=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DYZsinglenuclear")));
  Hist2DYZsinglenuclear->Draw("colz");
  c4->SaveAs(Form("ambe_Hist2DYZsinglenuclear_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c5=new TCanvas("c5",label.c_str(),1000,600);
  Hist2Dsingle=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2Dsingle")));
  Hist2Dsingle->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2Dsingle_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYsingleslice1=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYsingleslice1")));
  Hist2DXYsingleslice1->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice1_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYsingleslice2=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYsingleslice2")));
  Hist2DXYsingleslice2->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice2_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DXYsingleslice3=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DXYsingleslice3")));
  Hist2DXYsingleslice3->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DXYsingleslice3_%s%s.png",label.c_str(),Time.c_str()));

  Hist2DYZsingle=dynamic_cast<TH2F*>(list->FindObject(Form("Hist2DYZsingle")));
  Hist2DYZsingle->Draw("colz");
  c5->SaveAs(Form("ambe_Hist2DYZsingle_%s%s.png",label.c_str(),Time.c_str()));
  
  //generate the projection plots
  float leftend = -33.5;
  int leftendbin = Hist2DGap->GetXaxis()->FindBin(leftend);
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
  
  //Draw the Projection histograms
  TCanvas *c11=new TCanvas("c11",label.c_str(),1000,600);
  Hist2DXYslice1ProjectionY->Draw();
  c11->SaveAs(Form("ambe_Hist2DXYslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice2ProjectionY->Draw();
  c11->SaveAs(Form("ambe_Hist2DXYslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice3ProjectionY->Draw();
  c11->SaveAs(Form("ambe_Hist2DXYslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice1ProjectionX->Draw();
  c11->SaveAs(Form("ambe_Hist2DXYslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice2ProjectionX->Draw();
  c11->SaveAs(Form("ambe_Hist2DXYslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYslice3ProjectionX->Draw();
  c11->SaveAs(Form("ambe_Hist2DXYslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  py1->Draw();
  c11->SaveAs(Form("ambe_Hist2DGapProjectionY_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c12=new TCanvas("c12",label.c_str(),1000,600);
  Hist2DXYnuclearslice1ProjectionY->Draw();
  c12->SaveAs(Form("ambe_Hist2DXYnuclearslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice2ProjectionY->Draw();
  c12->SaveAs(Form("ambe_Hist2DXYnuclearslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice3ProjectionY->Draw();
  c12->SaveAs(Form("ambe_Hist2DXYnuclearslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice1ProjectionX->Draw();
  c12->SaveAs(Form("ambe_Hist2DXYnuclearslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice2ProjectionX->Draw();
  c12->SaveAs(Form("ambe_Hist2DXYnuclearslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYnuclearslice3ProjectionX->Draw();
  c12->SaveAs(Form("ambe_Hist2DXYnuclearslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c13=new TCanvas("c13",label.c_str(),1000,600);
  Hist2DXYsinglenuclearslice1ProjectionY->Draw();
  c13->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice2ProjectionY->Draw();
  c13->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice3ProjectionY->Draw();
  c13->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice1ProjectionX->Draw();
  c13->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice2ProjectionX->Draw();
  c13->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsinglenuclearslice3ProjectionX->Draw();
  c13->SaveAs(Form("ambe_Hist2DXYsinglenuclearslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));

  TCanvas *c14=new TCanvas("c14",label.c_str(),1000,600);
  Hist2DXYsingleslice1ProjectionY->Draw();
  c14->SaveAs(Form("ambe_Hist2DXYsingleslice1ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice2ProjectionY->Draw();
  c14->SaveAs(Form("ambe_Hist2DXYsingleslice2ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice3ProjectionY->Draw();
  c14->SaveAs(Form("ambe_Hist2DXYsingleslice3ProjectionY_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice1ProjectionX->Draw();
  c14->SaveAs(Form("ambe_Hist2DXYsingleslice1ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice2ProjectionX->Draw();
  c14->SaveAs(Form("ambe_Hist2DXYsingleslice2ProjectionX_%s%s.png",label.c_str(),Time.c_str()));
  Hist2DXYsingleslice3ProjectionX->Draw();
  c14->SaveAs(Form("ambe_Hist2DXYsingleslice3ProjectionX_%s%s.png",label.c_str(),Time.c_str()));

  //write the histograms into new root file
  TFile f2D(outputname.c_str(), "RECREATE");
  Hist2D->Write();
  Hist2DYZ->Write();
  Hist2DXZ->Write();
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
  Hist2DYZnuclear->Write();
  Hist2DXZnuclear->Write();
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
  Hist1DTPCedep->Write();
  Hist1DTPCedep_nuclear->Write();
  Hist1DTPCedep_electron->Write();
  Hist1DNVedep->Write();
  Hist1DNVedep_nuclear->Write();
  Hist1DNVedep_electron->Write();
  f2D.Write();
  f2D.Close();

  Info("ReconSelector::Terminate()","terminating ...");
  
}

void ReconSelector::FillHistograms(){
  
  double quenchingcut = 0.43;
  double dep_enecut= 23.0; //keV
  double dep_enecut2=269.0; //keV

  vector<double> nrx;
  vector<double> nry;
  vector<double> nrz;
  vector<double> rx;
  vector<double> ry;
  vector<double> rz;

  int eff;
  bool IsNR,IsER;
  int NNR, NER, ndeps, nsing, NRcounts;
  
  double topzslice=-3.8;
  double middlezslice1=-5.8;
  double middlezslice2=-7.8;
  double bottomzslice=-9.8;

  NNR=NER=0;
  nsing=0;
  eff=0;
  cout<<"eff=  "<<eff<<endl;
  
  ndeps=0;
  NRcounts=0;

  IsNR = false;
  IsER = false;
  
  for(size_t i=0; i< et->size(); i++)
    {
      if(volume->at(i) =="p_active")
	{ ndeps++;
	  IsER=true;
	  Hist2D->Fill(ex->at(i),ey->at(i));
	  Hist2DYZ->Fill(ey->at(i),ez->at(i));
	  Hist2DXZ->Fill(ex->at(i),ez->at(i));
	  Hist1DTPCedep->Fill(edep->at(i)/1000.);
	  Hist1DTPCedep_nuclear->Fill(edep_nuclear->at(i)/1000.);
	  Hist1DTPCedep_electron->Fill(edep_electron->at(i)/1000.);
	  QuenchingFactor->Fill(quenchingfactor->at(i));
	  if(ndeps==1)
	    { eff++;
	      rx.push_back(ex->at(i));
	      ry.push_back(ey->at(i));
	      rz.push_back(ez->at(i));
	    }
	  if(ez->at(i)<topzslice && ez->at(i)>middlezslice1)
	    Hist2DXYslice1->Fill(ex->at(i),ey->at(i));
	  if(ez->at(i)<middlezslice1 && ez->at(i)>middlezslice2)
	    Hist2DXYslice2->Fill(ex->at(i),ey->at(i));
	  if(ez->at(i)>bottomzslice && ez->at(i)<middlezslice2)
	    Hist2DXYslice3->Fill(ex->at(i),ey->at(i));
	  if(quenchingfactor->at(i) < quenchingcut && edep->at(i)<dep_enecut2 && edep->at(i)>dep_enecut )
	    {
	      //  NRcounts++;
	      IsNR = true;
	      Hist2Dnuclear->Fill(ex->at(i),ey->at(i));
	      Hist2DYZnuclear->Fill(ey->at(i),ez->at(i));
	      Hist2DXZnuclear->Fill(ex->at(i),ez->at(i));
	      nrx.push_back(ex->at(i));
	      nry.push_back(ey->at(i));
	      nrz.push_back(ez->at(i));
	      if(ez->at(i)>topzslice)
		{
		  NRcounts++;
		  Hist1DNRcountsslice4->Fill(NRcounts);
		}
	      else if(ez->at(i)<topzslice && ez->at(i)>middlezslice1)
		{  Hist2DXYnuclearslice1->Fill(ex->at(i),ey->at(i));
		  NRcounts++;
		  Hist1DNRcountsslice1->Fill(NRcounts);
		}
	      else if(ez->at(i)<middlezslice1 && ez->at(i)>middlezslice2)
		{ Hist2DXYnuclearslice2->Fill(ex->at(i),ey->at(i));
		  NRcounts++;
		  Hist1DNRcountsslice2->Fill(NRcounts);
		}
	      else if(ez->at(i)>bottomzslice && ez->at(i)<middlezslice2)
		{  Hist2DXYnuclearslice3->Fill(ex->at(i),ey->at(i));
		  NRcounts++;
		  Hist1DNRcountsslice3->Fill(NRcounts);
		}
	    }
	}

      if(volume->at(i) =="p_scint")
	{
	  Hist1DNVedep->Fill(edep->at(i)/1000.);
	  Hist1DNVedep_nuclear->Fill(edep_nuclear->at(i)/1000.);
	  Hist1DNVedep_electron->Fill(edep_electron->at(i)/1000.);	  
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
	  if(rz.at(a)<topzslice && rz.at(a)>middlezslice1)
	    Hist2DXYsingleslice1->Fill(rx.at(a),ry.at(a));
	  if(rz.at(a)<middlezslice1 && rz.at(a)>middlezslice2)
	    Hist2DXYsingleslice2->Fill(rx.at(a),ry.at(a));
	  if(rz.at(a)>bottomzslice && rz.at(a)<middlezslice2)
	    Hist2DXYsingleslice3->Fill(rx.at(a),ry.at(a));
	}
    }

  if(ndeps==1 && IsNR )
    {
      for(int h=0; h<ndeps; h++)
	{
	  Hist2Dsinglenuclear->Fill(nrx.at(h),nry.at(h));
	  Hist2DYZsinglenuclear->Fill(nry.at(h),nrz.at(h));
	  if(nrz.at(h)<topzslice && nrz.at(h)>middlezslice1)
	    Hist2DXYsinglenuclearslice1->Fill(nrx.at(h),nry.at(h));
	  if(nrz.at(h)<middlezslice1 && nrz.at(h)>middlezslice2)
	    Hist2DXYsinglenuclearslice2->Fill(nrx.at(h),nry.at(h));
	  if(nrz.at(h)>bottomzslice && nrz.at(h)<middlezslice2)
	    Hist2DXYsinglenuclearslice3->Fill(nrx.at(h),nry.at(h));
	}
    }
	      
}

void ReconSelector::BookHistograms(){
  Info("ReconSelector::BookHistograms()","creating histograms...");
  std::cout<<"Starting ..... ReconSelector::BookHistograms()"<<std::endl;
  
  string label = GetOption();
  TList* list = GetOutputList();

  string quchf = " quenchingfactor<0.43 ";
  string quche = " quenchingfactor>0.43 ";

  string zslice1 = " -5.8<dep_z<-3.8 ";
  string zslice2 = " -7.8<dep_z<-5.8 ";
  string zslice3 = " -9.8<dep_z<-7.8 ";
  
  string Hist2DTitle = "dep_y vs dep_x in TPC active volume "+ label;
  Hist2D = new TH2F("Hist2D",Hist2DTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2D->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2D->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2D);
 
  string Hist2DYZTitle = "dep_y vs dep_z in TPC active volume "+ label;
  Hist2DYZ = new TH2F("Hist2DYZ",Hist2DYZTitle.c_str(), 100,-20,20,100,-25,20);
  Hist2DYZ->GetXaxis()->SetTitle("dep_y [cm]");
  Hist2DYZ->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DYZ);

  string Hist2DXZTitle = "dep_x vs dep_z in TPC active volume "+ label;
  Hist2DXZ = new TH2F("Hist2DXZ",Hist2DXZTitle.c_str(), 100,-20,20,100,-25,20);
  Hist2DXZ->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXZ->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DXZ);

  string Hist2DXYslice1Title = "dep_y vs dep_x in TPC active volume"+zslice1+ label;
  Hist2DXYslice1 = new TH2F("Hist2DXYslice1",Hist2DXYslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYslice1->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYslice1);

  string Hist2DXYslice2Title = "dep_y vs dep_x in TPC active volume"+zslice2+ label;
  Hist2DXYslice2 = new TH2F("Hist2DXYslice2",Hist2DXYslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYslice2->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYslice2);

  string Hist2DXYslice3Title = "dep_y vs dep_x in TPC active volume"+zslice3+ label;
  Hist2DXYslice3 = new TH2F("Hist2DXYslice3",Hist2DXYslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYslice3->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYslice3);

  string Hist2DGapTitle = "dep_z vs dep_x with y slice "+ label;
  Hist2DGap = new TH2F("Hist2DGap",Hist2DGapTitle.c_str(), 150,-100,0,150,-50,100);
  Hist2DGap->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DGap->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DGap);
  
  string QuenchingFactorTitle = "quenchingfactor in TPC active volume "+ label;
  QuenchingFactor = new TH1F("QuenchingFactor",QuenchingFactorTitle.c_str(),100,0,1);
  QuenchingFactor->GetXaxis()->SetTitle("quenching factor");
  QuenchingFactor->GetYaxis()->SetTitle("counts");
  list->Add(QuenchingFactor);

  //quecnhing cuts for nuclear-like recoil
  string Hist2DnuclearTitle = "dep_y vs dep_x in TPC active volume"+quchf+ label;
  Hist2Dnuclear = new TH2F("Hist2Dnuclear",Hist2DnuclearTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2Dnuclear->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2Dnuclear->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2Dnuclear);

  string Hist2DYZnuclearTitle = "dep_y vs dep_z in TPC active volume"+quchf+ label;
  Hist2DYZnuclear = new TH2F("Hist2DYZnuclear",Hist2DYZnuclearTitle.c_str(), 100,-20,20,100,-25,20);
  Hist2DYZnuclear->GetXaxis()->SetTitle("dep_y [cm]");
  Hist2DYZnuclear->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DYZnuclear);

  string Hist2DXZnuclearTitle = "dep_x vs dep_z in TPC active volume"+quchf+ label;
  Hist2DXZnuclear = new TH2F("Hist2DXZnuclear",Hist2DXZnuclearTitle.c_str(), 100,-20,20,100,-25,20);
  Hist2DXZnuclear->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXZnuclear->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DXZnuclear);

  string Hist2DXYnuclearslice1Title = "dep_y vs dep_x in TPC active volume"+zslice1+quchf+ label;
  Hist2DXYnuclearslice1 = new TH2F("Hist2DXYnuclearslice1",Hist2DXYnuclearslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYnuclearslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYnuclearslice1->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYnuclearslice1);

  string Hist2DXYnuclearslice2Title = "dep_y vs dep_x in TPC active volume"+zslice2+quchf+ label;
  Hist2DXYnuclearslice2 = new TH2F("Hist2DXYnuclearslice2",Hist2DXYnuclearslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYnuclearslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYnuclearslice2->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYnuclearslice2);

  string Hist2DXYnuclearslice3Title = "dep_y vs dep_x in TPC active volume"+zslice3+quchf+ label;
  Hist2DXYnuclearslice3 = new TH2F("Hist2DXYnuclearslice3",Hist2DXYnuclearslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYnuclearslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYnuclearslice3->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYnuclearslice3);

  string Hist1DNRcountsslice1Title = "NR counts in slice1 in TPC active volume "+ label;
  Hist1DNRcountsslice1 = new TH1F("Hist1DNRcountsslice1",Hist1DNRcountsslice1Title.c_str(),100,0,20);
  Hist1DNRcountsslice1->GetXaxis()->SetTitle("NR counts");
  list->Add(Hist1DNRcountsslice1);
  
  string Hist1DNRcountsslice2Title = "NR counts in slice2 in TPC active volume "+ label;
  Hist1DNRcountsslice2 = new TH1F("Hist1DNRcountsslice2",Hist1DNRcountsslice2Title.c_str(),100,0,20);
  Hist1DNRcountsslice2->GetXaxis()->SetTitle("NR counts");
  list->Add(Hist1DNRcountsslice2);

  string Hist1DNRcountsslice3Title = "NR counts in slice3 in TPC active volume "+ label;
  Hist1DNRcountsslice3 = new TH1F("Hist1DNRcountsslice3",Hist1DNRcountsslice3Title.c_str(),100,0,20);
  Hist1DNRcountsslice3->GetXaxis()->SetTitle("NR counts");
  list->Add(Hist1DNRcountsslice3);

  string Hist1DNRcountsslice4Title = "NR counts in slice4 in TPC active volume "+ label;
  Hist1DNRcountsslice4 = new TH1F("Hist1DNRcountsslice4",Hist1DNRcountsslice4Title.c_str(),100,0,20);
  Hist1DNRcountsslice4->GetXaxis()->SetTitle("NR counts");
  list->Add(Hist1DNRcountsslice4);

  //single nuclear sacttering in TPC active volume
  string Hist2DsinglenuclearTitle = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+quchf+ label;
  Hist2Dsinglenuclear = new TH2F("Hist2Dsinglenuclear",Hist2DsinglenuclearTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2Dsinglenuclear->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2Dsinglenuclear->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2Dsinglenuclear);

  string Hist2DYZsinglenuclearTitle = "dep_z vs dep_y for single nuclear scattering in TPC active volume"+quchf+ label;
  Hist2DYZsinglenuclear = new TH2F("Hist2DYZsinglenuclear",Hist2DYZsinglenuclearTitle.c_str(),100,-20,20,100,-25,20);
  Hist2DYZsinglenuclear->GetXaxis()->SetTitle("dep_y [cm]");
  Hist2DYZsinglenuclear->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DYZsinglenuclear);

  string Hist2DXYsinglenuclearslice1Title = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+zslice1+quchf+ label;
  Hist2DXYsinglenuclearslice1 = new TH2F("Hist2DXYsinglenuclearslice1",Hist2DXYsinglenuclearslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsinglenuclearslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsinglenuclearslice1->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYsinglenuclearslice1);

  string Hist2DXYsinglenuclearslice2Title = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+zslice2+quchf+ label;
  Hist2DXYsinglenuclearslice2 = new TH2F("Hist2DXYsinglenuclearslice2",Hist2DXYsinglenuclearslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsinglenuclearslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsinglenuclearslice2->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYsinglenuclearslice2);

  string Hist2DXYsinglenuclearslice3Title = "dep_y vs dep_x for single nuclear scattering in TPC active volume"+zslice3+quchf+ label;
  Hist2DXYsinglenuclearslice3 = new TH2F("Hist2DXYsinglenuclearslice3",Hist2DXYsinglenuclearslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsinglenuclearslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsinglenuclearslice3->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYsinglenuclearslice3);

  //single sacttering in TPC active volume
  string Hist2DsingleTitle = "dep_y vs dep_x for single scattering in TPC active volume "+ label;
  Hist2Dsingle = new TH2F("Hist2Dsingle",Hist2DsingleTitle.c_str(), 100,-20,20,100,-20,20);
  Hist2Dsingle->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2Dsingle->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2Dsingle);

  string Hist2DYZsingleTitle = "dep_z vs dep_y for single scattering in TPC active volume "+ label;
  Hist2DYZsingle = new TH2F("Hist2DYZsingle",Hist2DYZsingleTitle.c_str(),100,-20,20,100,-25,20);
  Hist2DYZsingle->GetXaxis()->SetTitle("dep_y [cm]");
  Hist2DYZsingle->GetYaxis()->SetTitle("dep_z [cm]");
  list->Add(Hist2DYZsingle);

  string Hist2DXYsingleslice1Title = "dep_y vs dep_x for single scattering in TPC active volume"+zslice1+ label;
  Hist2DXYsingleslice1 = new TH2F("Hist2DXYsingleslice1",Hist2DXYsingleslice1Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsingleslice1->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsingleslice1->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYsingleslice1);

  string Hist2DXYsingleslice2Title = "dep_y vs dep_x for single scattering in TPC active volume"+zslice2+ label;
  Hist2DXYsingleslice2 = new TH2F("Hist2DXYsingleslice2",Hist2DXYsingleslice2Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsingleslice2->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsingleslice2->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYsingleslice2);

  string Hist2DXYsingleslice3Title = "dep_y vs dep_x for single scattering in TPC active volume"+zslice3+ label;
  Hist2DXYsingleslice3 = new TH2F("Hist2DXYsingleslice3",Hist2DXYsingleslice3Title.c_str(),100,-20,20,100,-20,20);
  Hist2DXYsingleslice3->GetXaxis()->SetTitle("dep_x [cm]");
  Hist2DXYsingleslice3->GetYaxis()->SetTitle("dep_y [cm]");
  list->Add(Hist2DXYsingleslice3);

  //energy spectrum inside TPC
  string Hist1DTPCedepTitle = "Energy Spectrum in TPC active volume "+ label;
  Hist1DTPCedep = new TH1F("Hist1DTPCedep",Hist1DTPCedepTitle.c_str(), 1000,0,20);
  Hist1DTPCedep->GetXaxis()->SetTitle("Energy [MeV]");
  Hist1DTPCedep->GetYaxis()->SetTitle("Counts");
  list->Add(Hist1DTPCedep);
  
  string Hist1DTPCedep_nuclearTitle = "Energy Spectrum of nuclear recoils in TPC active volume "+ label;
  Hist1DTPCedep_nuclear = new TH1F("Hist1DTPCedep_nuclear",Hist1DTPCedep_nuclearTitle.c_str(), 1000,0,20);
  Hist1DTPCedep_nuclear->GetXaxis()->SetTitle("Energy [MeV]");
  Hist1DTPCedep_nuclear->GetYaxis()->SetTitle("Counts");
  list->Add(Hist1DTPCedep_nuclear);
  
  string Hist1DTPCedep_electronTitle = "Energy Spectrum of electron recoils in TPC active volume "+ label;
  Hist1DTPCedep_electron = new TH1F("Hist1DTPCedep_electron",Hist1DTPCedep_electronTitle.c_str(), 1000,0,20);
  Hist1DTPCedep_electron->GetXaxis()->SetTitle("Energy [MeV]");
  Hist1DTPCedep_electron->GetYaxis()->SetTitle("Counts");
  list->Add(Hist1DTPCedep_electron);
  
  //energy spectrum inside NV
  string Hist1DNVedepTitle = "Energy Spectrum in NV active volume "+ label;
  Hist1DNVedep = new TH1F("Hist1DNVedep",Hist1DNVedepTitle.c_str(), 1000,0,20);
  Hist1DNVedep->GetXaxis()->SetTitle("Energy [MeV]");
  Hist1DNVedep->GetYaxis()->SetTitle("Counts");
  list->Add(Hist1DNVedep);
  
  string Hist1DNVedep_nuclearTitle = "Energy Spectrum of nuclear recoils in NV "+ label;
  Hist1DNVedep_nuclear = new TH1F("Hist1DNVedep_nuclear",Hist1DNVedep_nuclearTitle.c_str(), 1000,0,20);
  Hist1DNVedep_nuclear->GetXaxis()->SetTitle("Energy [MeV]");
  Hist1DNVedep_nuclear->GetYaxis()->SetTitle("Counts");
  list->Add(Hist1DNVedep_nuclear);
  
  string Hist1DNVedep_electronTitle = "Energy Spectrum of electron recoils in NV "+ label;
  Hist1DNVedep_electron = new TH1F("Hist1DNVedep_electron",Hist1DNVedep_electronTitle.c_str(), 1000,0,20);
  Hist1DNVedep_electron->GetXaxis()->SetTitle("Energy [MeV]");
  Hist1DNVedep_electron->GetYaxis()->SetTitle("Counts");
  list->Add(Hist1DNVedep_electron);
  

}
*/

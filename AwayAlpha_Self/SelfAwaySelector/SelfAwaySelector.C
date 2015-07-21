#define SelfAwaySelector_cxx
// The class definition in SelfAwaySelector.h has been generated automatically
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
// Root > T->Process("SelfAwaySelector.C")
// Root > T->Process("SelfAwaySelector.C","some options")
// Root > T->Process("SelfAwaySelector.C+")
//

#include "SelfAwaySelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <TH2.h>
#include <TStyle.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TBox.h>
#include <TColor.h>
#include <TLegend.h>

using namespace std;
//#define Write_Plots
//const string Time = "_away_Feb8PM";

void SelfAwaySelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void SelfAwaySelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   Info("SlaveBegin()","beginning .....");
   TString option = GetOption();
   BookHistograms();

}

Bool_t SelfAwaySelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either SelfAwaySelector::GetEntry() or TBranch::GetEntry()
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
  //  ++fNumberOfEvents;
  FillHistograms();

  return kTRUE;
}

void SelfAwaySelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void SelfAwaySelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  string Time = GetOption();
  TList* list = GetOutputList();
  double fail_ene = 500; //pe                                                                                                 
  int fail_bins = 125;

  nv_all_gps_hist  = dynamic_cast<TH1F*>(list->FindObject(Form("nv_all_gps_hist")));
  nv_all_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_all_ene_hist")));
  nv_all_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_all_small_ene_hist")));
  nv_all_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_all_gps_ene_hist")));
  nv_all_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_all_ntuple")));
  
  nv_all_charge_height_multiplicity_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_all_charge_height_multiplicity_hist")));
  nv_fail_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_fail_gps_hist")));
  nv_fail_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_fail_ene_hist")));
  nv_fail_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_fail_small_ene_hist")));
  nv_fail_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_fail_gps_ene_hist")));
  nv_fail_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_fail_ntuple")));

  nv_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gps_hist")));
  nv_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_ene_hist")));
  nv_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_small_ene_hist")));
  nv_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gps_ene_hist")));
  nv_ntuple  = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_ntuple")));
  nv_charge_height_multiplicity_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_charge_height_multiplicity_hist")));
  nv_gamma_full_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_full_ene_hist")));
  nv_gamma_full_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_full_small_ene_hist")));

  nv_gamma_coincidence_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_coincidence_gps_hist")));
  nv_gamma_coincidence_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_coincidence_ene_hist")));
  nv_gamma_coincidence_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_coincidence_small_ene_hist")));
  nv_gamma_coincidence_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gamma_coincidence_gps_ene_hist")));
  nv_gamma_coincidence_ntuple  = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_gamma_coincidence_ntuple")));
  
  nv_gamma_prompt_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_prompt_gps_hist")));
  nv_gamma_prompt_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_prompt_ene_hist")));
  nv_gamma_prompt_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_prompt_small_ene_hist")));
  nv_gamma_prompt_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gamma_prompt_gps_ene_hist")));
  nv_gamma_prompt_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_gamma_prompt_ntuple")));

  nv_gamma_delay_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_delay_gps_hist")));
  nv_gamma_delay_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_delay_ene_hist")));
  nv_gamma_delay_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_delay_small_ene_hist")));
  nv_gamma_delay_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gamma_delay_gps_ene_hist")));
  nv_gamma_delay_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_gamma_delay_ntuple")));
  
  nv_gamma_after_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_after_gps_hist")));
  nv_gamma_after_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_after_ene_hist")));
  nv_gamma_after_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_after_small_ene_hist")));
  nv_gamma_after_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gamma_after_gps_ene_hist")));
  nv_gamma_after_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_gamma_after_ntuple")));

  nv_gamma_first_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_first_gps_hist")));
  nv_gamma_first_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_first_ene_hist")));
  nv_gamma_first_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_first_small_ene_hist")));
  nv_gamma_first_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gamma_first_gps_ene_hist")));
  nv_gamma_first_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_gamma_first_ntuple")));

  nv_gamma_late_gps_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_late_gps_hist")));
  nv_gamma_late_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_late_ene_hist")));
  nv_gamma_late_small_ene_hist = dynamic_cast<TH1F*>(list->FindObject(Form("nv_gamma_late_small_ene_hist")));
  nv_gamma_late_gps_ene_hist = dynamic_cast<TH2F*>(list->FindObject(Form("nv_gamma_late_gps_ene_hist")));
  nv_gamma_late_ntuple = dynamic_cast<TNtuple*>(list->FindObject(Form("nv_gamma_late_ntuple")));

  id = dynamic_cast<TH1F*>(list->FindObject(Form("id")));

  double Entries = id->Integral();
  cout<<"Entries= "<<Entries<<endl;
  if(Entries>0)
    {
      nv_gamma_after_small_ene_hist->Scale(1./Entries);
      nv_gamma_first_small_ene_hist->Scale(1./Entries);
      nv_gamma_coincidence_small_ene_hist->Scale(1./Entries);
      nv_gamma_prompt_small_ene_hist->Scale(1./Entries);
      nv_gamma_late_small_ene_hist->Scale(1./Entries);
      nv_gamma_delay_small_ene_hist->Scale(1./Entries);
      nv_gamma_coincidence_ene_hist->Scale(1./Entries);
      nv_gamma_coincidence_small_ene_hist->Scale(1./Entries);
    }

  vector<int> linecolor;
  linecolor.push_back(TColor::GetColor("#FB78FF"));
  linecolor.push_back(TColor::GetColor("#B24F18"));
  linecolor.push_back(TColor::GetColor("#2FDAFF"));
  linecolor.push_back(TColor::GetColor("#5A1DE8"));
  linecolor.push_back(TColor::GetColor("#B26618"));
  linecolor.push_back(TColor::GetColor("#AE3EFF"));
  linecolor.push_back(TColor::GetColor("#FF0000"));
  linecolor.push_back(TColor::GetColor("#5A1DE8"));
  linecolor.push_back(TColor::GetColor("#1CFFDF"));
  linecolor.push_back(TColor::GetColor("#1D537F"));
  linecolor.push_back(TColor::GetColor("#000000"));
  linecolor.push_back(TColor::GetColor("#4A067F"));
  linecolor.push_back(TColor::GetColor("#FF379F"));
  linecolor.push_back(TColor::GetColor("#25DCFF"));
  linecolor.push_back(TColor::GetColor("#92B20B"));
  linecolor.push_back(TColor::GetColor("#59FF49"));
  
  nv_gamma_delay_ene_hist->SetLineColor(linecolor.at(0));
  nv_gamma_late_ene_hist->SetLineColor(linecolor.at(1));
  nv_gamma_late_small_ene_hist->SetLineColor(4);

  string plotdir="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/Plots/";
  string ext = "png";

  double pos1 = 200.;
  double pos2 = 350.;
  double pos3 = 0.;
  double pos4 = 50.;

  double height = 45;
  TBox *box1 = new TBox(pos3,0,pos4,height);
  box1->SetLineColor(2);
  box1->SetLineWidth(1);

  TBox *box2 = new TBox(pos1,0,pos2,height);
  box2->SetLineColor(3);
  box2->SetLineWidth(1);

  TH1F* nv_gamma_delay_small_ene_subtracted_hist = new TH1F("nv_gamma_delay_small_ene_subtracted_hist","Veto Gamma Subtracted Delay Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_delay_small_ene_subtracted_hist->Add(nv_gamma_delay_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
  TCanvas *c11 = new TCanvas("c11","Veto Gamma Delay Small Energy Subtracting BG",1000,400);
  c11->Divide(2,1);
  c11->cd(1);
  gPad->SetLogy();
  nv_gamma_delay_small_ene_hist->Draw();
  nv_gamma_late_small_ene_hist->Draw("same");
  box1->Draw();
  box2->Draw();
  c11->cd(2);
  gPad->SetLogy();
  nv_gamma_delay_small_ene_subtracted_hist->Draw();
  box1->Draw();
  box2->Draw();

  TH1F* nv_gamma_coincidence_small_ene_subtracted_hist = new TH1F("nv_gamma_coincidence_small_ene_subtracted_hist","Veto Gamma Subtracted Coincidence Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_coincidence_small_ene_subtracted_hist->Add(nv_gamma_coincidence_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
  TCanvas *c12 = new TCanvas("c12","Veto Gamma Coincidence Small Energy Subtracting BG",1000,400);
  c12->Divide(2,1);
  c12->cd(1);
  gPad->SetLogy();
  nv_gamma_coincidence_small_ene_hist->Draw();
  nv_gamma_late_small_ene_hist->Draw("same");
  box1->Draw();
  box2->Draw();
  c12->cd(2);
  //  gPad->SetLogy();
  nv_gamma_coincidence_small_ene_subtracted_hist->Draw();
  box1->Draw();
  box2->Draw();

  TH1F* nv_gamma_after_small_ene_subtracted_hist = new TH1F("nv_gamma_after_small_ene_subtracted_hist","Veto Gamma Subtracted After Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_after_small_ene_subtracted_hist->Add(nv_gamma_after_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
  TCanvas *c13 = new TCanvas("c13","Veto Gamma AFter Small Energy Subtracting BG",1000,400);
  c13->Divide(2,1);
  c13->cd(1);
  //  gPad->SetLogy();                                                                                                               
  nv_gamma_after_small_ene_hist->Draw();
  nv_gamma_late_small_ene_hist->Draw("same");
  box1->Draw();
  box2->Draw();
  c13->cd(2);
  // gPad->SetLogy();                                                                                                                
  nv_gamma_after_small_ene_subtracted_hist->Draw();
  box1->Draw();
  box2->Draw();
  double br1 = nv_gamma_after_small_ene_subtracted_hist->Integral(nv_gamma_after_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_after_small_ene_subtracted_hist->FindBin(pos4));
  double br2 = nv_gamma_after_small_ene_subtracted_hist->Integral(nv_gamma_after_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_after_small_ene_subtracted_hist->FindBin(pos2));
  if(br2>0)
    cout<<"after "<<"br1="<<br1<<"\t br2= "<<br2<<"\t "<<1.*br1/br2<<endl;

  TH1F* nv_gamma_first_small_ene_subtracted_hist = new TH1F("nv_gamma_first_small_ene_subtracted_hist","Veto Gamma Subtracted First Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_first_small_ene_subtracted_hist->Add(nv_gamma_first_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
  TCanvas *c14 = new TCanvas("c14","Veto Gamma First Small Energy Subtracting BG",1000,400);
  c14->Divide(2,1);
  c14->cd(1);
  //  gPad->SetLogy();                                                                                                               
  nv_gamma_first_small_ene_hist->Draw();
  nv_gamma_late_small_ene_hist->Draw("same");
  box1->Draw();
  box2->Draw();
  c14->cd(2);
  // gPad->SetLogy();                                                                                                                
  nv_gamma_first_small_ene_subtracted_hist->Draw();
  box1->Draw();
  box2->Draw();
  double br3 = nv_gamma_first_small_ene_subtracted_hist->Integral(nv_gamma_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_first_small_ene_subtracted_hist->FindBin(pos4));
  double br4 = nv_gamma_first_small_ene_subtracted_hist->Integral(nv_gamma_first_small_ene_subtracted_hist->FindBin(pos3),nv_gamma_first_small_ene_subtracted_hist->FindBin(pos2));
  if(br4>0)
    cout<<"first "<<"br3= "<<br3<<"\t br4= "<<br4<<"\t "<<1.*br3/br4<<endl;

  TH1F* nv_gamma_prompt_small_ene_subtracted_hist = new TH1F("nv_gamma_prompt_small_ene_subtracted_hist","Veto Gamma Subtracted Prompt Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_prompt_small_ene_subtracted_hist->Add(nv_gamma_prompt_small_ene_hist,nv_gamma_late_small_ene_hist,1,-1);
  TCanvas *c15 = new TCanvas("c15","Veto Gamma Prompt Small Energy Subtracting BG",1000,400);
  c15->Divide(2,1);
  c15->cd(1);
  gPad->SetLogy();
  nv_gamma_prompt_small_ene_hist->Draw();
  nv_gamma_late_small_ene_hist->Draw("same");
  box1->Draw();
  box2->Draw();
  c15->cd(2);
  gPad->SetLogy();
  nv_gamma_prompt_small_ene_subtracted_hist->Draw();
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
  nv_gamma_coincidence_gps_ene_hist->Draw("colz");

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

  TCanvas *c25 = new TCanvas("c25","Veto Gamma Late",1000,600);
  c25->Divide(2,2);
  c25->cd(1);
  gPad->SetLogy();
  nv_gamma_late_gps_hist->Draw();
  c25->cd(2);
  gPad->SetLogy();
  nv_gamma_late_ene_hist->Draw();
  c25->cd(3);
  gPad->SetLogy();
  nv_gamma_late_gps_ene_hist->Draw("colz");

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
  nv_gamma_first_gps_ene_hist->Draw("colz");

#ifdef Write_Plots
  c2->SaveAs(Form("%smulticut%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c10->SaveAs(Form("%snv_charge_height_multiplicity%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c11->SaveAs(Form("%snv_gamma_delay_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c12->SaveAs(Form("%snv_gamma_coincidence_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c13->SaveAs(Form("%snv_gamma_after_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c14->SaveAs(Form("%snv_gamma_first_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c15->SaveAs(Form("%snv_gamma_prompt_small_ene_subtracted%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c16->SaveAs(Form("%snv_gamma_coincidence_ene%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c21->SaveAs(Form("%snv_gamma_coincidence%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c22->SaveAs(Form("%snv_gamma_prompt%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c23->SaveAs(Form("%snv_gamma_delay%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c24->SaveAs(Form("%snv_gamma_after%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c25->SaveAs(Form("%snv_gamma_late%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
  c26->SaveAs(Form("%snv_gamma_first%s.%s",plotdir.c_str(),Time.c_str(),ext.c_str()));
#endif

  string outdirname="/darkside/users/hqian/AmBe10Hz_Calibration/AwaySelfTriggerResults/";
  string outputname=outdirname+Time+".root";
  TFile f2D(outputname.c_str(), "RECREATE");
  //  if(f2D.IsOpen()){
  nv_all_gps_hist->Write();
  nv_all_ene_hist->Write();
  nv_all_small_ene_hist->Write();
  nv_all_gps_ene_hist->Write();
  nv_all_ntuple->Write();
  
  nv_gps_hist->Write();
  nv_ene_hist->Write();
  nv_small_ene_hist->Write();
  nv_gps_ene_hist->Write();
  nv_ntuple->Write();

  nv_gamma_full_ene_hist->Write();
  nv_gamma_full_small_ene_hist->Write();
 
  nv_gamma_coincidence_gps_hist->Write();
  nv_gamma_coincidence_ene_hist->Write();
  nv_gamma_coincidence_small_ene_hist->Write();
  nv_gamma_coincidence_gps_ene_hist->Write();
  nv_gamma_coincidence_ntuple->Write();

  nv_gamma_prompt_gps_hist->Write();
  nv_gamma_prompt_ene_hist->Write();
  nv_gamma_prompt_small_ene_hist->Write();
  nv_gamma_prompt_gps_ene_hist->Write();
  nv_gamma_prompt_ntuple->Write();

  nv_gamma_delay_gps_hist->Write();
  nv_gamma_delay_ene_hist->Write();
  nv_gamma_delay_small_ene_hist->Write();
  nv_gamma_delay_gps_ene_hist->Write();
  nv_gamma_delay_ntuple->Write();

  nv_gamma_after_gps_hist->Write();
  nv_gamma_after_ene_hist->Write();
  nv_gamma_after_small_ene_hist->Write();
  nv_gamma_after_gps_ene_hist->Write();
  nv_gamma_after_ntuple->Write();

  nv_gamma_first_gps_hist->Write();
  nv_gamma_first_ene_hist->Write();
  nv_gamma_first_small_ene_hist->Write();
  nv_gamma_first_gps_ene_hist->Write();
  nv_gamma_first_ntuple->Write();

  nv_gamma_late_gps_hist->Write();
  nv_gamma_late_ene_hist->Write();
  nv_gamma_late_small_ene_hist->Write();
  nv_gamma_late_gps_ene_hist->Write();
  nv_gamma_late_ntuple->Write();
  
  nv_fail_gps_hist->Write();
  nv_fail_ene_hist->Write();
  nv_fail_small_ene_hist->Write();
  nv_fail_gps_ene_hist->Write();
  nv_fail_ntuple->Write();

  nv_gamma_delay_small_ene_subtracted_hist->Write();
  nv_gamma_after_small_ene_subtracted_hist->Write();
  nv_gamma_coincidence_small_ene_subtracted_hist->Write();
  nv_gamma_first_small_ene_subtracted_hist->Write();
  nv_gamma_prompt_small_ene_subtracted_hist->Write();

  nv_charge_height_multiplicity_hist->Write();
  nv_all_charge_height_multiplicity_hist->Write();

  c2->Write();
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
    //  id->Write();
  
  f2D.Write();
  //  }  
  //  else cout<<"!!!=====EROOR: Cannot open the root results file=====!!!"<<endl;

  f2D.Close();

  cout<<"4"<<endl;

  Info("Terminate()","terminating ...%s....%f",outputname.c_str(),Entries);
}

void SelfAwaySelector::FillHistograms()
{
  double prompt_time = 2.e+4;
  double middle_time = 0;
  double delay_time = 6.e+4;
  double first_time = 2.e+4;
  double after_time = 2.e+4;
  double late_time = 1.e+5;
  double end_time = 1.4e+5;

  id->Fill(0);
  //**************************Start Loop Over the OD clusters**********************************************                       
  //  for(int ncluster=0; ncluster<od_nclusters; ncluster++)
  for(size_t ncluster=0; ncluster<od_cluster_charge->size(); ncluster++)
    {
      if(od_cluster_charge->at(ncluster)>0)
	{
	  //*********************All OD cluster******************************************************************                
	  double gps_all_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;  //ns                                               
	  nv_all_gps_hist->Fill(gps_all_diff_temp);
	  nv_all_ene_hist->Fill(od_cluster_charge->at(ncluster));
	  nv_all_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
	  nv_all_gps_ene_hist->Fill(gps_all_diff_temp,od_cluster_charge->at(ncluster));
	  nv_all_ntuple->Fill(gps_all_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
	  nv_all_charge_height_multiplicity_hist->Fill(od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster)/od_cluster_multiplicity->at(ncluster));
	  //*********************Failing MultiCut cluster****************************************************************** 
	  if(od_cluster_pass_multcut->at(ncluster)==0)
	    {
	      double gps_fail_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;  //ns                                          
	      nv_fail_gps_hist->Fill(gps_fail_diff_temp);
	      nv_fail_ene_hist->Fill(od_cluster_charge->at(ncluster));
	      nv_fail_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
	      nv_fail_gps_ene_hist->Fill(gps_fail_diff_temp,od_cluster_charge->at(ncluster));
	      nv_fail_ntuple->Fill(gps_fail_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
	    }
	  //*********************Passing MultiCut cluster**********************************************************
	  if(od_cluster_pass_multcut->at(ncluster)==1)
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
	      double gps_gamma_diff_temp = od_cluster_dtprompt->at(ncluster)*1.e+3;
	      nv_gamma_full_ene_hist->Fill(od_cluster_charge->at(ncluster));
	      nv_gamma_full_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
	      int g=0;
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
		  //     nv_gamma_prompt_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);                         
		  nv_gamma_prompt_gps_hist->Fill(gps_gamma_diff_temp);
		  nv_gamma_prompt_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_gamma_prompt_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_gamma_prompt_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		  nv_gamma_prompt_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
		}
	      if(gps_gamma_diff_temp>middle_time && gps_gamma_diff_temp<delay_time)
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
	      if(gps_gamma_diff_temp>first_time && gps_gamma_diff_temp<delay_time)
		{ ++g;
		  if(g==1)
		    {
		      //  nv_gamma_first_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);                         
		      nv_gamma_first_gps_hist->Fill(gps_gamma_diff_temp);
		      nv_gamma_first_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_first_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		      nv_gamma_first_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		      nv_gamma_first_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
		    }
		}
	      if(gps_gamma_diff_temp>late_time && gps_gamma_diff_temp<end_time)
		{
		  //     nv_gamma_late_ene_s1_hist->Fill(od_cluster_charge->at(ncluster),tpc_total_s1);                           
		  nv_gamma_late_gps_hist->Fill(gps_gamma_diff_temp);
		  nv_gamma_late_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_gamma_late_small_ene_hist->Fill(od_cluster_charge->at(ncluster));
		  nv_gamma_late_gps_ene_hist->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster));
		  nv_gamma_late_ntuple->Fill(gps_gamma_diff_temp,od_cluster_charge->at(ncluster),od_cluster_height->at(ncluster),od_cluster_multiplicity->at(ncluster));
		}
	    }
	}
    }
}


void SelfAwaySelector::BookHistograms()
{
  Info("ReconSelector::BookHistograms()","creating histograms...");
  string label = GetOption();
  TList* list = GetOutputList();

  //[ns]
  double prompt_time = 2.e+4;
  double middle_time = 0;
  double delay_time = 6.e+4;
  double first_time = 2.e+4;
  double after_time = 2.e+4;
  double late_time = 1.e+5;
  double end_time = 1.4e+5;

  int gamma_bins = 1000;
  int gamma_gps_bins = 5000;
  double gamma_coin_ene = 5.e+3;
  int neutron_bins = 1000;
  int neutron_gps_bins = 5000;
  double neutron_coin_ene = 5.e+3;
  double fail_ene = 500; //pe                                                                                                
  int fail_bins = 125;

  nv_all_gps_hist = new TH1F("nv_all_gps_hist","All Clusters Veto Time;Time [ns]",1000,middle_time,end_time);
  nv_all_ene_hist = new TH1F("nv_all_ene_hist","All Clusters Veto Energy;Energy [PE]",500,0,1.e+4);
  nv_all_small_ene_hist = new TH1F("nv_all_small_ene_hist","All Clusters Veto Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_all_gps_ene_hist = new TH2F("nv_all_gps_ene_hist","All ClustersVeto Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,end_time,500,0,1.e+4);
  nv_all_ntuple = new TNtuple("nv_all_ntuple","All Clusters Veto Data","time:ene:height:mult");
  nv_all_charge_height_multiplicity_hist = new TH2F("nv_all_charge_height_multiplicity_hist","Veto: height/multiplicity Vs Charge;od_cluster_charge[PE];height/multicut",1000,0,1000,10000,0,2.e+8);

  nv_fail_gps_hist = new TH1F("nv_fail_gps_hist","Fail Multicut Veto Time;Time [ns]",1000,middle_time,end_time);
  nv_fail_ene_hist = new TH1F("nv_fail_ene_hist","Fail Multicut Veto Energy;Energy [PE]",500,0,fail_ene);
  nv_fail_small_ene_hist = new TH1F("nv_fail_small_ene_hist","Fail Multicut Veto Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_fail_gps_ene_hist = new TH2F("nv_fail_gps_ene_hist","Fail MulticutVeto Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,end_time,500,0,fail_ene);
  nv_fail_ntuple = new TNtuple("nv_fail_ntuple","Fail Multicut Veto Data","time:ene:height:mult");

  nv_gps_hist = new TH1F("nv_gps_hist","Veto Time;Time [ns]",1000,middle_time,end_time);
  nv_ene_hist = new TH1F("nv_ene_hist","Veto Energy;Energy [PE]",500,0,1.e+4);
  nv_small_ene_hist = new TH1F("nv_small_ene_hist","Veto Small Energy;Energy [PE]",fail_bins,0,fail_ene);
  nv_gps_ene_hist = new TH2F("nv_gps_ene_hist","Veto Time Vs Energy;Time [ns];Energy[PE]",1000,middle_time,end_time,500,0,1.e+4);
  nv_ntuple = new TNtuple("nv_ntuple","Veto Data","time:ene:height:mult");
  nv_charge_height_multiplicity_hist = new TH2F("nv_charge_height_multiplicity_hist","Veto: height/multiplicity Vs Charge;od_cluster_charge[PE];height/multicut",1000,0,1000,10000,0,2.e+8);

  nv_gamma_full_ene_hist = new TH1F("nv_gamma_full_ene_hist","Veto Gamma Full Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  nv_gamma_full_small_ene_hist = new TH1F("nv_gamma_full_small_ene_hist","Veto Gamma Full Energy(small range);Energy [PE]",fail_bins,0,fail_ene);

  nv_gamma_coincidence_gps_hist = new TH1F("nv_gamma_coincidence_gps_hist","Veto Gamma Coincidence Time;Time [ns]",gamma_bins,middle_time,end_time);
  nv_gamma_coincidence_ene_hist = new TH1F("nv_gamma_coincidence_ene_hist","Veto Gamma Coincidence Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  nv_gamma_coincidence_small_ene_hist = new TH1F("nv_gamma_coincidence_small_ene_hist","Veto Gamma Coincidence Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_coincidence_gps_ene_hist = new TH2F("nv_gamma_coincidence_gps_ene_hist","Veto Gamma Coincidence Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,middle_time,end_time,gamma_bins,0,gamma_coin_ene);
  nv_gamma_coincidence_ntuple = new TNtuple("nv_gamma_coincidence_ntuple","Veto Gamma Coincidence Data","time:ene:height:mult");

  nv_gamma_prompt_gps_hist = new TH1F("nv_gamma_prompt_gps_hist","Veto Gamma Prompt Time;Time [ns]",100,middle_time,prompt_time);
  nv_gamma_prompt_ene_hist = new TH1F("nv_gamma_prompt_ene_hist","Veto Gamma Prompt Energy;Energy [PE]",100,0,gamma_coin_ene);
  nv_gamma_prompt_small_ene_hist = new TH1F("nv_gamma_prompt_small_ene_hist","Veto Gamma Prompt Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_prompt_gps_ene_hist = new TH2F("nv_gamma_prompt_gps_ene_hist","Veto Gamma Prompt Time Vs Energy;Time [ns];Energy[PE]",100,middle_time,prompt_time,100,0,gamma_coin_ene);
  nv_gamma_prompt_ntuple = new TNtuple("nv_gamma_prompt_ntuple","Veto Gamma Prompt Data","time:ene:height:mult");

  nv_gamma_delay_gps_hist = new TH1F("nv_gamma_delay_gps_hist","Veto Gamma Delay Time;Time [ns]",gamma_bins,middle_time,delay_time);
  nv_gamma_delay_ene_hist = new TH1F("nv_gamma_delay_ene_hist","Veto Gamma Delay Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  nv_gamma_delay_small_ene_hist = new TH1F("nv_gamma_delay_small_ene_hist","Veto Gamma Delay Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_delay_gps_ene_hist = new TH2F("nv_gamma_delay_gps_ene_hist","Veto Gamma Delay Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,middle_time,delay_time,gamma_bins,0,gamma_coin_ene);
  nv_gamma_delay_ntuple = new TNtuple("nv_gamma_delay_ntuple","Veto Gamma Delay Data","time:ene:height:mult");

  nv_gamma_after_gps_hist = new TH1F("nv_gamma_after_gps_hist","Veto Gamma After Time;Time [ns]",gamma_bins,after_time,delay_time);
  nv_gamma_after_ene_hist = new TH1F("nv_gamma_after_ene_hist","Veto Gamma After Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  nv_gamma_after_small_ene_hist = new TH1F("nv_gamma_after_small_ene_hist","Veto Gamma After Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_after_gps_ene_hist = new TH2F("nv_gamma_after_gps_ene_hist","Veto Gamma After Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,after_time,delay_time,gamma_bins,0,gamma_coin_ene);
  nv_gamma_after_ntuple = new TNtuple("nv_gamma_after_ntuple","Veto Gamma After Data","time:ene:height:mult");

  int gamma_first_bins = 125;

  nv_gamma_first_gps_hist = new TH1F("nv_gamma_first_gps_hist","Veto Gamma First Time;Time [ns]",gamma_first_bins,first_time,delay_time);
  nv_gamma_first_ene_hist = new TH1F("nv_gamma_first_ene_hist","Veto Gamma First Energy;Energy [PE]",gamma_first_bins,0,gamma_coin_ene);
  nv_gamma_first_small_ene_hist = new TH1F("nv_gamma_first_small_ene_hist","Veto First Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_first_gps_ene_hist = new TH2F("nv_gamma_first_gps_ene_hist","Veto Gamma First Time Vs Energy;Time [ns];Energy[PE]",gamma_first_bins,first_time,delay_time,gamma_first_bins,0,gamma_coin_ene);
  nv_gamma_first_ntuple = new TNtuple("nv_gamma_first_ntuple","Veto Gamma First Data","time:ene:height:mult");

  nv_gamma_late_gps_hist = new TH1F("nv_gamma_late_gps_hist","Veto Gamma Late Time;Time [ns]",gamma_bins,late_time,end_time);
  nv_gamma_late_ene_hist = new TH1F("nv_gamma_late_ene_hist","Veto Gamma Late Energy;Energy [PE]",gamma_bins,0,gamma_coin_ene);
  nv_gamma_late_small_ene_hist = new TH1F("nv_gamma_late_small_ene_hist","Veto Gamma Late Energy(small range);Energy [PE]",fail_bins,0,fail_ene);
  nv_gamma_late_gps_ene_hist = new TH2F("nv_gamma_late_gps_ene_hist","Veto Gamma Late Time Vs Energy;Time [ns];Energy[PE]",gamma_bins,late_time,end_time,gamma_bins,0,gamma_coin_ene);
  nv_gamma_late_ntuple = new TNtuple("nv_gamma_late_ntuple","Veto Gamma Late Data","time:ene:height:mult");

  id = new TH1F("id","event ID",1,0,1);

  list->Add(id);
  list->Add(nv_all_gps_hist);
  list->Add(nv_all_ene_hist);
  list->Add(nv_all_small_ene_hist);
  list->Add(nv_all_gps_ene_hist);
  list->Add(nv_all_ntuple);

  list->Add(nv_all_charge_height_multiplicity_hist);
  list->Add(nv_fail_gps_hist);
  list->Add(nv_fail_ene_hist);
  list->Add(nv_fail_small_ene_hist);
  list->Add(nv_fail_gps_ene_hist);
  list->Add(nv_fail_ntuple);

  list->Add(nv_gps_hist);
  list->Add(nv_ene_hist);
  list->Add(nv_small_ene_hist);
  list->Add(nv_gps_ene_hist);
  list->Add(nv_ntuple);
  list->Add(nv_charge_height_multiplicity_hist);
  list->Add(nv_gamma_full_ene_hist);
  list->Add(nv_gamma_full_small_ene_hist);

  list->Add(nv_gamma_coincidence_gps_hist);
  list->Add(nv_gamma_coincidence_ene_hist);
  list->Add(nv_gamma_coincidence_small_ene_hist);
  list->Add(nv_gamma_coincidence_gps_ene_hist);
  list->Add(nv_gamma_coincidence_ntuple);

  list->Add(nv_gamma_prompt_gps_hist);
  list->Add(nv_gamma_prompt_ene_hist);
  list->Add(nv_gamma_prompt_small_ene_hist);
  list->Add(nv_gamma_prompt_gps_ene_hist);
  list->Add(nv_gamma_prompt_ntuple);
  
  list->Add(nv_gamma_delay_gps_hist);
  list->Add(nv_gamma_delay_ene_hist);
  list->Add(nv_gamma_delay_small_ene_hist);
  list->Add(nv_gamma_delay_gps_ene_hist);
  list->Add(nv_gamma_delay_ntuple);
  
  list->Add(nv_gamma_after_gps_hist);
  list->Add(nv_gamma_after_ene_hist);
  list->Add(nv_gamma_after_small_ene_hist);
  list->Add(nv_gamma_after_gps_ene_hist);
  list->Add(nv_gamma_after_ntuple);
  
  list->Add(nv_gamma_first_gps_hist);
  list->Add(nv_gamma_first_ene_hist);
  list->Add(nv_gamma_first_small_ene_hist);
  list->Add(nv_gamma_first_gps_ene_hist);
  list->Add(nv_gamma_first_ntuple);
  
  list->Add(nv_gamma_late_gps_hist);
  list->Add(nv_gamma_late_ene_hist);
  list->Add(nv_gamma_late_small_ene_hist);
  list->Add(nv_gamma_late_gps_ene_hist);
  list->Add(nv_gamma_late_ntuple);


}

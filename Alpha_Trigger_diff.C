#include <sstream>
#include <vector>
#include <iostream> 
#include "TFile.h"
#include "THStack.h"
#include "TTree.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH2F.h"
#include "TRint.h"
using namespace std;
string Time = "_Jan8PM";

//TRint* theApp;
void Alpha_Trigger_diff()
//void main()
{
  //theApp = new TRint("App",NULL,NULL,NULL,0);
  
  gStyle->SetOptStat(0);
  //  THStack *Neutron_QEnergyMC_norm = new THStack("Neutron_QEnergyMC_norm","Energy Spectrum;Energy [keV]");
  THStack *tpc_gamma_energy_norm = new THStack("tpc_gamma_energy_norm","TPC Gamma Energy Spectrum;Energy [keV]");
  THStack *nv_alpha_qenergy_norm = new THStack("nv_alpha_qenergy_norm","NV last alpha Energy Spectrum;Energy [keV]");
  THStack *nv_remain_time_norm = new THStack("nv_remain_time_norm","Remain Time;Time [ns]");
  THStack *nv_thermal_time_norm = new THStack("nv_thermal_time_norm","Thermal Time;Time [ns]");
  THStack *nv_residual_time_norm = new THStack("nv_residual_time_norm","Residual Time;Time [ns]");
  THStack *nv_first_time_norm = new THStack("nv_first_time_norm","First Time;Time [ns]");

  THStack *nv_remain_qenergy_norm = new THStack("nv_remain_qenergy_norm","Remain Quench Energy;Energy [keV]");
  THStack *nv_thermal_qenergy_norm = new THStack("nv_thermal_qenergy_norm","Thermal Quench Energy;Energy [keV]");
  THStack *nv_residual_qenergy_norm = new THStack("nv_residual_qenergy_norm","Residual Quench Energy;Energy [keV]");
  THStack *nv_first_qenergy_norm = new THStack("nv_first_qenergy_norm","First Quench Energy;Energy [keV]");

  vector<int> linecolor = Colors();
  TCanvas* c10 = new TCanvas("c10","TPC Gamma Energy",1000,600);
  c10->Divide(2,2);

  TCanvas* c11 = new TCanvas("c11","remain time",1000,600);
  c11->Divide(2,2);
  TCanvas* c12 = new TCanvas("c12","thermal time",1000,600);
  c12->Divide(2,2);
  TCanvas* c13 = new TCanvas("c13","residual time",1000,600);
  c13->Divide(2,2);
  TCanvas* c14 = new TCanvas("c14","first time",1000,600);
  c14->Divide(2,2);

  TCanvas* c20 = new TCanvas("c20","NV last alphaEnergy",1000,600);
  c20->Divide(2,2);

  TCanvas* c21 = new TCanvas("c21","remain energy",1000,600);
  c21->Divide(2,2);
  TCanvas* c22 = new TCanvas("c22","thermal energy",1000,600);
  c22->Divide(2,2);
  TCanvas* c23 = new TCanvas("c23","residual energy",1000,600);
  c23->Divide(2,2);
  TCanvas* c24 = new TCanvas("c24","first energy",1000,600);
  c24->Divide(2,2);

  TCanvas* c31 = new TCanvas("c31","remain time vs energy",1000,600);
  c31->Divide(2,2);
  TCanvas* c32 = new TCanvas("c32","thermal time vs energy",1000,600);
  c32->Divide(2,2);
  TCanvas* c33 = new TCanvas("c33","residual time vs energy",1000,600);
  c33->Divide(2,2);
  TCanvas* c34 = new TCanvas("c34","first time vs energy",1000,600);
  c34->Divide(2,2);

  const int NUM = 3;
  //#define Obj
#ifdef Obj
  TObjArray tpc_gamma_energy_list(0);
  TObjArray nv_remain_time_list(0);
  TObjArray nv_thermal_time_list(0);
  TObjArray nv_residual_time_list(0);
  TObjArray nv_first_time_list(0);
  TObjArray nv_remain_qenergy_list(0);
  TObjArray nv_thermal_qenergy_list(0);
  TObjArray nv_residual_qenergy_list(0);
  TObjArray nv_first_qenergy_list(0);
#endif
  TLegend *leg = new TLegend(0.47,0.7,0.87,0.9);
  //  for(int i=0; i<NUM; i++)
  for(int i=3; i<6; i++)
    { 
      TFile* t = new TFile(Readdatafile(i).c_str());
      TH1F* tpc_gamma_energy_hist = (TH1F*) t->Get("tpc_gamma_energy_hist");
      TH1F* nv_alpha_qenergy_hist = (TH1F*) t->Get("nv_alpha_qenergy_hist");
      TH1F* nv_remain_time_hist = (TH1F*) t->Get("nv_remain_time_hist");
      TH1F* nv_thermal_time_hist = (TH1F*) t->Get("nv_thermal_time_hist");
      TH1F* nv_residual_time_hist = (TH1F*) t->Get("nv_residual_time_hist");
      TH1F* nv_first_time_hist = (TH1F*) t->Get("nv_first_time_hist");

      TH1F* nv_remain_qenergy_hist = (TH1F*) t->Get("nv_remain_quchenergy_hist");
      TH1F* nv_thermal_qenergy_hist = (TH1F*) t->Get("nv_thermal_quchenergy_hist");
      TH1F* nv_residual_qenergy_hist = (TH1F*) t->Get("nv_residual_quchenergy_hist");
      TH1F* nv_first_qenergy_hist = (TH1F*) t->Get("nv_first_quchenergy_hist");
      
      TH2F* nv_remain_eof_qenergy_hist = (TH2F*) t->Get("nv_remain_eof_quchenergy_hist");
      TH2F* nv_thermal_eof_qenergy_hist = (TH2F*) t->Get("nv_thermal_eof_quchenergy_hist");
      TH2F* nv_residual_eof_qenergy_hist = (TH2F*) t->Get("nv_residual_eof_quchenergy_hist");
      TH2F* nv_first_eof_qenergy_hist = (TH2F*) t->Get("nv_first_eof_quchenergy_hist");

      nv_remain_time_hist->Rebin(4);
      nv_residual_time_hist->Rebin(4);
      nv_alpha_qenergy_hist->Rebin(10);
      nv_residual_qenergy_hist->Rebin(4);
      nv_first_time_hist->Rebin(4);
      //  Calculate_Probability(nv_pdf_hist,fraction,N,ene);
      //  Calculate_Probability(nv_ER_pdf_hist,fraction,N,ene);	  
      int color = linecolor.at(i);
      tpc_gamma_energy_hist->SetLineColor(color);
      nv_alpha_qenergy_hist->SetLineColor(color);
      nv_remain_time_hist->SetLineColor(color);
      nv_thermal_time_hist->SetLineColor(color);
      nv_residual_time_hist->SetLineColor(color);
      nv_first_time_hist->SetLineColor(color);

      nv_remain_qenergy_hist->SetLineColor(color);
      nv_thermal_qenergy_hist->SetLineColor(color);
      nv_residual_qenergy_hist->SetLineColor(color);
      nv_first_qenergy_hist->SetLineColor(color);
    
      tpc_gamma_energy_hist->SetLineWidth(3);
      nv_alpha_qenergy_hist->SetLineWidth(3);
      nv_remain_time_hist->SetLineWidth(3);
      nv_thermal_time_hist->SetLineWidth(3);
      nv_residual_time_hist->SetLineWidth(3);
      nv_first_time_hist->SetLineWidth(3);

      nv_remain_qenergy_hist->SetLineWidth(3);
      nv_thermal_qenergy_hist->SetLineWidth(3);
      nv_residual_qenergy_hist->SetLineWidth(3);
      nv_first_qenergy_hist->SetLineWidth(3);

      string entryname;
      /*      if(i==0) entryname="Gamma Only Channel";
      else if(i==1) entryname = "Neutron Only Channel";  
      else if(i==2) entryname = "All Channels";  
      */
      if(i==5) entryname="Gamma Only Channel";
      else if(i==4) entryname = "Neutron Only Channel";  
      else if(i==3) entryname = "All Channels";  

      TLegend *leg1 = new TLegend(0.47,0.7,0.87,0.9);
      leg1->AddEntry(nv_thermal_qenergy_hist,entryname.c_str(),"l");
      leg1->SetTextColor(color);
      leg->AddEntry(nv_thermal_qenergy_hist,entryname.c_str(),"l");
      i=i-3;
      c10->cd(i+1);
      gPad->SetLogy();
      tpc_gamma_energy_hist->Draw();
      leg1->Draw();

      c11->cd(i+1);
      gPad->SetLogy();
      nv_remain_time_hist->Draw();
      leg1->Draw();

      c12->cd(i+1);
      gPad->SetLogy();
      nv_thermal_time_hist->Draw();
      leg1->Draw();

      c13->cd(i+1);
      gPad->SetLogy();
      nv_residual_time_hist->Draw();
      leg1->Draw();

      c14->cd(i+1);
      gPad->SetLogy();
      nv_first_time_hist->Draw();
      leg1->Draw();

      c20->cd(i+1);
      gPad->SetLogy();
      nv_alpha_qenergy_hist->Draw();
      leg1->Draw();

      c21->cd(i+1);
      gPad->SetLogy();
      nv_remain_qenergy_hist->Draw();
      leg1->Draw();

      c22->cd(i+1);
      gPad->SetLogy();
      nv_thermal_qenergy_hist->Draw();
      leg1->Draw();

      c23->cd(i+1);
      gPad->SetLogy();
      nv_residual_qenergy_hist->Draw();
      leg1->Draw();

      c24->cd(i+1);
      gPad->SetLogy();
      nv_first_qenergy_hist->Draw();
      leg1->Draw();

      c31->cd(i+1);
      //  gPad->SetLogy();
      nv_remain_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      c32->cd(i+1);
      //      gPad->SetLogy();
      nv_thermal_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      c33->cd(i+1);
      gPad->SetLogy();
      nv_residual_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      c34->cd(i+1);
      gPad->SetLogy();
      nv_first_eof_qenergy_hist->Draw("colz");
      leg1->Draw();

      tpc_gamma_energy_norm->Add(tpc_gamma_energy_hist);
      nv_remain_time_norm->Add(nv_remain_time_hist);
      nv_thermal_time_norm->Add(nv_thermal_time_hist);
      nv_residual_time_norm->Add(nv_residual_time_hist);
      nv_first_time_norm->Add(nv_first_time_hist);

      nv_alpha_qenergy_norm->Add(nv_alpha_qenergy_hist);
      nv_remain_qenergy_norm->Add(nv_remain_qenergy_hist);
      nv_thermal_qenergy_norm->Add(nv_thermal_qenergy_hist);
      nv_residual_qenergy_norm->Add(nv_residual_qenergy_hist);
      nv_first_qenergy_norm->Add(nv_first_qenergy_hist);

#ifdef Obj
      tpc_gamma_energy_list.Add(tpc_gamma_energy_hist);
      nv_remain_time_list.Add(nv_remain_time_hist);
      nv_thermal_time_list.Add(nv_thermal_time_hist);
      nv_residual_time_list.Add(nv_residual_time_hist);
      nv_first_time_list.Add(nv_first_time_hist);

      nv_remain_qenergy_list.Add(nv_remain_qenergy_hist);
      nv_thermal_qenergy_list.Add(nv_thermal_qenergy_hist);
      nv_residual_qenergy_list.Add(nv_residual_qenergy_hist);
      nv_first_qenergy_list.Add(nv_first_qenergy_hist);
#endif
      i=i+3;
    }
  /*  c31->Close();
  c32->Close();
  c33->Close();
  c34->Close();
  */
  c10->cd(4);
  gPad->SetLogy();
  tpc_gamma_energy_norm->Draw("nostack");
  leg->Draw();

  c11->cd(4);
  gPad->SetLogy();
  nv_remain_time_norm->Draw("nostack");
  leg->Draw();

  c12->cd(4);
  gPad->SetLogy();
  nv_thermal_time_norm->Draw("nostack");
  leg->Draw();
  
  c13->cd(4);
  gPad->SetLogy();
  nv_residual_time_norm->Draw("nostack");
  leg->Draw();
  
  c14->cd(4);
  gPad->SetLogy();
  nv_first_time_norm->Draw("nostack");
  leg->Draw();

  c20->cd(4);
  gPad->SetLogy();
  nv_alpha_qenergy_norm->Draw("nostack");
  leg->Draw();
  
  c21->cd(4);
  gPad->SetLogy();
  nv_remain_qenergy_norm->Draw("nostack");
  leg->Draw();

  c22->cd(4);
  gPad->SetLogy();
  nv_thermal_qenergy_norm->Draw("nostack");
  leg->Draw();
  
  c23->cd(4);
  gPad->SetLogy();
  nv_residual_qenergy_norm->Draw("nostack");
  leg->Draw();
  
  c24->cd(4);
  gPad->SetLogy();
  nv_first_qenergy_norm->Draw("nostack");
  leg->Draw();

  /*  TCanvas* c1 = new TCanvas("c1","",600,400);
  c1->SetLogy();
  nv_remain_time_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c2 = new TCanvas("c2","",600,400);
  c2->SetLogy();
  nv_residual_time_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c3 = new TCanvas("c3","",600,400);
  c3->SetLogy();
  nv_first_time_norm->Draw("nostack");
  leg->Draw();
  */

#define save_plots
#ifdef save_plots
  c10->SaveAs(Form("tpc_gamma_energy_list_Jan22PM.png"));
  c11->SaveAs(Form("nv_remain_time_list_Jan22PM.png"));
  c12->SaveAs(Form("nv_thermal_time_list_Jan22PM.png"));
  c13->SaveAs(Form("nv_residual_time_list_Jan22PM.png"));
  c14->SaveAs(Form("nv_first_time_list_Jan22PM.png"));
  c20->SaveAs(Form("nv_alpha_qenergy_list_Jan22PM.png"));
  c21->SaveAs(Form("nv_remain_qenergy_list_Jan22PM.png"));
  c22->SaveAs(Form("nv_thermal_qenergy_list_Jan22PM.png"));
  c23->SaveAs(Form("nv_residual_qenergy_list_Jan22PM.png"));
  c24->SaveAs(Form("nv_first_qenergy_list_Jan22PM.png"));
  c31->SaveAs(Form("nv_remain_eof_qenergy_list_Jan22PM.png"));
  c32->SaveAs(Form("nv_thermal_eof_qenergy_list_Jan22PM.png"));
  c33->SaveAs(Form("nv_residual_eof_qenergy_list_Jan22PM.png"));
  c34->SaveAs(Form("nv_first_eof_qenergy_list_Jan22PM.png"));
#endif

  string dirname="/darkside/users/hqian/TMBfraction/results/";
  string output1=dirname+"diffAlpha_tmb"+"_Jan22PM"+".root"; 
  TFile out1(output1.c_str(),"RECREATE");
#ifdef Obj
  tpc_gamma_energy_list.Write();
  nv_remain_time_list.Write();
  nv_thermal_time_list.Write();
  nv_residual_time_list.Write();
  nv_first_time_list.Write();
  nv_remain_qenergy_list.Write();
  nv_thermal_qenergy_list.Write();
  nv_residual_qenergy_list.Write();
  nv_first_qenergy_list.Write();
#endif
  //  c1->Write();
  // c2->Write();
  //  c3->Write();
  c10->Write();
  c11->Write();
  c12->Write();
  c13->Write();
  c14->Write();
  c20->Write();
  c21->Write();
  c22->Write();
  c23->Write();
  c24->Write();
  c31->Write();
  c32->Write();
  c33->Write();
  c34->Write();
  out1.Write();
  out1.Close();
}

string Readdatafile(int i)
{
  string dirname="/darkside/users/hqian/TMBfraction/results/";
  string middle="tmb_alpha";
  string last=".root";
  string filename;
  stringstream oss;
  if(i==0)
    filename=dirname+middle+Time+last;
  else {
    oss<<i;
    filename=dirname+middle+"_v"+oss.str()+Time+last;
    oss.str("");
  }
  cout<<"Processing Data file: "<<filename<<endl;
  return filename;
}

void Calculate_Probability(TH1F* nv_pdf_hist, string label, const int N, double ene[], double* prob) 
{
  for(int i=0; i<N; i++)
    {
      int bin =  nv_pdf_hist->GetXaxis()->FindBin(ene[i]);
      prob[i] = nv_pdf_hist->GetBinContent(bin);
      cout<<"For "<<label<<", Probability corresponding to Energy "<<ene[i]<<" is "<<prob[i]<<endl;
    }
}

void Calculate_Probability(TH1F* nv_pdf_hist, double fraction, const int N, double ene[]) 
{
  for(int i=0; i<N; i++)
    {
      int bin =  nv_pdf_hist->GetXaxis()->FindBin(ene[i]);
      double prob = nv_pdf_hist->GetBinContent(bin);
      cout<<"For TMB concentration "<<fraction<<", Probability corresponding to Energy "<<ene[i]<<" is "<<prob*100.<<endl;
    }
}

vector<int> Colors()
{
  vector<int> color;
  color.push_back(TColor::GetColor("#5A1DE8"));
  color.push_back(TColor::GetColor("#B26618"));
  color.push_back(TColor::GetColor("#FB78FF"));
  color.push_back(TColor::GetColor("#AE3EFF"));
  color.push_back(TColor::GetColor("#B24F18"));
  color.push_back(TColor::GetColor("#2FDAFF"));      
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


  /*  THStack *nv_pdf_fine_hist_norm = new THStack("nv_pdf_fine_hist_norm","Cumulative distribution function for [-100ns,500ns] window;Energy [keVee];Probability");
  THStack *nv_ER_energy_hist_norm = new THStack("nv_ER_energy_hist_norm","Energy Spectrum for [1.5us,1ms] window;Energy [keVee]");
  THStack *nv_ER_pdf_hist_norm = new THStack("nv_ER_pdf_hist_norm","Cumulative distribution function for [1.5us,1ms] window;Energy [keVee];Probability");
  THStack *nv_ER_pdf_fine_hist_norm = new THStack("nv_ER_pdf_fine_hist_norm","Cumulative distribution function for [1.5us,1ms] window;Energy [keVee];Probability");  
  TObjArray nv_ER_pdf_list(0);
  TObjArray nv_ER_pdf_fine_list(0);
  TH1F* nv_pdf_fine_hist = (TH1F*) t->Get("nv_pdf_fine_hist");
  TH1F* nv_ER_energy_hist = (TH1F*) t->Get("nv_ER_energy_hist");
  TH1F* nv_ER_pdf_hist = (TH1F*) t->Get("nv_ER_pdf_hist");
  TH1F* nv_ER_pdf_fine_hist = (TH1F*) t->Get("nv_ER_pdf_fine_hist");        
  nv_pdf_hist->SetLineColor(color);
  nv_pdf_fine_hist->SetLineColor(color);
  nv_ER_pdf_hist->SetLineColor(color);
  nv_ER_pdf_fine_hist->SetLineColor(color);
  nv_ER_energy_hist->SetLineColor(color);      
  nv_ER_energy_hist_norm->Add(nv_ER_energy_hist);
  nv_pdf_hist_norm->Add(nv_pdf_hist);
  nv_pdf_fine_hist_norm->Add(nv_pdf_fine_hist);
  nv_ER_pdf_hist_norm->Add(nv_ER_pdf_hist);
  nv_ER_pdf_fine_hist_norm->Add(nv_ER_pdf_fine_hist);
  nv_pdf_fine_list.Add(nv_pdf_fine_hist);
  nv_ER_pdf_list.Add(nv_ER_pdf_hist);
  nv_ER_pdf_fine_list.Add(nv_ER_pdf_fine_hist);
  TCanvas* c3 = new TCanvas("c3","pdf",600,400);
  nv_pdf_fine_hist_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c4 = new TCanvas("c4","ER pdf",600,400);
  nv_ER_pdf_fine_hist_norm->Draw("nostack");
  leg2->Draw();
//  L2->Draw();

  TCanvas* c5 = new TCanvas("c5","energy",600,400);
  nv_energy_hist_norm->Draw("nostack");
  leg->Draw();

  TCanvas* c6 = new TCanvas("c6","energy",600,400);
  nv_ER_energy_hist_norm->Draw("nostack");
  leg2->Draw();

  TCanvas* c7 = new TCanvas("c7","Total",1200,800);
  c7->Divide(2,2);
  c7->cd(1);
  gPad->SetLogy();
  nv_energy_hist_norm->Draw("nostack");
  leg->Draw();
  c7->cd(2);
  gPad->SetLogy();
  nv_ER_energy_hist_norm->Draw("nostack");
  leg2->Draw();
  c7->cd(3);
  gPad->SetLogy();
  nv_pdf_hist_norm->Draw("nostack");
  leg->Draw();
  c7->cd(4);
  gPad->SetLogy();
  nv_ER_pdf_hist_norm->Draw("nostack");
  leg2->Draw();  
  */
